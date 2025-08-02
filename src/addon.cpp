#include <napi.h>
#include <raylib.h>

#include <atomic>
#include <thread>

Color NapiObjectToColor(const Napi::Object& obj) {
  const Color color = {
      (unsigned char)obj.Get("r").As<Napi::Number>().Uint32Value(),
      (unsigned char)obj.Get("g").As<Napi::Number>().Uint32Value(),
      (unsigned char)obj.Get("b").As<Napi::Number>().Uint32Value(),
      (unsigned char)obj.Get("a").As<Napi::Number>().Uint32Value()};
  return color;
}

Vector3 NapiObjectToVector3(const Napi::Object& obj) {
  const Vector3 vec = {obj.Get("x").As<Napi::Number>().FloatValue(),
                       obj.Get("y").As<Napi::Number>().FloatValue(),
                       obj.Get("z").As<Napi::Number>().FloatValue()};
  return vec;
}

Camera3D NapiObjectToCamera3D(const Napi::Object& obj) {
  Camera3D camera;

  camera.position = NapiObjectToVector3(obj.Get("position").As<Napi::Object>());
  camera.target = NapiObjectToVector3(obj.Get("target").As<Napi::Object>());
  camera.up = NapiObjectToVector3(obj.Get("up").As<Napi::Object>());
  camera.fovy = obj.Get("fovy").As<Napi::Number>().FloatValue();
  camera.projection = obj.Get("projection").As<Napi::Number>().Int32Value();

  return camera;
}

Napi::Value BindInitWindow(const Napi::CallbackInfo& info) {
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

  return env.Undefined();
}

Napi::Value BindCloseWindow(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (::IsWindowReady()) {
    ::CloseWindow();
  }
  return env.Undefined();
}

Napi::Value BindIsWindowReady(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::Boolean::New(env, ::IsWindowReady());
}

Napi::Value BindIsKeyPressed(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected 1 argument")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  int key = info[0].As<Napi::Number>().Int32Value();
  return Napi::Boolean::New(env, ::IsKeyPressed(key));
}

Napi::Value BindWindowShouldClose(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::Boolean::New(env, ::WindowShouldClose());
}

Napi::Value BindBeginDrawing(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  ::BeginDrawing();
  return env.Undefined();
}

Napi::Value BindEndDrawing(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  ::EndDrawing();
  return env.Undefined();
}

Napi::Value BindClearBackground(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  Color color;
  if (info.Length() >= 1 && info[0].IsObject()) {
    Napi::Object colorObj = info[0].As<Napi::Object>();
    color = NapiObjectToColor(colorObj);
  }

  ::ClearBackground(color);
  return env.Undefined();
}

Napi::Value BindDrawText(const Napi::CallbackInfo& info) {
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

  Color color;

  if (info[4].IsObject()) {
    Napi::Object colorObj = info[4].As<Napi::Object>();
    color = NapiObjectToColor(colorObj);
  }

  ::DrawText(text.c_str(), posX, posY, fontSize, color);
  return env.Undefined();
}

Napi::Value BindDrawCube(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 5) {
    Napi::TypeError::New(env, "Expected 5 arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Object posObj = info[0].As<Napi::Object>();
  Vector3 position = NapiObjectToVector3(posObj);

  float width = info[1].As<Napi::Number>().FloatValue();
  float height = info[2].As<Napi::Number>().FloatValue();
  float depth = info[3].As<Napi::Number>().FloatValue();

  Napi::Object colorObj = info[4].As<Napi::Object>();
  Color color = NapiObjectToColor(colorObj);

  ::DrawCube(position, width, height, depth, color);
  return env.Undefined();
}

Napi::Value BindBeginMode3D(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1 || !info[0].IsObject()) {
    Napi::TypeError::New(env, "Expected a camera object")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Object cameraObj = info[0].As<Napi::Object>();
  Camera3D camera = NapiObjectToCamera3D(cameraObj);

  ::BeginMode3D(camera);
  return env.Undefined();
}

Napi::Value BindEndMode3D(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  ::EndMode3D();
  return env.Undefined();
}

Napi::Value BindUpdateCamera(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Expected 2 arguments: camera object and mode")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[0].IsObject()) {
    Napi::TypeError::New(env, "Expected a camera object")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!info[1].IsNumber()) {
    Napi::TypeError::New(env, "Expected a number for camera mode")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  Napi::Object cameraObj = info[0].As<Napi::Object>();
  Camera3D camera = NapiObjectToCamera3D(cameraObj);
  const int cameraMode = info[1].As<Napi::Number>().Int32Value();
  ::UpdateCamera(&camera, cameraMode);

  // Update the JS object with the new camera state
  cameraObj.Set("position", Napi::Object::New(env));
  cameraObj.Get("position").As<Napi::Object>().Set("x", camera.position.x);
  cameraObj.Get("position").As<Napi::Object>().Set("y", camera.position.y);
  cameraObj.Get("position").As<Napi::Object>().Set("z", camera.position.z);

  return env.Undefined();
}

Napi::Value BindDrawGrid(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Expected 2 arguments: slices and spacing")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  const int slices = info[0].As<Napi::Number>().Int32Value();
  const float spacing = info[1].As<Napi::Number>().FloatValue();

  ::DrawGrid(slices, spacing);
  return env.Undefined();
}

Napi::Value BindMeasureText(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Expected 2 arguments")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  // TODO: validate input types?
  const std::string text = info[0].As<Napi::String>().Utf8Value();
  const int fontSize = info[1].As<Napi::Number>().Int32Value();

  const int width = ::MeasureText(text.c_str(), fontSize);
  return Napi::Number::New(env, width);
}

Napi::Value BindSetTargetFPS(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected 1 argument")
        .ThrowAsJavaScriptException();
    return env.Null();
  }

  const int fps = info[0].As<Napi::Number>().Int32Value();
  ::SetTargetFPS(fps);
  return env.Undefined();
};

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  exports.Set("initWindow", Napi::Function::New(env, BindInitWindow));
  exports.Set("closeWindow", Napi::Function::New(env, BindCloseWindow));
  exports.Set("isWindowReady", Napi::Function::New(env, BindIsWindowReady));
  exports.Set("windowShouldClose",
              Napi::Function::New(env, BindWindowShouldClose));
  exports.Set("beginDrawing", Napi::Function::New(env, BindBeginDrawing));
  exports.Set("endDrawing", Napi::Function::New(env, BindEndDrawing));
  exports.Set("isKeyPressed", Napi::Function::New(env, BindIsKeyPressed));
  exports.Set("clearBackground", Napi::Function::New(env, BindClearBackground));
  exports.Set("drawText", Napi::Function::New(env, BindDrawText));
  exports.Set("setTargetFPS", Napi::Function::New(env, BindSetTargetFPS));
  exports.Set("measureText", Napi::Function::New(env, BindMeasureText));
  exports.Set("drawCube", Napi::Function::New(env, BindDrawCube));
  exports.Set("beginMode3D", Napi::Function::New(env, BindBeginMode3D));
  exports.Set("endMode3D", Napi::Function::New(env, BindEndMode3D));
  exports.Set("updateCamera", Napi::Function::New(env, BindUpdateCamera));
  exports.Set("drawGrid", Napi::Function::New(env, BindDrawGrid));

  return exports;
}

NODE_API_MODULE(raylib_addon, Init)
