#include QMK_KEYBOARD_H
#include <stdio.h>

extern char wpm_str[10];

enum layers {
    _BASE = 0,
    _MEDIA,
    _NAV,
    _MOUSE,
    _SYM,
    _NUM,
    _FUN
};

#ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    // master = 180 (flip). slave = default rotation (0).
    // kyria 의 좌/우 OLED 가 서로 반전 설치라 한쪽만 180 적용.
    if (is_keyboard_master()) {
        return OLED_ROTATION_180;
    }
    return rotation;
}

static void render_qmk_logo(void) {
  static const char PROGMEM qmk_logo[] = {
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0};

  oled_write_P(qmk_logo, false);
}

static void render_status(void) {
    // QMK Logo and version information
    render_qmk_logo();
    oled_write_P(PSTR("Kyria rev1.0\n\n"), false);

    // Host Keyboard Layer Status
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_P(PSTR("Base\n"), false);
            break;
        case _MEDIA:
            oled_write_P(PSTR("Media\n"), false);
            break;
        case _NAV:
            oled_write_P(PSTR("Nav\n"), false);
            break;
        case _MOUSE:
            oled_write_P(PSTR("Mouse\n"), false);
            break;
        case _SYM:
            oled_write_P(PSTR("Sym\n"), false);
            break;
        case _NUM:
            oled_write_P(PSTR("Num\n"), false);
            break;
        case _FUN:
            oled_write_P(PSTR("Fun\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
    }

    // Host Keyboard LED Status
    led_t led_usb_state = host_keyboard_led_state();
    oled_write_P(led_usb_state.num_lock    ? PSTR("NUMLCK ") : PSTR("       "), false);
    oled_write_P(led_usb_state.caps_lock   ? PSTR("CAPLCK ") : PSTR("       "), false);
    oled_write_P(led_usb_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("       "), false);
}

// Slave 측 정보 표시 — master 의 layer/caps/mod state 가 split sync 로 전달됨.
// (config.h 의 SPLIT_LAYER_STATE_ENABLE / SPLIT_MODS_ENABLE / SPLIT_WPM_ENABLE / SPLIT_LED_STATE_ENABLE)
static void render_slave_status(void) {
    oled_write_P(PSTR("Kyria\n\n"), false);

    // 현재 layer
    oled_write_P(PSTR("Layer:\n "), false);
    switch (get_highest_layer(layer_state)) {
        case _BASE:  oled_write_P(PSTR("Base\n"),  false); break;
        case _MEDIA: oled_write_P(PSTR("Media\n"), false); break;
        case _NAV:   oled_write_P(PSTR("Nav\n"),   false); break;
        case _MOUSE: oled_write_P(PSTR("Mouse\n"), false); break;
        case _SYM:   oled_write_P(PSTR("Sym\n"),   false); break;
        case _NUM:   oled_write_P(PSTR("Num\n"),   false); break;
        case _FUN:   oled_write_P(PSTR("Fun\n"),   false); break;
        default:     oled_write_P(PSTR("?\n"),     false);
    }

    // Layer Lock / Caps Word 상태
    if (is_layer_locked(get_highest_layer(layer_state))) {
        oled_write_P(PSTR("[LOCK]\n"), false);
    } else if (is_caps_word_on()) {
        oled_write_P(PSTR("[CAPS]\n"), false);
    } else {
        oled_write_P(PSTR("\n"), false);
    }

    // WPM (하단)
    oled_set_cursor(0, 7);
    sprintf(wpm_str, "WPM:%3d", get_current_wpm());
    oled_write(wpm_str, false);
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status();
    } else {
        render_slave_status();
    }
    return false;
}
#endif
