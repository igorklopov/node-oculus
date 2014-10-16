#ifndef BUILDING_NODE_EXTENSION
  #define BUILDING_NODE_EXTENSION
#endif

#include <node.h>
#include "Device.h"
#include <string>
#include <sstream>

namespace nodeOculus {
  v8::Persistent<v8::Function> Device::constructor;

  Device::Device() {
    printf("Initializing OVR.\n");
    ovr_Initialize();
    printf("OVR Initialized\n");
  }

  Device::~Device() {
  }


  JS_METHOD(Device, destroyResources) {
    SCOPE_IN;
    Device* obj = JS_OBJECT(Device, args.This());

    if (obj->hmd != NULL) {
      printf("Destroying HMD.\n");
      ovrHmd_Destroy(obj->hmd);
    }

    printf("Shutting Down OVR.\n");
    ovr_Shutdown();
    printf("OVR Destroyed.\n");
    SCOPE_OUT(JS_BOOL(true));
  }


  JS_METHOD(Device, discoverSensor) {
    SCOPE_IN;

    Device* obj = JS_OBJECT(Device, args.This());

    printf("1: Detecting HMDs.\n");
    int numHMDs = ovrHmd_Detect();

    printf("before creating, HMD == NULL? %s\n", (obj->hmd == NULL ? "true" : "false"));

    if (numHMDs > 0) {
      printf("2: Creating HMD.\n");
      obj->hmd = ovrHmd_Create(0);

      if (obj->hmd != NULL) {
        printf("3: HMD Created.\n");

        printf("4: Configuring Tracking\n");
        if (ovrHmd_ConfigureTracking(obj->hmd, ovrTrackingCap_Orientation | ovrTrackingCap_Position, 0) != 0) {
          printf("5: Configured Tracking Successfully\n");

          printf("6: Testing Tracking\n");
          if ((ovrHmd_GetTrackingState(obj->hmd, 0.0f).StatusFlags & ovrStatus_HmdConnected)) {
            printf("7: Sensor Connected\n");
            SCOPE_OUT(JS_BOOL(true));
          }
        }
      }
    }

    printf("FAILED\n");

    ovrHmd_Destroy(obj->hmd);

    SCOPE_OUT(JS_BOOL(false));
  }

  // TODO update to DK2 if necessary
  JS_METHOD(Device, getDeviceInfo) {
    SCOPE_IN;

    // Device* obj = JS_OBJECT(Device, args.This());
    v8::Local<v8::Object> res = v8::Object::New();

    // OVR::HMDInfo info;
    // if (obj->hmd != NULL && obj->hmd->GetDeviceInfo(&info)) {
    //   res->Set(v8::String::NewSymbol("displayDeviceName"), v8::String::NewSymbol(info.DisplayDeviceName));
    //   res->Set(v8::String::NewSymbol("displayId"), JS_FLOAT(info.DisplayId));
    //   res->Set(v8::String::NewSymbol("screenCenter"), JS_FLOAT(info.VScreenCenter));
    //   res->Set(v8::String::NewSymbol("eyeToScreenDistance"), JS_FLOAT(info.EyeToScreenDistance));
    //   res->Set(v8::String::NewSymbol("lensSeperationDistance"), JS_FLOAT(info.LensSeparationDistance));
    //   res->Set(v8::String::NewSymbol("interpupillaryDistance"), JS_FLOAT(info.InterpupillaryDistance));


    //   v8::Local<v8::Array> screenSize = v8::Array::New(2);
    //   screenSize->Set(0, JS_FLOAT(info.HScreenSize));
    //   screenSize->Set(1, JS_FLOAT(info.VScreenSize));
    //   res->Set(v8::String::NewSymbol("screenSize"), screenSize);

    //   v8::Local<v8::Array> resolution = v8::Array::New(2);
    //   resolution->Set(0, JS_FLOAT(info.HResolution));
    //   resolution->Set(1, JS_FLOAT(info.VResolution));
    //   res->Set(v8::String::NewSymbol("resolution"), resolution);

    //   v8::Local<v8::Array> distortion = v8::Array::New(4);
    //   distortion->Set(0, JS_FLOAT(info.DistortionK[0]));
    //   distortion->Set(1, JS_FLOAT(info.DistortionK[1]));
    //   distortion->Set(2, JS_FLOAT(info.DistortionK[2]));
    //   distortion->Set(3, JS_FLOAT(info.DistortionK[3]));
    //   res->Set(v8::String::NewSymbol("distortion"), distortion);
    // }

    SCOPE_OUT(res);
  }

  JS_METHOD(Device, getOrientationQuat) {
    SCOPE_IN;

    Device* obj = JS_OBJECT(Device, args.This());
    v8::Local<v8::Array> res = args.Length() > 0 && args[0]->IsArray() ? v8::Array::Cast(*args[0]) : v8::Array::New(4);
    ovrTrackingState trackingInfo;

    if (obj->hmd != NULL) {
      trackingInfo = ovrHmd_GetTrackingState(obj->hmd, 0.0);
      res->Set(0, JS_FLOAT(trackingInfo.HeadPose.ThePose.Orientation.x));
      res->Set(1, JS_FLOAT(trackingInfo.HeadPose.ThePose.Orientation.y));
      res->Set(2, JS_FLOAT(trackingInfo.HeadPose.ThePose.Orientation.z));
      res->Set(3, JS_FLOAT(trackingInfo.HeadPose.ThePose.Orientation.w));
    } else {
      printf("obj->sensor is NULL\n");
      res->Set(0, JS_FLOAT(0.0));
      res->Set(1, JS_FLOAT(0.0));
      res->Set(2, JS_FLOAT(0.0));
      res->Set(3, JS_FLOAT(1.0));
    }

    SCOPE_OUT(res);
  }

  // alternative to to_string for pre-c++11 compilers
  template <class T>
  std::string toString(T num) {
    std::ostringstream ss;
    ss << num;
    return ss.str();
  }

  JS_METHOD(Device, getTrackingData) {
    SCOPE_IN;

    Device * obj = JS_OBJECT(Device, args.This());
    
    ovrTrackingState trackingInfo;
    std::string data = "";

    if (obj->hmd != NULL) {
      trackingInfo = ovrHmd_GetTrackingState(obj->hmd, 0.0f);
      data += "--------------------------------------------\n";
      data += ".HeadPose\n";
      data += "  .ThePose\n";
      data += "    .Orientation\n";
      data += "      [ " + toString<float>(trackingInfo.HeadPose.ThePose.Orientation.x) + "\n";
      data += "        " + toString<float>(trackingInfo.HeadPose.ThePose.Orientation.y) + "\n";
      data += "        " + toString<float>(trackingInfo.HeadPose.ThePose.Orientation.z) + "\n";
      data += "        " + toString<float>(trackingInfo.HeadPose.ThePose.Orientation.w) + " ]\n";
      data += "    .Position\n";
      data += "      [ " + toString<float>(trackingInfo.HeadPose.ThePose.Position.x) + "\n";
      data += "        " + toString<float>(trackingInfo.HeadPose.ThePose.Position.y) + "\n";
      data += "        " + toString<float>(trackingInfo.HeadPose.ThePose.Position.z) + " ]\n";
      data += "  .AngularVelocity\n";
      data += "    [ " + toString<float>(trackingInfo.HeadPose.AngularVelocity.x) + "\n";
      data += "      " + toString<float>(trackingInfo.HeadPose.AngularVelocity.y) + "\n";
      data += "      " + toString<float>(trackingInfo.HeadPose.AngularVelocity.z) + " ]\n";
      data += "  .LinearVelocity\n";
      data += "    [ " + toString<float>(trackingInfo.HeadPose.LinearVelocity.x) + "\n";
      data += "      " + toString<float>(trackingInfo.HeadPose.LinearVelocity.y) + "\n";
      data += "      " + toString<float>(trackingInfo.HeadPose.LinearVelocity.z) + " ]\n";
      data += "  .AngularAcceleration\n";
      data += "    [ " + toString<float>(trackingInfo.HeadPose.AngularAcceleration.x) + "\n";
      data += "      " + toString<float>(trackingInfo.HeadPose.AngularAcceleration.y) + "\n";
      data += "      " + toString<float>(trackingInfo.HeadPose.AngularAcceleration.z) + " ]\n";
      data += "  .LinearAcceleration\n";
      data += "    [ " + toString<float>(trackingInfo.HeadPose.LinearAcceleration.x) + "\n";
      data += "      " + toString<float>(trackingInfo.HeadPose.LinearAcceleration.y) + "\n";
      data += "      " + toString<float>(trackingInfo.HeadPose.LinearAcceleration.z) + " ]\n";
      data += "  .TimeInSeconds\n";
      data += "    " + toString<double>(trackingInfo.HeadPose.TimeInSeconds) + "\n\n";
      data += ".CameraPose\n";
      data += "  .Orientation\n";
      data += "    [ " + toString<float>(trackingInfo.CameraPose.Orientation.x) + "\n";
      data += "      " + toString<float>(trackingInfo.CameraPose.Orientation.y) + "\n";
      data += "      " + toString<float>(trackingInfo.CameraPose.Orientation.z) + "\n";
      data += "      " + toString<float>(trackingInfo.CameraPose.Orientation.w) + " ]\n";
      data += "  .Position\n";
      data += "    [ " + toString<float>(trackingInfo.CameraPose.Position.x) + "\n";
      data += "      " + toString<float>(trackingInfo.CameraPose.Position.y) + "\n";
      data += "      " + toString<float>(trackingInfo.CameraPose.Position.z) + " ]\n";
      data += ".LeveledCameraPose\n";
      data += "  .Orientation\n";
      data += "    [ " + toString<float>(trackingInfo.LeveledCameraPose.Orientation.x) + "\n";
      data += "      " + toString<float>(trackingInfo.LeveledCameraPose.Orientation.y) + "\n";
      data += "      " + toString<float>(trackingInfo.LeveledCameraPose.Orientation.z) + "\n";
      data += "      " + toString<float>(trackingInfo.LeveledCameraPose.Orientation.w) + " ]\n";
      data += "  .Position\n";
      data += "    [ " + toString<float>(trackingInfo.LeveledCameraPose.Position.x) + "\n";
      data += "      " + toString<float>(trackingInfo.LeveledCameraPose.Position.y) + "\n";
      data += "      " + toString<float>(trackingInfo.LeveledCameraPose.Position.z) + " ]\n";
    }

    v8::Local<v8::String> res = v8::String::New(data.c_str(), data.length());
    SCOPE_OUT(res);
  }

  void Device::Init(v8::Handle<v8::Object> exports) {
    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
    tpl->SetClassName(v8::String::NewSymbol("Device"));    
    tpl->InstanceTemplate()->SetInternalFieldCount(4);

    // Prototype
    JS_PROTOTYPE(tpl, destroyResources);
    JS_PROTOTYPE(tpl, discoverSensor);
    JS_PROTOTYPE(tpl, getDeviceInfo);
    JS_PROTOTYPE(tpl, getOrientationQuat);
    JS_PROTOTYPE(tpl, getTrackingData);

    // Declare contructor
    constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
    exports->Set(v8::String::NewSymbol("Device"), constructor);
  }

  JS_METHOD(Device, New) {
    SCOPE_IN;

    Device* device = new Device();
    device->Wrap(args.This());

    SCOPE_OUT(args.This());
  }

  JS_METHOD(Device, NewInstance) {
    SCOPE_IN;

    const unsigned argc = 1;
    v8::Handle<v8::Value> argv[argc] = { args[0] };
    v8::Local<v8::Object> instance = constructor->NewInstance(argc, argv);

    SCOPE_OUT(instance);
  }
}

