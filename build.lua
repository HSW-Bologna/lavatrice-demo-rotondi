local PROJECT = "lavatrice-demo"
local BUILD_FOLDER = "cache"
local NINJA_FILE = string.format("%s/build.ninja", BUILD_FOLDER)

local LVGL = "libs/lvgl/src"
local PMAN = "libs/c-page-manager/src"
local STOPWATCH = "libs/c-stopwatch/src"

return {
    configure = {
        description = "Configure the build",
        action = function()
            print("Configuring build...")

            local svad = require("svadilfari")

            local build = svad.newBuildConfiguration { buildFolder = BUILD_FOLDER }

            local rules = build:addCComponent {
                sourceDirs = { "src", LVGL, PMAN, STOPWATCH },
                includes = { ".", "src", "src/config", LVGL, PMAN, STOPWATCH },
                defines = { "BUILD_CONFIG_SIMULATED_APP=1", "BUILD_CONFIG_DISPLAY_HORIZONTAL_RESOLUTION=1280", "BUILD_CONFIG_DISPLAY_VERTICAL_RESOLUTION=400" },
                cflags = "-g",
                ldflags = "-lSDL2",
                app = PROJECT,
            }

            build:run { name = "run", input = rules.binary, command = rules.binary }

            build:defaults(rules.binary)

            io.popen(string.format("mkdir -p %s", BUILD_FOLDER))
            build:export(NINJA_FILE)
            io.popen(string.format("ninja -f %s -t compdb > compile_commands.json", NINJA_FILE))

            print("Build ready!")
        end
    },
    build = "ninja -f " .. NINJA_FILE,
    ninja = NINJA_FILE,
}
