/*
 * libjingle
 * Copyright 2013, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "talk/app/webrtc/peerconnectioninterface.h"

#import "RTCPeerConnection.h"
#import "RTCPeerConnectionDelegate.h"

// These objects are created by RTCPeerConnectionFactory to wrap an
// id<RTCPeerConnectionDelegate> and call methods on that interface.

namespace webrtc {

class RTCPeerConnectionObserver : public PeerConnectionObserver {

 public:
  explicit RTCPeerConnectionObserver(id<RTCPeerConnectionDelegate> delegate);

  void SetPeerConnection(RTCPeerConnection *peerConnection);

  void OnError();

  // Triggered when the SignalingState changed.
  void OnSignalingChange(PeerConnectionInterface::SignalingState new_state);

  // Triggered when media is received on a new stream from remote peer.
  void OnAddStream(MediaStreamInterface* stream);

  // Triggered when a remote peer close a stream.
  void OnRemoveStream(MediaStreamInterface* stream);

  // Triggered when a remote peer open a data channel.
  void OnDataChannel(DataChannelInterface* data_channel);

  // Triggered when renegotation is needed, for example the ICE has restarted.
  void OnRenegotiationNeeded();

  // Called any time the ICEConnectionState changes
  void OnICEConnectionChange(
      PeerConnectionInterface::IceConnectionState new_state);

  // Called any time the ICEGatheringState changes
  void OnICEGatheringChange(
      PeerConnectionInterface::IceGatheringState new_state);

  // New Ice candidate have been found.
  void OnIceCandidate(const IceCandidateInterface* candidate);

 private:
  id<RTCPeerConnectionDelegate> _delegate;
  RTCPeerConnection *_peerConnection;
};

} // namespace webrtc
