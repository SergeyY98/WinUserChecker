#include <napi.h>
#include <windows.h>
#include <lm.h>
#include <locale>
#include <codecvt>

#pragma comment(lib, "netapi32.lib")

Napi::Boolean IsUserExists(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wUsername = converter.from_bytes(info[0].As<Napi::String>().Utf8Value());

    USER_INFO_0 *buffer;
    DWORD entriesRead = 0;
    DWORD totalEntries = 0;
    DWORD resumeHandle = 0;

    NET_API_STATUS nStatus = NetUserEnum(NULL, 0, FILTER_NORMAL_ACCOUNT, (LPBYTE *)&buffer, MAX_PREFERRED_LENGTH, &entriesRead, &totalEntries, &resumeHandle);

    if (nStatus == NERR_Success) {
        for (DWORD i = 0; i < entriesRead; i++) {
            std::wstring wUser(buffer[i].usri0_name);
            if (wUser == wUsername) {
                NetApiBufferFree(buffer);
                return Napi::Boolean::New(env, true);
            }
        }
        NetApiBufferFree(buffer);
    }

    return Napi::Boolean::New(env, false);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "isUserExists"), Napi::Function::New(env, IsUserExists));
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)