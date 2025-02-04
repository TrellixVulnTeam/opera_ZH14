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

static bool bridgeWinding(SkTArray<SkOpContour*, true>& contourList, SkPathWriter* simple) {
    bool firstContour = true;
    bool unsortable = false;
    bool topUnsortable = false;
    SkPoint topLeft = {SK_ScalarMin, SK_ScalarMin};
    do {
        int index, endIndex;
        bool topDone;
        SkOpSegment* current = FindSortableTop(contourList, &firstContour, &index, &endIndex,
                &topLeft, &topUnsortable, &topDone, false);
        if (!current) {
            if (topUnsortable || !topDone) {
                topUnsortable = false;
                SkASSERT(topLeft.fX != SK_ScalarMin && topLeft.fY != SK_ScalarMin);
                topLeft.fX = topLeft.fY = SK_ScalarMin;
                continue;
            }
            break;
        }
        SkTDArray<SkOpSpan*> chaseArray;
        do {
            if (current->activeWinding(index, endIndex)) {
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
                    SkOpSegment* next = current->findNextWinding(&chaseArray, &nextStart, &nextEnd,
                            &unsortable);
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
                        current->markDoneUnary(min);
                    }
                }
                simple->close();
            } else {
                SkOpSpan* last = current->markAndChaseDoneUnary(index, endIndex);
                if (last && !last->fLoop) {
                    *chaseArray.append() = last;
                }
            }
            current = FindChase(chaseArray, index, endIndex);
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

// returns true if all edges were processed
static bool bridgeXor(SkTArray<SkOpContour*, true>& contourList, SkPathWriter* simple) {
    SkOpSegment* current;
    int start, end;
    bool unsortable = false;
    bool closable = true;
    while ((current = FindUndone(contourList, &start, &end))) {
        do {
    #if DEBUG_ACTIVE_SPANS
            if (!unsortable && current->done()) {
                DebugShowActiveSpans(contourList);
            }
    #endif
            SkASSERT(unsortable || !current->done());
            int nextStart = start;
            int nextEnd = end;
            SkOpSegment* next = current->findNextXor(&nextStart, &nextEnd, &unsortable);
            if (!next) {
                if (!unsortable && simple->hasMove()
                        && current->verb() != SkPath::kLine_Verb
                        && !simple->isClosed()) {
                    current->addCurveTo(start, end, simple, true);
                    SkASSERT(simple->isClosed());
                }
                break;
            }
        #if DEBUG_FLOW
            SkDebugf("%s current id=%d from=(%1.9g,%1.9g) to=(%1.9g,%1.9g)\n", __FUNCTION__,
                    current->debugID(), current->xyAtT(start).fX, current->xyAtT(start).fY,
                    current->xyAtT(end).fX, current->xyAtT(end).fY);
        #endif
            current->addCurveTo(start, end, simple, true);
            current = next;
            start = nextStart;
            end = nextEnd;
        } while (!simple->isClosed() && (!unsortable || !current->done(SkMin32(start, end))));
        if (!simple->isClosed()) {
            SkASSERT(unsortable);
            int min = SkMin32(start, end);
            if (!current->done(min)) {
                current->addCurveTo(start, end, simple, true);
                current->markDone(min, 1);
            }
            closable = false;
        }
        simple->close();
    #if DEBUG_ACTIVE_SPANS
        DebugShowActiveSpans(contourList);
    #endif
    }
    return closable;
}

// FIXME : add this as a member of SkPath
bool Simplify(const SkPath& path, SkPath* result) {
#if DEBUG_SORT || DEBUG_SWAP_TOP
    gDebugSortCount = gDebugSortCountDefault;
#endif
    // returns 1 for evenodd, -1 for winding, regardless of inverse-ness
    SkPath::FillType fillType = path.isInverseFillType() ? SkPath::kInverseEvenOdd_FillType
            : SkPath::kEvenOdd_FillType;

    // turn path into list of segments
    SkTArray<SkOpContour> contours;
    SkOpEdgeBuilder builder(path, contours);
    if (!builder.finish()) {
        return false;
    }
    SkTArray<SkOpContour*, true> contourList;
    MakeContourList(contours, contourList, false, false);
    SkOpContour** currentPtr = contourList.begin();
    result->setFillType(fillType);
    result->reset();
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
    CoincidenceCheck(&contourList, 0);
    FixOtherTIndex(&contourList);
    SortSegments(&contourList);
#if DEBUG_ACTIVE_SPANS || DEBUG_ACTIVE_SPANS_FIRST_ONLY
    DebugShowActiveSpans(contourList);
#endif
    // construct closed contours
    SkPathWriter simple(*result);
    if (builder.xorMask() == kWinding_PathOpsMask ? bridgeWinding(contourList, &simple)
                : !bridgeXor(contourList, &simple))
    {  // if some edges could not be resolved, assemble remaining fragments
        SkPath temp;
        temp.setFillType(fillType);
        SkPathWriter assembled(temp);
        Assemble(simple, &assembled);
        *result = *assembled.nativePath();
        result->setFillType(fillType);
    }
    return true;
}
