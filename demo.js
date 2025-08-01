const RaylibWindow = require("./index");

async function main() {
  let window;

  try {
    window = new RaylibWindow();
  } catch (error) {
    console.error(`Failed to create window: ${error.message}`);
    return;
  }

  try {
    const width = 1920;
    const height = 1080;

    window.initWindow(width, height, "Hello Raylib from Node.js");

    const camera = {
      position: { x: 5.0, y: 5.0, z: 5.0 },
      target: { x: 0.0, y: 0.0, z: 0.0 },
      up: { x: 0.0, y: 1.0, z: 0.0 },
      fovy: 45.0,
      projection: 0, // CAMERA_PERSPECTIVE
    };

    await window.gameLoop(() => {
      window.clearBackground(RaylibWindow.BLACK);

      window.beginMode3D(camera);
      window.drawCube({ x: 0, y: 0, z: 0 }, 2.0, 2.0, 2.0, RaylibWindow.GREEN);
      window.endMode3D();

      const text = "Hello from Node.js + Raylib";
      const fontSize = 20;
      const textWidth = window.measureText(text, fontSize);

      window.drawText(
        text,
        width / 2 - textWidth / 2,
        height / 2 - fontSize / 2,
        fontSize,
        RaylibWindow.WHITE
      );
    });
  } catch (error) {
    console.error(`Runtime error: ${error.message}`);
    if (window) {
      try {
        window.closeWindow();
      } catch (closeError) {
        console.error("Error closing window:", closeError.message);
      }
    }
  }
}

main().catch(console.error);
