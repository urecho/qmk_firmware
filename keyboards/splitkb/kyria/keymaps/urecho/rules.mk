OLED_ENABLE = yes          # Enables the use of OLED displays
ENCODER_ENABLE = yes       # Enables the use of one or more encoders
RGBLIGHT_ENABLE = yes      # Enable keyboard RGB underglow
WPM_ENABLE = yes
MOUSEKEY_ENABLE = yes
EXTRAKEY_ENABLE = yes
LTO_ENABLE = yes
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = pimoroni_trackball
CAPS_WORD_ENABLE = yes     # _NAV 의 CW_TOGG 동작

TAP_DANCE_ENABLE = no
STENO_ENABLE = no
BOOTMAGIC_ENABLE = no

GRAVE_ESC_ENABLE = no
MAGIC_ENABLE = no
SPACE_CADET_ENABLE = no
KEY_LOCK_ENABLE = no
AUDIO_ENABLE = no
AUTO_SHIFT_ENABLE = no
MIDI_ENABLE = no
CONSOLE_ENABLE = no
VELOCIKEY_ENABLE = no
LEADER_ENABLE = no
BACKLIGHT_ENABLE = no

EXTRAFLAGS += -flto

SRC += rotary_encoder.c
SRC += oled.c

