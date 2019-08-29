
#include "GL/glew.h"

#include "glm/glm.hpp"

#include "env.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <memory>

namespace renderframework { namespace vrhell {

	Env::Env(vr::ETrackingUniverseOrigin trackingType)
    : mTrackingType(trackingType)
	{
		if (!vr::VR_IsHmdPresent()) throw std::runtime_error("No HMD present");
		if (!vr::VR_IsRuntimeInstalled()) throw std::runtime_error("OpenVR runtime not installed");

		vr::HmdError err;
		mContext = vr::VR_Init(&err, vr::EVRApplicationType::VRApplication_Scene);
		if (!mContext) throw std::runtime_error("Failed to create OpenVR context");

		for (auto id = vr::k_unTrackedDeviceIndex_Hmd; id < vr::k_unMaxTrackedDeviceCount; ++id)
		{
      auto dev = Device::enumerate(mContext, id);
			if( !dev ) continue;
			mDevices[id] = std::move(dev);
		}
	}

	Env::~Env()
	{
		vr::VR_Shutdown();
	}

  void Env::update()
  {
    if( !mContext ) return;

    // Pump the event queue
    vr::VREvent_t ev;
    while (mContext->PollNextEvent(&ev, sizeof(ev)))
    {
      if (ev.eventType == vr::VREvent_TrackedDeviceActivated || 
          ev.eventType == vr::VREvent_TrackedDeviceUpdated)
      {
        // (re)enumerate the device
        auto dev = Device::enumerate(mContext, ev.trackedDeviceIndex);
        if( !dev ) continue;
        mDevices[ev.trackedDeviceIndex] = std::move(dev);

        if( mLogging ) std::cout << "EVENT: Device activated/Updated: TYPE: " << ev.eventType << " DEV: " << ev.trackedDeviceIndex << std::endl;
      }
      else if (ev.eventType == vr::VREvent_TrackedDeviceDeactivated)
      {
        mDevices.erase(ev.trackedDeviceIndex);
        if (mLogging) std::cout << "EVENT: Device Deactivated DEV: " << ev.trackedDeviceIndex << std::endl;
      }
      else
      {
        auto dev = mDevices.find(ev.trackedDeviceIndex);
        if( dev == mDevices.end() ) continue;
        auto controller = dynamic_cast<Controller*>(dev->second.get());
        if( !controller ) continue;

        switch (ev.eventType)
        {
        case vr::VREvent_ButtonPress:
          controller->buttonPress(ev.data.controller.button);
          if( mLogging ) std::cout << "EVENT: ButtonPress: DEV: " << ev.trackedDeviceIndex << " BUTTON: " << ev.data.controller.button << std::endl;
          break;
        case vr::VREvent_ButtonUnpress:
          controller->buttonUnPress(ev.data.controller.button);
          if (mLogging) std::cout << "EVENT: ButtonUnPress: DEV: " << ev.trackedDeviceIndex << " BUTTON: " << ev.data.controller.button << std::endl;
          break;
        case vr::VREvent_ButtonTouch:
          controller->buttonTouch(ev.data.controller.button);
          if (mLogging) std::cout << "EVENT: ButtonTouch: DEV: " << ev.trackedDeviceIndex << " BUTTON: " << ev.data.controller.button << std::endl;
          break;
        case vr::VREvent_ButtonUntouch:
          controller->buttonUntouch(ev.data.controller.button);
          if (mLogging) std::cout << "EVENT: ButtonUntouch: DEV: " << ev.trackedDeviceIndex << " BUTTON: " << ev.data.controller.button << std::endl;
          break;
        default:
          break;
        }
      }
    }
  }

  vrhell::Controller* Env::leftHand() const
  {
    // TODO: For now left is the first controller
    // Should make better use of openvr here
    auto dev = std::find_if(mDevices.begin(), mDevices.end(), [&](const auto& d){ return dynamic_cast<vrhell::Controller*>(d.second.get()); });
    if( dev == mDevices.end() ) return nullptr;
    return dynamic_cast<vrhell::Controller*>(dev->second.get());
  }

  vrhell::Controller* Env::rightHand() const
  {
    // TODO: For now right is the second controller
    // Should make better use of openvr here
    auto dev = std::find_if(mDevices.rbegin(), mDevices.rend(), [&](const auto& d) { return dynamic_cast<vrhell::Controller*>(d.second.get()); });
    if (dev == mDevices.rend()) return nullptr;
    return dynamic_cast<vrhell::Controller*>(dev->second.get());
  }

  vrhell::Device* Env::hmd() const
  {
    auto dev = std::find_if(mDevices.begin(), mDevices.end(), [&](const auto& d) { 
      return d.second->mDevClass == vr::TrackedDeviceClass_HMD;
    });
    if (dev == mDevices.end()) return nullptr;
    return dev->second.get();
  }

  vrhell::Device* Env::device(uint32_t id)
	{
		auto it = std::find_if(mDevices.begin(), mDevices.end(), [&](const auto& d) {
			return d.second->mID == id;
		});
		if (it == mDevices.end()) return nullptr;
		return it->second.get();
	}

  glm::mat4x4 Env::getProjectionMatrix(vr::EVREye eye, float nearZ, float farZ)
  {
    auto projMat = mContext->GetProjectionMatrix(eye, nearZ, farZ);
    auto m = projMat.m;
    return {
      m[0][0], m[1][0], m[2][0], m[3][0],
      m[0][1], m[1][1], m[2][1], m[3][1],
      m[0][2], m[1][2], m[2][2], m[3][2],
      m[0][3], m[1][3], m[2][3], m[3][3],
    };
  }

  glm::mat4x4 Env::getEyeToHeadTransform(vr::EVREye eye)
  {
    auto transform = mContext->GetEyeToHeadTransform(eye);
    auto m = transform.m;
    return {
      m[0][0], m[1][0], m[2][0], 0.0f,
      m[0][1], m[1][1], m[2][1], 0.0f,
      m[0][2], m[1][2], m[2][2], 0.0f,
      m[0][3], m[1][3], m[2][3], 1.0f,
    };
  }

  void Env::submitFrames(GLuint leftTex, GLuint rightTex)
  {
    if( !mContext ) return;

    vr::EColorSpace cSpace = vr::ColorSpace_Gamma;

    vr::Texture_t leftVRTex{ reinterpret_cast<void*>(leftTex), vr::ETextureType::TextureType_OpenGL, cSpace };
    vr::Texture_t rightVRTex{ reinterpret_cast<void*>(rightTex), vr::ETextureType::TextureType_OpenGL, cSpace };

    vr::VRCompositor()->Submit(vr::Eye_Left, &leftVRTex);
    vr::VRCompositor()->Submit(vr::Eye_Right, &rightVRTex);

    vr::VRCompositor()->PostPresentHandoff();
  }

  void Env::waitGetPoses()
  {
    // TODO: Okay I don't know why this is needed but it is
    // If we don't wait here the compositor doesn't accept the frames we're trying to give it
    // Of course that does mean we're probably setting all the transforms/etc at the wrong point...
    vr::VRCompositor()->WaitGetPoses(mTrackedDevicePoses, vr::k_unMaxTrackedDeviceCount, nullptr, 0);

    // Update poses of tracked devices
    for (uint32_t id = 0; id < vr::k_unMaxTrackedDeviceCount; ++id)
    {
      auto dev = mDevices.find(id);
      if (dev == mDevices.end()) continue;
      auto& pose = mTrackedDevicePoses[id];
      if (!pose.bDeviceIsConnected) continue;
      dev->second->mPose = pose;
    }
  }


	std::string Env::to_string(vr::ETrackedDeviceClass devClass)
	{
		switch (devClass)
		{
		case vr::TrackedDeviceClass_Invalid:
      return "Invalid";
		case vr::TrackedDeviceClass_HMD:
      return "HMD";
		case vr::TrackedDeviceClass_Controller:
      return "Controller";
		case vr::TrackedDeviceClass_GenericTracker:
      return "Generic Tracker";
		case vr::TrackedDeviceClass_TrackingReference:
      return "Base Station";
		case vr::TrackedDeviceClass_DisplayRedirect:
      return "Display Redirect";
		case vr::TrackedDeviceClass_Max:
      return "Max";
		}
    return "Unknown";
	}

  void Env::getRecommendedTargetSize(uint32_t& width, uint32_t& height)
  {
    if( !mContext ) return;
    mContext->GetRecommendedRenderTargetSize(&width, &height);
  }
 } }
