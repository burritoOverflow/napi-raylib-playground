const addon = require("./build/Release/raylib_addon");

class RaylibWindow extends addon.RaylibWindow {
  constructor() {
    super();
  }

  static Color(r, g, b, a = 255) {
    return { r, g, b, a };
  }

  async gameLoop(updateCallback) {
    this.setTargetFPS(60);

    while (!this.windowShouldClose()) {
      this.beginDrawing();

      if (updateCallback) {
        await updateCallback();
      }

      this.endDrawing();
    }

    this.closeWindow();
  }
}

RaylibWindow.BLACK = { r: 0, g: 0, b: 0, a: 255 };
RaylibWindow.WHITE = { r: 255, g: 255, b: 255, a: 255 };
RaylibWindow.RED = { r: 255, g: 0, b: 0, a: 255 };
RaylibWindow.GREEN = { r: 0, g: 255, b: 0, a: 255 };
RaylibWindow.BLUE = { r: 0, g: 0, b: 255, a: 255 };

module.exports = RaylibWindow;
