LOCAL_PATH := $(my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := myps
LOCAL_C_INCLUDES := $(LOCAL_PATH)
LOCAL_SRC_FILES:=myps.cpp
include $(BUILD_EXECUTABLE)

# include $(CLEAR_VARS)
# LOCAL_MODULE := test_main
# LOCAL_C_INCLUDES := $(LOCAL_PATH)/include 
# LOCAL_SRC_FILES := main_excute.cpp
# LOCAL_LDLIBS := -llog
# include $(BUILD_EXECUTABLE)

# include $(CLEAR_VARS)
# LOCAL_MODULE := libtest
# LOCAL_C_INCLUDES := $(LOCAL_PATH)/include 
# LOCAL_C_INCLUDES += $(STL_PATH)
# LOCAL_STATIC_LIBRARIES :=  libfs  libkernel   libproc     libselinux   
# LOCAL_SRC_FILES  :=  main.cpp
# LOCAL_LDLIBS := -llog
# LOCAL_CFLAGS += -fno-stack-protector
# LOCAL_CFLAGS += -fvisibility=hidden
# LOCAL_CPPFLAGS += -fvisibility=hidden
# include $(BUILD_SHARED_LIBRARY)

# include $(CLEAR_VARS)
# LOCAL_MODULE := vmos_debug_testcase
# LOCAL_C_INCLUDES += $(STL_PATH)
# LOCAL_SRC_FILES  := test/ltp_bash_test.cpp
# include $(BUILD_EXECUTABLE)

# include $(CLEAR_VARS)
# LOCAL_MODULE := test_lock
# LOCAL_C_INCLUDES += $(STL_PATH)
# LOCAL_C_INCLUDES := $(LOCAL_PATH)/include
# LOCAL_STATIC_LIBRARIES :=  libutils libfs  libkernel   libproc  libselinux   
# LOCAL_SRC_FILES  := test/test.cpp
# LOCAL_LDLIBS := -llog
# LOCAL_CFLAGS += -fno-stack-protector
# LOCAL_CFLAGS += -fvisibility=hidden
# LOCAL_CPPFLAGS += -fvisibility=hidden
# include $(BUILD_EXECUTABLE)



# subdirs := $(addprefix $(LOCAL_PATH)/,$(addsuffix /Android.mk, \
# 	    utils \
# 		kernel \
# 		dev \
# 		fs \
# 		drivers \
# 		hardware \
# 		ipc \
# 		proc \
# 		selinux \
# 		net \
# 		syscall \
# 	))

# include $(subdirs)











