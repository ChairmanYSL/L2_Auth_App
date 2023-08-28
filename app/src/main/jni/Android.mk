# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

##pure
include $(CLEAR_VARS)
LOCAL_MODULE := szzt_purejni
LOCAL_SRC_FILES := sdk/externlib/libszzt_sdkpure.so
include $(PREBUILT_SHARED_LIBRARY)

#ddi
include $(CLEAR_VARS)
LOCAL_MODULE := trendit_ddi
LOCAL_SRC_FILES := sdk/externlib/lib_ddi.so
include $(PREBUILT_SHARED_LIBRARY)

### sdkemvtest
include $(CLEAR_VARS)
LOCAL_MODULE    := sdkemvtest

#################################################include
##export
LOCAL_C_INCLUDES := $(LOCAL_PATH)/export

### extern
LOCAL_C_INCLUDES += $(LOCAL_PATH)/extern/devapi
LOCAL_C_INCLUDES += $(LOCAL_PATH)/extern/inc

### application
LOCAL_C_INCLUDES += $(LOCAL_PATH)/application/inc

### sdk
LOCAL_C_INCLUDES += $(LOCAL_PATH)/sdk/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/sdk/privateinc

##emvlib
LOCAL_C_INCLUDES += $(LOCAL_PATH)/emvlib/dllemvbase/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/emvlib/dllemvmath/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/emvlib/dllpure/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/emvlib/dllpure/privateinc

########################################################################source code

### extern
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/extern/devapi/*.c)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/extern/src/*.c)

### application
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/application/src/*.c)

### sdk
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/sdk/libsdk*/*.c)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/sdk/libsdk*/*/*.c)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/sdk/libsdk*/*/*/*.c)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/sdk/libsdk*/*/*/*/*.c)

### emv
#MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/emvlib/*.c)
#MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/emvlib/*/*.c)
#MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/emvlib/*/*/*.c)
#MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/emvlib/*/*/*/*.c)


LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

LOCAL_SHARED_LIBRARIES += szzt_purejni
LOCAL_SHARED_LIBRARIES += trendit_ddi

# LOCAL_STATIC_LIBRARIES += android_support
LOCAL_CFLAGS := -DANDROID
#LOCAL_CFLAGS += -DXGD_SDK_DEBUG
LOCAL_LDLIBS := -llog
LOCAL_CFLAGS += -Wno-error=format-security
include $(BUILD_SHARED_LIBRARY)
$(call import-module, android/support)























