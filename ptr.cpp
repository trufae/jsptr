// buffer_example.cpp
#include <nan.h>
using namespace Nan;
using namespace v8;

#if 0
NAN_METHOD(ptr) {
	v8::Local<v8::Context> ctxt = info.GetIsolate()->GetCurrentContext();


	int *buffer = (int *)&ctxt;

#if 0
	size_t buffer = info[0];
    char* buffer = (char*) node::Buffer::Data(info[0]->ToObject());
    unsigned int size = info[1]->Uint32Value();
    unsigned int rot = info[2]->Uint32Value();
#endif

    fprintf (stderr, "%p\n", buffer);
   
#if 0
    char * retval = new char[size];
    for(unsigned int i = 0; i < size; i++ ) {
        retval[i] = buffer[i] - rot;
        buffer[i] += rot;
    }   
    
   info.GetReturnValue().Set(Nan::NewBuffer(retval, size).ToLocalChecked());
#endif
}
#endif
void ptr(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  if (info.Length() < 1) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }
  char *data;
   if (info[0]->IsUint8Array()) {
        // It's a Buffer (which is a Uint8Array)
        v8::Local<v8::Uint8Array> buffer = info[0].As<v8::Uint8Array>();
        std::shared_ptr<v8::BackingStore> backing = buffer->Buffer()->GetBackingStore();
        data = static_cast<char*>(backing->Data());
    } else if (info[0]->IsArrayBuffer()) {
        // It's an ArrayBuffer
        v8::Local<v8::ArrayBuffer> arrayBuffer = info[0].As<v8::ArrayBuffer>();
        std::shared_ptr<v8::BackingStore> backing = arrayBuffer->GetBackingStore();
	data = static_cast<char*>(backing->Data());
    } else if (info[0]->IsTypedArray()) {
	    // Handle Typed Arrays such as Int32Array
	    v8::Local<v8::TypedArray> typedArray = info[0].As<v8::TypedArray>();
	    std::shared_ptr<v8::BackingStore> backing = typedArray->Buffer()->GetBackingStore();
	    data = static_cast<char*>(backing->Data());
    } else {
        Nan::ThrowTypeError("Argument must be a Buffer or ArrayBuffer");
        return;
    }
    fprintf (stderr, "%p\n", data);
    int *tb = (int*)data;
    fprintf (stderr, "%d %d %d %d %d %d\n", tb[0], tb[1], tb[2], tb[3], tb[4], tb[5]);
}

NAN_MODULE_INIT(Init) {
   Nan::Set(target, New<String>("ptr").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(ptr)).ToLocalChecked());
}

NODE_MODULE(ptr, Init)
