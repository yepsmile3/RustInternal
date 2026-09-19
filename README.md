# iOS 卡密验证 dylib · 完整使用说明

## 一、这是什么

一个用于 **TrollFools 注入目标 App** 的卡密验证 dylib。注入后，目标 App 打开会先弹「卡密验证」界面（机器码 + 卡密输入 + 立即验证），验证通过才进入 App。

与「星火幻化器」电脑版卡密系统**同一套制卡端**，但 iOS 用 **HMAC-SHA256 离线验签**（不联网，永远不会卡"验证中"）。

## 二、目录结构

```
iOS卡密dylib/
├── Sources/
│   ├── KamiCrypto.swift    # 卡密核心（HMAC-SHA256 离线验卡 + 机器码）
│   ├── KamiUI.swift        # 卡密验证界面（卡密输入 + 立即验证按钮）
│   ├── KamiHook.swift      # 注入入口（dylib 加载时弹 UI）
│   └── kami_entry.c        # C constructor 入口
├── Makefile               # 本地（Mac）编译脚本
├── build.yml              # GitHub Actions 云端编译（免费 Mac）
└── README.md              # 本文件
```

## 三、机器码（重要）

- **iOS 机器码 = UDID**（identifierForVendor 的 UUID，去横线、大写）
- 客户在卡密界面看到机器码（32 位 hex，如 `A1B2C3D4...`），发给你
- 你在制卡端「iOS 手机版」模式输入这个机器码 + 选卡类型 → 生成卡密

## 四、编译 dylib（三选一）

### 方式 A：GitHub Actions 云端编译（推荐，无需 Mac）

1. 把 `iOS卡密dylib` 整个文件夹推到 GitHub 仓库
2. GitHub 会自动用 `.github/workflows/build.yml`（本目录的 `build.yml` 改名后放入）编译
3. 编译成功后，在 Actions 页面下载 `KamiVerify.dylib` 产物

**具体步骤：**
```
1. 建 GitHub 仓库，把本目录上传
2. 建 .github/workflows/build.yml，内容 = 本目录 build.yml
3. push 后 Actions 自动跑 macos-latest 编译
4. 下载 artifact: KamiVerify.dylib
```

### 方式 B：本地 Mac + Xcode

```bash
cd iOS卡密dylib
bash Makefile
# 产物在 build/KamiVerify.dylib
```

### 方式 C：任何 Mac（命令行）

```bash
SDK=$(xcrun --sdk iphoneos --show-sdk-path)
xcrun --sdk iphoneos swiftc \
  -target arm64-apple-ios14.0 \
  -sdk "$SDK" \
  -O -whole-module-optimization \
  -emit-library -emit-module \
  -module-name KamiVerify -parse-as-library \
  Sources/*.swift Sources/kami_entry.c \
  -o KamiVerify.dylib
```

## 五、注入到目标 App

1. 手机上装好 **TrollFools**（就是那个 tipa）
2. 打开 TrollFools → 选要注入的目标 App → 选「注入」→ 选 `KamiVerify.dylib`
3. 注入成功后，打开目标 App → 弹出「卡密验证」界面

## 六、发卡流程

```
1. 客户打开被注入的 App → 弹出卡密界面，显示机器码（UDID）
2. 客户把机器码发给你
3. 你打开 制卡端.exe → 选「iOS 手机版」平台 → 输入 UDID → 选卡类型 → 生成卡密
4. 卡密发给客户 → 客户在 App 里输入 → 验证通过 → 进入 App
5. 之后客户再打开 App 直接进（已激活自动跳过，卡密有效期内）
```

## 七、密钥说明（重要）

- **HMAC 密钥**已内嵌两处，必须一致：
  - 制卡端：`卡密系统\kami_ios_secret.txt`（64位hex）
  - dylib：`iOS卡密dylib\Sources\KamiCrypto.swift` 里 `KamiSecret.hex`
- 当前两处都是 `6003897b6063574ecd5bae7be7ab369bc9897586811ba3018998585197d9e5ee`
- 如果换了密钥，两处都要同步改，否则验卡失败

## 八、卡类型

| 类型 | 前缀 | 天数 |
|------|------|------|
| 月卡 | SFKM | 30 |
| 半年卡 | SFKH | 180 |
| 年卡 | SFKY | 365 |

## 九、与电脑版的关系

| | 电脑版（星火幻化器） | iOS（本 dylib） |
|---|---|---|
| 机器码 | MachineGuid | UDID |
| 签名 | Ed25519 | HMAC-SHA256 |
| 卡密前缀 | SFKM/SFKH/SFKY | 相同 |
| 制卡端 | 同一个制卡端 | 同一个（选 iOS 平台） |

**同一把制卡端，通过「平台」切换，分别给电脑和 iOS 生成对应卡密。**