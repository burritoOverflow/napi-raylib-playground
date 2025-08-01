{
    "targets": [
        {
            "target_name": "raylib_addon",
            "sources": ["src/addon.cpp"],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
                "/opt/raylib/include",
            ],
            "libraries": ["/opt/raylib/lib/libraylib.a"],
            "cflags!": ["-fno-exceptions"],
            "cflags_cc!": ["-fno-exceptions"],
            "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
            "conditions": [
                [
                    "OS=='linux'",
                    {
                        "libraries": [
                            "-lGL",
                            "-lm",
                            "-lpthread",
                            "-ldl",
                            "-lrt",
                            "-lX11",
                        ]
                    },
                ],
            ],
        }
    ]
}
