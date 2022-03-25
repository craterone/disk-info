
#include <napi.h>
#include <sys/statvfs.h>

#include <string>

Napi::Value DiskInfo(const Napi::CallbackInfo &info) {
  auto env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  const std::string path = info[0].ToString();

  struct statvfs stats = {};
  if (statvfs(path.c_str(), &stats)) {
    int errnum = errno;
    std::string err_mes = "Stats error, num:" + std::to_string(errno) +
                          ", message:" + strerror(errnum);
    Napi::TypeError::New(env, err_mes).ThrowAsJavaScriptException();
    return env.Null();
  }

  auto available = stats.f_bavail * stats.f_frsize ;
  auto free = stats.f_bfree * stats.f_frsize;
  auto all = stats.f_blocks * stats.f_frsize;
  auto used = all - free;
  
  auto result = Napi::Object::New(env);
  result.Set("size", all);
  result.Set("free", free);
  result.Set("used", used);
  result.Set("available", available);

  return result;
}

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {

  exports.Set(Napi::String::New(env, "info"),
              Napi::Function::New(env, DiskInfo));

  return exports;
}

NODE_API_MODULE(addon, InitAll)