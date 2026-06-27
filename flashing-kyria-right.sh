#!/bin/bash -ex

# 오른쪽 절반 = Pro Micro (Caterina 부트로더) 용 스크립트.
# 왼쪽 절반 = Elite-C (atmel-dfu) 는 flashing-kyria.sh 사용.
#
# Reset 방법 (Pro Micro 는 USB DFU 모드가 없음):
#   1) 보드에 reset 스위치가 있으면 1 회 누름
#   2) 없으면 Pro Micro 의 RST 핀을 인접 GND 핀에 핀셋/철사로 잠시 단락
#   3) 위가 안 먹으면 빠르게 더블 탭
# Caterina 부트로더 창은 reset 후 약 8 초. 이 안에 avrdude 가 시작돼야 함.
# → 미리 이 스크립트를 실행 → make 가 USB 포트 탐지 시작했을 때 reset.

# brew keg-only 우회 (flashing-kyria.sh 와 동일 이유)
export PATH="/opt/homebrew/opt/avr-gcc@8/bin:/opt/homebrew/opt/arm-none-eabi-gcc@8/bin:/opt/homebrew/bin:$PATH"

make splitkb/kyria/rev1:urecho:avrdude
