APP_ABI :=  armeabi-v7a
#ndk-build APP_ABI=all                  //编译所有平台
#ndk-build APP_ABI=armeabi-v7a  //编译arm
#ndk-build APP_ABI=mips              //编译mips
# NDK_TOOLCHAIN_VERSION=4.8
APP_PLATFORM=android-21
# APP_CPPFLAGS := -frtti #允许异常功能，及运行时类型识别  
APP_CPPFLAGS +=-std=c++11 -lstdc++  -static-libstdc++#允许使用c++11的函数等功能   
APP_CPPFLAGS +=-fpermissive  -g -O2  -save-temps=obj#此项有效时表示宽松的编译形式，比如没有用到的代码中有错误也可以通过编译；使用GNU STL时不用此项std::string 居然编译不通过！！
LOCAL_CPPFLAGS += -ffunction-sections -fdata-sections 
LOCAL_CFLAGS += -ffunction-sections -fdata-sections 
LOCAL_LDFLAGS += -Wl,--gc-sections
APP_STL   :=  c++_static
APP_ALLOW_MISSING_DEPS :=true
APP_OPTIM :=release
APP_LLVM := true
