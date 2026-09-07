/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// Left-hand home row mods
#define HOME_A LCTL_T(KC_A)
#define HOME_S LALT_T(KC_S)
#define HOME_D LGUI_T(KC_D)
#define HOME_F LSFT_T(KC_F)

// // Right-hand home row mods
#define HOME_J RSFT_T(KC_J)
#define HOME_K RGUI_T(KC_K)
#define HOME_L LALT_T(KC_L)
#define HOME_SCLN RCTL_T(KC_SCLN)

// macOS screenshot: Ctrl+Cmd+Shift+4 = 드래그로 영역 선택 + 클립보드 저장
#define SS_DRAG LCTL(LGUI(LSFT(KC_4)))

// Custom keycode: tap = Layer Lock, hold = LCAG (Hyper).
// QK_LLCK 가 basic keycode 범위 밖이라 MT() 매크로 안 됨 → process_record_user 에서 직접 처리.
enum custom_keycodes {
    LCAG_LCK = SAFE_RANGE,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    static uint16_t lcag_timer = 0;
    if (keycode == LCAG_LCK) {
        if (record->event.pressed) {
            lcag_timer = timer_read();
            register_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI));
        } else {
            unregister_mods(MOD_BIT(KC_LCTL) | MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI));
            if (timer_elapsed(lcag_timer) < TAPPING_TERM) {
                // 짧게 tap → 현재 활성 layer 잠금/해제 토글
                // tap_code16(QK_LLCK) 가 흐름상 호출 안 됐던 듯, layer_lock_invert 직접 호출.
                uint8_t cur = get_highest_layer(layer_state);
                if (cur != 0) {  // 0 = _BASE — base 는 잠글 layer 없음
                    layer_lock_invert(cur);
                }
            }
        }
        return false;
    }
    return true;
}

// layer-tap 엄지 키는 다른 키가 눌리는 즉시 hold(레이어)로 확정.
// → SYM 등 레이어 조합 시 tap(ENT/BSPC/...)으로 오작동하던 문제 방지.
//   home row mod(mod-tap)·기타 키는 기본값 유지 (롤링 타이핑 오발화 방지).
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    if (IS_QK_LAYER_TAP(keycode)) {
        return true;
    }
    return false;
}

// Chordal Hold 예외.
// 엄지 layer-tap 은 손 판정 없이 hold 를 허용한다 — SYM/NUM/FUN 의 심볼이 레이어를 무는
// 엄지와 같은 손에 있어도 레이어로 동작해야 한다.
// 왼손 Cmd(HOME_D)는 같은 손 단축키 조합에 한해 hold 를 허용한다. 목록에 없는 키
// (스페이스 엄지 등)는 같은 손 규칙을 그대로 따라 tap 이 된다.
bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record,
                      uint16_t other_keycode, keyrecord_t *other_record) {
    if (IS_QK_LAYER_TAP(tap_hold_keycode)) {
        return true;
    }
    if (tap_hold_keycode == HOME_D) {
        switch (other_keycode) {
            case KC_Q: case KC_W: case KC_R: case KC_T: case KC_G:
            case KC_Z: case KC_X: case KC_C: case KC_V:
            case HOME_A: case HOME_S: case HOME_F:
                return true;
        }
    }
    return get_chordal_hold_default(tap_hold_record, other_record);
}

// Miryoku-style layer enum. Thumb cluster activates each layer via LT().
// _MEDIA / _NAV / _MOUSE → left thumb (ESC / SPC / TAB)
// _SYM / _NUM / _FUN → right thumb (ENT / BSPC / DEL)
enum layers {
    _BASE = 0,
    _MEDIA,
    _NAV,
    _MOUSE,
    _SYM,
    _NUM,
    _FUN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY + home row mods + miryoku-style thumb (only middle 3 used per side)
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |  ~     |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  | Bspc   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * | ESC    |   A  |   S  |  D   |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : | Enter  |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |LShift|LShift|  |LShift|LShift|   N  |   M  | ,  < | . >  | /  ? | RShift |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | XXX  | MEDIA| NAV  |MOUSE | LCAG |  | LCAG | SYM  | NUM  | FUN  | XXX  |
 *                        |      | /ESC | /SPC | /TAB |      |  |      | /ENT | /BSPC| /DEL |      |
 *                        `----------------------------------'  `----------------------------------'
 *                        외측 1 = 인코더 점유 (XXX). 내측 1 = LCAG (옛 Hyper 자리).
 */
    [_BASE] = LAYOUT(
      KC_GRV,         KC_Q,   KC_W,   KC_E,   KC_R,     KC_T,                                                   KC_Y,    KC_U,         KC_I,    KC_O,    KC_P,      KC_BSLS,
      KC_ESC,          HOME_A, HOME_S, HOME_D, HOME_F,   KC_G,                                                   KC_H,    HOME_J,       HOME_K,  HOME_L,  HOME_SCLN, KC_QUOT,
      KC_LSFT,         KC_Z,   KC_X,   KC_C,   KC_V,     KC_B,            KC_LSFT, KC_LSFT,   KC_LSFT, KC_LSFT,  KC_N,    KC_M,         KC_COMM, KC_DOT,  KC_SLSH,   KC_RSFT,
                                  XXXXXXX, LT(_MEDIA,KC_ESC), LT(_NAV,KC_SPC), LT(_MOUSE,KC_TAB), LCAG_LCK,   LCAG_LCK, LT(_SYM,KC_ENT), LT(_NUM,KC_BSPC), LT(_FUN,KC_DEL), XXXXXXX
    ),

/*
 * MEDIA Layer (left thumb outer = ESC hold): RGB + 미디어 + 스크린샷
 * 옛 _ADJUST 의 내용 + macOS 영역 스크린샷 (행 2 우측 외곽).
 */
    [_MEDIA] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, UG_TOGG, UG_SATU, UG_HUEU, UG_VALU, UG_NEXT,                                     KC_MUTE, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY, SS_DRAG,
      _______, _______, UG_SATD, UG_HUED, UG_VALD, UG_PREV, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

/*
 * NAV Layer (left thumb middle = SPC hold): vim 식 hjkl 화살표 + 편집 cluster + 보조 nav
 *
 *  좌측 (mod row)                          우측 (활성 hand)
 *  ,----+----+----+----+----+----.         ,----+----+----+----+----+----.
 *  |    |    |    |    |    |    |         |UNDO| CUT|COPY|PSTE|AGAN|    |
 *  +----+----+----+----+----+----+         +----+----+----+----+----+----+
 *  |    |LCTL|LALT|LGUI|LSFT|    |         |LEFT|DOWN| UP |RGHT|CWRD|    |   ← vim hjkl (H 자리부터)
 *  +----+----+----+----+----+----+         +----+----+----+----+----+----+
 *  |    |    |    |    |    |    |         | INS|HOME|PGDN|PGUP| END|    |
 *  `----+----+----+----+----+----+         +----+----+----+----+----+----'
 *  thumb 우측: 명시적 ENT/BSPC/DEL (NAV+SYM/NUM/FUN 동시 활성 충돌 회피)
 */
    [_NAV] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     KC_UNDO, KC_CUT,  KC_COPY, KC_PASTE,KC_AGAIN,_______,
      _______, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, _______,                                     KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, CW_TOGG, SS_DRAG,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_INS,  KC_HOME, KC_PGDN, KC_PGUP, KC_END,  _______,
                                 _______, _______, _______, _______, _______, _______, KC_ENT,  KC_BSPC, KC_DEL,  _______
    ),

/*
 * MOUSE Layer (left thumb inner = TAB hold): vim 식 hjkl 자리에 마우스 커서 + 휠 + 버튼
 *
 *  좌측 (mod row)                          우측 (활성 hand)
 *  ,----+----+----+----+----+----.         ,----+----+----+----+----+----.
 *  |    |    |    |    |    |    |         |UNDO| CUT|COPY|PSTE|AGAN|    |
 *  +----+----+----+----+----+----+         +----+----+----+----+----+----+
 *  |    |LCTL|LALT|LGUI|LSFT|    |         |MS_L|MS_D|MS_U|MS_R|    |    |
 *  +----+----+----+----+----+----+         +----+----+----+----+----+----+
 *  |    |    |    |    |    |    |         |WHLL|WHLD|WHLU|WHLR|    |    |
 *  `----+----+----+----+----+----+         +----+----+----+----+----+----'
 *  우 thumb: BTN2 BTN1 BTN3 (bare, MOUSE+SYM/NUM/FUN 동시 활성화 회피)
 */
    [_MOUSE] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     KC_UNDO, KC_CUT,  KC_COPY, KC_PASTE,KC_AGAIN,_______,
      _______, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, _______,                                     MS_LEFT, MS_DOWN, MS_UP,   MS_RGHT, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, MS_WHLL, MS_WHLD, MS_WHLU, MS_WHLR, _______, _______,
                                 _______, _______, _______, _______, _______, _______, MS_BTN2, MS_BTN1, MS_BTN3, _______
    ),

/*
 * SYM Layer (right thumb inner = ENT hold): 심볼 + 괄호. 양쪽 hand 에 전개.
 * 좌측 자리 = _NUM 의 숫자 자리 — 각 키는 그 숫자의 shift 문자.
 * 우측 행 1 외곽(base 의 \ 자리)은 파이프.
 *
 *  좌측 (숫자 자리 대응)                    우측
 *  ,----+----+----+----+----+----.         ,----+----+----+----+----+----.
 *  | ~  | !  | @  | #  | $  | %  |         | ^  | &  | *  | (  | )  | |  |
 *  +----+----+----+----+----+----+         +----+----+----+----+----+----+
 *  |    | $  | %  | ^  | &  | *  |         |    | -  | =  | [  | ]  |    |
 *  +----+----+----+----+----+----+         +----+----+----+----+----+----+
 *  |    | &  | *  | (  | )  | >  |         |    | _  | +  | {  | }  |    |
 *  `----+----+----+----+----+----'         `----+----+----+----+----+----'
 *  좌 thumb: ESC SPC TAB (bare, SYM+MEDIA/NAV/MOUSE 동시 활성화 회피)
 */
    [_SYM] = LAYOUT(
      KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                                     KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
      _______, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR,                                     XXXXXXX, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, _______,
      _______, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_GT,   _______, _______, _______, _______, XXXXXXX, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, _______,
                                 _______, KC_ESC,  KC_SPC,  KC_TAB,  _______, _______, _______, _______, _______, _______
    ),

/*
 * NUM Layer (right thumb middle = BSPC hold): 양쪽 hand 에 숫자 전개.
 * 행 1 = 일반 키보드 숫자행. 그 아래는 양쪽 모두 numpad 격자.
 *
 *  좌측                                    우측
 *  ,----+----+----+----+----+----.         ,----+----+----+----+----+----.
 *  | `  | 1  | 2  | 3  | 4  | 5  |         | 6  | 7  | 8  | 9  | 0  |    |   행 1: 숫자행 1-0
 *  +----+----+----+----+----+----+         +----+----+----+----+----+----+
 *  |    | 4  | 5  | 6  | 7  | 8  |         |    | 4  | 5  | 6  |    |    |   행 2: numpad mid
 *  +----+----+----+----+----+----+         +----+----+----+----+----+----+
 *  |    | 7  | 8  | 9  | 0  | .  |         |    | 1  | 2  | 3  | .  |    |   행 3: 좌 7-0, 우 1-3
 *  `----+----+----+----+----+----'         `----+----+----+----+----+----'
 *  좌 thumb: ESC SPC TAB (bare, NUM+MEDIA/NAV/MOUSE 동시 활성화 회피)
 */
    [_NUM] = LAYOUT(
      KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
      _______, KC_4,    KC_5,    KC_6,    KC_7,    KC_8,                                        _______, KC_4,    KC_5,    KC_6,    _______, _______,
      _______, KC_7,    KC_8,    KC_9,    KC_0,    KC_DOT,  _______, _______, _______, _______, _______, KC_1,    KC_2,    KC_3,    KC_DOT,  _______,
                                 _______, KC_ESC,  KC_SPC,  KC_TAB,  _______, _______, _______, _______, _______, _______
    ),

/*
 * FUN Layer (right thumb outer = DEL hold): F-키. 양쪽 hand 에 전개.
 * 좌측 자리 = _NUM 의 숫자 자리 — 숫자 N 자리에 F<N> (0 은 F10).
 * F11/F12 는 양쪽 외곽 열의 행 2/행 3.
 *
 *  좌측 (숫자 자리 대응)                    우측
 *  ,----+----+----+----+----+----.         ,----+----+----+----+----+----.
 *  |    | F1 | F2 | F3 | F4 | F5 |         | F6 | F7 | F8 | F9 |F10 |    |
 *  +----+----+----+----+----+----+         +----+----+----+----+----+----+
 *  |F11 | F4 | F5 | F6 | F7 | F8 |         |    | F4 | F5 | F6 |    |F11 |
 *  +----+----+----+----+----+----+         +----+----+----+----+----+----+
 *  |F12 | F7 | F8 | F9 |F10 |    |         |    | F1 | F2 | F3 |    |F12 |
 *  `----+----+----+----+----+----'         `----+----+----+----+----+----'
 *  좌 thumb: ESC SPC TAB (bare, FUN+MEDIA/NAV/MOUSE 동시 활성화 회피)
 */
    [_FUN] = LAYOUT(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
      KC_F11,  KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,                                       _______, KC_F4,   KC_F5,   KC_F6,   _______, KC_F11,
      KC_F12,  KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______, _______, _______, _______, _______, _______, KC_F1,   KC_F2,   KC_F3,   _______, KC_F12,
                                 _______, KC_ESC,  KC_SPC,  KC_TAB,  _______, _______, _______, _______, _______, _______
    ),
};


void keyboard_post_init_user(void) {
  uint8_t red   = 0xFF;
  uint8_t green = 0x00;
  uint8_t blue  = 0x00;
  uint8_t white = 0x00;

  pimoroni_trackball_set_rgbw(red, green, blue, white);
}
