/*
 * Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
 * Copyright (C) 2004, 2005, 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2008-2009 Torch Mobile, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BitmapImage_h
#define BitmapImage_h

#include "core/platform/graphics/Color.h"
#include "core/platform/graphics/Image.h"
#include "core/platform/graphics/ImageOrientation.h"
#include "core/platform/graphics/ImageSource.h"
#include "core/platform/graphics/IntSize.h"
#include "wtf/Forward.h"

namespace WebCore {
struct FrameData;
}

namespace WTF {
template<> struct VectorTraits<WebCore::FrameData> : public SimpleClassVectorTraits {
    static const bool canInitializeWithMemset = false; // Not all FrameData members initialize to 0.
};
}

namespace WebCore {

class NativeImageSkia;
template <typename T> class Timer;

// ================================================
// FrameData Class
// ================================================

struct FrameData {
    WTF_MAKE_NONCOPYABLE(FrameData);
public:
    FrameData()
        : m_frame(0)
        , m_orientation(DefaultImageOrientation)
        , m_duration(0)
        , m_haveMetadata(false)
        , m_isComplete(false)
        , m_hasAlpha(true)
        , m_frameBytes(0)
    {
    }

    ~FrameData()
    {
        clear(true);
    }

    // Clear the cached image data on the frame, and (optionally) the metadata.
    // Returns whether there was cached image data to clear.
    bool clear(bool clearMetadata);

    void reportMemoryUsage(MemoryObjectInfo*) const;

    RefPtr<NativeImageSkia> m_frame;
    ImageOrientation m_orientation;
    float m_duration;
    bool m_haveMetadata : 1;
    bool m_isComplete : 1;
    bool m_hasAlpha : 1;
    unsigned m_frameBytes;
};

// =================================================
// BitmapImage Class
// =================================================

class BitmapImage : public Image {
    friend class GeneratedImage;
    friend class CrossfadeGeneratedImage;
    friend class GeneratorGeneratedImage;
    friend class GraphicsContext;
public:
    static PassRefPtr<BitmapImage> create(PassRefPtr<NativeImageSkia> nativeImage, ImageObserver* observer = 0)
    {
        return adoptRef(new BitmapImage(nativeImage, observer));
    }
    static PassRefPtr<BitmapImage> create(ImageObserver* observer = 0)
    {
        return adoptRef(new BitmapImage(observer));
    }
    virtual ~BitmapImage();

    virtual bool isBitmapImage() const OVERRIDE;

    virtual bool hasSingleSecurityOrigin() const OVERRIDE;

    virtual IntSize size() const OVERRIDE;
    IntSize sizeRespectingOrientation() const;
    IntSize currentFrameSize() const;
    virtual bool getHotSpot(IntPoint&) const OVERRIDE;

    virtual bool dataChanged(bool allDataReceived) OVERRIDE;
    virtual String filenameExtension() const OVERRIDE;

    // It may look unusual that there is no start animation call as public API.  This is because
    // we start and stop animating lazily.  Animation begins whenever someone draws the image.  It will
    // automatically pause once all observers no longer want to render the image anywhere.
    virtual void stopAnimation() OVERRIDE;
    virtual void resetAnimation() OVERRIDE;

    virtual unsigned decodedSize() const OVERRIDE;

    virtual PassRefPtr<NativeImageSkia> nativeImageForCurrentFrame() OVERRIDE;
    virtual bool currentFrameKnownToBeOpaque() OVERRIDE;

    ImageOrientation currentFrameOrientation();

#if !ASSERT_DISABLED
    virtual bool notSolidColor() OVERRIDE;
#endif

    void reportMemoryUsage(MemoryObjectInfo*) const OVERRIDE;

private:
    void updateSize() const;

protected:
    enum RepetitionCountStatus {
      Unknown,    // We haven't checked the source's repetition count.
      Uncertain,  // We have a repetition count, but it might be wrong (some GIFs have a count after the image data, and will report "loop once" until all data has been decoded).
      Certain     // The repetition count is known to be correct.
    };

    BitmapImage(PassRefPtr<NativeImageSkia>, ImageObserver* = 0);
    BitmapImage(ImageObserver* = 0);

    virtual void draw(GraphicsContext*, const FloatRect& dstRect, const FloatRect& srcRect, CompositeOperator, BlendMode) OVERRIDE;
    virtual void draw(GraphicsContext*, const FloatRect& dstRect, const FloatRect& srcRect, CompositeOperator, BlendMode, RespectImageOrientationEnum) OVERRIDE;

    size_t currentFrame() const { return m_currentFrame; }
    size_t frameCount();
    PassRefPtr<NativeImageSkia> frameAtIndex(size_t);
    bool frameIsCompleteAtIndex(size_t);
    float frameDurationAtIndex(size_t);
    bool frameHasAlphaAtIndex(size_t);
    ImageOrientation frameOrientationAtIndex(size_t);

    // Decodes and caches a frame. Never accessed except internally.
    void cacheFrame(size_t index);
    // Called before accessing m_frames[index]. Returns false on index out of bounds.
    bool ensureFrameIsCached(size_t index);

    // Called to invalidate cached data. This is used while animating large
    // images to keep memory footprint low. The decoder may preserve some frames
    // to avoid redecoding the whole image on every frame.
    virtual void destroyDecodedData() OVERRIDE;

    // If the image is large enough, calls destroyDecodedData().
    void destroyDecodedDataIfNecessary();

    // Generally called by destroyDecodedData(), destroys whole-image metadata
    // and notifies observers that the memory footprint has (hopefully)
    // decreased by |frameBytesCleared|.
    void destroyMetadataAndNotify(size_t frameBytesCleared);

    // Whether or not size is available yet.
    bool isSizeAvailable();

    // Called after asking the source for any information that may require
    // decoding part of the image (e.g., the image size).  We need to report
    // the partially decoded data to our observer so it has an accurate
    // account of the BitmapImage's memory usage.
    void didDecodeProperties() const;

    // Animation.
    int repetitionCount(bool imageKnownToBeComplete);  // |imageKnownToBeComplete| should be set if the caller knows the entire image has been decoded.
    bool shouldAnimate();
    virtual void startAnimation(bool catchUpIfNecessary = true) OVERRIDE;
    void advanceAnimation(Timer<BitmapImage>*);

    // Function that does the real work of advancing the animation.  When
    // skippingFrames is true, we're in the middle of a loop trying to skip over
    // a bunch of animation frames, so we should not do things like decode each
    // one or notify our observers.
    // Returns whether the animation was advanced.
    bool internalAdvanceAnimation(bool skippingFrames);

    // Checks to see if the image is a 1x1 solid color.  We optimize these images and just do a fill rect instead.
    // This check should happen regardless whether m_checkedForSolidColor is already set, as the frame may have
    // changed.
    void checkForSolidColor();

    virtual bool mayFillWithSolidColor();
    virtual Color solidColor() const;

    ImageSource m_source;
    mutable IntSize m_size; // The size to use for the overall image (will just be the size of the first image).
    mutable IntSize m_sizeRespectingOrientation;

    size_t m_currentFrame; // The index of the current frame of animation.
    Vector<FrameData, 1> m_frames; // An array of the cached frames of the animation. We have to ref frames to pin them in the cache.

    Timer<BitmapImage>* m_frameTimer;
    int m_repetitionCount; // How many total animation loops we should do.  This will be cAnimationNone if this image type is incapable of animation.
    RepetitionCountStatus m_repetitionCountStatus;
    int m_repetitionsComplete;  // How many repetitions we've finished.
    double m_desiredFrameStartTime;  // The system time at which we hope to see the next call to startAnimation().

    Color m_solidColor;  // If we're a 1x1 solid color, this is the color to use to fill.

    unsigned m_decodedSize; // The current size of all decoded frames.
    mutable unsigned m_decodedPropertiesSize; // The size of data decoded by the source to determine image properties (e.g. size, frame count, etc).
    size_t m_frameCount;

    bool m_isSolidColor : 1; // Whether or not we are a 1x1 solid image.
    bool m_checkedForSolidColor : 1; // Whether we've checked the frame for solid color.

    bool m_animationFinished : 1; // Whether or not we've completed the entire animation.

    bool m_allDataReceived : 1; // Whether or not we've received all our data.
    mutable bool m_haveSize : 1; // Whether or not our |m_size| member variable has the final overall image size yet.
    bool m_sizeAvailable : 1; // Whether or not we can obtain the size of the first image frame yet from ImageIO.
    mutable bool m_hasUniformFrameSize : 1;
    mutable bool m_haveFrameCount : 1;
};

}

#endif
