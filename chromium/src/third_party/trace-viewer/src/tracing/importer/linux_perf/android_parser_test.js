// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


base.require('tracing.test_utils');
base.require('tracing.importer.linux_perf_importer');

'use strict';

base.unittest.testSuite('tracing.importer.linux_perf.android_parser',
                        function() {
      test('androidUserlandImport', function() {
        var lines = [
          'SurfaceFlinger-4831  [001] ...1 80909.598554: tracing_mark_write: B|4829|onMessageReceived', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.598572: tracing_mark_write: B|4829|handleMessageInvalidate', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.598590: tracing_mark_write: B|4829|latchBuffer', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.598604: tracing_mark_write: E',
          'SurfaceFlinger-4831  [001] ...1 80909.598627: tracing_mark_write: B|4829|latchBuffer', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.598651: tracing_mark_write: B|4829|updateTexImage', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.598675: tracing_mark_write: B|4829|acquireBuffer', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.598695: tracing_mark_write: B|4829|' + // @suppress longLineCheck
              'com.android.launcher/com.android.launcher2.Launcher: 0',
          'SurfaceFlinger-4831  [001] ...1 80909.598709: tracing_mark_write: E',
          'SurfaceFlinger-4831  [001] ...1 80909.598733: tracing_mark_write: C|4829|' + // @suppress longLineCheck
              'com.android.launcher/com.android.launcher2.Launcher|0',
          'SurfaceFlinger-4831  [001] ...1 80909.598746: tracing_mark_write: E',
          'SurfaceFlinger-4831  [001] ...1 80909.598844: tracing_mark_write: B|4829|releaseBuffer', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.598862: tracing_mark_write: B|4829|' + // @suppress longLineCheck
              'com.android.launcher/com.android.launcher2.Launcher: 2',
          'SurfaceFlinger-4831  [001] ...1 80909.598876: tracing_mark_write: E',
          'SurfaceFlinger-4831  [001] ...1 80909.598892: tracing_mark_write: E',
          'SurfaceFlinger-4831  [001] ...1 80909.598925: tracing_mark_write: E',
          'SurfaceFlinger-4831  [001] ...1 80909.598955: tracing_mark_write: E',
          'SurfaceFlinger-4831  [001] ...1 80909.598988: tracing_mark_write: B|4829|latchBuffer', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.599001: tracing_mark_write: E',
          'SurfaceFlinger-4831  [001] ...1 80909.599021: tracing_mark_write: B|4829|latchBuffer', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.599036: tracing_mark_write: E',
          'SurfaceFlinger-4831  [001] ...1 80909.599068: tracing_mark_write: E',
          'SurfaceFlinger-4831  [001] ...1 80909.599087: tracing_mark_write: E',
          'SurfaceFlinger-4831  [001] ...1 80909.599104: tracing_mark_write: E'
        ];
        var m = new tracing.TraceModel(lines.join('\n'), false);
        assertEquals(0, m.importErrors.length);

        var threads = m.getAllThreads();
        assertEquals(1, threads.length);

        var thread = threads[0];
        assertEquals(4829, thread.parent.pid);
        assertEquals(4831, thread.tid);
        assertEquals('SurfaceFlinger', thread.name);
        assertEquals(11, thread.slices.length);
      });

      test('androidUserlandImportWithSpacesInThreadName', function() {
        var lines = [
          'Surface Flinger -4831  [001] ...1 80909.598590: tracing_mark_write: B|4829|latchBuffer', // @suppress longLineCheck
          'Surface Flinger -4831  [001] ...1 80909.598604: tracing_mark_write: E' // @suppress longLineCheck
        ];
        var m = new tracing.TraceModel(lines.join('\n'), false);
        assertEquals(0, m.importErrors.length);

        var threads = m.getAllThreads();
        assertEquals(1, threads.length);

        var thread = threads[0];
        assertEquals(4829, thread.parent.pid);
        assertEquals(4831, thread.tid);
        assertEquals('Surface Flinger ', thread.name);
        assertEquals(1, thread.slices.length);
      });

      test('androidUserlandLegacyKernelImport', function() {
        var lines = [
          'SurfaceFlinger-4831  [001] ...1 80909.598554: 0: B|4829|onMessageReceived', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.598572: 0: B|4829|handleMessageInvalidate', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.598590: 0: B|4829|latchBuffer',
          'SurfaceFlinger-4831  [001] ...1 80909.598604: 0: E',
          'SurfaceFlinger-4831  [001] ...1 80909.598627: 0: B|4829|latchBuffer',
          'SurfaceFlinger-4831  [001] ...1 80909.598651: 0: B|4829|updateTexImage', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.598675: 0: B|4829|acquireBuffer', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.598695: 0: B|4829|' +
              'com.android.launcher/com.android.launcher2.Launcher: 0',
          'SurfaceFlinger-4831  [001] ...1 80909.598709: 0: E',
          'SurfaceFlinger-4831  [001] ...1 80909.598733: 0: C|4829|' +
              'com.android.launcher/com.android.launcher2.Launcher|0',
          'SurfaceFlinger-4831  [001] ...1 80909.598746: 0: E',
          'SurfaceFlinger-4831  [001] ...1 80909.598844: 0: B|4829|releaseBuffer', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.598862: 0: B|4829|' +
              'com.android.launcher/com.android.launcher2.Launcher: 2',
          'SurfaceFlinger-4831  [001] ...1 80909.598876: 0: E',
          'SurfaceFlinger-4831  [001] ...1 80909.598892: 0: E',
          'SurfaceFlinger-4831  [001] ...1 80909.598925: 0: E',
          'SurfaceFlinger-4831  [001] ...1 80909.598955: 0: E',
          'SurfaceFlinger-4831  [001] ...1 80909.598988: 0: B|4829|latchBuffer', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.599001: 0: E',
          'SurfaceFlinger-4831  [001] ...1 80909.599021: 0: B|4829|latchBuffer', // @suppress longLineCheck
          'SurfaceFlinger-4831  [001] ...1 80909.599036: 0: E',
          'SurfaceFlinger-4831  [001] ...1 80909.599068: 0: E',
          'SurfaceFlinger-4831  [001] ...1 80909.599087: 0: E',
          'SurfaceFlinger-4831  [001] ...1 80909.599104: 0: E'
        ];
        var m = new tracing.TraceModel(lines.join('\n'), false);
        assertEquals(0, m.importErrors.length);

        var threads = m.getAllThreads();
        assertEquals(1, threads.length);

        var thread = threads[0];
        assertEquals(4829, thread.parent.pid);
        assertEquals(4831, thread.tid);
        assertEquals('SurfaceFlinger', thread.name);
        assertEquals(11, thread.slices.length);
      });

      test('androidUserlandChromiumImport', function() {
        var lines = [
          'SandboxedProces-2894  [001] ...1   253.780659: tracing_mark_write: B|2867|DoWorkLoop|arg1=1|cat1', // @suppress longLineCheck
          'SandboxedProces-2894  [001] ...1   253.780671: tracing_mark_write: B|2867|DeferOrRunPendingTask|source=test=test;task=xyz|cat2', // @suppress longLineCheck
          'SandboxedProces-2894  [001] ...1   253.780671: tracing_mark_write: E|2867|DeferOrRunPendingTask||cat1', // @suppress longLineCheck
          'SandboxedProces-2894  [001] ...1   253.780686: tracing_mark_write: B|2867|MessageLoop::RunTask|source=ipc/ipc_sync_message_filter.cc:Send|cat2', // @suppress longLineCheck
          'SandboxedProces-2894  [001] ...1   253.780700: tracing_mark_write: E|2867|MessageLoop::RunTask||cat1', // @suppress longLineCheck
          'SandboxedProces-2894  [001] ...1   253.780750: tracing_mark_write: C|2867|counter1|10|cat1', // @suppress longLineCheck
          'SandboxedProces-2894  [001] ...1   253.780859: tracing_mark_write: E|2867|DoWorkLoop|arg2=2|cat2' // @suppress longLineCheck
        ];
        var m = new tracing.TraceModel(lines.join('\n'), false);
        assertEquals(0, m.importErrors.length);

        var threads = m.getAllThreads();
        assertEquals(1, threads.length);

        var thread = threads[0];
        assertEquals(2867, thread.parent.pid);
        assertEquals(2894, thread.tid);
        assertEquals('SandboxedProces', thread.name);
        assertEquals(3, thread.slices.length);

        assertEquals('test=test', thread.slices[0].args['source']);
        assertEquals('cat2', thread.slices[0].category);
        assertEquals('DeferOrRunPendingTask', thread.slices[0].title);
        assertEquals('xyz', thread.slices[0].args['task']);
        assertEquals('ipc/ipc_sync_message_filter.cc:Send', thread.slices[1].args['source']); // @suppress longLineCheck
        assertEquals('1', thread.slices[2].args['arg1']);
        assertEquals('2', thread.slices[2].args['arg2']);

        var counters = m.getAllCounters();
        assertEquals(1, counters.length);
        assertEquals('cat1', counters[0].category);
        assertEquals('counter1', counters[0].name);
        assertEquals(1, counters[0].numSamples);
        assertEquals(10, counters[0].getSampleValue(0, 0));
      });
    });
