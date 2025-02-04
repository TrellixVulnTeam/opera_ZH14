/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#include "SkAddIntersections.h"
#include "SkOpEdgeBuilder.h"
#include "SkPathOpsCommon.h"
#include "SkPathWriter.h"

// FIXME: this and find chase should be merge together, along with
// other code that walks winding in angles
// OPTIMIZATION: Probably, the walked winding should be rolled into the angle structure
// so it isn't duplicated by walkers like this one
static SkOpSegment* findChaseOp(SkTDArray<SkOpSpan*>& chase, int& nextStart, int& nextEnd) {
    while (chase.count()) {
        SkOpSpan* span;
        chase.pop(&span);
        const SkOpSpan& backPtr = span->fOther->span(span->fOtherIndex);
        SkOpSegment* segment = backPtr.fOther;
        nextStart = backPtr.fOtherIndex;
        SkSTArray<SkOpAngle::kStackBasedCount, SkOpAngle, true> angles;
        int done = 0;
        if (segment->activeAngle(nextStart, &done, &angles)) {
            SkOpAngle* last = angles.end() - 1;
            nextStart = last->start();
            nextEnd = last->end();
   #if TRY_ROTATE
            *chase.insert(0) = span;
   #else
            *chase.append() = span;
   #endif
            return last->segment();
        }
        if (done == angles.count()) {
            continue;
        }
        SkSTArray<SkOpAngle::kStackBasedCount, SkOpAngle*, true> sorted;
        bool sortable = SkOpSegment::SortAngles(angles, &sorted,
                SkOpSegment::kMayBeUnordered_SortAngleKind);
        int angleCount = sorted.count();
#if DEBUG_SORT
        sorted[0]->segment()->debugShowSort(__FUNCTION__, sorted, 0);
#endif
        if (!sortable) {
            continue;
        }
        // find first angle, initialize winding to computed fWindSum
        int firstIndex = -1;
        const SkOpAngle* angle;
        do {
            angle = sorted[++firstIndex];
            segment = angle->segment();
        } while (segment->windSum(angle) == SK_MinS32);
    #if DEBUG_SORT
        segment->debugShowSort(__FUNCTION__, sorted, firstIndex);
    #endif
        int sumMiWinding = segment->updateWindingReverse(angle);
        int sumSuWinding = segment->updateOppWindingReverse(angle);
        if (segment->operand()) {
            SkTSwap<int>(sumMiWinding, sumSuWinding);
        }
        int nextIndex = firstIndex + 1;
        int lastIndex = firstIndex != 0 ? firstIndex : angleCount;
        SkOpSegment* first = NULL;
        do {
            SkASSERT(nextIndex != firstIndex);
            if (nextIndex == angleCount) {
                nextIndex = 0;
            }
            angle = sorted[nextIndex];
            segment = angle->segment();
            int start = angle->start();
            int end = angle->end();
            int maxWinding, sumWinding, oppMaxWinding, oppSumWinding;
            segment->setUpWindings(start, end, &sumMiWinding, &sumSuWinding,
                    &maxWinding, &sumWinding, &oppMaxWinding, &oppSumWinding);
            if (!segment->done(angle)) {
                if (!first) {
                    first = segment;
                    nextStart = start;
                    nextEnd = end;
                }
                (void) segment->markAngle(maxWinding, sumWinding, oppMaxWinding,
                    oppSumWinding, true, angle);
            }
        } while (++nextIndex != lastIndex);
        if (first) {
       #if TRY_ROTATE
            *chase.insert(0) = span;
       #else
            *chase.append() = span;
       #endif
            return first;
        }
    }
    return NULL;
}

/*
static bool windingIsActive(int winding, int oppWinding, int spanWinding, int oppSpanWinding,
        bool windingIsOp, PathOp op) {
    bool active = windingIsActive(winding, spanWinding);
    if (!active) {
        return false;
    }
    if (oppSpanWinding && windingIsActive(oppWinding, oppSpanWinding)) {
        switch (op) {
            case kIntersect_Op:
            case kUnion_Op:
                return true;
            case kDifference_Op: {
                int absSpan = abs(spanWinding);
                int absOpp = abs(oppSpanWinding);
                return windingIsOp ? absSpan < absOpp : absSpan > absOpp;
            }
            case kXor_Op:
                return spanWinding != oppSpanWinding;
            default:
                SkASSERT(0);
        }
    }
    bool opActive = oppWinding != 0;
    return gOpLookup[op][opActive][windingIsOp];
}
*/

static bool bridgeOp(SkTArray<SkOpContour*, true>& contourList, const SkPathOp op,
        const int xorMask, const int xorOpMask, SkPathWriter* simple) {
    bool firstContour = true;
    bool unsortable = false;
    bool topUnsortable = false;
    SkPoint topLeft = {SK_ScalarMin, SK_ScalarMin};
    do {
        int index, endIndex;
        bool done;
        SkOpSegment* current = FindSortableTop(contourList, &firstContour, &index, &endIndex,
                &topLeft, &topUnsortable, &done, true);
        if (!current) {
            if (topUnsortable || !done) {
                topUnsortable = false;
                SkASSERT(topLeft.fX != SK_ScalarMin && topLeft.fY != SK_ScalarMin);
                topLeft.fX = topLeft.fY = SK_ScalarMin;
                continue;
            }
            break;
        }
        SkTDArray<SkOpSpan*> chaseArray;
        do {
            if (current->activeOp(index, endIndex, xorMask, xorOpMask, op)) {
                do {
                    if (!unsortable && current->done()) {
            #if DEBUG_ACTIVE_SPANS
                        DebugShowActiveSpans(contourList);
            #endif
                        if (simple->isEmpty()) {
                            simple->init();
                            break;
                        }
                    }
                    SkASSERT(unsortable || !current->done());
                    int nextStart = index;
                    int nextEnd = endIndex;
                    SkOpSegment* next = current->findNextOp(&chaseArray, &nextStart, &nextEnd,
                            &unsortable, op, xorMask, xorOpMask);
                    if (!next) {
                        if (!unsortable && simple->hasMove()
                                && current->verb() != SkPath::kLine_Verb
                                && !simple->isClosed()) {
                            current->addCurveTo(index, endIndex, simple, true);
                            SkASSERT(simple->isClosed());
                        }
                        break;
                    }
        #if DEBUG_FLOW
            SkDebugf("%s current id=%d from=(%1.9g,%1.9g) to=(%1.9g,%1.9g)\n", __FUNCTION__,
                    current->debugID(), current->xyAtT(index).fX, current->xyAtT(index).fY,
                    current->xyAtT(endIndex).fX, current->xyAtT(endIndex).fY);
        #endif
                    current->addCurveTo(index, endIndex, simple, true);
                    current = next;
                    index = nextStart;
                    endIndex = nextEnd;
                } while (!simple->isClosed() && (!unsortable
                        || !current->done(SkMin32(index, endIndex))));
                if (current->activeWinding(index, endIndex) && !simple->isClosed()) {
                    SkASSERT(unsortable || simple->isEmpty());
                    int min = SkMin32(index, endIndex);
                    if (!current->done(min)) {
                        current->addCurveTo(index, endIndex, simple, true);
                        current->markDoneBinary(min);
                    }
                }
                simple->close();
            } else {
                SkOpSpan* last = current->markAndChaseDoneBinary(index, endIndex);
                if (last && !last->fLoop) {
                    *chaseArray.append() = last;
                }
            }
            current = findChaseOp(chaseArray, index, endIndex);
        #if DEBUG_ACTIVE_SPANS
            DebugShowActiveSpans(contourList);
        #endif
            if (!current) {
                break;
            }
        } while (true);
    } while (true);
    return simple->someAssemblyRequired();
}

// pretty picture:
// https://docs.google.com/a/google.com/drawings/d/1sPV8rPfpEFXymBp3iSbDRWAycp1b-7vD9JP2V-kn9Ss/edit?usp=sharing
static const SkPathOp gOpInverse[kReverseDifference_PathOp + 1][2][2] = {
//                  inside minuend                               outside minuend
//     inside subtrahend     outside subtrahend      inside subtrahend     outside subtrahend
    {{ kDifference_PathOp,    kIntersect_PathOp }, { kUnion_PathOp, kReverseDifference_PathOp }},
    {{ kIntersect_PathOp,    kDifference_PathOp }, { kReverseDifference_PathOp, kUnion_PathOp }},
    {{ kUnion_PathOp, kReverseDifference_PathOp }, { kDifference_PathOp,    kIntersect_PathOp }},
    {{ kXOR_PathOp,                 kXOR_PathOp }, { kXOR_PathOp,                 kXOR_PathOp }},
    {{ kReverseDifference_PathOp, kUnion_PathOp }, { kIntersect_PathOp,    kDifference_PathOp }},
};

static const bool gOutInverse[kReverseDifference_PathOp + 1][2][2] = {
    {{ false, false }, { true, false }},  // diff
    {{ false, false }, { false, true }},  // sect
    {{ false, true }, { true, true }},    // union
    {{ false, true }, { true, false }},   // xor
    {{ false, true }, { false, false }},  // rev diff
};

bool Op(const SkPath& one, const SkPath& two, SkPathOp op, SkPath* result) {
#if DEBUG_SHOW_PATH
    ShowFunctionHeader();
    ShowPath(one, "path");
    ShowPath(two, "pathB");
    ShowOp(op, "path", "pathB");
#endif
    op = gOpInverse[op][one.isInverseFillType()][two.isInverseFillType()];
    SkPath::FillType fillType = gOutInverse[op][one.isInverseFillType()][two.isInverseFillType()]
            ? SkPath::kInverseEvenOdd_FillType : SkPath::kEvenOdd_FillType;
    const SkPath* minuend = &one;
    const SkPath* subtrahend = &two;
    if (op == kReverseDifference_PathOp) {
        minuend = &two;
        subtrahend = &one;
        op = kDifference_PathOp;
    }
#if DEBUG_SORT || DEBUG_SWAP_TOP
    gDebugSortCount = gDebugSortCountDefault;
#endif
    // turn path into list of segments
    SkTArray<SkOpContour> contours;
    // FIXME: add self-intersecting cubics' T values to segment
    SkOpEdgeBuilder builder(*minuend, contours);
    const int xorMask = builder.xorMask();
    builder.addOperand(*subtrahend);
    if (!builder.finish()) {
        return false;
    }
    result->reset();
    result->setFillType(fillType);
    const int xorOpMask = builder.xorMask();
    SkTArray<SkOpContour*, true> contourList;
    MakeContourList(contours, contourList, xorMask == kEvenOdd_PathOpsMask,
            xorOpMask == kEvenOdd_PathOpsMask);
    SkOpContour** currentPtr = contourList.begin();
    if (!currentPtr) {
        return true;
    }
    SkOpContour** listEnd = contourList.end();
    // find all intersections between segments
    do {
        SkOpContour** nextPtr = currentPtr;
        SkOpContour* current = *currentPtr++;
        if (current->containsCubics()) {
            AddSelfIntersectTs(current);
        }
        SkOpContour* next;
        do {
            next = *nextPtr++;
        } while (AddIntersectTs(current, next) && nextPtr != listEnd);
    } while (currentPtr != listEnd);
    // eat through coincident edges

    int total = 0;
    int index;
    for (index = 0; index < contourList.count(); ++index) {
        total += contourList[index]->segments().count();
    }
#if DEBUG_SHOW_WINDING
    SkOpContour::debugShowWindingValues(contourList);
#endif
    CoincidenceCheck(&contourList, total);
#if DEBUG_SHOW_WINDING
    SkOpContour::debugShowWindingValues(contourList);
#endif
    FixOtherTIndex(&contourList);
    SortSegments(&contourList);
#if DEBUG_ACTIVE_SPANS || DEBUG_ACTIVE_SPANS_FIRST_ONLY
    DebugShowActiveSpans(contourList);
#endif
    // construct closed contours
    SkPathWriter wrapper(*result);
    bridgeOp(contourList, op, xorMask, xorOpMask, &wrapper);
    {  // if some edges could not be resolved, assemble remaining fragments
        SkPath temp;
        temp.setFillType(fillType);
        SkPathWriter assembled(temp);
        Assemble(wrapper, &assembled);
        *result = *assembled.nativePath();
        result->setFillType(fillType);
    }
    return true;
}
