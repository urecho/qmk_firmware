/* Copyright 2024 Yoonseok Woo (@urecho)
 *
 * BoardSource uniCorne (RP2040) — kyria/urecho 의 miryoku 7-layer 를 이식.
 * Source of truth = keyboards/splitkb/kyria/keymaps/urecho.
 *
 * kyria 와 main 3x6 그리드는 1:1, thumb 만 3+3 (kyria 5+5 중 외곽 encoder 자리 +
 * 내측 LCAG 자리는 corne 에 없어 생략 — Hyper/Layer Lock 미이식).
 * uniCorne 전용 HW(analog joystick, RGB matrix, audio, OLED)는 keyboard 기본값 사용.
 */
#include QMK_KEYBOARD_H

// Left-hand home row mods (kyria 와 동일)
#define HOME_A LCTL_T(KC_A)
#define HOME_S LALT_T(KC_S)
#define HOME_D LGUI_T(KC_D)
#define HOME_F LSFT_T(KC_F)
// Right-hand home row mods
#define HOME_J RSFT_T(KC_J)
#define HOME_K RGUI_T(KC_K)
#define HOME_L LALT_T(KC_L)
#define HOME_SCLN RCTL_T(KC_SCLN)

// macOS 영역 스크린샷: Ctrl+Cmd+Shift+4 (드래그 영역 선택 + 클립보드)
#define SS_DRAG LCTL(LGUI(LSFT(KC_4)))

// Miryoku-style layers. 엄지 layer-tap 으로 각 layer 활성화 (kyria 와 동일 의미).
enum layers {
    _BASE = 0,
    _MEDIA,
    _NAV,
    _MOUSE,
    _SYM,
    _NUM,
    _FUN
};

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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * BASE: QWERTY + home row mods + miryoku thumb (3+3)
 * 좌 thumb: MEDIA/ESC NAV/SPC MOUSE/TAB   우 thumb: SYM/ENT NUM/BSPC FUN/DEL
 * (kyria 의 외곽 encoder 자리 + 내측 LCAG 는 corne 자리 없음 -> 생략)
 */
    [_BASE] = LAYOUT_split_3x6_3(
      KC_GRV,  KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                  KC_Y,   KC_U,   KC_I,    KC_O,   KC_P,      KC_BSLS,
      KC_ESC,  HOME_A, HOME_S, HOME_D, HOME_F, KC_G,                  KC_H,   HOME_J, HOME_K,  HOME_L, HOME_SCLN, KC_QUOT,
      KC_LSFT, KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,                  KC_N,   KC_M,   KC_COMM, KC_DOT, KC_SLSH,   KC_RSFT,
                       LT(_MEDIA,KC_ESC), LT(_NAV,KC_SPC), LT(_MOUSE,KC_TAB),   LT(_SYM,KC_ENT), LT(_NUM,KC_BSPC), LT(_FUN,KC_DEL)
    ),

/*
 * MEDIA (좌 thumb 외 = ESC hold): RGB matrix + 미디어 + 스크린샷
 * (kyria 는 underglow UG_* -> uniCorne 은 RGB matrix RM_* 로 대응)
 */
    [_MEDIA] = LAYOUT_split_3x6_3(
      _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______,
      _______, RM_TOGG, RM_SATU, RM_HUEU, RM_VALU, RM_NEXT,           KC_MUTE, KC_VOLD, KC_VOLU, KC_MNXT, KC_MPLY, SS_DRAG,
      _______, _______, RM_SATD, RM_HUED, RM_VALD, RM_PREV,           _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______,           _______, _______, _______
    ),

/*
 * NAV (좌 thumb 중 = SPC hold): vim hjkl + 편집 cluster + 보조 nav
 * 우 thumb 명시 = bare ENT/BSPC/DEL (NAV + SYM/NUM/FUN 동시활성 충돌 회피)
 */
    [_NAV] = LAYOUT_split_3x6_3(
      _______, _______, _______, _______, _______, _______,          KC_UNDO, KC_CUT,  KC_COPY, KC_PASTE, KC_AGAIN, _______,
      _______, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, _______,          KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT,  CW_TOGG,  SS_DRAG,
      _______, _______, _______, _______, _______, _______,          KC_INS,  KC_HOME, KC_PGDN, KC_PGUP,  KC_END,   _______,
                                 _______, _______, _______,           KC_ENT,  KC_BSPC, KC_DEL
    ),

/*
 * MOUSE (좌 thumb 내 = TAB hold): hjkl 자리에 마우스 커서 + 휠 + 버튼
 */
    [_MOUSE] = LAYOUT_split_3x6_3(
      _______, _______, _______, _______, _______, _______,          KC_UNDO, KC_CUT,  KC_COPY, KC_PASTE, KC_AGAIN, _______,
      _______, KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, _______,          MS_LEFT, MS_DOWN, MS_UP,   MS_RGHT,  _______,  _______,
      _______, _______, _______, _______, _______, _______,          MS_WHLL, MS_WHLD, MS_WHLU, MS_WHLR,  _______,  _______,
                                 _______, _______, _______,           MS_BTN2, MS_BTN1, MS_BTN3
    ),

/*
 * SYM (우 thumb 내 = ENT hold): 심볼 + 괄호. 양쪽 hand 에 전개.
 * 좌측 자리 = _NUM 의 숫자 자리 — 각 키는 그 숫자의 shift 문자.
 * 우측 행 1 외곽(base 의 \ 자리)은 파이프.
 * 좌 thumb = bare ESC/SPC/TAB (SYM + MEDIA/NAV/MOUSE 동시활성 회피)
 */
    [_SYM] = LAYOUT_split_3x6_3(
      KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,           KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
      _______, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR,           XXXXXXX, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, _______,
      _______, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_GT,             XXXXXXX, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, _______,
                                 KC_ESC,  KC_SPC,  KC_TAB,            _______, _______, _______
    ),

/*
 * NUM (우 thumb 중 = BSPC hold): 양쪽 hand 에 숫자 전개.
 * 행 1 = 숫자행 1-0. 좌측 행 2 는 4-8, 행 3 은 7-0 + 소수점. 우측은 numpad 격자.
 * 좌 thumb = bare ESC/SPC/TAB
 */
    [_NUM] = LAYOUT_split_3x6_3(
      KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,              KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
      _______, KC_4,    KC_5,    KC_6,    KC_7,    KC_8,             _______, KC_4,    KC_5,    KC_6,    _______, _______,
      _______, KC_7,    KC_8,    KC_9,    KC_0,    KC_DOT,           _______, KC_1,    KC_2,    KC_3,    KC_DOT,  _______,
                                 KC_ESC,  KC_SPC,  KC_TAB,            _______, _______, _______
    ),

/*
 * FUN (우 thumb 외 = DEL hold): F-키. 양쪽 hand 에 전개.
 * 좌측 자리 = _NUM 의 숫자 자리 — 숫자 N 자리에 F<N> (0 은 F10).
 * F11/F12 는 양쪽 외곽 열의 행 2/행 3. 좌 thumb = bare ESC/SPC/TAB
 */
    [_FUN] = LAYOUT_split_3x6_3(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,             KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
      KC_F11,  KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,             _______, KC_F4,   KC_F5,   KC_F6,   _______, KC_F11,
      KC_F12,  KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,           _______, KC_F1,   KC_F2,   KC_F3,   _______, KC_F12,
                                 KC_ESC,  KC_SPC,  KC_TAB,            _______, _______, _______
    ),
};

#ifdef OLED_ENABLE
// 마스터: urecho + 현재 layer 이름. 슬레이브: 라벨.
// (false 반환으로 keyboard 기본 OLED 그리기를 대체)
bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_write_P(PSTR("urecho\n\n"), false);
        switch (get_highest_layer(layer_state)) {
            case _BASE:  oled_write_P(PSTR("BASE \n"), false); break;
            case _MEDIA: oled_write_P(PSTR("MEDIA\n"), false); break;
            case _NAV:   oled_write_P(PSTR("NAV  \n"), false); break;
            case _MOUSE: oled_write_P(PSTR("MOUSE\n"), false); break;
            case _SYM:   oled_write_P(PSTR("SYM  \n"), false); break;
            case _NUM:   oled_write_P(PSTR("NUM  \n"), false); break;
            case _FUN:   oled_write_P(PSTR("FUN  \n"), false); break;
            default:     oled_write_P(PSTR("?    \n"), false); break;
        }
    } else {
        oled_write_P(PSTR("urecho corne\n"), false);
    }
    return false;
}
#endif
