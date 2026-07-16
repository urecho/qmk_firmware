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

// Chordal Hold: tap-hold 키가 "같은 손" 다음 키와 눌리면 무조건 tap 확정.
// → join(j+o, 같은손) 같은 롤은 글자로, ㅆ(j+t, 반대손) 같은 조합만 hold 후보로 남김.
//   반대손 조합의 hold 최종 확정은 PERMISSIVE_HOLD 가 release 순서로 판정
//   (의도적 shift-hold=ㅆ 는 hold, 빠른 tap=fuck 의 f·레이어의 ㄹ 는 tap).
// Flow Tap(시간 기반) 대체 — Flow Tap 은 shift 를 tap 으로 죽여 한글 ㅆ/? 를 깨뜨렸고
// 역할이 Chordal Hold(손 기반)와 겹쳐 제거함 (AVR flash 여유 확보 겸).
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

