const raylib = require("./build/Release/raylib_addon");

const BLACK = { r: 0, g: 0, b: 0, a: 255 };
const WHITE = { r: 255, g: 255, b: 255, a: 255 };
const RED = { r: 255, g: 0, b: 0, a: 255 };
const GREEN = { r: 0, g: 255, b: 0, a: 255 };
const BLUE = { r: 0, g: 0, b: 255, a: 255 };
const KEY_Q = 81;

const CAMERA_PERSPECTIVE = 0;
const CAMERA_ORBITAL = 2;

const WIDTH = 1920;
const HEIGHT = 1080;

raylib.initWindow(WIDTH, HEIGHT, "Hello Raylib from Node.js");
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
    // avoid closing the window manually
    break;
  }

  raylib.updateCamera(camera, CAMERA_ORBITAL);

  raylib.beginDrawing();
  raylib.clearBackground(BLACK);

  raylib.beginMode3D(camera);

  raylib.drawCube({ x: 0, y: 1, z: 0 }, 2.0, 2.0, 2.0, GREEN);
  raylib.drawGrid(15, 1.0);

  raylib.endMode3D();

  const text = "Hello from Node.js + Raylib";
  const fontSize = 35;
  const textWidth = raylib.measureText(text, fontSize);

  raylib.drawText(
    text,
    WIDTH / 2 - textWidth / 2,
    HEIGHT / 2 - fontSize / 2,
    fontSize,
    WHITE
  );

  raylib.endDrawing();
}

raylib.closeWindow();
