// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// A struct for managing webkit's settings.
//
// Adding new values to this class probably involves updating
// WebKit::WebSettings, content/common/view_messages.h, browser/tab_contents/
// render_view_host_delegate_helper.cc, and browser/profiles/profile.cc.

#ifndef WEBKIT_COMMON_WEBPREFERENCES_H__
#define WEBKIT_COMMON_WEBPREFERENCES_H__

#include <map>
#include <string>
#include <vector>

#include "base/strings/string16.h"
#include "googleurl/src/gurl.h"
#include "webkit/common/webkit_common_export.h"

namespace WebKit {
class WebView;
typedef unsigned WebColor;
}

struct WebPreferences;

namespace webkit_glue {

// Map of ISO 15924 four-letter script code to font family.  For example,
// "Arab" to "My Arabic Font".
typedef std::map<std::string, base::string16> ScriptFontFamilyMap;

typedef std::vector<std::pair<std::string, std::string> >
    WebInspectorPreferences;

enum EditingBehavior {
  EDITING_BEHAVIOR_MAC,
  EDITING_BEHAVIOR_WIN,
  EDITING_BEHAVIOR_UNIX,
  EDITING_BEHAVIOR_ANDROID
};

enum TextLayoutStrategy {
  TEXT_LAYOUT_NORMAL,
  TEXT_LAYOUT_AUTOSIZING,
  TEXT_LAYOUT_WRAPPING,
};


// The ISO 15924 script code for undetermined script aka Common. It's the
// default used on WebKit's side to get/set a font setting when no script is
// specified.
WEBKIT_COMMON_EXPORT extern const char kCommonScript[];

}  // namespace webkit_glue

struct WEBKIT_COMMON_EXPORT WebPreferences {

  webkit_glue::ScriptFontFamilyMap standard_font_family_map;
  webkit_glue::ScriptFontFamilyMap fixed_font_family_map;
  webkit_glue::ScriptFontFamilyMap serif_font_family_map;
  webkit_glue::ScriptFontFamilyMap sans_serif_font_family_map;
  webkit_glue::ScriptFontFamilyMap cursive_font_family_map;
  webkit_glue::ScriptFontFamilyMap fantasy_font_family_map;
  webkit_glue::ScriptFontFamilyMap pictograph_font_family_map;

  int default_font_size;
  int default_fixed_font_size;
  int minimum_font_size;
  int minimum_logical_font_size;
  std::string default_encoding;
  bool context_menu_on_mouse_up;
  bool javascript_enabled;
  bool web_security_enabled;
  bool javascript_can_open_windows_automatically;
  bool loads_images_automatically;
  bool images_enabled;
  bool plugins_enabled;
  bool dom_paste_enabled;
  webkit_glue::WebInspectorPreferences inspector_settings;
  bool site_specific_quirks_enabled;
  bool shrinks_standalone_images_to_fit;
  bool uses_universal_detector;
  bool text_areas_are_resizable;
  bool java_enabled;
  bool allow_scripts_to_close_windows;
  bool remote_fonts_enabled;
  bool javascript_can_access_clipboard;
  bool xss_auditor_enabled;
  // We don't use dns_prefetching_enabled to disable DNS prefetching.  Instead,
  // we disable the feature at a lower layer so that we catch non-WebKit uses
  // of DNS prefetch as well.
  bool dns_prefetching_enabled;
  bool local_storage_enabled;
  bool databases_enabled;
  bool application_cache_enabled;
  bool tabs_to_links;
  bool caret_browsing_enabled;
  bool hyperlink_auditing_enabled;
  bool is_online;
  bool user_style_sheet_enabled;
  GURL user_style_sheet_location;
  bool author_and_user_styles_enabled;
  bool allow_universal_access_from_file_urls;
  bool allow_file_access_from_file_urls;
  bool webaudio_enabled;
  bool experimental_webgl_enabled;
  bool flash_3d_enabled;
  bool flash_stage3d_enabled;
  bool flash_stage3d_baseline_enabled;
  bool gl_multisampling_enabled;
  bool privileged_webgl_extensions_enabled;
  bool webgl_errors_to_console_enabled;
  bool accelerated_compositing_for_overflow_scroll_enabled;
  bool accelerated_compositing_for_scrollable_frames_enabled;
  bool composited_scrolling_for_frames_enabled;
  bool mock_scrollbars_enabled;
  bool threaded_html_parser;
  bool show_paint_rects;
  bool asynchronous_spell_checking_enabled;
  bool unified_textchecker_enabled;
  bool accelerated_compositing_enabled;
  bool force_compositing_mode;
  bool accelerated_compositing_for_3d_transforms_enabled;
  bool accelerated_compositing_for_animation_enabled;
  bool accelerated_compositing_for_video_enabled;
  bool accelerated_2d_canvas_enabled;
  int minimum_accelerated_2d_canvas_size;
  bool antialiased_2d_canvas_disabled;
  bool accelerated_filters_enabled;
  bool gesture_tap_highlight_enabled;
  bool accelerated_compositing_for_plugins_enabled;
  bool memory_info_enabled;
  bool fullscreen_enabled;
  bool allow_displaying_insecure_content;
  bool allow_running_insecure_content;
  bool password_echo_enabled;
  bool should_print_backgrounds;
  bool enable_scroll_animator;
  bool visual_word_movement_enabled;
  bool css_sticky_position_enabled;
  bool css_shaders_enabled;
  bool css_variables_enabled;
  bool css_grid_layout_enabled;
  bool lazy_layout_enabled;
  bool region_based_columns_enabled;
  bool media_blocking_enabled;
  bool touch_enabled;
  bool device_supports_touch;
  bool device_supports_mouse;
  bool touch_adjustment_enabled;
  bool fixed_position_creates_stacking_context;
  bool sync_xhr_in_documents_enabled;
  bool deferred_image_decoding_enabled;
  bool should_respect_image_orientation;
  int number_of_cpu_cores;
  webkit_glue::EditingBehavior editing_behavior;
  bool supports_multiple_windows;
  WebKit::WebColor default_background_color;
  WebKit::WebColor default_text_color;
  bool viewport_enabled;
  bool initialize_at_minimum_page_scale;
  bool smart_insert_delete_enabled;
  bool spatial_navigation_enabled;
  bool experimental_websocket_enabled;
  bool pinch_virtual_viewport_enabled;
  int pinch_overlay_scrollbar_thickness;

  // This flags corresponds to a Page's Settings' setCookieEnabled state. It
  // only controls whether or not the "document.cookie" field is properly
  // connected to the backing store, for instance if you wanted to be able to
  // define custom getters and setters from within a unique security content
  // without raising a DOM security exception.
  bool cookie_enabled;

  webkit_glue::TextLayoutStrategy text_layout_strategy;
#if defined(OS_ANDROID)
  float font_scale_factor;
  bool force_enable_zoom;
  bool double_tap_to_zoom_enabled;
  bool user_gesture_required_for_media_playback;
  GURL default_video_poster_url;
  bool support_deprecated_target_density_dpi;
  bool use_wide_viewport;
#endif

  // We try to keep the default values the same as the default values in
  // chrome, except for the cases where it would require lots of extra work for
  // the embedder to use the same default value.
  WebPreferences();
  ~WebPreferences();
};

#endif  // WEBKIT_COMMON_WEBPREFERENCES_H__
