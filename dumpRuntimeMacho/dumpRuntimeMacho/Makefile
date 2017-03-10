GO_EASY_ON_ME = 1

include $(THEOS)/makefiles/common.mk

ARCHS = arm64

TOOL_NAME = dumpRuntimeMacho

TARGET_CODESIGN = /usr/local/Cellar/ldid/1.2.1/bin/ldid
TARGET_CODESIGN_FLAGS = -Sent.xml

dumpRuntimeMacho_FILES = main.cpp macho_rt.cpp macho_fd.cpp common.cpp


# GCC_BIN=`xcrun --sdk iphoneos --find gcc`
# GCC_UNIVERSAL=$(GCC_BASE) -arch armv7 -arch armv7s -arch arm64
# SDK=`xcrun --sdk iphoneos --show-sdk-path`

# CFLAGS =
# GCC_BASE = $(GCC_BIN) -Os $(CFLAGS) -Wimplicit -isysroot $(SDK) -F$(SDK)/System/Library/Frameworks -F$(SDK)/System/Library/PrivateFrameworks



include $(THEOS_MAKE_PATH)/tool.mk
