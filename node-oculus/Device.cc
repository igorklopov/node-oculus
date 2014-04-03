#ifndef BUILDING_NODE_EXTENSION
  #define BUILDING_NODE_EXTENSION
#endif

#include <node.h>
#include "Device.h"

namespace nodeOculus {
  v8::Persistent<v8::Function> Device::constructor;

  Device::Device() {
    printf("Initializing OVR.\n");
    OVR::System::Init(OVR::Log::ConfigureDefaultLog(OVR::LogMask_All));
  }

  Device::~Device() {
  }


  JS_METHOD(Device, destroyResources) {
    SCOPE_IN;
    Device* obj = JS_OBJECT(Device, args.This());

    if (obj->sensor != NULL) { 
      printf("Deleting Sensor.\n");
      //delete obj->sensor; 
      obj->sensor.Clear();
    }

    if (obj->hmd != NULL) { 
      printf("Deleting HMD.\n");
      //delete obj->hmd; 
      obj->hmd.Clear();
    }

    if (obj->deviceManager != NULL) { 
      printf("Deleting Device Manager.\n");
      //delete obj->deviceManager; 
      obj->deviceManager.Clear();
    }

    if (obj->sensorFusion != NULL) { 
      printf("Deleting Sensor Fusion.\n");
      delete obj->sensorFusion; 
    }

    printf("Destroying OVR.\n");
    OVR::System::Destroy();
    printf("OVR Destroyed.\n");
    SCOPE_OUT(JS_BOOL(true));
  }


  JS_METHOD(Device, discoverSensor) {
    v8::HandleScope scope;
    Device* obj = JS_OBJECT(Device, args.This());

    printf("1: Creating DeviceManager.\n");
    obj->deviceManager = *OVR::DeviceManager::Create();

    printf("2: Creating SensorFusion.\n");
    obj->sensorFusion = new OVR::SensorFusion();

    if (obj->deviceManager != NULL) {
      printf("3: Creating HMD by Enumerating Devices.\n");
      obj->hmd = *obj->deviceManager->EnumerateDevices<OVR::HMDDevice>().CreateDevice();
      if (obj->hmd != NULL) {
        printf("3: HMD Created Sucessfully.\n\n");

        printf("4: Creating Sensor.\n");
        obj->sensor = *obj->hmd->GetSensor();
        if (obj->sensor != NULL) {
          printf("4: Sensor Created Sucessfully.\n\n");
          if (obj->sensorFusion != NULL) {
            printf("5: Attaching Sensor to SensorFusion.\n");
            obj->sensorFusion->AttachToSensor(obj->sensor);
            printf("5: Sensor Attached to SensorFusion.\n");
            printf("Sensor Sucessfully discovered!\n");
            return scope.Close(v8::Boolean::New(true));
          }
        }
      }
    }
    
    printf("Sensor Discovery Failed.\n");

    if (obj->deviceManager != NULL) { 
      printf("Deleting Device Manager.\n");
      delete obj->deviceManager; 
    }
    if (obj->sensorFusion != NULL) { 
      printf("Deleting Sensor Fusion.\n");
      delete obj->sensorFusion; 
    }
    if (obj->hmd != NULL) { 
      printf("Deleting hmd.\n");
      delete obj->hmd; 
    }
    if (obj->sensor != NULL) { 
      printf("Deleting sensor.\n");
      delete obj->sensor; 
    }
    
    return scope.Close(v8::Boolean::New(false));
  }

  JS_METHOD(Device, getDeviceInfo) {
    v8::HandleScope scope;
    Device* obj = JS_OBJECT(Device, args.This());
    v8::Local<v8::Object> res = v8::Object::New();

    OVR::HMDInfo info;
    if (obj->hmd != NULL && obj->hmd->GetDeviceInfo(&info)) {
      res->Set(v8::String::NewSymbol("displayDeviceName"), v8::String::NewSymbol(info.DisplayDeviceName));
      res->Set(v8::String::NewSymbol("displayId"), v8::Number::New(info.DisplayId));
      res->Set(v8::String::NewSymbol("vScreenCenter"), v8::Number::New(info.VScreenCenter));
      res->Set(v8::String::NewSymbol("eyeToScreenDistance"), v8::Number::New(info.EyeToScreenDistance));
      res->Set(v8::String::NewSymbol("lensSeperationDistance"), v8::Number::New(info.LensSeparationDistance));
      res->Set(v8::String::NewSymbol("interpupillaryDistance"), v8::Number::New(info.InterpupillaryDistance));


      v8::Local<v8::Array> screenSize = v8::Array::New(2);
      screenSize->Set(0, v8::Number::New(info.HScreenSize));
      screenSize->Set(1, v8::Number::New(info.VScreenSize));
      res->Set(v8::String::NewSymbol("screenSize"), screenSize);

      v8::Local<v8::Array> resolution = v8::Array::New(2);
      resolution->Set(0, v8::Number::New(info.HResolution));
      resolution->Set(1, v8::Number::New(info.VResolution));
      res->Set(v8::String::NewSymbol("resolution"), resolution);

      v8::Local<v8::Array> distortion = v8::Array::New(4);
      distortion->Set(0, v8::Number::New(info.DistortionK[0]));
      distortion->Set(1, v8::Number::New(info.DistortionK[1]));
      distortion->Set(2, v8::Number::New(info.DistortionK[2]));
      distortion->Set(3, v8::Number::New(info.DistortionK[3]));
      res->Set(v8::String::NewSymbol("distortion"), distortion);
    }

    return scope.Close(res);
  }

  JS_METHOD(Device, getOrientationQuat) {
    v8::HandleScope scope;
    Device* obj = JS_OBJECT(Device, args.This());
    v8::Local<v8::Array> res = v8::Array::New(4);

    if (obj->sensor != NULL) {
      OVR::Quatf orientation = obj->sensorFusion->GetOrientation();
      res->Set(0, v8::Number::New(orientation.x));
      res->Set(1, v8::Number::New(orientation.y));
      res->Set(2, v8::Number::New(orientation.z));
      res->Set(3, v8::Number::New(orientation.w));
    } else {
      res->Set(0, v8::Number::New(0.0));
      res->Set(1, v8::Number::New(0.0));
      res->Set(2, v8::Number::New(0.0));
      res->Set(3, v8::Number::New(1.0));
    }

    return scope.Close(res);
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

