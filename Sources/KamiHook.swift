// KamiHook.swift — 注入入口
// dylib 加载时，kami_entry.c 的 constructor 会调用 kamiEntryPoint()
import UIKit
import Foundation

// C 可调用的入口：dylib 加载后由 kami_entry.c 调用
@_cdecl("kamiEntryPoint")
public func kamiEntryPoint() {
    // 等主线程就绪后显示卡密界面
    DispatchQueue.main.asyncAfter(deadline: .now() + 0.5) {
        KamiUI.shared.show()
    }

    // 监听 target App 进入前台，若未验证则再次弹出
    NotificationCenter.default.addObserver(
        forName: UIApplication.didBecomeActiveNotification,
        object: nil,
        queue: .main
    ) { _ in
        // 只在未验证时弹出
        DispatchQueue.main.asyncAfter(deadline: .now() + 0.2) {
            if !KamiUI.checkVerified() {
                KamiUI.shared.show()
            }
        }
    }
}