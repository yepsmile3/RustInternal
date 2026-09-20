#!/bin/bash
# 编译 KamiVerify.dylib（iOS ARM64）
# HMAC 密钥已内嵌在 Sources/KamiCrypto.swift（与制卡端 kami_ios_secret.txt 一致）
set -e

SDK_PATH=$(xcrun --sdk iphoneos --show-sdk-path)

echo "SDK: $SDK_PATH"

mkdir -p build
xcrun --sdk iphoneos swiftc \
    -target arm64-apple-ios14.0 \
    -sdk "$SDK_PATH" \
    -O -whole-module-optimization \
    -emit-library -emit-module \
    -module-name KamiVerify -parse-as-library \
    Sources/*.swift Sources/kami_entry.c \
    -o build/KamiVerify.dylib

echo "编译完成: build/KamiVerify.dylib"
ls -la build/KamiVerify.dylib