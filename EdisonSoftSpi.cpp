#include "softspi.h"

using v8::FunctionTemplate;

NAN_MODULE_INIT(InitAll) {
  Nan::Set(target, Nan::New("transfer").ToLocalChecked(),
    Nan::GetFunction(Nan::New<FunctionTemplate>(softSpi_transfer)).ToLocalChecked());
    Nan::Set(target, Nan::New("MODE0").ToLocalChecked(), Nan::New(0));
    Nan::Set(target, Nan::New("MODE1").ToLocalChecked(), Nan::New(1));
    Nan::Set(target, Nan::New("MODE2").ToLocalChecked(), Nan::New(2));
    Nan::Set(target, Nan::New("MODE3").ToLocalChecked(), Nan::New(3));
}

NODE_MODULE(EdisonSoftSpi, InitAll)
