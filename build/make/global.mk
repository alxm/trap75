Z_PROJECT_ROOT := $(realpath $(dir $(lastword $(MAKEFILE_LIST)))/../..)

F_CONFIG_APP_AUTHOR := alxm
F_CONFIG_APP_NAME := Trap75

F_CONFIG_BUILD_DEBUG := 1

F_CONFIG_COLOR_SCREEN_BORDER := 0x201040

F_CONFIG_DIR_SRC := trap75

F_CONFIG_FPS_RATE_TICK := 30
F_CONFIG_FPS_RATE_DRAW := $(F_CONFIG_FPS_RATE_TICK)

F_CONFIG_RENDER := SOFTWARE

F_CONFIG_PATH_GFX := $(shell find $(Z_PROJECT_ROOT)/assets/gfx -name "*.png")
F_CONFIG_PATH_GFX := $(F_CONFIG_PATH_GFX:$(Z_PROJECT_ROOT)/%=%)
F_CONFIG_PATH_SFX := $(shell find $(Z_PROJECT_ROOT)/assets/sfx -name "*.wav")
F_CONFIG_PATH_SFX := $(F_CONFIG_PATH_SFX:$(Z_PROJECT_ROOT)/%=%)
F_CONFIG_PATH_SRC_GEN := 1

F_CONFIG_SCREEN_FORMAT := F_COLOR_FORMAT_RGB_565
F_CONFIG_SCREEN_SIZE_WIDTH := 80
F_CONFIG_SCREEN_SIZE_HEIGHT := 64

F_CONFIG_SOUND_VOLUME_ADJUSTABLE := 0
F_CONFIG_SOUND_VOLUME_SCALE_MUSIC := 20
F_CONFIG_SOUND_VOLUME_SCALE_SAMPLE := 20
