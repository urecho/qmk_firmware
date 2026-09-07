# mousekey / extrakey / rgb_matrix / oled / audio 는 uniCorne keyboard.json 에서 이미 활성.
CAPS_WORD_ENABLE = yes        # _NAV 의 CW_TOGG

# analog joystick 포인팅 비활성:
# uniCorne 은 keyboard 기본값으로 analog joystick(GP26/GP27)을 포인팅으로 켜는데,
# 조이스틱 미장착 시 핀이 floating 으로 읽혀 커서가 계속 우하단으로 흐름.
# MOUSE 레이어의 마우스 키(MS_*)는 mousekey 라 이와 무관하게 동작.
POINTING_DEVICE_ENABLE = no
