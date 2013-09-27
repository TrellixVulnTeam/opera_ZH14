#!/usr/bin/python
# Copyright (c) 2011 The Chromium OS Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""Script that syncs the repo manifest with .DEPS.git."""


import logging
import multiprocessing
import os
import sys
import StringIO

from chromite.scripts import chrome_set_ver
from chromite.lib import commandline
from chromite.lib import cros_build_lib
from chromite.lib import git
from chromite.lib import osutils
from chromite.buildbot import constants
from chromite.buildbot import repository



_MANIFEST_TEMPLATE = """\
<?xml version="1.0" encoding="UTF-8"?>
<manifest>
<!-- @@@@ THIS CONTENT IS AUTOGENERATED; DO NOT MODIFY! @@@@ -->

  <include name="%(include_target)s" />
%(content)s
</manifest>
"""

_TYPE_MARKER = """\

<!-- @@@@ THIS IS THE %(type)s CONTENT @@@@ -->

"""

_PROJECT = """\
  <project remote="%(remote)s"
           path="%(path)s"
           name="%(name)s"
           revision="%(ref)s" />
"""


_EXTERNAL_MANIFEST_PROJECT = 'chromiumos/manifest'
_INTERNAL_MANIFEST_PROJECT = 'chromeos/manifest-internal'

TEST_BRANCH = "temp-newly-synced"
MANIFEST_FILE = 'gerrit-source.xml'
INCLUDE_TARGET = 'full.xml'


def _MkProject(path, name, remote, branch='master'):
  return _PROJECT % {'path':path, 'name':name, 'remote':remote,
                     'ref':'refs/heads/%s' % (branch,)}


def ConvertDepsToManifest(deps_file, project_blacklist, manifest_file,
                          remote):
  """Convert dependencies in .DEPS.git files to manifest entries.

  Arguments:
    deps_file: The path to the .DEPS.git file.
    project_blacklist: Set of project names that we shouldn't write
      entries for; specifically since they're already in our parent.
    manifest_file: The file object to write manifest entries to.
    remote: The remote value to write to the new manifest.
  """
  _, merged_deps = chrome_set_ver.GetParsedDeps(deps_file)
  mappings = chrome_set_ver.GetPathToProjectMappings(merged_deps)

  # Check for double checkouts and blacklisted projects.
  previous_projects = set(project_blacklist)
  for rel_path, project in sorted(mappings.iteritems(), key=lambda x: x[1]):
    rel_path = os.path.join('chromium', rel_path)
    if project.startswith('chromiumos'):
      cros_build_lib.Warning('Skipping project %s in %s', project, deps_file)
    elif project in previous_projects:
      cros_build_lib.Warning('Found double checkout of %s to %s',
                             project, rel_path)
    else:
      manifest_file.write(_MkProject(rel_path, project, remote))
      previous_projects.add(project)


class Manifest(object):
  """Encapsulates manifest update logic for an external or internal manifest."""

  EXTERNAL_REMOTE = 'chromium'
  INTERNAL_REMOTE = 'chrome'

  CHROMIUM_PROJECT = CHROMIUM_ROOT = 'chromium/src'
  CHROME_ROOT = 'chromium/src-internal'
  CHROME_PROJECT = 'chrome/src-internal'

  def __init__(self, repo_root, manifest_dir, internal, dryrun=True,
               reference=None):
    self.repo_root = repo_root
    self.manifest_dir = manifest_dir
    self.manifest_path = os.path.join(self.manifest_dir, MANIFEST_FILE)
    self.internal = internal
    self.dryrun = dryrun
    self.reference = reference

  def SyncSources(self):
    repo = repository.RepoRepository(
        self.manifest_dir, self.repo_root, referenced_repo=self.reference,
        manifest=MANIFEST_FILE, branch='master')
    # Trigger the network sync
    repo.Sync(jobs=multiprocessing.cpu_count() + 1, network_only=True)
    projects = [self.CHROMIUM_ROOT]
    if self.internal:
      projects.append(self.CHROME_ROOT)

    for project in projects:
      path = os.path.join(self.repo_root, project)
      if os.path.exists(path):
        try:
          git.CleanAndCheckoutUpstream(path, refresh_upstream=False)
          continue
        except cros_build_lib.RunCommandError:
          cros_build_lib.Error("Failed cleaning %r; wiping.", path)
          cros_build_lib.SudoRunCommand(['rm', '-rf', path], print_cmd=False)

        cros_build_lib.RunCommand(['repo', 'sync', '-ld', project],
                                  cwd=self.repo_root)

  def CreateNewManifest(self):
    """Generates a new manifest with updated Chrome entries."""
    # Prepare git repo for push
    git.CreatePushBranch(
        TEST_BRANCH, self.manifest_dir,
        remote_push_branch=('origin', 'refs/heads/master'))

    content = StringIO.StringIO()
    content.write(_TYPE_MARKER % {'type': 'EXTERNAL'})
    content.write(_MkProject(self.CHROMIUM_ROOT, self.CHROMIUM_PROJECT,
                             self.EXTERNAL_REMOTE, branch='git-svn'))
    # Grab the repo manifest, and ensure that we're not adding a project that
    # our inherit target already has.
    include_target = os.path.join(self.manifest_dir, INCLUDE_TARGET)
    existing_projects = frozenset(git.Manifest.Cached(
        include_target, manifest_include_dir=self.manifest_dir).projects)

    ConvertDepsToManifest(
        os.path.join(self.repo_root, self.CHROMIUM_ROOT, '.DEPS.git'),
        existing_projects, content, self.EXTERNAL_REMOTE)

    if self.internal:
      content.write(_TYPE_MARKER % {'type': 'INTERNAL'})
      content.write(_MkProject(self.CHROME_ROOT, self.CHROME_PROJECT,
                               self.INTERNAL_REMOTE))
      ConvertDepsToManifest(
          os.path.join(self.repo_root, self.CHROME_ROOT,
                       '.DEPS.git'),
          existing_projects, content, self.INTERNAL_REMOTE)

    osutils.WriteFile(
        self.manifest_path,
        _MANIFEST_TEMPLATE % {'content':content.getvalue(),
                              'include_target':INCLUDE_TARGET})

  def IsNewManifestDifferent(self):
    """Checks if newly generated manifest is different from old manifest."""
    return bool(
        git.RunGit(self.manifest_dir, ['status', '--porcelain']).output.strip())

  def CommitNewManifest(self):
    git.RunGit(self.manifest_dir, ['add', self.manifest_path])
    git.RunGit(self.manifest_dir, [
        'commit', '-m',
        'Auto-updating manifest to match .DEPS.git file'])

  def TestNewManifest(self):
    """Runs a 'repo sync' off of new manifest to verify things aren't broken."""

    # Do as cheap a sync as possible; network only is good enough,
    # allow shallow cloning if we don't have a reference, and sync
    # strictly the target branch.
    repo = repository.RepoRepository(
        self.manifest_dir, self.repo_root, branch=TEST_BRANCH,
        referenced_repo=self.reference, manifest=MANIFEST_FILE)
    try:
      repo.Sync(jobs=multiprocessing.cpu_count() + 1, network_only=True)
    except Exception:
      cros_build_lib.Error('Failed to sync with new manifest!')
      raise

  def PushChanges(self):
    """Push changes to manifest live."""
    git.PushWithRetry(
        TEST_BRANCH, self.manifest_dir, dryrun=self.dryrun)

  def PerformUpdate(self):
    self.SyncSources()
    self.CreateNewManifest()
    if not self.IsNewManifestDifferent():
      return
    self.CommitNewManifest()
    self.TestNewManifest()
    self.PushChanges()


def main(argv=None):
  if argv is None:
    argv = sys.argv[1:]

  usage = 'usage: %prog'
  parser = commandline.OptionParser(usage=usage)

  parser.add_option('-r', '--testroot', action='store', type='path',
                    help=('Directory where test checkout is stored.'))
  parser.add_option('-f', '--force', default=True, action='store_false',
                    dest='dryrun',
                    help='If enabled, allow committing.')
  parser.add_option(
      '--internal-manifest-url',
      default='%s/%s' % (constants.GERRIT_INT_SSH_URL,
                         _INTERNAL_MANIFEST_PROJECT),
      help='Url to fetch the internal manifest from.')
  parser.add_option(
      '--external-manifest-url',
      default='%s/%s' % (constants.GERRIT_SSH_URL,
                         _EXTERNAL_MANIFEST_PROJECT),
      help='Url to fetch the external manifest from.')
  parser.add_option('-v', '--verbose', default=False, action='store_true',
                    help='Run with debug output.')
  parser.add_option('--reference', default=None,
                    help="Repository to reference")
  (options, _inputs) = parser.parse_args(argv)

  if not options.testroot:
    parser.error('Please specify a test root!')

  # Set cros_build_lib debug level to hide RunCommand spew.
  logging.getLogger().setLevel(
      logging.DEBUG if options.verbose else logging.WARN)

  if options.reference is None:
    options.reference = git.FindRepoCheckoutRoot(os.getcwd())

  def process_target(internal, manifest_url, reference):
    subdir = 'internal' if internal else 'external'
    root = os.path.join(options.testroot, subdir)
    repo_dir = os.path.join(root, 'repo')
    osutils.SafeMakedirs(repo_dir)
    manifest_dir = os.path.join(root, 'manifest')

    if os.path.exists(manifest_dir):
      git.CleanAndCheckoutUpstream(manifest_dir)
      git.RunGit(manifest_dir,
                 ['checkout', '-B', 'master', '-t', 'origin/master'])
    else:
      repository.CloneGitRepo(manifest_dir, manifest_url)
    m = Manifest(repo_dir, manifest_dir, internal,
                 reference=reference,
                 dryrun=options.dryrun)
    m.PerformUpdate()

    if options.dryrun:
      print "%s manifest is now:" % subdir
      print osutils.ReadFile(os.path.join(manifest_dir, MANIFEST_FILE))

  # Process internal first so that any references to the passed in repo
  # are utilized in full, w/ external then slaving from the new internal.
  process_target(True, options.internal_manifest_url, options.reference)
  process_target(False, options.external_manifest_url,
                 os.path.join(options.testroot, 'internal', 'repo'))