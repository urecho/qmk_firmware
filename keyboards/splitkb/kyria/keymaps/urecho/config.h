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

#pragma once

#ifdef OLED_ENABLE
  #define OLED_DISPLAY_128X64
#endif

#ifdef RGBLIGHT_ENABLE
  // Static underglow only (no animation effects) to keep firmware under 28K limit
  #define RGBLIGHT_HUE_STEP 8
  #define RGBLIGHT_SAT_STEP 8
  #define RGBLIGHT_VAL_STEP 8
  #define RGBLIGHT_LIMIT_VAL 150
#endif

#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
#define NO_MUSIC_MODE

// #ifndef NO_DEBUG
// #define NO_DEBUG
// #endif // !NO_DEBUG
// #if !defined(NO_PRINT) && !defined(CONSOLE_ENABLE)
// #define NO_PRINT
// #endif // !NO_PRINT

// If you are using an Elite C rev3 on the slave side, uncomment the lines below:
// #define SPLIT_USB_DETECT
// #define NO_USB_STARTUP_CHECK

#define TAPPING_TERM 200
#define PERMISSIVE_HOLD
#define TAPPING_FORCE_HOLD_PER_KEY

// layer-tap 엄지 키(SYM/ENT 등)가 조합 시 tap(엔터 등)으로 오판정되는 문제 해결.
// 다른 키가 눌리는 순간 즉시 hold(레이어)로 확정. per-key 로 layer-tap 에만 적용
// (home row mod 에는 미적용 — 빠른 타이핑 롤링 오발화 방지). → keymap.c get_hold_on_other_key_press
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

// tap-hold 키와 다음 키가 같은 손이면 tap 으로 확정한다.
// 반대손 조합은 TAPPING_TERM / PERMISSIVE_HOLD 로 판정된다.
// 예외는 keymap.c 의 get_chordal_hold() 참조.
#define CHORDAL_HOLD

#define ENCODER_RESOLUTION 2
#define POINTING_DEVICE_ROTATION_180

// Split sync — slave OLED 에서 master 의 layer/mod/wpm 표시 가능하도록
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_MODS_ENABLE
#define SPLIT_WPM_ENABLE
#define SPLIT_LED_STATE_ENABLE

// kyria upstream 의 sync.oled = true 를 비활성화.
// master (왼쪽) 에 OLED 없으니 mirror 가 slave 의 빈 화면을 강제. slave 가 자기 render 하도록.
#undef SPLIT_OLED_ENABLE

