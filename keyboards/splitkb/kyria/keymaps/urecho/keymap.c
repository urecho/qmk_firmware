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
#include <stdio.h>

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

char wpm_str[10];

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
                                  XXXXXXX, LT(_MEDIA,KC_ESC), LT(_NAV,KC_SPC), LT(_MOUSE,KC_TAB), LCAG(KC_NO),   LCAG(KC_NO), LT(_SYM,KC_ENT), LT(_NUM,KC_BSPC), LT(_FUN,KC_DEL), XXXXXXX
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
                                 _______, _______, _______, _______, _______, KC_ENT,  KC_BSPC, KC_DEL,  _______, _______
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
                                 _______, _______, _______, _______, _______, MS_BTN2, MS_BTN1, MS_BTN3, _______, _______
    ),

/*
 * SYM Layer (right thumb inner = ENT hold): 심볼 + 괄호.
 * 좌측 행 2 의 HOME/END 와 행 3 의 PGUP/PGDN 은 _NAV 로 이동했음.
 */
    [_SYM] = LAYOUT(
      KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,                                     KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, _______,
      _______, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX,                                     XXXXXXX, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, _______,
      _______, _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, _______, _______, _______, _______, XXXXXXX, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),

/*
 * NUM Layer (right thumb middle = BSPC hold): top row 자체가 numpad 위행 역할 + 그 아래 4-6 / 1-3 덧붙임
 *
 *  좌측 (top row 1-5)                      우측 (top row 6-0 = numpad 7-9 자리 겸용)
 *  ,----+----+----+----+----+----.         ,----+----+----+----+----+----.
 *  | `  | 1  | 2  | 3  | 4  | 5  |         | 6  | 7  | 8  | 9  | 0  |    |   행 1: top row 우측 = numpad top (7/8/9 동일 자리)
 *  +----+----+----+----+----+----+         +----+----+----+----+----+----+
 *  |    |    |    |    |    |    |         |    | 4  | 5  | 6  |    |    |   행 2: numpad mid (J/K/L)
 *  +----+----+----+----+----+----+         +----+----+----+----+----+----+
 *  |    |    |    |    |    |    |         |    | 1  | 2  | 3  |    |    |   행 3: numpad bottom (M/,/.)
 *  `----+----+----+----+----+----+         +----+----+----+----+----+----'
 *  좌 thumb: . 0 -  (numpad 보조)
 *
 *  사용자 의도 시각화:
 *    12345 67890   ← 행 1 (좌 top row + 우 top row = numpad 위행 겸용)
 *           456    ← 행 2
 *           123    ← 행 3
 *
 *  좌측 hand 행 2/3 은 _______ — base 의 home row mod 그대로 통과 (NUM 활성 중 mod 사용 가능).
 */
    [_NUM] = LAYOUT(
      KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
      _______, KC_4,    KC_5,    KC_6,    _______, _______,                                     _______, KC_4,    KC_5,    KC_6,    _______, _______,
      _______, KC_7,    KC_8,    KC_9,    KC_0,    _______, _______, _______, _______, _______, _______, KC_1,    KC_2,    KC_3,    KC_DOT,  _______,
                                 _______, KC_ESC,  KC_SPC,  KC_TAB,  _______, _______, _______, _______, _______, _______
    ),

/*
 * FUN Layer (right thumb outer = DEL hold): F-키 위치 = NUM 의 숫자 위치 (사용자 머슬메모리 원칙)
 *
 *  좌측 (top row 자리 = F1-F5)             우측 (top row = F6-F10, 그 아래 = numpad 식 F-key)
 *  ,----+----+----+----+----+----.         ,----+----+----+----+----+----.
 *  |    | F1 | F2 | F3 | F4 | F5 |         | F6 | F7 | F8 | F9 |F10 |F11 |   행 1: top row 자리 = F1~F10
 *  +----+----+----+----+----+----+         +----+----+----+----+----+----+
 *  |    | F11| F12|    |    |    |         |    | F4 | F5 | F6 |    |F12 |   행 2: numpad mid 자리 = F4-F6
 *  +----+----+----+----+----+----+         +----+----+----+----+----+----+
 *  |    |    |    |    |    |    |         |    | F1 | F2 | F3 |    |    |   행 3: numpad bottom 자리 = F1-F3
 *  `----+----+----+----+----+----+         +----+----+----+----+----+----'
 *  좌 thumb: ESC SPC TAB (bare, FUN+MEDIA/NAV/MOUSE 동시 활성화 회피)
 *
 *  F11/F12 는 별도 자리 안 둠 (사용자가 numpad 격자 외 안 둘 의도로 추정).
 */
    [_FUN] = LAYOUT(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,
      _______, KC_F11,  KC_F12,  _______, _______, _______,                                     _______, KC_F4,   KC_F5,   KC_F6,   _______, KC_F12,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_F1,   KC_F2,   KC_F3,   _______, _______,
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
