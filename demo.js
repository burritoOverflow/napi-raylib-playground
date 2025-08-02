const raylib = require("./build/Release/raylib_addon");

BLACK = { r: 0, g: 0, b: 0, a: 255 };
WHITE = { r: 255, g: 255, b: 255, a: 255 };
RED = { r: 255, g: 0, b: 0, a: 255 };
GREEN = { r: 0, g: 255, b: 0, a: 255 };
BLUE = { r: 0, g: 0, b: 255, a: 255 };
KEY_Q = 81;

const width = 1920;
const height = 1080;

raylib.initWindow(width, height, "Hello Raylib from Node.js");
raylib.setTargetFPS(60);

const camera = {
  position: { x: 5.0, y: 5.0, z: 5.0 },
  target: { x: 0.0, y: 0.0, z: 0.0 },
  up: { x: 0.0, y: 1.0, z: 0.0 },
  fovy: 45.0,
  projection: 0, // CAMERA_PERSPECTIVE
};

while (!raylib.windowShouldClose()) {
  // perhaps this should be exposed to the callers?
  if (raylib.isKeyPressed(KEY_Q)) {
    raylib.closeWindow();
  }

  raylib.clearBackground(BLACK);
  raylib.beginDrawing();

  raylib.beginMode3D(camera);
  raylib.drawCube({ x: 0, y: 0, z: 0 }, 2.0, 2.0, 2.0, GREEN);
  raylib.endMode3D();

  const text = "Hello from Node.js + Raylib";
  const fontSize = 20;
  const textWidth = raylib.measureText(text, fontSize);

  raylib.drawText(
    text,
    width / 2 - textWidth / 2,
    height / 2 - fontSize / 2,
    fontSize,
    BLACK
  );

  raylib.endDrawing();
}

raylib.closeWindow();
