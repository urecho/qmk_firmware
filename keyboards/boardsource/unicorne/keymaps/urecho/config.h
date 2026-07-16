// Copyright 2024 Yoonseok Woo (@urecho)
// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once

// Tap-hold 튜닝 — kyria/urecho 와 동일
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
// 역할이 Chordal Hold(손 기반)와 겹쳐 제거함.
#define CHORDAL_HOLD

// split 양쪽 상태 동기화 (layer/mods 표시 일관성)
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_MODS_ENABLE
