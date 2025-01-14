// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

'use strict';

/**
 * @fileoverview Interactive visualizaiton of TraceModel objects
 * based loosely on gantt charts. Each thread in the TraceModel is given a
 * set of Tracks, one per subrow in the thread. The TimelineTrackView class
 * acts as a controller, creating the individual tracks, while Tracks
 * do actual drawing.
 *
 * Visually, the TimelineTrackView produces (prettier) visualizations like the
 * following:
 *    Thread1:  AAAAAAAAAA         AAAAA
 *                  BBBB              BB
 *    Thread2:     CCCCCC                 CCCCC
 *
 */
base.requireStylesheet('tracing.timeline_track_view');
base.require('base.event_target');
base.require('tracing.filter');
base.require('tracing.selection');
base.require('tracing.timeline_viewport');
base.require('tracing.tracks.trace_model_track');
base.require('tracing.tracks.ruler_track');
base.require('ui');

base.exportTo('tracing', function() {

  var Selection = tracing.Selection;
  var Viewport = tracing.TimelineViewport;

  function intersectRect_(r1, r2) {
    var results = new Object;
    if (r2.left > r1.right || r2.right < r1.left ||
        r2.top > r1.bottom || r2.bottom < r1.top) {
      return false;
    }
    results.left = Math.max(r1.left, r2.left);
    results.top = Math.max(r1.top, r2.top);
    results.right = Math.min(r1.right, r2.right);
    results.bottom = Math.min(r1.bottom, r2.bottom);
    results.width = (results.right - results.left);
    results.height = (results.bottom - results.top);
    return results;
  }

  /**
   * Renders a TraceModel into a div element, making one
   * Track for each subrow in each thread of the model, managing
   * overall track layout, and handling user interaction with the
   * viewport.
   *
   * @constructor
   * @extends {HTMLDivElement}
   */
  var TimelineTrackView = ui.define('div');

  TimelineTrackView.prototype = {
    __proto__: HTMLDivElement.prototype,

    model_: null,

    decorate: function() {
      this.classList.add('timeline-track-view');

      this.categoryFilter_ = new tracing.CategoryFilter();

      this.viewport_ = new Viewport(this);

      // Add the viewport track.
      this.rulerTrack_ = new tracing.tracks.RulerTrack();
      this.rulerTrack_.viewport = this.viewport_;
      this.appendChild(this.rulerTrack_);

      this.modelTrackContainer_ = document.createElement('div');
      this.modelTrackContainer_.className = 'model-track-container';
      this.appendChild(this.modelTrackContainer_);

      this.modelTrack_ = new tracing.tracks.TraceModelTrack();
      this.modelTrackContainer_.appendChild(this.modelTrack_);

      this.dragBox_ = this.ownerDocument.createElement('div');
      this.dragBox_.className = 'drag-box';
      this.appendChild(this.dragBox_);
      this.hideDragBox_();

      this.bindEventListener_(document, 'keypress', this.onKeypress_, this);
      this.bindEventListener_(document, 'keydown', this.onKeydown_, this);
      this.bindEventListener_(document, 'keyup', this.onKeyup_, this);
      this.bindEventListener_(document, 'mousemove', this.onMouseMove_, this);
      this.bindEventListener_(document, 'mouseup', this.onMouseUp_, this);

      this.addEventListener('mousewheel', this.onMouseWheel_);
      this.addEventListener('mousedown', this.onMouseDown_);
      this.addEventListener('dblclick', this.onDblClick_);

      this.lastMouseViewPos_ = {x: 0, y: 0};
      this.maxHeadingWidth_ = 0;

      this.selection_ = new Selection();
    },

    /**
     * Wraps the standard addEventListener but automatically binds the provided
     * func to the provided target, tracking the resulting closure. When detach
     * is called, these listeners will be automatically removed.
     */
    bindEventListener_: function(object, event, func, target) {
      if (!this.boundListeners_)
        this.boundListeners_ = [];
      var boundFunc = func.bind(target);
      this.boundListeners_.push({object: object,
        event: event,
        boundFunc: boundFunc});
      object.addEventListener(event, boundFunc);
    },

    detach: function() {
      this.modelTrack_.detach();

      for (var i = 0; i < this.boundListeners_.length; i++) {
        var binding = this.boundListeners_[i];
        binding.object.removeEventListener(binding.event, binding.boundFunc);
      }
      this.boundListeners_ = undefined;
      this.viewport_.detach();
    },

    get viewport() {
      return this.viewport_;
    },

    get categoryFilter() {
      return this.categoryFilter_;
    },

    set categoryFilter(filter) {
      this.categoryFilter_ = filter;
      this.modelTrack_.categoryFilter = filter;
    },

    get model() {
      return this.model_;
    },

    set model(model) {
      if (!model)
        throw new Error('Model cannot be null');

      var modelInstanceChanged = this.model_ != model;
      this.model_ = model;
      this.modelTrack_.model = model;
      this.modelTrack_.viewport = this.viewport_;
      this.modelTrack_.categoryFilter = this.categoryFilter;
      this.rulerTrack_.headingWidth = this.modelTrack_.headingWidth;

      // Set up a reasonable viewport.
      if (modelInstanceChanged)
        this.viewport_.setWhenPossible(this.setInitialViewport_.bind(this));
    },

    get numVisibleTracks() {
      return this.modelTrack_.numVisibleTracks;
    },

    setInitialViewport_: function() {
      var w = this.firstCanvas.width;

      var min;
      var range;

      if (this.model_.bounds.isEmpty) {
        min = 0;
        range = 1000;
      } else if (this.model_.bounds.range == 0) {
        min = this.model_.bounds.min;
        range = 1000;
      } else {
        min = this.model_.bounds.min;
        range = this.model_.bounds.range;
      }
      var boost = range * 0.15;
      this.viewport_.xSetWorldBounds(min - boost,
                                     min + range + boost,
                                     w);
    },

    /**
     * @param {Filter} filter The filter to use for finding matches.
     * @param {Selection} selection The selection to add matches to.
     * @return {Array} An array of objects that match the provided
     * TitleFilter.
     */
    addAllObjectsMatchingFilterToSelection: function(filter, selection) {
      this.modelTrack_.addAllObjectsMatchingFilterToSelection(filter,
                                                              selection);
    },

    /**
     * @return {Element} The element whose focused state determines
     * whether to respond to keyboard inputs.
     * Defaults to the parent element.
     */
    get focusElement() {
      if (this.focusElement_)
        return this.focusElement_;
      return this.parentElement;
    },

    /**
     * Sets the element whose focus state will determine whether
     * to respond to keybaord input.
     */
    set focusElement(value) {
      this.focusElement_ = value;
    },

    get listenToKeys_() {
      if (!this.viewport_.isAttachedToDocument_)
        return false;
      if (this.activeElement instanceof tracing.FindControl)
        return false;
      if (!this.focusElement_)
        return true;
      if (this.focusElement.tabIndex >= 0) {
        if (document.activeElement == this.focusElement)
          return true;
        return ui.elementIsChildOf(document.activeElement, this.focusElement);
      }
      return true;
    },

    onKeypress_: function(e) {
      var vp = this.viewport_;
      if (!this.firstCanvas)
        return;
      if (!this.listenToKeys_)
        return;
      if (document.activeElement.nodeName == 'INPUT')
        return;
      var viewWidth = this.firstCanvas.clientWidth;
      var curMouseV, curCenterW;
      switch (e.keyCode) {
        case 119:  // w
        case 44:   // ,
          this.zoomBy_(1.5);
          break;
        case 115:  // s
        case 111:  // o
          this.zoomBy_(1 / 1.5);
          break;
        case 103:  // g
          this.onGridToggle_(true);
          break;
        case 71:  // G
          this.onGridToggle_(false);
          break;
        case 87:  // W
        case 60:  // <
          this.zoomBy_(10);
          break;
        case 83:  // S
        case 79:  // O
          this.zoomBy_(1 / 10);
          break;
        case 97:  // a
          vp.panX += vp.xViewVectorToWorld(viewWidth * 0.1);
          break;
        case 100:  // d
        case 101:  // e
          vp.panX -= vp.xViewVectorToWorld(viewWidth * 0.1);
          break;
        case 65:  // A
          vp.panX += vp.xViewVectorToWorld(viewWidth * 0.5);
          break;
        case 68:  // D
          vp.panX -= vp.xViewVectorToWorld(viewWidth * 0.5);
          break;
        case 48:  // 0
        case 122: // z
          this.setInitialViewport_();
          break;
        case 102:  // f
          this.zoomToSelection_();
          break;
      }
    },

    onMouseWheel_: function(e) {
      if (e.altKey) {
        var delta = e.wheelDeltaY / 120;
        var zoomScale = Math.pow(1.5, delta);
        this.zoomBy_(zoomScale);
        e.preventDefault();
      }
    },

    // Not all keys send a keypress.
    onKeydown_: function(e) {
      if (!this.listenToKeys_)
        return;
      var sel;
      var vp = this.viewport_;
      var viewWidth = this.firstCanvas.clientWidth;
      switch (e.keyCode) {
        case 37:   // left arrow
          sel = this.selection.getShiftedSelection(-1);
          if (sel) {
            this.setSelectionAndMakeVisible(sel);
            e.preventDefault();
          } else {
            if (!this.firstCanvas)
              return;
            vp.panX += vp.xViewVectorToWorld(viewWidth * 0.1);
          }
          break;
        case 39:   // right arrow
          sel = this.selection.getShiftedSelection(1);
          if (sel) {
            this.setSelectionAndMakeVisible(sel);
            e.preventDefault();
          } else {
            if (!this.firstCanvas)
              return;
            vp.panX -= vp.xViewVectorToWorld(viewWidth * 0.1);
          }
          break;
        case 9:    // TAB
          if (this.focusElement.tabIndex == -1) {
            if (e.shiftKey)
              this.selectPrevious_(e);
            else
              this.selectNext_(e);
            e.preventDefault();
          }
          break;
      }
      if (e.shiftKey && this.dragBeginEvent_) {
        var vertical = e.shiftKey;
        if (this.dragBeginEvent_) {
          this.setDragBoxPosition_(this.dragBoxXStart_, this.dragBoxYStart_,
              this.dragBoxXEnd_, this.dragBoxYEnd_, vertical);
        }
      }
    },

    onKeyup_: function(e) {
      if (!this.listenToKeys_)
        return;
      if (!e.shiftKey) {
        if (this.dragBeginEvent_) {
          var vertical = e.shiftKey;
          this.setDragBoxPosition_(this.dragBoxXStart_, this.dragBoxYStart_,
              this.dragBoxXEnd_, this.dragBoxYEnd_, vertical);
        }
      }
    },

    /**
     * Zoom in or out on the timeline by the given scale factor.
     * @param {integer} scale The scale factor to apply.  If <1, zooms out.
     */
    zoomBy_: function(scale) {
      if (!this.firstCanvas)
        return;
      var vp = this.viewport_;
      var viewWidth = this.firstCanvas.clientWidth;
      var pixelRatio = window.devicePixelRatio || 1;
      var curMouseV = this.lastMouseViewPos_.x * pixelRatio;
      var curCenterW = vp.xViewToWorld(curMouseV);
      vp.scaleX = vp.scaleX * scale;
      vp.xPanWorldPosToViewPos(curCenterW, curMouseV, viewWidth);
    },

    /**
     * Zoom into the current selection.
     */
    zoomToSelection_: function() {
      if (!this.selection)
        return;
      var bounds = this.selection.bounds;
      var worldCenter = bounds.min + (bounds.max - bounds.min) * 0.5;
      var worldBounds = (bounds.max - bounds.min) * 0.5;
      var boost = worldBounds * 0.15;
      this.viewport_.xSetWorldBounds(worldCenter - worldBounds - boost,
                                    worldCenter + worldBounds + boost,
                                    this.firstCanvas.width);
    },

    get keyHelp() {
      var mod = navigator.platform.indexOf('Mac') == 0 ? 'cmd' : 'ctrl';
      var help = 'Qwerty Controls\n' +
          ' w/s           : Zoom in/out    (with shift: go faster)\n' +
          ' a/d           : Pan left/right\n\n' +
          'Dvorak Controls\n' +
          ' ,/o           : Zoom in/out     (with shift: go faster)\n' +
          ' a/e           : Pan left/right\n\n' +
          'Mouse Controls\n' +
          ' drag          : Select slices   (with ' + mod +
                                                        ': zoom to slices)\n' +
          ' drag + shift  : Select all slices vertically\n\n';

      if (this.focusElement.tabIndex) {
        help +=
            ' <-            : Select previous event on current timeline\n' +
            ' ->            : Select next event on current timeline\n';
      } else {
        help += 'General Navigation\n' +
            ' g/General     : Shows grid at the start/end of the selected' +
            ' task\n' +
            ' <-,^TAB       : Select previous event on current timeline\n' +
            ' ->, TAB       : Select next event on current timeline\n';
      }
      help +=
          '\n' +
          'Alt + Scroll to zoom in/out\n' +
          'Dbl-click to zoom in; Shift dbl-click to zoom out\n' +
          'f to zoom into selection\n' +
          'z to reset zoom and pan to initial view\n';
      return help;
    },

    get selection() {
      return this.selection_;
    },

    set selection(selection) {
      if (!(selection instanceof Selection))
        throw new Error('Expected Selection');

      // Clear old selection.
      var i;
      for (i = 0; i < this.selection_.length; i++)
        this.selection_[i].selected = false;

      this.selection_ = selection;

      base.dispatchSimpleEvent(this, 'selectionChange');
      for (i = 0; i < this.selection_.length; i++)
        this.selection_[i].selected = true;
      if (this.selection_.length &&
          this.selection_[0].track)
        this.selection_[0].track.scrollIntoViewIfNeeded();
      this.viewport_.dispatchChangeEvent(); // Triggers a redraw.
    },

    setSelectionAndMakeVisible: function(selection, zoomAllowed) {
      if (!(selection instanceof Selection))
        throw new Error('Expected Selection');
      this.selection = selection;
      var bounds = this.selection.bounds;
      var size = this.viewport_.xWorldVectorToView(bounds.max - bounds.min);
      if (zoomAllowed && size < 50) {
        var worldCenter = bounds.min + (bounds.max - bounds.min) * 0.5;
        var worldBounds = (bounds.max - bounds.min) * 5;
        this.viewport_.xSetWorldBounds(worldCenter - worldBounds * 0.5,
                                      worldCenter + worldBounds * 0.5,
                                      this.firstCanvas.width);
        return;
      }

      this.viewport_.xPanWorldBoundsIntoView(bounds.min, bounds.max,
          this.firstCanvas.width);
    },

    get firstCanvas() {
      if (this.rulerTrack_)
        return this.rulerTrack_.firstCanvas;
      if (this.modelTrack_)
        return this.modelTrack_.firstCanvas;
      return undefined;
    },

    hideDragBox_: function() {
      this.dragBox_.style.left = '-1000px';
      this.dragBox_.style.top = '-1000px';
      this.dragBox_.style.width = 0;
      this.dragBox_.style.height = 0;
    },

    setDragBoxPosition_: function(xStart, yStart, xEnd, yEnd, vertical) {
      var loY;
      var hiY;
      var loX = Math.min(xStart, xEnd);
      var hiX = Math.max(xStart, xEnd);
      var modelTrackRect = this.modelTrack_.getBoundingClientRect();

      if (vertical) {
        loY = modelTrackRect.top;
        hiY = modelTrackRect.bottom;
      } else {
        loY = Math.min(yStart, yEnd);
        hiY = Math.max(yStart, yEnd);
      }

      var dragRect = {left: loX, top: loY, width: hiX - loX, height: hiY - loY};
      dragRect.right = dragRect.left + dragRect.width;
      dragRect.bottom = dragRect.top + dragRect.height;
      var modelTrackContainerRect =
          this.modelTrackContainer_.getBoundingClientRect();
      var clipRect = {
        left: modelTrackContainerRect.left,
        top: modelTrackContainerRect.top,
        right: modelTrackContainerRect.right,
        bottom: modelTrackContainerRect.bottom
      };
      var trackTitleWidth = parseInt(this.modelTrack_.headingWidth);
      clipRect.left = clipRect.left + trackTitleWidth;

      var finalDragBox = intersectRect_(clipRect, dragRect);

      this.dragBox_.style.left = finalDragBox.left + 'px';
      this.dragBox_.style.width = finalDragBox.width + 'px';
      this.dragBox_.style.top = finalDragBox.top + 'px';
      this.dragBox_.style.height = finalDragBox.height + 'px';

      var pixelRatio = window.devicePixelRatio || 1;
      var canv = this.firstCanvas;
      var loWX = this.viewport_.xViewToWorld(
          (loX - canv.offsetLeft) * pixelRatio);
      var hiWX = this.viewport_.xViewToWorld(
          (hiX - canv.offsetLeft) * pixelRatio);

      var roundedDuration = Math.round((hiWX - loWX) * 100) / 100;
      this.dragBox_.textContent = roundedDuration + 'ms';

      var e = new base.Event('selectionChanging');
      e.loWX = loWX;
      e.hiWX = hiWX;
      this.dispatchEvent(e);
    },

    onGridToggle_: function(left) {
      var tb = left ? this.selection_.bounds.min : this.selection_.bounds.max;

      // Toggle the grid off if the grid is on, the marker position is the same
      // and the same element is selected (same timebase).
      if (this.viewport_.gridEnabled &&
          this.viewport_.gridSide === left &&
          this.viewport_.gridTimebase === tb) {
        this.viewport_.gridside = undefined;
        this.viewport_.gridEnabled = false;
        this.viewport_.gridTimebase = undefined;
        return;
      }

      // Shift the timebase left until its just left of model_.bounds.min.
      var numInterfvalsSinceStart = Math.ceil((tb - this.model_.bounds.min) /
          this.viewport_.gridStep_);
      this.viewport_.gridTimebase = tb -
          (numInterfvalsSinceStart + 1) * this.viewport_.gridStep_;

      this.viewport_.gridEnabled = true;
      this.viewport_.gridSide = left;
      this.viewport_.gridTimebase = tb;
    },

    onMouseDown_: function(e) {
      if (e.button !== 0)
        return;

      if (e.shiftKey) {
        this.rulerTrack_.placeAndBeginDraggingMarker(e.clientX);
        return;
      }

      var canv = this.firstCanvas;
      var rect = this.modelTrack_.getBoundingClientRect();
      var canvRect = this.firstCanvas.getBoundingClientRect();

      var inside = rect &&
          e.clientX >= rect.left &&
          e.clientX < rect.right &&
          e.clientY >= rect.top &&
          e.clientY < rect.bottom &&
          e.clientX >= canvRect.left &&
          e.clientX < canvRect.right;

      if (!inside)
        return;

      var pos = {
        x: e.clientX - canv.offsetLeft,
        y: e.clientY - canv.offsetTop
      };

      var wX = this.viewport_.xViewToWorld(pos.x);

      this.dragBeginEvent_ = e;
      e.preventDefault();
      if (document.activeElement)
        document.activeElement.blur();
      if (this.focusElement.tabIndex >= 0)
        this.focusElement.focus();
    },

    onMouseMove_: function(e) {
      if (!this.firstCanvas)
        return;
      var canv = this.firstCanvas;
      var pos = {
        x: e.clientX - canv.offsetLeft,
        y: e.clientY - canv.offsetTop
      };

      // Remember position. Used during keyboard zooming.
      this.lastMouseViewPos_ = pos;

      // Update the drag box
      if (this.dragBeginEvent_) {
        this.dragBoxXStart_ = this.dragBeginEvent_.clientX;
        this.dragBoxXEnd_ = e.clientX;
        this.dragBoxYStart_ = this.dragBeginEvent_.clientY;
        this.dragBoxYEnd_ = e.clientY;
        var vertical = e.shiftKey;
        this.setDragBoxPosition_(this.dragBoxXStart_, this.dragBoxYStart_,
            this.dragBoxXEnd_, this.dragBoxYEnd_, vertical);
      }
    },

    onMouseUp_: function(e) {
      var i;
      if (this.dragBeginEvent_) {
        // Stop the dragging.
        this.hideDragBox_();
        var eDown = this.dragBeginEvent_;
        this.dragBeginEvent_ = null;

        // Figure out extents of the drag.
        var loY;
        var hiY;
        var loX = Math.min(eDown.clientX, e.clientX);
        var hiX = Math.max(eDown.clientX, e.clientX);
        var tracksContainer = this.modelTrackContainer_.getBoundingClientRect();
        var topBoundary = tracksContainer.height;
        var vertical = e.shiftKey;
        if (vertical) {
          var modelTrackRect = this.modelTrack_.getBoundingClientRect();
          loY = modelTrackRect.top;
          hiY = modelTrackRect.bottom;
        } else {
          loY = Math.min(eDown.clientY, e.clientY);
          hiY = Math.max(eDown.clientY, e.clientY);
        }

        // Convert to worldspace.
        var canv = this.firstCanvas;
        var loVX = loX - canv.offsetLeft;
        var hiVX = hiX - canv.offsetLeft;

        // Figure out what has been hit.
        var selection = new Selection();
        this.modelTrack_.addIntersectingItemsInRangeToSelection(
            loVX, hiVX, loY, hiY, selection);

        // Activate the new selection, and zoom if ctrl key held down.
        this.selection = selection;
        if ((base.isMac && e.metaKey) || (!base.isMac && e.ctrlKey)) {
          this.zoomToSelection_();
        }
      }
    },

    onDblClick_: function(e) {
      var modelTrackContainerRect =
          this.modelTrackContainer_.getBoundingClientRect();
      var clipBounds = {
        left: modelTrackContainerRect.left,
        right: modelTrackContainerRect.right
      };
      var trackTitleWidth = parseInt(this.modelTrack_.headingWidth);
      clipBounds.left = clipBounds.left + trackTitleWidth;

      if (e.clientX < clipBounds.left || e.clientX > clipBounds.right)
        return;

      var canv = this.firstCanvas;

      var scale = 4;
      if (e.shiftKey)
        scale = 1 / scale;
      this.zoomBy_(scale);
      e.preventDefault();
    }
  };

  /**
   * The Model being viewed by the timeline
   * @type {Model}
   */
  base.defineProperty(TimelineTrackView, 'model', base.PropertyKind.JS);

  return {
    TimelineTrackView: TimelineTrackView
  };
});
