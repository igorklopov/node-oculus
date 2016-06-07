#ifndef DEVICE_H
#define DEVICE_H

#include <nan.h>
#include <OVR_CAPI.h>

namespace nodeOculus {
  class Device : public Nan::ObjectWrap {
    public:
      static void Init(v8::Handle<v8::Object> target);

    private:
      Device();
      ~Device();

      static NAN_METHOD(destroyResources);
/*
      static NAN_METHOD(discoverSensor);
      static NAN_METHOD(getDeviceInfo);
      static NAN_METHOD(getPositionDeltas);
      static NAN_METHOD(getOrientationQuat);
      static NAN_METHOD(getTrackingData);
      static NAN_METHOD(getOvrMatrix4f_Projection);
*/
      static NAN_METHOD(New);

      ovrSession session;
      float lastX;
      float lastY;
      float lastZ;
  };
}

#endif
