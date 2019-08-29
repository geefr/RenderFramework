
#ifndef VRHELL_H
#define VRHELL_H

#ifdef _WIN32
# include "GL/glew.h"
#endif

#include "GL/gl.h"
#include "glm/glm.hpp"
#include "device.h"
#include "controller.h"

#include "openvr/openvr.h"

#include <string>
#include <map>


namespace renderframework { namespace vrhell {
  /**
   * VR Env class
   *
   * So this is like a helper class around OpenVR
   */
	class Env
	{
	public:
    using DeviceList = std::map<uint32_t, std::unique_ptr<vrhell::Device>>;

		Env(vr::ETrackingUniverseOrigin trackingType);
		~Env();

    /**
     * Update the state of the vr environment
     *
     * Will update positions of each device,
     * handle device addition/removal and similar stuff
     */
    void update();

		vr::IVRSystem* context() const;
    const DeviceList& devices() const;
    vrhell::Device* device(uint32_t id);

    vrhell::Controller* leftHand() const;
    vrhell::Controller* rightHand() const;
    vrhell::Device* hmd() const;

    glm::mat4x4 getProjectionMatrix(vr::EVREye eye, float nearZ, float farZ);
    glm::mat4x4 getEyeToHeadTransform(vr::EVREye eye);

    /**
     * Submit textures to OpenVR
     *
     * @param left The ID of the left eye texture
     * @param right The ID of the right eye texture
     */
    void submitFrames( GLuint leftTex, GLuint rightTex );

    /// TODO
    void waitGetPoses();

    /// Enable/disable verbose logging
    void logging( bool enabled );

		static std::string to_string(vr::ETrackedDeviceClass devClass);

    void getRecommendedTargetSize(uint32_t& width, uint32_t& height);

	private:
		vr::IVRSystem* mContext = nullptr;
    vr::TrackedDevicePose_t mTrackedDevicePoses[vr::k_unMaxTrackedDeviceCount];
    DeviceList mDevices;
    vr::ETrackingUniverseOrigin mTrackingType;
    bool mLogging = false;
	};

	inline vr::IVRSystem* Env::context() const { return mContext; }
	inline const Env::DeviceList& Env::devices() const { return mDevices; }
  inline void Env::logging(bool enabled) { mLogging = enabled; }
} }
#endif


