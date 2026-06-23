#include <stdio.h>
#include QMK_KEYBOARD_H

enum layers {
    _BASE = 0,
    _MEDIA,
    _NAV,
    _MOUSE,
    _SYM,
    _NUM,
    _FUN
};

#ifdef ENCODER_ENABLE

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

void encoder_alt_tab(bool clockwise) {
  if (!is_alt_tab_active) {
    is_alt_tab_active = true;
    register_code(KC_LGUI);
  }

	if (clockwise) {
		alt_tab_timer = timer_read();
		tap_code16(S(KC_TAB));
	} else {
		alt_tab_timer = timer_read();
		tap_code16(KC_TAB);
	}
}

void matrix_scan_user(void) {
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1250) {
      unregister_code(KC_LGUI);
      is_alt_tab_active = false;
    }
  }
}

// Mouse scroll
void encoder_mouse_scroll(bool clockwise) {
    // Volume control
    if (clockwise) {
        tap_code(MS_WHLD);
        tap_code(MS_WHLD);
    } else {
        tap_code(MS_WHLU);
        tap_code(MS_WHLU);
    }
}

// Volume control
void encoder_volume_control(bool clockwise) {
    if (clockwise) {
        tap_code(KC_VOLD);
    } else {
        tap_code(KC_VOLU);
    }
}

// left/right
void encoder_left_right(bool clockwise) {
    if (clockwise) {
        tap_code(KC_LEFT);
    } else {
        tap_code(KC_RGHT);
    }
}

// page up/down
void encoder_page_up_down(bool clockwise) {
    if (clockwise) {
        tap_code(KC_PGUP);
    } else {
        tap_code(KC_PGDN);
    }
}

// zoom in/out
void encoder_zoom_in_out(bool clockwise) {
    if (clockwise) {
        tap_code16(LCTL(KC_MINS));
    } else {
        tap_code16(LCTL(KC_PLUS));
    }
}

// mouse vertical
void encoder_mouse_vertical(bool clockwise) {
    if (clockwise) {
        tap_code(MS_UP);
        tap_code(MS_UP);
    } else {
        tap_code(MS_DOWN);
        tap_code(MS_DOWN);
    }
}

// mouse horizontal
void encoder_mouse_horizontal(bool clockwise) {
    if (clockwise) {
        tap_code(MS_LEFT);
        tap_code(MS_LEFT);
    } else {
        tap_code(MS_RGHT);
        tap_code(MS_RGHT);
    }
}

// track prev/next (media)
void encoder_track_skip(bool clockwise) {
    if (clockwise) {
        tap_code(KC_MNXT);
    } else {
        tap_code(KC_MPRV);
    }
}


bool encoder_update_user(uint8_t index, bool clockwise) {
    // 의미 정합성 매핑:
    //   BASE  → mouse scroll / zoom         (그대로)
    //   MEDIA → volume       / track skip   (미디어 layer 본연)
    //   NAV   → alt-tab      / page up-down (navigation)
    //   MOUSE → mouse vert   / mouse horiz  (마우스 layer 본연)
    if (index == 0) {
        switch (get_highest_layer(layer_state)) {
            case _BASE:
                encoder_mouse_scroll(clockwise);
                break;
            case _MEDIA:
                encoder_volume_control(clockwise);
                break;
            case _NAV:
                encoder_alt_tab(clockwise);
                break;
            case _MOUSE:
                encoder_mouse_vertical(clockwise);
                break;
            default:
                break;
        }
    }
    else if (index == 1) {
        switch (get_highest_layer(layer_state)) {
            case _BASE:
                encoder_zoom_in_out(clockwise);
                break;
            case _MEDIA:
                encoder_track_skip(clockwise);
                break;
            case _NAV:
                encoder_page_up_down(clockwise);
                break;
            case _MOUSE:
                encoder_mouse_horizontal(clockwise);
                break;
            default:
                break;
        }
    }
    return false;
}

#endif

