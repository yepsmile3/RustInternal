# 云端编译 dylib · 手把手操作（GitHub Actions，免费，无需 Mac）

这套 iOS 卡密 dylib 是 Swift 写的，**编译必须用 macOS**。你没有 Mac 也能编译——用 GitHub 的免费 `macos-latest` 云端机器，全程零成本，跟着下面做。

---

## 前提：有一个 GitHub 账号（免费注册 https://github.com）

---

## 第一步：创建仓库

1. 登录 GitHub，点右上角 **＋** → **New repository**
2. Repository name 随便填，比如 `kami-verify`
3. 选 **Private**（私有，别公开你的卡密密钥！）
4. 不要勾选任何初始化选项（README/license 都不打勾）
5. 点 **Create repository**

---

## 第二步：上传代码

你有两种方式，选一个：

### 方式 A：网页直接上传（最简单，不用装 git）

1. 进到你刚建的仓库页面
2. 点 **Add file** → **Upload files**
3. 把你电脑上的 `iOS卡密dylib` 文件夹里这些文件**全部拖进去**：
   ```
   .github\workflows\build.yml
   .gitignore
   Makefile
   README.md
   Sources\kami_entry.c
   Sources\KamiCrypto.swift
   Sources\KamiHook.swift
   Sources\KamiUI.swift
   ```
   > 注意：`.github` 是隐藏文件夹，拖拽时可能要手动点"choose files"后进去选；或者直接把整个 `iOS卡密dylib` 文件夹内容拖进去（保持目录结构，尤其是 `Sources\` 和 `.github\workflows\`）。

4. 拖完后，底部 **Commit changes** 提交

### 方式 B：用 git 命令（你电脑装了 git 的话）

在 `iOS卡密dylib` 目录里打开命令行，执行：

```bash
git init
git add .
git commit -m "kami verify dylib"
git branch -M main
git remote add origin https://github.com/你的用户名/kami-verify.git
git push -u origin main
```

---

## 第三步：触发编译

1. 上传/推送成功后，仓库顶部点 **Actions** 标签页
2. 应该能看到一个 workflow 叫 **"Build KamiVerify dylib"**，已经自动开始跑了（commit 触发）
   - 如果没自动跑，点右侧 **Run workflow** 手动触发
3. 等它跑完（约 2-4 分钟），黄色转绿色 ✓

---

## 第四步：下载 dylib

1. 进入那条**绿色 ✓ 的 workflow run**
2. 页面底部 **Artifacts** 区域，有一个 **`KamiVerify.dylib`**
3. 点它下载，得到一个 zip
4. 解压，得到 `KamiVerify.dylib`

> 顺便：如果之前 `build.yml` 里 seed 生成了 secret，artifacts 里也会有个 `secret.txt`。**你的密钥已经内嵌在 KamiCrypto.swift 里了**，不需要那个，可忽略。

---

## 第五步：注入到目标 App

1. 把 `KamiVerify.dylib` 传到手机（TrollStore 的文件共享、Filza、AirDrop、微信文件传输助手都行）
2. 打开 **TrollFools**
3. 选你要加卡密的目标 App → 点 **注入（Inject）** → 选 `KamiVerify.dylib`
4. 注入完成，打开目标 App → 弹出我们自己的「卡密验证」界面（纯离线，不会再卡"验证中"）

---

## 第六步：发卡

- 客户打开 App 看到 **机器码（UDID）**，发给你
- 你打开 `制卡端.exe` → 平台选 **iOS 手机版** → 输入 UDID → 选卡类型 → 生成卡密
- 卡密发给客户 → 输入 → 验证通过 → 进 App

---

## 常见问题

**Q：Actions 里没有 workflow？**
A：确认 `.github/workflows/build.yml` 上传成功了（隐藏文件夹容易漏）。点仓库里 `.github` → `workflows` 看有没有 build.yml。

**Q：编译报错？**
A：把报错截图。常见原因是 Swift 文件有中文编码问题或缺少文件。

**Q：想改密钥？**
A：改两处，保持一样：
1. `卡密系统\kami_ios_secret.txt`
2. `iOS卡密dylib\Sources\KamiCrypto.swift` 里 `KamiSecret.hex`
改完重新上传编译。

**Q：Security 里 Actions 被禁用了？**
A：仓库 **Settings** → **Actions** → **General** → 允许 actions 运行。