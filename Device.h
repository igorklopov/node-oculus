#ifndef DEVICE_VR_H
#define DEVICE_VR_H

#include <node.h>
#include <OVR.h>
#include "v8Macros.h"

namespace nodeOculus {
  class Device : public node::ObjectWrap {
    public:
      static void Init(v8::Handle<v8::Object> exports);
      static JS_FUNCTION(NewInstance);

    private:
      explicit Device();
      ~Device();

      static JS_FUNCTION(destroyResources);
      static JS_FUNCTION(discoverSensor);
      static JS_FUNCTION(getDeviceInfo);
      static JS_FUNCTION(getOrientationQuat);

      static JS_FUNCTION(New);
      static v8::Persistent<v8::Function> constructor;

      ovrHmd hmd; // already a pointer

      // OVR::Ptr<OVR::DeviceManager> deviceManager;
      // OVR::Ptr<OVR::HMDDevice> hmd;
      // OVR::Ptr<OVR::SensorDevice> sensor;
      // OVR::SensorFusion* sensorFusion;
  };
}

#endif