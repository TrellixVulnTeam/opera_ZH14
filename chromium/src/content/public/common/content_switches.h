// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Defines all the "content" command-line switches.

#ifndef CONTENT_PUBLIC_COMMON_CONTENT_SWITCHES_H_
#define CONTENT_PUBLIC_COMMON_CONTENT_SWITCHES_H_

#include "build/build_config.h"
#include "content/common/content_export.h"

namespace switches {

CONTENT_EXPORT extern const char kAllowFileAccessFromFiles[];
extern const char kAllowSandboxDebugging[];
CONTENT_EXPORT extern const char kAllowWebUICompositing[];
extern const char kAuditHandles[];
extern const char kAuditAllHandles[];
CONTENT_EXPORT extern const char kBrowserAssertTest[];
CONTENT_EXPORT extern const char kBrowserCrashTest[];
CONTENT_EXPORT extern const char kBrowserSubprocessPath[];
CONTENT_EXPORT extern const char kDisable2dCanvasAntialiasing[];
CONTENT_EXPORT extern const char kEnableExperimentalCanvasFeatures[];
CONTENT_EXPORT extern const char kDisable3DAPIs[];
CONTENT_EXPORT extern const char kDisableAccelerated2dCanvas[];
CONTENT_EXPORT extern const char kDisableAcceleratedCompositing[];
CONTENT_EXPORT extern const char kDisableAcceleratedLayers[];
CONTENT_EXPORT extern const char kDisableAcceleratedPlugins[];
CONTENT_EXPORT extern const char kDisableAcceleratedVideo[];
CONTENT_EXPORT extern const char kDisableAltWinstation[];
CONTENT_EXPORT extern const char kDisableApplicationCache[];
CONTENT_EXPORT extern const char kDisableAudio[];
extern const char kDisableBackingStoreLimit[];
CONTENT_EXPORT extern const char kDisableDatabases[];
extern const char kDisableDesktopNotifications[];
CONTENT_EXPORT extern const char kDisableDeviceOrientation[];
#if defined(OS_ANDROID)
CONTENT_EXPORT extern const char kEnableExperimentalWebGL[];
#else
CONTENT_EXPORT extern const char kDisableExperimentalWebGL[];
#endif
CONTENT_EXPORT extern const char kBlacklistAcceleratedCompositing[];
CONTENT_EXPORT extern const char kBlacklistWebGL[];
extern const char kDisableFileSystem[];
CONTENT_EXPORT extern const char kDisableFlash3d[];
CONTENT_EXPORT extern const char kDisableFlashStage3d[];
CONTENT_EXPORT extern const char kDisableForceCompositingMode[];
extern const char kDisableGeolocation[];
CONTENT_EXPORT extern const char kUseGpuInTests[];
extern const char kDisableGpu[];
CONTENT_EXPORT extern const char kDisableGLMultisampling[];
CONTENT_EXPORT extern const char kDisableGpuProcessPrelaunch[];
extern const char kDisableGpuSandbox[];
extern const char kReduceGpuSandbox[];
extern const char kDisableGpuWatchdog[];
CONTENT_EXPORT extern const char kDisableHangMonitor[];
CONTENT_EXPORT extern const char kDisableHTMLNotifications[];
extern const char kDisableHistogramCustomizer[];
extern const char kDisableImageTransportSurface[];
CONTENT_EXPORT extern const char kDisableJava[];
CONTENT_EXPORT extern const char kDisableJavaScript[];
extern const char kDisableJavaScriptI18NAPI[];
CONTENT_EXPORT extern const char kDisableLocalStorage[];
CONTENT_EXPORT extern const char kDisableLogging[];
CONTENT_EXPORT extern const char kDisableSmoothScrolling[];
CONTENT_EXPORT extern const char kDisablePlugins[];
extern const char kDisableRemoteFonts[];
extern const char kDisableRendererAccessibility[];
extern const char kDisableSSLFalseStart[];
extern const char kDisableSeccompFilterSandbox[];
extern const char kDisableSessionStorage[];
extern const char kDisableSetuidSandbox[];
extern const char kDisableSharedWorkers[];
extern const char kDisableSiteSpecificQuirks[];
CONTENT_EXPORT extern const char kDisableSpeechInput[];
extern const char kSpeechRecognitionWebserviceKey[];
CONTENT_EXPORT extern const char kEnableSpeechSynthesis[];
#if defined(OS_ANDROID)
CONTENT_EXPORT extern const char kDisableWebRTC[];
CONTENT_EXPORT extern const char kEnableSpeechRecognition[];
#endif
CONTENT_EXPORT extern const char kDisableWebAudio[];
#if defined(ENABLE_WEBRTC)
extern const char kEnableWebRtcAecRecordings[];
CONTENT_EXPORT extern const char kEnableSCTPDataChannels[];
#endif
extern const char kEnableWebRtcTcpServerSocket[];
extern const char kEnableWebMIDI[];
extern const char kDisableWebSecurity[];
CONTENT_EXPORT extern const char kEnableExperimentalWebSocket[];
extern const char kDisableXSSAuditor[];
CONTENT_EXPORT extern const char kDomAutomationController[];
CONTENT_EXPORT extern const char kStatsCollectionController[];
CONTENT_EXPORT extern const char kEnableAcceleratedFilters[];
extern const char kEnableAccessibilityLogging[];
extern const char kEnableAudibleNotifications[];
CONTENT_EXPORT extern const char kDisableBrowserPluginCompositing[];
CONTENT_EXPORT extern const char kEnableBrowserPluginForAllViewTypes[];
CONTENT_EXPORT extern const char kEnableBrowserPluginDragDrop[];
CONTENT_EXPORT extern const char kEnableCompositingForFixedPosition[];
CONTENT_EXPORT extern const char kEnableHighDpiCompositingForFixedPosition[];
CONTENT_EXPORT extern const char kDisableCompositingForFixedPosition[];
CONTENT_EXPORT extern const char kEnableCompositingForTransition[];
CONTENT_EXPORT extern const char kDisableCompositingForTransition[];
CONTENT_EXPORT extern const char kEnableCssShaders[];
CONTENT_EXPORT extern const char kEnableDelegatedRenderer[];
CONTENT_EXPORT extern const char kEnableDeviceMotion[];
CONTENT_EXPORT extern const char kEnableDownloadResumption[];
CONTENT_EXPORT extern const char kEnableExperimentalWebKitFeatures[];
CONTENT_EXPORT extern const char kEnableRegionBasedColumns[];
CONTENT_EXPORT extern const char kDisableThreadedHTMLParser[];
CONTENT_EXPORT extern const char kEnableFixedLayout[];
CONTENT_EXPORT extern const char kDisableFullScreen[];
CONTENT_EXPORT extern const char kEnableTextServicesFramework[];
CONTENT_EXPORT extern const char kEnableGestureTapHighlight[];
CONTENT_EXPORT extern const char kDisableGestureTapHighlight[];
extern const char kEnableGpuBenchmarking[];
extern const char kEnableGpuClientTracing[];
extern const char kEnableMemoryBenchmarking[];
extern const char kEnableSkiaBenchmarking[];
CONTENT_EXPORT extern const char kEnableLogging[];
#if defined(ANDROID) && !defined(GOOGLE_TV)
CONTENT_EXPORT extern const char kEnableWebKitMediaSource[];
#else
CONTENT_EXPORT extern const char kDisableWebKitMediaSource[];
#endif
CONTENT_EXPORT extern const char kEnableEncryptedMedia[];
CONTENT_EXPORT extern const char kDisableLegacyEncryptedMedia[];
CONTENT_EXPORT extern const char kUseFakeDeviceForMediaStream[];
CONTENT_EXPORT extern const char kUseFakeUIForMediaStream[];
extern const char kEnableMonitorProfile[];
extern const char kEnableUserMediaScreenCapturing[];
extern const char kEnablePinch[];
CONTENT_EXPORT extern const char kEnableOfflineCacheAccess[];
extern const char kDisablePinch[];
extern const char kEnablePreparsedJsCaching[];
CONTENT_EXPORT extern const char kEnablePrivilegedWebGLExtensions[];
extern const char kEnablePruneGpuCommandBuffers[];
extern const char kEnableSSLCachedInfo[];
extern const char kEnableSandboxLogging[];
extern const char kEnableSpatialNavigation[];
CONTENT_EXPORT extern const char kEnableSoftwareCompositingGLAdapter[];
CONTENT_EXPORT extern const char kEnableSmoothScrolling[];
CONTENT_EXPORT extern const char kEnableStatsTable[];
extern const char kEnableStrictSiteIsolation[];
CONTENT_EXPORT extern const char kEnableThreadedCompositing[];
CONTENT_EXPORT extern const char kDisableThreadedCompositing[];
extern const char kEnableVirtualGLContexts[];
CONTENT_EXPORT extern const char kEnableTcpFastOpen[];
CONTENT_EXPORT extern const char kDisableAcceleratedVideoDecode[];
extern const char kEnableViewport[];
CONTENT_EXPORT extern const char kExperimentalLocationFeatures[];
CONTENT_EXPORT extern const char kForceCompositingMode[];
extern const char kForceFieldTrials[];
CONTENT_EXPORT extern const char kForceRendererAccessibility[];
extern const char kGpuDeviceID[];
extern const char kGpuDriverVendor[];
extern const char kGpuDriverVersion[];
extern const char kGpuLauncher[];
CONTENT_EXPORT extern const char kGpuProcess[];
extern const char kGpuStartupDialog[];
extern const char kGpuVendorID[];
CONTENT_EXPORT extern const char kHostResolverRules[];
CONTENT_EXPORT extern const char kIgnoreCertificateErrors[];
CONTENT_EXPORT extern const char kIgnoreGpuBlacklist[];
extern const char kInProcessGPU[];
extern const char kInProcessPlugins[];
CONTENT_EXPORT extern const char kJavaScriptFlags[];
CONTENT_EXPORT extern const char kExtraPluginDir[];
extern const char kLoadPlugin[];
CONTENT_EXPORT extern const char kDisablePluginsDiscovery[];
CONTENT_EXPORT extern const char kLoggingLevel[];
extern const char kLogPluginMessages[];
extern const char kMemoryMetrics[];
CONTENT_EXPORT extern const char kNoReferrers[];
CONTENT_EXPORT extern const char kNoSandbox[];
CONTENT_EXPORT extern const char kAllowNoSandboxJob[];
extern const char kPluginLauncher[];
CONTENT_EXPORT extern const char kPluginPath[];
CONTENT_EXPORT extern const char kPluginProcess[];
extern const char kPluginStartupDialog[];
CONTENT_EXPORT extern const char kPpapiBrokerProcess[];
CONTENT_EXPORT extern const char kPpapiInProcess[];
extern const char kPpapiPluginLauncher[];
CONTENT_EXPORT extern const char kPpapiPluginProcess[];
extern const char kPpapiStartupDialog[];
CONTENT_EXPORT extern const char kProcessPerSite[];
CONTENT_EXPORT extern const char kProcessPerTab[];
CONTENT_EXPORT extern const char kProcessType[];
CONTENT_EXPORT extern const char kRegisterPepperPlugins[];
CONTENT_EXPORT extern const char kRemoteDebuggingPort[];
#if defined(OS_ANDROID)
CONTENT_EXPORT extern const char kRemoteDebuggingSocketName[];
#endif
CONTENT_EXPORT extern const char kRendererAssertTest[];
extern const char kRendererCmdPrefix[];
CONTENT_EXPORT extern const char kRendererProcess[];
CONTENT_EXPORT extern const char kEnableVtune[];
extern const char kRendererProcessLimit[];
extern const char kRendererStartupDialog[];
CONTENT_EXPORT extern const char kEnableAcceleratedOverflowScroll[];
CONTENT_EXPORT extern const char kDisableAcceleratedOverflowScroll[];
extern const char kEnableAcceleratedScrollableFrames[];
extern const char kEnableCompositedScrollingForFrames[];
extern const char kShowPaintRects[];
CONTENT_EXPORT extern const char kSimulateTouchScreenWithMouse[];
CONTENT_EXPORT extern const char kSingleProcess[];
CONTENT_EXPORT extern const char kSitePerProcess[];
CONTENT_EXPORT extern const char kSkipGpuDataLoading[];
extern const char kTabCaptureUpscaleQuality[];
extern const char kTabCaptureDownscaleQuality[];
extern const char kTapDownDeferralTimeMs[];
CONTENT_EXPORT extern const char kTestSandbox[];
CONTENT_EXPORT extern const char kTestingFixedHttpPort[];
CONTENT_EXPORT extern const char kTestingFixedHttpsPort[];
extern const char kTraceStartup[];
extern const char kTraceStartupFile[];
extern const char kTraceStartupDuration[];
CONTENT_EXPORT extern const char kUIPrioritizeInGpuProcess[];
CONTENT_EXPORT extern const char kUseExynosVda[];
CONTENT_EXPORT extern const char kUserAgent[];
extern const char kUtilityCmdPrefix[];
CONTENT_EXPORT extern const char kUtilityProcess[];
extern const char kUtilityProcessAllowedDir[];
CONTENT_EXPORT extern const char kWaitForDebuggerChildren[];
extern const char kWebCoreLogChannels[];
CONTENT_EXPORT extern const char kWorkerProcess[];
CONTENT_EXPORT extern const char kZygoteCmdPrefix[];
CONTENT_EXPORT extern const char kZygoteProcess[];

#if defined(OPERA_TVSDK)
CONTENT_EXPORT extern const char kZygoteSubprocessPath[];
#endif

CONTENT_EXPORT extern const char kDisableSoftwareRasterizer[];
CONTENT_EXPORT extern const char kDefaultTileWidth[];
CONTENT_EXPORT extern const char kDefaultTileHeight[];
CONTENT_EXPORT extern const char kMaxUntiledLayerWidth[];
CONTENT_EXPORT extern const char kMaxUntiledLayerHeight[];
CONTENT_EXPORT extern const char kEnableFixedPositionCreatesStackingContext[];
CONTENT_EXPORT extern const char kDisableFixedPositionCreatesStackingContext[];
CONTENT_EXPORT extern const char kEnableDeferredImageDecoding[];
CONTENT_EXPORT extern const char kEnableBeginFrameScheduling[];
CONTENT_EXPORT extern const char kEnableBrowserInputController[];

extern const char kEnableVisualWordMovement[];
CONTENT_EXPORT extern const char kUseMobileUserAgent[];

#if defined(OS_ANDROID)
extern const char kDisableMediaHistoryLogging[];
CONTENT_EXPORT extern const char kDisableGestureRequirementForMediaPlayback[];
extern const char kNetworkCountryIso[];
CONTENT_EXPORT extern const char kDisableOverscrollEdgeEffect[];
#endif

#if defined(OS_CHROMEOS)
CONTENT_EXPORT extern const char kDisablePanelFitting[];
#endif

#if defined(OS_POSIX)
extern const char kChildCleanExit[];
#endif

#if defined(OS_MACOSX) && !defined(OS_IOS)
extern const char kDisableCarbonInterposing[];
extern const char kUseCoreAnimation[];
#endif

#if defined(USE_AURA)
CONTENT_EXPORT extern const char kTestCompositor[];
#endif

CONTENT_EXPORT extern const char kDisableBrowserSideCompositor[];

CONTENT_EXPORT extern const char kOverscrollHistoryNavigation[];
CONTENT_EXPORT extern const char kEnableOverscrollNotifications[];
CONTENT_EXPORT extern const char kEnableWebGLDraftExtensions[];

}  // namespace switches

#endif  // CONTENT_PUBLIC_COMMON_CONTENT_SWITCHES_H_
