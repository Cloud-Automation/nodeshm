#include <node.h>

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

void unlink(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    auto* isolate = args.GetIsolate();

    if (args.Length() != 1) {
        isolate->ThrowException(
            v8::Exception::Error(
                v8::String::NewFromUtf8(isolate, "shm_unlink() takes 1 arguments: name.")));
        return;
    }

    v8::String::Utf8Value str(args[0]->ToString());

    char* name = (char*)calloc(1, str.length() + 1);

    strcpy(name, (char*)*str);

    int retVal = shm_unlink(name);

    free(name);

    args.GetReturnValue().Set(retVal);
}

void open(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    auto* isolate = args.GetIsolate();

    if (args.Length() != 3) {
        isolate->ThrowException(
            v8::Exception::Error(
                v8::String::NewFromUtf8(isolate, "shm_open() takes 3 arguments: name, oflag and mode.")));
        return;
    }

    const int oflag = args[1]->ToInteger()->Value();
    const mode_t mode = args[2]->ToInteger()->Value();

    v8::String::Utf8Value str(args[0]->ToString());
    char* name = (char*)calloc(1, str.length());
    strcpy(name, (char*)*str);

    int retVal = shm_open(name, oflag, mode);

    free(name);

    args.GetReturnValue().Set(retVal);
}

static void RegisterModule(v8::Local<v8::Object> exports)
{
    NODE_SET_METHOD(exports, "shm_open", open);
    NODE_SET_METHOD(exports, "shm_unlink", unlink);
    NODE_DEFINE_CONSTANT(exports, O_RDONLY);
    NODE_DEFINE_CONSTANT(exports, O_RDWR);
    NODE_DEFINE_CONSTANT(exports, O_CREAT);
    NODE_DEFINE_CONSTANT(exports, O_EXCL);
    NODE_DEFINE_CONSTANT(exports, O_TRUNC);
}

NODE_MODULE(mmap, RegisterModule);
