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

// split 양쪽 상태 동기화 (layer/mods 표시 일관성)
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_MODS_ENABLE
