#!/bin/bash
PROJECT_NAME=kiwiwonder
PLATFORM="${PLATFORM:=android}"
NDK_VERSION="${NDK_VERSION:=android-9}"
#NDK_ABI="${NDK_ABI:=all}"
#NDK_ABI="${NDK_ABI:=armeabi-v7a}"
#NDK_ABI="${NDK_ABI:=armeabi}"
NDK_ABI="${NDK_ABI:="armeabi armeabi-v7a"}"
#NDK_ABI="${NDK_ABI:=arm}"
#NDK_ABI:=armeabi armeabi-v7a
NDK_DEBUG="${NDK_DEBUG:=0}"
#TOOLCHAIN_ABIS="armeabi armeabi-v7a"

export PROJECT_NAME
export PLATFORM
export NDK_VERSION
export NDK_ABI
export NDK_DEBUG

$DJENGINE_DIR/build/build_android_internal.sh $1 $2 $3
