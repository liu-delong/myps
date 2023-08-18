export NDK_ROOT=/home/xiaosuanliu/android-ndk-r16b
export PATH=$PATH:/home/xiaosuanliu/android-ndk-r16b
ndk-build NDK_PROJECT_PATH=./ APP_BUILD_SCRIPT=./Android.mk  NDK_APPLICATION_MK=./Application.mk NDK_OUT=./obj -j16  # -Wl,--allow-multiple-definition
chmod 777 libs/armeabi-v7a/myps
adb push libs/armeabi-v7a/myps /data/test
# export NDK_ROOT=/home/leeshenyou/android-ndk-r16b

# adb push libs/arm64-v8a/libtest.so /data/local/tmp

# adb -s HT7380201347 push libs/armeabi-v7a/libtest.so /data/local/tmp
# adb -s HT7380201347 push libs/armeabi-v7a/test_main /data/local/tmp

#cp libs/armeabi-v7a/libtest.so ~/hdd/Share/moe/sdk/libtest.so

#adb push libs/armeabi-v7a/libtest.so /data/local/tmp
#adb push libs/armeabi-v7a/test_main /data/local/tmp

# adb push libs/armeabi-v7a/test /data/local/tmp

# adb push libs/armeabi-v7a/moe_test /data/local/tmp

# adb -s HT7380201347 push libs/arm64-v8a/libtest.so /data/data/com.vmos.two
# adb -s HT7380201347 push libs/arm64-v8a/test_main /data/data/com.vmos.two
# cp libs/arm64-v8a/vmos_debug_testcase /home/eason/sad/roms/android-7.1_libos/out/target/product/generic_arm64/system/bin/
# adb -s HT7380201347 push libs/arm64-v8a/vmos_debug_testcase /data/local/tmp/ltp
# adb -s HT7380201347 push libs/arm64-v8a/test /data/data/com.vmos.two
