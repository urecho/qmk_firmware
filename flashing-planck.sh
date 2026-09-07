#!/bin/bash -ex

# QMK 툴체인 keg-only 우회. (flashing-kyria.sh 와 동일 이유)
export PATH="/opt/homebrew/opt/avr-gcc@8/bin:/opt/homebrew/opt/arm-none-eabi-gcc@8/bin:/opt/homebrew/bin:$PATH"

make planck/rev4:urecho \
  && sudo make planck/rev4:urecho:dfu
#sudo dfu-programmer atmega32u4 erase
#sudo dfu-programmer atmega32u4 flash ./planck_rev4_urecho.hex
#sudo dfu-programmer atmega32u4 reset
