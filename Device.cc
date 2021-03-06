#include "Device.h"
#include <sstream>
#include <string>
#include <nan.h>

namespace nodeOculus {
  static Nan::Persistent<v8::FunctionTemplate> constructor;

  Device::Device()
    : lastX(0.0), lastY(0.0), lastZ(0.0)
  {
    printf("Initializing OVR\n");
    ovrResult result = ovr_Initialize(nullptr);
    if (!OVR_SUCCESS(result)) {
      printf("OVR Not Initialized: %d\n", result);
      return;
    }
    printf("OVR Initialized\n");
  }

  Device::~Device() {
  }

  NAN_METHOD(Device::destroyResources) {
    Device* obj = Nan::ObjectWrap::Unwrap<Device>(info.This());

    if (obj->session != NULL) {
      printf("Destroying Session\n");
      ovr_Destroy(obj->session);
    }

    printf("Shutting Down OVR\n");
    ovr_Shutdown();
    printf("OVR Destroyed\n");
  }

  NAN_METHOD(Device::discoverSensor) {
    Device* obj = Nan::ObjectWrap::Unwrap<Device>(info.This());

    printf("1: Creating Session\n");
    ovrResult result = ovr_Create(&obj->session, &obj->luid);
    if (!OVR_SUCCESS(result)) {
      printf("Session Not Created: %d\n", result);
      info.GetReturnValue().Set(Nan::False());
      return;
    }

    printf("2: Session Created\n");
    info.GetReturnValue().Set(Nan::True());
  }

/*
  JS_METHOD(Device, getDeviceInfo) {
    SCOPE_IN;

    Device* obj = JS_OBJECT(Device, args.This());
    v8::Local<v8::Object> res = v8::Object::New();

    if (obj->hmd != NULL) {
      deviceInfo info = ovrHmd_GetDeviceInfo(obj->hmd);
      res->Set(v8::String::NewSymbol("displayDeviceName"), v8::String::NewSymbol(info.DisplayDeviceName));
      res->Set(v8::String::NewSymbol("displayId"), JS_FLOAT(info.DisplayId));
      res->Set(v8::String::NewSymbol("lensSeperationDistance"), JS_FLOAT(info.LensSeparationDistance));
      res->Set(v8::String::NewSymbol("eyeToScreenDistance"), JS_FLOAT(info.EyeToScreenDistance));
      res->Set(v8::String::NewSymbol("screenCenter"), JS_FLOAT(info.ScreenCenter));
      res->Set(v8::String::NewSymbol("interpupillaryDistance"), JS_FLOAT(info.InterpupillaryDistance));
      res->Set(v8::String::NewSymbol("leftMaxR"), JS_FLOAT(info.LeftMaxR));
      res->Set(v8::String::NewSymbol("rightMaxR"), JS_FLOAT(info.RightMaxR));

      v8::Local<v8::Array> screenSize = v8::Array::New(2);
      screenSize->Set(0, JS_FLOAT(info.ScreenSizeW));
      screenSize->Set(1, JS_FLOAT(info.ScreenSizeH));
      res->Set(v8::String::NewSymbol("screenSize"), screenSize);

      v8::Local<v8::Array> resolution = v8::Array::New(2);
      resolution->Set(0, JS_FLOAT(info.ResolutionW));
      resolution->Set(1, JS_FLOAT(info.ResolutionH));
      res->Set(v8::String::NewSymbol("resolution"), resolution);

      // distortion coefficients
      v8::Local<v8::Array> leftDistortion = v8::Array::New(11);
      v8::Local<v8::Array> rightDistortion = v8::Array::New(11);
      for(int i = 0; i < 11; i++) {
        leftDistortion->Set(i,  JS_FLOAT(info.LeftDistortionK[i]));
        rightDistortion->Set(i, JS_FLOAT(info.RightDistortionK[i]));
      }
      res->Set(v8::String::NewSymbol("leftDistortion"), leftDistortion);
      res->Set(v8::String::NewSymbol("rightDistortion"), rightDistortion);

      // chromatic aberration coefficients
      v8::Local<v8::Array> leftChromaticAberration = v8::Array::New(4);
      v8::Local<v8::Array> rightChromaticAberration = v8::Array::New(4);
      for(int i = 0; i < 4; i++) {
        leftChromaticAberration->Set(i,  JS_FLOAT(info.LeftChromaticAberration[i]));
        rightChromaticAberration->Set(i, JS_FLOAT(info.RightChromaticAberration[i]));
      }
      res->Set(v8::String::NewSymbol("leftChromaticAberration"), leftChromaticAberration);
      res->Set(v8::String::NewSymbol("rightChromaticAberration"), rightChromaticAberration);

      // some distortion info
      v8::Local<v8::Array> leftDistortionTanEyeAngleScale = v8::Array::New(2);
      v8::Local<v8::Array> rightDistortionTanEyeAngleScale = v8::Array::New(2);
      v8::Local<v8::Array> leftDistortionLensCenter = v8::Array::New(2);
      v8::Local<v8::Array> rightDistortionLensCenter = v8::Array::New(2);
      for(int i = 0; i < 2; i++) {
        leftDistortionTanEyeAngleScale->Set(i,  JS_FLOAT(info.LeftDistortionTanEyeAngleScale[i]));
        rightDistortionTanEyeAngleScale->Set(i, JS_FLOAT(info.RightDistortionTanEyeAngleScale[i]));
        leftDistortionLensCenter->Set(i,  JS_FLOAT(info.LeftDistortionLensCenter[i]));
        rightDistortionLensCenter->Set(i, JS_FLOAT(info.RightDistortionLensCenter[i]));
      }
      res->Set(v8::String::NewSymbol("leftDistortionTanEyeAngleScale"), leftDistortionTanEyeAngleScale);
      res->Set(v8::String::NewSymbol("rightDistortionTanEyeAngleScale"), rightDistortionTanEyeAngleScale);
      res->Set(v8::String::NewSymbol("leftDistortionLensCenter"), leftDistortionLensCenter);
      res->Set(v8::String::NewSymbol("rightDistortionLensCenter"), rightDistortionLensCenter);

      v8::Local<v8::Array> leftEyeFov = v8::Array::New(4);
      leftEyeFov->Set(0, JS_FLOAT(info.LeftEyeFov[0]));
      leftEyeFov->Set(1, JS_FLOAT(info.LeftEyeFov[1]));
      leftEyeFov->Set(2, JS_FLOAT(info.LeftEyeFov[2]));
      leftEyeFov->Set(3, JS_FLOAT(info.LeftEyeFov[3]));
      res->Set(v8::String::NewSymbol("leftEyeFov"), leftEyeFov);

      v8::Local<v8::Array> rightEyeFov = v8::Array::New(4);
      rightEyeFov->Set(0, JS_FLOAT(info.RightEyeFov[0]));
      rightEyeFov->Set(1, JS_FLOAT(info.RightEyeFov[1]));
      rightEyeFov->Set(2, JS_FLOAT(info.RightEyeFov[2]));
      rightEyeFov->Set(3, JS_FLOAT(info.RightEyeFov[3]));
      res->Set(v8::String::NewSymbol("rightEyeFov"), rightEyeFov);
    }

    SCOPE_OUT(res);
  }

  // returns the delta from when the positional tracking data was last requested
  JS_METHOD(Device, getPositionDeltas) {
    SCOPE_IN;

    Device* obj = JS_OBJECT(Device, args.This());
    v8::Local<v8::Array> res = args.Length() > 0 && args[0]->IsArray() ? v8::Array::Cast(*args[0]) : v8::Array::New(3);
    ovrTrackingState trackingInfo;

    if (obj->hmd != NULL) {
      trackingInfo = ovrHmd_GetTrackingState(obj->hmd, 0.0);
      float currX = trackingInfo.HeadPose.ThePose.Position.x;
      float currY = trackingInfo.HeadPose.ThePose.Position.y;
      float currZ = trackingInfo.HeadPose.ThePose.Position.z;
      res->Set(0, JS_FLOAT(currX - obj->lastX));
      res->Set(1, JS_FLOAT(currY - obj->lastY));
      res->Set(2, JS_FLOAT(currZ - obj->lastZ));
      obj->lastX = currX;
      obj->lastY = currY;
      obj->lastZ = currZ;
    } else {
      printf("obj->hmd is NULL\n");
      res->Set(0, JS_FLOAT(0.0));
      res->Set(1, JS_FLOAT(0.0));
      res->Set(2, JS_FLOAT(0.0));
    }

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
      printf("obj->hmd is NULL\n");
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

  // return a flat row-major projection matrix
  JS_METHOD(Device, getOvrMatrix4f_Projection) {
    SCOPE_IN;
    Device * obj = JS_OBJECT(Device, args.This());
    v8::Local<v8::Array> res = v8::Array::New(16);

    if (obj->hmd == NULL) {
      SCOPE_OUT(res);
    }

    ovrFovPort fov;
    float znear;
    float zfar;
    ovrBool rightHanded;

    // if an object is passed, use that to construct our fov struct; if not, use hmd's default with input eye index (or 0 if no index specified)
    if (args[0]->IsObject()) {
      v8::Handle<v8::Object> object = v8::Handle<v8::Object>::Cast(args[0]);
      fov.UpTan = (float) object->Get(v8::String::New("UpTan"))->NumberValue();
      fov.DownTan = (float) object->Get(v8::String::New("DownTan"))->NumberValue();
      fov.LeftTan = (float) object->Get(v8::String::New("LeftTan"))->NumberValue();
      fov.RightTan = (float) object->Get(v8::String::New("RightTan"))->NumberValue();
    } else if (args[0]->IsNumber()) {
      int eye = args[0]->Int32Value();
      fov = obj->hmd->DefaultEyeFov[eye];
    } else {
      fov = obj->hmd->DefaultEyeFov[0];
    }
    
    // defaults to 0.1
    if (args[1]->IsNumber()) {
      znear = (float) args[1]->NumberValue();
    } else {
      znear = 0.1;
    }

    // defaults to 1000
    if (args[2]->IsNumber()) {
      zfar = (float) args[2]->NumberValue();
    } else {
      zfar = 1000.0f;
    }

    // defaults to true
    if (args[3]->IsNumber()) {
      rightHanded = (ovrBool) args[3]->Int32Value();
    } else {
      rightHanded = (ovrBool) 1;
    }

    ovrMatrix4f proj = ovrMatrix4f_Projection(fov, znear, zfar, rightHanded);

    // stores in row-major order
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        res->Set(4 * i + j, JS_FLOAT(proj.M[i][j]));
      }
    }

    SCOPE_OUT(res);
  }
*/

  NAN_METHOD(Device::getTrackingData) {
    Device* obj = Nan::ObjectWrap::Unwrap<Device>(info.This());

    ovrTrackingState ts = ovr_GetTrackingState(
      obj->session, ovr_GetTimeInSeconds(), ovrTrue);

    v8::Local<v8::Object> result = Nan::New<v8::Object>();
    v8::Local<v8::Object> headPose = Nan::New<v8::Object>();
    Nan::Set(result, Nan::New<v8::String>("headPose").ToLocalChecked(), headPose);
    v8::Local<v8::Object> headPoseThePose = Nan::New<v8::Object>();
    Nan::Set(headPose, Nan::New<v8::String>("thePose").ToLocalChecked(), headPoseThePose);
    v8::Local<v8::Object> headPoseThePoseOrientation = Nan::New<v8::Object>();
    Nan::Set(headPoseThePose, Nan::New<v8::String>("orientation").ToLocalChecked(), headPoseThePoseOrientation);

    if (ts.StatusFlags & ovrStatus_OrientationTracked) {
      Nan::Set(headPoseThePoseOrientation, Nan::New<v8::String>("x").ToLocalChecked(),
        Nan::New<v8::Number>(ts.HeadPose.ThePose.Orientation.x));
      Nan::Set(headPoseThePoseOrientation, Nan::New<v8::String>("y").ToLocalChecked(),
        Nan::New<v8::Number>(ts.HeadPose.ThePose.Orientation.y));
      Nan::Set(headPoseThePoseOrientation, Nan::New<v8::String>("z").ToLocalChecked(),
        Nan::New<v8::Number>(ts.HeadPose.ThePose.Orientation.z));
      Nan::Set(headPoseThePoseOrientation, Nan::New<v8::String>("w").ToLocalChecked(),
        Nan::New<v8::Number>(ts.HeadPose.ThePose.Orientation.w));
    } else {
      Nan::Set(headPoseThePoseOrientation, Nan::New<v8::String>("error").ToLocalChecked(),
        Nan::New<v8::String>("Orientation not tracked").ToLocalChecked());
    }

    v8::Local<v8::Object> headPoseThePosePosition = Nan::New<v8::Object>();
    Nan::Set(headPoseThePose, Nan::New<v8::String>("position").ToLocalChecked(), headPoseThePosePosition);

    if (ts.StatusFlags & ovrStatus_OrientationTracked) {
      Nan::Set(headPoseThePosePosition, Nan::New<v8::String>("x").ToLocalChecked(),
        Nan::New<v8::Number>(ts.HeadPose.ThePose.Position.x));
      Nan::Set(headPoseThePosePosition, Nan::New<v8::String>("y").ToLocalChecked(),
        Nan::New<v8::Number>(ts.HeadPose.ThePose.Position.y));
      Nan::Set(headPoseThePosePosition, Nan::New<v8::String>("z").ToLocalChecked(),
        Nan::New<v8::Number>(ts.HeadPose.ThePose.Position.z));
    } else {
      Nan::Set(headPoseThePosePosition, Nan::New<v8::String>("error").ToLocalChecked(),
        Nan::New<v8::String>("Position not tracked").ToLocalChecked());
    }

/*
    if (obj->hmd != NULL) {
      trackingInfo = ovrHmd_GetTrackingState(obj->hmd, 0.0f);
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
*/

    info.GetReturnValue().Set(result);
  }

  void Device::Init(v8::Handle<v8::Object> target) {
    Nan::HandleScope scope;

    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New<v8::String>("Device").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(4);

    Nan::SetPrototypeMethod(tpl, "destroyResources", destroyResources);
    Nan::SetPrototypeMethod(tpl, "discoverSensor", discoverSensor);
    // Nan::SetPrototypeMethod(tpl, "getDeviceInfo", getDeviceInfo);
    // Nan::SetPrototypeMethod(tpl, "getPositionDeltas", getPositionDeltas);
    // Nan::SetPrototypeMethod(tpl, "getOrientationQuat", getOrientationQuat);
    Nan::SetPrototypeMethod(tpl, "getTrackingData", getTrackingData);
    // Nan::SetPrototypeMethod(tpl, "getOvrMatrix4f_Projection", getOvrMatrix4f_Projection);

    // Declare constructor
    constructor.Reset(tpl);
    Nan::Set(target, Nan::New<v8::String>("Device").ToLocalChecked(),
      Nan::GetFunction(tpl).ToLocalChecked());
  }

  NAN_METHOD(Device::New) {
    Device* device = new Device();
    device->Wrap(info.This());
    info.GetReturnValue().Set(info.This());
  }

}
