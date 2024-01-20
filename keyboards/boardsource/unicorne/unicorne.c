// Copyright 2023 jack (@waffle87)
// SPDX-License-Identifier: GPL-2.0-or-later
#include "unicorne.h"

#ifdef OLED_ENABLE
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    if (!is_keyboard_master()) {
        return OLED_ROTATION_180;
    }
    return rotation;
}

bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }
    if (is_keyboard_master()) {
        switch (get_highest_layer(layer_state)) {
            case 0:
                oled_write_raw(layer_zero, sizeof(layer_zero));
                break;
            case 1:
                oled_write_raw(layer_one, sizeof(layer_one));
                oled_write_P(PSTR("LAYER: FUNCTIONS\n"), false);
                break;
            case 2:
                oled_write_raw(layer_two, sizeof(layer_two));
                oled_write_P(PSTR("LAYER: NUMBERS\n"), false);
                break;
            case 3:
                oled_write_raw(layer_three, sizeof(layer_three));
                oled_write_P(PSTR("LAYER: SYMBOLS\n"), false);
                break;
            case 4:
                oled_write_P(PSTR("LAYER: MEDIA\n"), false);
                break;
            case 5:
                oled_write_P(PSTR("LAYER: NAVIGATION\n"), false);
                break;
            case 6:
                oled_write_P(PSTR("LAYER: MANAGEMENT\n"), false);
                break;
        }
    } else {
        oled_write_raw(logo, sizeof(logo));
    }
    return false;
}
#endif

void leader_start_user(void) {
    // Do something when the leader key is pressed
}

void leader_end_user(void) {
    if (leader_sequence_one_key(KC_F)) {
        // Leader, f => Types the below string
        //         SEND_STRING("QMK is awesome.");
        //             
    } else if (leader_sequence_two_keys(KC_D, KC_D)) {
        // Leader, d, d => Ctrl+A, Ctrl+C
        //         SEND_STRING(SS_LCTL("a") SS_LCTL("c"));
        //             
    } else if (leader_sequence_three_keys(KC_D, KC_D, KC_S)) {
        // Leader, d, d, s => Types the below string
        //         SEND_STRING("https://start.duckduckgo.com\n");
        //             
    } else if (leader_sequence_two_keys(KC_A, KC_S)) {
        // Leader, a, s => GUI+S
        //         tap_code16(LGUI(KC_S));
        //             
    }
}

