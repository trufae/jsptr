#include <nan.h>
#include <sstream>

using namespace Nan;
using namespace v8;

void ptr(const Nan::FunctionCallbackInfo<v8::Value>& info) {
	if (info.Length() < 1) {
		Nan::ThrowTypeError("Wrong number of arguments");
		return;
	}
	const char *data;
	if (info[0]->IsUint8Array()) {
		// It's a Buffer (which is a Uint8Array)
		v8::Local<v8::Uint8Array> buffer = info[0].As<v8::Uint8Array>();
		std::shared_ptr<v8::BackingStore> backing = buffer->Buffer()->GetBackingStore();
		data = static_cast<const char*>(backing->Data());
	} else if (info[0]->IsArrayBuffer()) {
		// It's an ArrayBuffer
		v8::Local<v8::ArrayBuffer> arrayBuffer = info[0].As<v8::ArrayBuffer>();
		std::shared_ptr<v8::BackingStore> backing = arrayBuffer->GetBackingStore();
		data = static_cast<const char*>(backing->Data());
	} else if (info[0]->IsTypedArray()) {
		// Handle Typed Arrays such as Int32Array
		v8::Local<v8::TypedArray> typedArray = info[0].As<v8::TypedArray>();
		std::shared_ptr<v8::BackingStore> backing = typedArray->Buffer()->GetBackingStore();
		data = static_cast<const char*>(backing->Data());
	} else if (info[0]->IsString()) {
		v8::String::Utf8Value utf8Value(info.GetIsolate(), info[0]);
		const char* str = *utf8Value;
		data = (const char *)static_cast<const char*>(str);
		printf ("STR(%s) = %p\n", data, data);
#if 0
		v8::Local<v8::String::Utf8Value> str = info[0].As<v8::String::Utf8Value>();
		//v8::Local<v8::String> str2 = info[0].As<v8::String>();
		// v8::String::Utf8Value str(info.GetIsolate(), info[0]);
		std::string cppStr(*str, str.length());
		// const void* address = *reinterpret_cast<const void* const*>(&str);
#endif
		v8::Local<v8::String> v8String = info[0].As<v8::String>();
		const void* addr = *reinterpret_cast<const void* const*>(&v8String);
		printf ("STR(%s) = %p\n", addr , addr );


		// info.GetReturnValue().Set(Nan::New(cppStr).ToLocalChecked());
	} else {
		Nan::ThrowTypeError("Argument must be a Buffer or ArrayBuffer");
		return;
	}
#if 0
	int *tb = (int*)data;
	fprintf (stderr, "%p\n", data);
	fprintf (stderr, "%d %d %d %d %d %d\n", tb[0], tb[1], tb[2], tb[3], tb[4], tb[5]);
#endif
	std::stringstream addressStream;
	addressStream << static_cast<const void *>(data);

	info.GetReturnValue().Set(Nan::New(addressStream.str()).ToLocalChecked());
}

NAN_MODULE_INIT(Init) {
   Nan::Set(target, New<String>("ptr").ToLocalChecked(),
        GetFunction(New<FunctionTemplate>(ptr)).ToLocalChecked());
}

NODE_MODULE(ptr, Init)
