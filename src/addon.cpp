#include <napi.h>
#include <raylib.h>

#include <atomic>
#include <thread>

class RaylibWindow : public Napi::ObjectWrap<RaylibWindow> {
 private:
  bool windowInitialized{false};

 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(
        env, "RaylibWindow",
        {InstanceMethod("initWindow", &RaylibWindow::InitWindow),
         InstanceMethod("closeWindow", &RaylibWindow::CloseWindow),
         InstanceMethod("isWindowReady", &RaylibWindow::IsWindowReady),
         InstanceMethod("windowShouldClose", &RaylibWindow::WindowShouldClose),
         InstanceMethod("beginDrawing", &RaylibWindow::BeginDrawing),
         InstanceMethod("endDrawing", &RaylibWindow::EndDrawing),
         InstanceMethod("isKeyPressed", &RaylibWindow::IsKeyPressed),
         InstanceMethod("clearBackground", &RaylibWindow::ClearBackground),
         InstanceMethod("drawText", &RaylibWindow::DrawText),
         InstanceMethod("setTargetFPS", &RaylibWindow::SetTargetFPS),
         InstanceMethod("measureText", &RaylibWindow::MeasureText),
         InstanceMethod("drawCube", &RaylibWindow::DrawCube),
         InstanceMethod("beginMode3D", &RaylibWindow::BeginMode3D),
         InstanceMethod("endMode3D", &RaylibWindow::EndMode3D)});

    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("RaylibWindow", func);
    return exports;
  }

  RaylibWindow(const Napi::CallbackInfo& info)
      : Napi::ObjectWrap<RaylibWindow>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);
  }

  ~RaylibWindow() {
    if (windowInitialized && ::IsWindowReady()) {
      ::CloseWindow();
      windowInitialized = false;
    }
  }

  Napi::Value InitWindow(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 3) {
      Napi::TypeError::New(env, "Expected 3 arguments")
          .ThrowAsJavaScriptException();
      return env.Null();
    }

    const int width = info[0].As<Napi::Number>().Int32Value();
    const int height = info[1].As<Napi::Number>().Int32Value();
    const std::string title = info[2].As<Napi::String>().Utf8Value();

    ::InitWindow(width, height, title.c_str());
    windowInitialized = true;

    return env.Undefined();
  }

  Napi::Value CloseWindow(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (windowInitialized && ::IsWindowReady()) {
      ::CloseWindow();
      windowInitialized = false;
    }
    return env.Undefined();
  }

  Napi::Value IsWindowReady(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, ::IsWindowReady());
  }

  Napi::Value IsKeyPressed(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
      Napi::TypeError::New(env, "Expected 1 argument")
          .ThrowAsJavaScriptException();
      return env.Null();
    }

    int key = info[0].As<Napi::Number>().Int32Value();
    return Napi::Boolean::New(env, ::IsKeyPressed(key));
  }

  Napi::Value WindowShouldClose(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::Boolean::New(env, ::WindowShouldClose());
  }

  Napi::Value BeginDrawing(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    ::BeginDrawing();
    return env.Undefined();
  }

  Napi::Value EndDrawing(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    ::EndDrawing();
    return env.Undefined();
  }

  Napi::Value ClearBackground(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    // Default to black if no color provided
    Color color = BLACK;

    if (info.Length() >= 1 && info[0].IsObject()) {
      Napi::Object colorObj = info[0].As<Napi::Object>();
      color.r = colorObj.Get("r").As<Napi::Number>().Uint32Value();
      color.g = colorObj.Get("g").As<Napi::Number>().Uint32Value();
      color.b = colorObj.Get("b").As<Napi::Number>().Uint32Value();
      color.a = colorObj.Get("a").As<Napi::Number>().Uint32Value();
    }

    ::ClearBackground(color);
    return env.Undefined();
  }

  Napi::Value DrawText(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 5) {
      Napi::TypeError::New(env, "Expected 5 arguments")
          .ThrowAsJavaScriptException();
      return env.Null();
    }

    std::string text = info[0].As<Napi::String>().Utf8Value();
    int posX = info[1].As<Napi::Number>().Int32Value();
    int posY = info[2].As<Napi::Number>().Int32Value();
    int fontSize = info[3].As<Napi::Number>().Int32Value();

    Color color = WHITE;
    if (info[4].IsObject()) {
      Napi::Object colorObj = info[4].As<Napi::Object>();
      color.r = colorObj.Get("r").As<Napi::Number>().Uint32Value();
      color.g = colorObj.Get("g").As<Napi::Number>().Uint32Value();
      color.b = colorObj.Get("b").As<Napi::Number>().Uint32Value();
      color.a = colorObj.Get("a").As<Napi::Number>().Uint32Value();
    }

    ::DrawText(text.c_str(), posX, posY, fontSize, color);
    return env.Undefined();
  }

  Napi::Value DrawCube(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 5) {
      Napi::TypeError::New(env, "Expected 5 arguments")
          .ThrowAsJavaScriptException();
      return env.Null();
    }

    Napi::Object posObj = info[0].As<Napi::Object>();

    Vector3 position = {posObj.Get("x").As<Napi::Number>().FloatValue(),
                        posObj.Get("y").As<Napi::Number>().FloatValue(),
                        posObj.Get("z").As<Napi::Number>().FloatValue()};

    float width = info[1].As<Napi::Number>().FloatValue();
    float height = info[2].As<Napi::Number>().FloatValue();
    float depth = info[3].As<Napi::Number>().FloatValue();

    Napi::Object colorObj = info[4].As<Napi::Object>();
    Color color = {
        (unsigned char)colorObj.Get("r").As<Napi::Number>().Uint32Value(),
        (unsigned char)colorObj.Get("g").As<Napi::Number>().Uint32Value(),
        (unsigned char)colorObj.Get("b").As<Napi::Number>().Uint32Value(),
        (unsigned char)colorObj.Get("a").As<Napi::Number>().Uint32Value()};

    ::DrawCube(position, width, height, depth, color);
    return env.Undefined();
  }

  Napi::Value BeginMode3D(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsObject()) {
      Napi::TypeError::New(env, "Expected a camera object")
          .ThrowAsJavaScriptException();
      return env.Null();
    }

    Napi::Object cameraObj = info[0].As<Napi::Object>();
    Napi::Object posObj = cameraObj.Get("position").As<Napi::Object>();
    Napi::Object targetObj = cameraObj.Get("target").As<Napi::Object>();
    Napi::Object upObj = cameraObj.Get("up").As<Napi::Object>();
    float fovy = cameraObj.Get("fovy").As<Napi::Number>().FloatValue();
    int projection =
        (int)cameraObj.Get("projection").As<Napi::Number>().Int32Value();

    Vector3 position = {posObj.Get("x").As<Napi::Number>().FloatValue(),
                        posObj.Get("y").As<Napi::Number>().FloatValue(),
                        posObj.Get("z").As<Napi::Number>().FloatValue()};

    Vector3 target = {targetObj.Get("x").As<Napi::Number>().FloatValue(),
                      targetObj.Get("y").As<Napi::Number>().FloatValue(),
                      targetObj.Get("z").As<Napi::Number>().FloatValue()};

    Vector3 up = {upObj.Get("x").As<Napi::Number>().FloatValue(),
                  upObj.Get("y").As<Napi::Number>().FloatValue(),
                  upObj.Get("z").As<Napi::Number>().FloatValue()};

    Camera3D camera = {position, target, up, fovy, projection};

    ::BeginMode3D(camera);
    return env.Undefined();
  }

  Napi::Value EndMode3D(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    ::EndMode3D();
    return env.Undefined();
  }

  Napi::Value MeasureText(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2) {
      Napi::TypeError::New(env, "Expected 2 arguments")
          .ThrowAsJavaScriptException();
      return env.Null();
    }

    const std::string text = info[0].As<Napi::String>().Utf8Value();
    const int fontSize = info[1].As<Napi::Number>().Int32Value();

    const int width = ::MeasureText(text.c_str(), fontSize);
    return Napi::Number::New(env, width);
  }

  Napi::Value SetTargetFPS(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
      Napi::TypeError::New(env, "Expected 1 argument")
          .ThrowAsJavaScriptException();
      return env.Null();
    }

    const int fps = info[0].As<Napi::Number>().Int32Value();
    ::SetTargetFPS(fps);
    return env.Undefined();
  }
};

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  return RaylibWindow::Init(env, exports);
}

NODE_API_MODULE(raylib_addon, Init)