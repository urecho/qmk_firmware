#!/bin/bash -ex

# QMK 의 avr-gcc@8 / arm-none-eabi-gcc@8 는 brew keg-only formula 라
# PATH 에 자동 등록 안 됨. 셸 PATH 무관하게 동작하도록 명시 prepend.
export PATH="/opt/homebrew/opt/avr-gcc@8/bin:/opt/homebrew/opt/arm-none-eabi-gcc@8/bin:/opt/homebrew/bin:$PATH"

make splitkb/kyria/rev1:urecho:dfu

# make splitkb/kyria/rev1:urecho:avrdude

#  bin/qmk flash -kb kyria -km urecho -bl avrdude
