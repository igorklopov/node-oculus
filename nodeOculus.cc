#include "Device.h"

void init(v8::Handle<v8::Object> target) {
  Nan::HandleScope scope;
  nodeOculus::Device::Init(target);
}

NODE_MODULE(nodeOculus, init);
