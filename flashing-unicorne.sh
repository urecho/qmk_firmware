#!/bin/bash -e

# boardsource/uniCorne (urecho 키맵) 플래싱 스크립트.
#
# kyria 와 다른 점:
#   - MCU = RP2040, 부트로더 = rp2040 UF2 (드래그앤드롭). avrdude/dfu 아님.
#   - 좌/우 절반이 동일한 uf2 를 사용 → 한쪽씩 두 번 플래싱.
#   - 환경이 Manjaro(Linux) 라 brew PATH 우회 불필요. qmk CLI 사용.
#
# 부트로더 진입 (한쪽 절반만 USB-C 연결한 상태에서):
#   1) BOOT 버튼을 누른 채 케이블 연결  (또는 BOOT 누른 채 RESET 클릭)
#   2) 또는 RESET 을 빠르게 더블 탭
#   3) 또는 키맵의 QK_BOOT 키
#   → 진입하면 'RPI-RP2' USB 드라이브가 마운트됨. qmk flash 가 자동 감지·복사.

KB=boardsource/unicorne
KM=urecho

echo "==> 컴파일: $KB:$KM"
qmk compile -kb "$KB" -km "$KM"

for HALF in "왼쪽" "오른쪽"; do
    echo
    echo "==================================================================="
    echo " [$HALF] 절반을 USB-C 로 연결하고 부트로더에 진입시키세요."
    echo "   (BOOT 누른 채 연결 / RESET 더블탭 / QK_BOOT 키 → 'RPI-RP2' 마운트)"
    echo "==================================================================="
    read -r -p " 준비됐으면 Enter (건너뛰려면 s + Enter): " ANS
    [ "$ANS" = "s" ] && { echo " → [$HALF] 건너뜀"; continue; }

    qmk flash -kb "$KB" -km "$KM"
    echo " → [$HALF] 완료. 케이블을 뽑으세요."
done

echo
echo "==> 양쪽 플래싱 종료."
