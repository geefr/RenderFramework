#ifndef VRHELL_DEVICE_H
#define VRHELL_DEVICE_H

#include "openvr/openvr.h"
#include "glm/glm.hpp"

#include <cstdint>
#include <memory>

namespace renderframework { namespace vrhell {
	class Device
	{
	public:
    Device(vr::IVRSystem* context, uint32_t id, vr::ETrackedDeviceClass devClass);
		virtual ~Device();

    /// Factory method to create device given generic stuff
    static std::unique_ptr<Device> enumerate(vr::IVRSystem* context, uint32_t id);

    /// Device -> Absolute matrix, glm format
    glm::mat4x4 deviceToAbsoluteMatrix() const;

    /// Position of device in world space
    glm::vec3 positionAbsolute() const;

		uint32_t mID = 0;
		vr::ETrackedDeviceClass mDevClass;
		std::string mName;
		std::string mSerial;
    vr::TrackedDevicePose_t mPose;

		static std::string getDeviceString(vr::IVRSystem* context, vr::TrackedDeviceIndex_t dev,
			vr::TrackedDeviceProperty prop);
	};
} }
#endif
