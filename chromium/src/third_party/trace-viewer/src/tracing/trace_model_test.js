// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

base.require('tracing.test_utils');
base.require('tracing.trace_model');
base.require('tracing.importer');

'use strict';

base.unittest.testSuite('tracing.trace_model', function() {
  var ThreadSlice = tracing.trace_model.ThreadSlice;
  var TraceModel = tracing.TraceModel;
  var TitleFilter = tracing.TitleFilter;

  var createTraceModelWithOneOfEverything = function() {
    var m = new TraceModel();
    var cpu = m.getOrCreateCpu(1);
    cpu.slices.push(tracing.test_utils.newSlice(1, 3));

    var p = m.getOrCreateProcess(1);
    var t = p.getOrCreateThread(1);
    t.pushSlice(new ThreadSlice('', 'a', 0, 1, {}, 4));
    t.asyncSlices.push(tracing.test_utils.newAsyncSlice(0, 1, t, t));

    var c = p.getOrCreateCounter('', 'ProcessCounter');
    c.seriesNames.push('a', 'b');
    c.seriesColors.push(0, 1);
    c.timestamps.push(0, 1, 2, 3);
    c.samples.push(5, 10, 6, 15, 5, 12, 7, 16);

    var c1 = cpu.getOrCreateCounter('', 'CpuCounter');
    c1.seriesNames.push('a', 'b');
    c1.seriesColors.push(0, 1);
    c1.timestamps.push(0, 1, 2, 3);
    c1.samples.push(5, 10, 6, 15, 5, 12, 7, 16);

    m.updateBounds();

    return m;
  };

  test('traceModelBounds_EmptyTraceModel', function() {
    var m = new TraceModel();
    m.updateBounds();
    assertEquals(undefined, m.bounds.min);
    assertEquals(undefined, m.bounds.max);
  });

  test('traceModelBounds_OneEmptyThread', function() {
    var m = new TraceModel();
    var t = m.getOrCreateProcess(1).getOrCreateThread(1);
    m.updateBounds();
    assertEquals(undefined, m.bounds.min);
    assertEquals(undefined, m.bounds.max);
  });

  test('traceModelBounds_OneThread', function() {
    var m = new TraceModel();
    var t = m.getOrCreateProcess(1).getOrCreateThread(1);
    t.pushSlice(new ThreadSlice('', 'a', 0, 1, {}, 3));
    m.updateBounds();
    assertEquals(1, m.bounds.min);
    assertEquals(4, m.bounds.max);
  });

  test('traceModelBounds_OneThreadAndOneEmptyThread', function() {
    var m = new TraceModel();
    var t1 = m.getOrCreateProcess(1).getOrCreateThread(1);
    t1.pushSlice(new ThreadSlice('', 'a', 0, 1, {}, 3));
    var t2 = m.getOrCreateProcess(1).getOrCreateThread(1);
    m.updateBounds();
    assertEquals(1, m.bounds.min);
    assertEquals(4, m.bounds.max);
  });

  test('traceModelBounds_OneCpu', function() {
    var m = new TraceModel();
    var cpu = m.getOrCreateCpu(1);
    cpu.slices.push(tracing.test_utils.newSlice(1, 3));
    m.updateBounds();
    assertEquals(1, m.bounds.min);
    assertEquals(4, m.bounds.max);
  });

  test('traceModelBounds_OneCpuOneThread', function() {
    var m = new TraceModel();
    var cpu = m.getOrCreateCpu(1);
    cpu.slices.push(tracing.test_utils.newSlice(1, 3));

    var t = m.getOrCreateProcess(1).getOrCreateThread(1);
    t.pushSlice(new ThreadSlice('', 'a', 0, 1, {}, 4));

    m.updateBounds();
    assertEquals(1, m.bounds.min);
    assertEquals(5, m.bounds.max);
  });

  test('traceModelCanImportEmpty', function() {
    var m;
    m = new TraceModel([]);
    m = new TraceModel('');
  });

  test('traceModelCanImportSubtraces', function() {
    var systraceLines = [
      'SurfaceFlinger-2  [001] ...1 1000.0: 0: B|1|taskA',
      'SurfaceFlinger-2  [001] ...1 2000.0: 0: E'
    ];
    var traceEvents = [
      {ts: 1000, pid: 1, tid: 3, ph: 'B', cat: 'c', name: 'taskB', args: {
        my_object: {id_ref: '0x1000'}
      }},
      {ts: 2000, pid: 1, tid: 3, ph: 'E', cat: 'c', name: 'taskB', args: {}}
    ];

    var combined = JSON.stringify({
      traceEvents: traceEvents,
      systemTraceEvents: systraceLines.join('\n')
    });

    var m = new TraceModel();
    m.importTraces([combined]);
    assertEquals(1, base.dictionaryValues(m.processes).length);

    var p1 = m.processes[1];
    assertNotUndefined(p1);

    var t2 = p1.threads[2];
    var t3 = p1.threads[3];
    assertNotUndefined(t2);
    assertNotUndefined(t3);

    assertEquals(1, t2.slices.length, 1);
    assertEquals('taskA', t2.slices[0].title);

    assertEquals(1, t3.slices.length);
    assertEquals('taskB', t3.slices[0].title);
  });

  test('traceModelWithImportFailure', function() {
    var malformed = '{traceEvents: [{garbage';
    var m = new TraceModel();
    assertThrows(function() {
      m.importTraces([malformed]);
    });
  });

  test('titleFilter', function() {
    var s0 = tracing.test_utils.newSlice(1, 3);
    assertFalse(new TitleFilter('').matchSlice(s0));

    assertTrue(new TitleFilter('a').matchSlice(s0));
    assertFalse(new TitleFilter('x').matchSlice(s0));

    var s1 = tracing.test_utils.newSliceNamed('ba', 1, 3);
    assertTrue(new TitleFilter('a').matchSlice(s1));
    assertTrue(new TitleFilter('ba').matchSlice(s1));
    assertFalse(new TitleFilter('x').matchSlice(s1));
  });

  test('traceModel_toJSON', function() {
    var m = createTraceModelWithOneOfEverything();
    assertNotNull(JSON.stringify(m));
  });

  test('traceModel_findAllThreadsNamed', function() {
    var m = new TraceModel();
    var t = m.getOrCreateProcess(1).getOrCreateThread(1);
    t.name = 'CrBrowserMain';

    m.updateBounds();
    var f = m.findAllThreadsNamed('CrBrowserMain');
    assertArrayEquals([t], f);
    f = m.findAllThreadsNamed('NoSuchThread');
    assertEquals(0, f.length);
  });

  test('traceModel_updateCategories', function() {
    var m = new TraceModel();
    var t = m.getOrCreateProcess(1).getOrCreateThread(1);
    t.pushSlice(new ThreadSlice('categoryA', 'a', 0, 1, {}, 3));
    t.pushSlice(new ThreadSlice('categoryA', 'a', 0, 1, {}, 3));
    t.pushSlice(new ThreadSlice('categoryB', 'a', 0, 1, {}, 3));
    t.pushSlice(new ThreadSlice('categoryA', 'a', 0, 1, {}, 3));
    t.pushSlice(new ThreadSlice('', 'a', 0, 1, {}, 3));
    m.updateCategories_();
    assertArrayEquals(['categoryA', 'categoryB'], m.categories);
  });
});
