// Copyright 2023 jack (@waffle87)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

#define RGB_MATRIX_LED_COUNT 54
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_ALPHAS_MODS
#define RGB_DISABLE_WHEN_USB_SUSPENDED

#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET

#define I2C_DRIVER I2CD1
#define I2C1_SDA_PIN GP22
#define I2C1_SCL_PIN GP23

#define AUDIO_PIN GP28
#define AUDIO_PWM_DRIVER PWMD6
#define AUDIO_INIT_DELAY
#define AUDIO_PWM_CHANNEL RP2040_PWM_CHANNEL_A
#define STARTUP_SONG SONG(ODE_TO_JOY)

#define ANALOG_JOYSTICK_Y_AXIS_PIN GP26
#define ANALOG_JOYSTICK_X_AXIS_PIN GP27
#define POINTING_DEVICE_INVERT_Y
#define POINTING_DEVICE_INVERT_X

// hyper
#define HPR_ESC HYPR_T(KC_ESC)

// Home row mods : Left-side 
#define HR_A LCTL_T(KC_A)
#define HR_S LALT_T(KC_S)
#define HR_D LGUI_T(KC_D)
#define HR_F LSFT_T(KC_F)

// Home row mods : Right-side 
#define HR_J RSFT_T(KC_J)
#define HR_K RGUI_T(KC_K)
#define HR_L LALT_T(KC_L)
#define HR_SCLN RCTL_T(KC_SCLN)

// Layer Tap
// function
#define LT_1 LT(1,KC_TAB)
// number
#define LT_2 LT(2,KC_ESC)
// symbol
#define LT_3 LT(3,KC_SPC)
// media
#define LT_4 LT(4,KC_ENT)
// navigation
#define LT_5 LT(5,KC_BSPC)
// mamagement
#define LT_6 LT(6,KC_DEL)
// layer toggle
#define TAPPING_TOGGLE 2
