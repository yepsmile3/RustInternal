// KamiCrypto.swift — 卡密签名/验签（HMAC-SHA256，iOS 原生 CommonCrypto）
// 为什么用 HMAC-SHA256 而不是 Ed25519：
//   - iOS CommonCrypto 原生支持 HMAC-SHA256，零外部依赖、零手写密码学（最可靠）
//   - Python 制卡端 hmac 模块一行实现，两端零出错风险
//   - 对称密钥：制卡端和 dylib 各内置一份，密钥泄露才可能导致他人制卡（对个人场景足够）
import Foundation
import CommonCrypto

enum KamiCrypto {

    // 卡类型 ID（与制卡端一致：monthly=1, halfyear=2, yearly=3）
    static let typeIds: [String: UInt8] = ["monthly": 1, "halfyear": 2, "yearly": 3]
    static let typeNames: [UInt8: String] = [1: "月卡", 2: "半年卡", 3: "年卡"]

    // 内置密钥（HMAC 密钥，与制卡端一致；编译时用脚本注入）
    // 32 字节十六进制字符串，如 "a1b2c3..."
    static let secretHex = KamiSecret.hex

    static var secretKey: [UInt8] {
        // 把 secretHex 转成字节
        var bytes = [UInt8]()
        var idx = secretHex.startIndex
        while idx < secretHex.endIndex {
            let next = secretHex.index(idx, offsetBy: 2)
            let byteStr = secretHex[idx..<next]
            if let byte = UInt8(byteStr, radix: 16) {
                bytes.append(byte)
            }
            idx = next
        }
        return bytes
    }

    /// 生成卡密（仅制卡端需要，dylib 只需 verify；此函数供测试用）
    static func makeCard(machine: String, type: String, issued: UInt32, expires: UInt32, nonce: UInt16) -> String {
        // payload: version(1) + machine(8) + type(1) + issued(4) + expires(4) + nonce(2) = 20
        var payload = [UInt8](repeating: 0, count: 20)
        payload[0] = 1
        let machineBytes = machineBytesFromCode(machine)
        for i in 0..<8 { payload[1 + i] = machineBytes[i] }
        payload[9] = typeIds[type] ?? 1
        writeUInt32BE(issued, into: &payload, at: 10)
        writeUInt32BE(expires, into: &payload, at: 14)
        payload[18] = UInt8(nonce >> 8)
        payload[19] = UInt8(nonce & 0xff)

        let mac = hmacSHA256(key: secretKey, data: payload)
        let raw = payload + mac
        let body = base64URLEncode(raw)
        let prefix = ["monthly": "SFKM", "halfyear": "SFKH", "yearly": "SFKY"][type] ?? "SFKM"
        return prefix + "-" + body
    }

    /// 验卡（离线）
    static func verify(card: String, machine: String) -> (ok: Bool, error: String?, typeName: String?, expiresAt: UInt32?) {
        let trimmed = card.trimmingCharacters(in: .whitespacesAndNewlines)
        guard let dash = trimmed.firstIndex(of: "-") else {
            return (false, "卡密格式错误", nil, nil)
        }
        let prefix = String(trimmed[..<dash]).uppercased()
        let body = String(trimmed[trimmed.index(after: dash)...])

        guard ["SFKM", "SFKH", "SFKY"].contains(prefix) else {
            return (false, "卡密前缀无效", nil, nil)
        }

        guard let raw = base64URLDecode(body) else {
            return (false, "卡密内容无效", nil, nil)
        }
        // raw = payload(20) + HMAC(32) = 52 字节
        guard raw.count == 52 else {
            return (false, "卡密内容无效", nil, nil)
        }
        let payload = Array(raw[0..<20])
        let mac = Array(raw[20..<52])

        // 验 HMAC
        let expected = hmacSHA256(key: secretKey, data: payload)
        guard constantTimeEqual(expected, mac) else {
            return (false, "卡密校验失败", nil, nil)
        }

        // 解析 payload
        let version = payload[0]
        guard version == 1 else { return (false, "卡密版本不兼容", nil, nil) }
        let typeId = payload[9]
        guard let typeName = typeNames[typeId] else { return (false, "卡密类型无效", nil, nil) }

        // 机器码匹配
        let machineBytes = Array(payload[1...8])
        let cardMachine = machineBytes.map { String(format: "%02X", $0) }.joined().uppercased()
        let realMachine = MachineCode.normalize(machine)
        // 比较：cardMachine 去掉尾部 00 填充后与 realMachine 前缀匹配
        guard cardMachine.hasPrefix(realMachine) || realMachine.hasPrefix(cardMachine) else {
            return (false, "卡密与本机机器码不匹配", nil, nil)
        }

        let issued = readUInt32BE(Array(payload[10...13]))
        let expires = readUInt32BE(Array(payload[14...17]))
        _ = issued
        let now = UInt32(Date().timeIntervalSince1970)
        guard expires > now else {
            return (false, "卡密已过期", nil, expires)
        }

        return (true, nil, typeName, expires)
    }

    // MARK: - 工具

    static func machineBytesFromCode(_ code: String) -> [UInt8] {
        let hex = MachineCode.normalize(code)
        var bytes = [UInt8]()
        var idx = hex.startIndex
        while idx < hex.endIndex && bytes.count < 8 {
            let next = hex.index(idx, offsetBy: 2, limitedBy: hex.endIndex) ?? hex.endIndex
            let byteStr = hex[idx..<next]
            if let b = UInt8(byteStr, radix: 16) { bytes.append(b) }
            idx = next
        }
        while bytes.count < 8 { bytes.append(0) }
        return Array(bytes.prefix(8))
    }

    static func writeUInt32BE(_ v: UInt32, into bytes: inout [UInt8], at offset: Int) {
        bytes[offset] = UInt8((v >> 24) & 0xff)
        bytes[offset + 1] = UInt8((v >> 16) & 0xff)
        bytes[offset + 2] = UInt8((v >> 8) & 0xff)
        bytes[offset + 3] = UInt8(v & 0xff)
    }

    static func readUInt32BE(_ bytes: [UInt8]) -> UInt32 {
        guard bytes.count == 4 else { return 0 }
        return (UInt32(bytes[0]) << 24) | (UInt32(bytes[1]) << 16) | (UInt32(bytes[2]) << 8) | UInt32(bytes[3])
    }

    static func hmacSHA256(key: [UInt8], data: [UInt8]) -> [UInt8] {
        var mac = [UInt8](repeating: 0, count: Int(CC_SHA256_DIGEST_LENGTH))
        data.withUnsafeBufferPointer { dataBuf in
            key.withUnsafeBufferPointer { keyBuf in
                CCHmac(CCHmacAlgorithm(kCCHmacAlgSHA256), keyBuf.baseAddress, key.count, dataBuf.baseAddress, data.count, &mac)
            }
        }
        return mac
    }

    static func constantTimeEqual(_ a: [UInt8], _ b: [UInt8]) -> Bool {
        guard a.count == b.count else { return false }
        var diff: UInt8 = 0
        for i in 0..<a.count { diff |= a[i] ^ b[i] }
        return diff == 0
    }

    static func base64URLEncode(_ data: [UInt8]) -> String {
        var s = Data(data).base64EncodedString()
        s = s.replacingOccurrences(of: "+", with: "-").replacingOccurrences(of: "/", with: "_")
        s = s.replacingOccurrences(of: "=", with: "")
        return s
    }

    static func base64URLDecode(_ s: String) -> [UInt8]? {
        var t = s.replacingOccurrences(of: "-", with: "+").replacingOccurrences(of: "_", with: "/")
        let pad = (4 - t.count % 4) % 4
        t += String(repeating: "=", count: pad)
        guard let data = Data(base64Encoded: t) else { return nil }
        return [UInt8](data)
    }
}

/// 机器码
enum MachineCode {
    static func normalize(_ s: String) -> String {
        return s.uppercased().filter { $0.isHexDigit }
    }
    static func current() -> String {
        // UDID（identifierForVendor 或持久化）
        if let saved = UserDefaults.standard.string(forKey: "__KAMI_DEVICE_ID__") {
            return normalize(saved)
        }
        #if os(iOS)
        let id = (UIDevice.current.identifierForVendor?.uuidString ?? UUID().uuidString)
            .replacingOccurrences(of: "-", with: "")
        #else
        let id = UUID().uuidString.replacingOccurrences(of: "-", with: "")
        #endif
        UserDefaults.standard.set(id, forKey: "__KAMI_DEVICE_ID__")
        return normalize(id)
    }
}

/// 密钥占位（编译脚本注入）
enum KamiSecret {
    // 32 字节十六进制 HMAC 密钥（与制卡端 kami_ios_secret.txt 一致）
    static let hex = "6003897b6063574ecd5bae7be7ab369bc9897586811ba3018998585197d9e5ee"
}