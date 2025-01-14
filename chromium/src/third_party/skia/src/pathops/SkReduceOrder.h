/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef SkReduceOrder_DEFINED
#define SkReduceOrder_DEFINED

#include "SkPath.h"
#include "SkPathOpsCubic.h"
#include "SkPathOpsLine.h"
#include "SkPathOpsQuad.h"
#include "SkTArray.h"

union SkReduceOrder {
    enum Quadratics {
        kNo_Quadratics,
        kAllow_Quadratics
    };
    enum Style {
        kStroke_Style,
        kFill_Style
    };

    int reduce(const SkDCubic& cubic, Quadratics, Style);
    int reduce(const SkDLine& line);
    int reduce(const SkDQuad& quad, Style);

    static SkPath::Verb Cubic(const SkPoint pts[4], SkTArray<SkPoint, true>* reducePts);
    static SkPath::Verb Quad(const SkPoint pts[3], SkTArray<SkPoint, true>* reducePts);

    SkDLine fLine;
    SkDQuad fQuad;
    SkDCubic fCubic;
};

#endif
