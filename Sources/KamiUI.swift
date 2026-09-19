// KamiUI.swift — 卡密验证界面（注入目标 App 启动时弹出）
// 验证通过后移除，放行目标 App；未通过则一直盖住
import UIKit

final class KamiUI {

    static let shared = KamiUI()
    private var overlayWindow: UIWindow?

    /// 是否已验证
    static var isVerified: Bool {
        get { UserDefaults.standard.bool(forKey: "__KAMI_VERIFIED__") }
        set { UserDefaults.standard.set(newValue, forKey: "__KAMI_VERIFIED__") }
    }
    static var savedCard: String? {
        get { UserDefaults.standard.string(forKey: "__SAVED_CARD__") }
        set { UserDefaults.standard.set(newValue, forKey: "__SAVED_CARD__") }
    }
    static var savedExpires: Double {
        get { UserDefaults.standard.double(forKey: "__KAMI_EXPIRES__") }
        set { UserDefaults.standard.set(newValue, forKey: "__KAMI_EXPIRES__") }
    }
    static var savedTypeName: String? {
        get { UserDefaults.standard.string(forKey: "__KAMI_TYPE_NAME__") }
        set { UserDefaults.standard.set(newValue, forKey: "__KAMI_TYPE_NAME__") }
    }

    /// 检查是否已通过验证（卡密有效期内）
    static func checkVerified() -> Bool {
        guard isVerified, let card = savedCard else { return false }
        let machine = MachineCode.current()
        let result = KamiCrypto.verify(card: card, machine: machine)
        if result.ok {
            return true
        }
        // 卡密失效（过期/机器码变了），清状态
        isVerified = false
        return false
    }

    /// 显示卡密验证界面
    func show() {
        // 已显示则跳过
        if overlayWindow != nil { return }
        // 已验证则跳过
        if KamiUI.checkVerified() { return }

        DispatchQueue.main.async { [weak self] in
            guard let self = self else { return }
            guard let windowScene = self.activeScene() else { return }

            let window = UIWindow(windowScene: windowScene)
            window.windowLevel = .alert + 1
            let vc = KamiViewController()
            window.rootViewController = vc
            window.makeKeyAndVisible()
            self.overlayWindow = window
        }
    }

    /// 隐藏（验证通过后调用）
    func hide() {
        DispatchQueue.main.async { [weak self] in
            self?.overlayWindow?.isHidden = true
            self?.overlayWindow = nil
        }
    }

    private func activeScene() -> UIWindowScene? {
        #if os(iOS)
        if #available(iOS 13.0, *) {
            return UIApplication.shared.connectedScenes
                .compactMap { $0 as? UIWindowScene }
                .first { $0.activationState == .foregroundActive }
                ?? UIApplication.shared.connectedScenes.compactMap { $0 as? UIWindowScene }.first
        }
        #endif
        return nil
    }
}

final class KamiViewController: UIViewController {

    private let cardField = UITextField()
    private let machineLabel = UILabel()
    private let statusLabel = UILabel()
    private let verifyButton = UIButton(type: .system)

    override func viewDidLoad() {
        super.viewDidLoad()
        setupUI()
    }

    private func setupUI() {
        view.backgroundColor = UIColor(red: 0.05, green: 0.07, blue: 0.10, alpha: 1.0)

        let icon = UIImageView()
        icon.backgroundColor = UIColor(red: 0.12, green: 0.55, blue: 0.45, alpha: 1.0)
        icon.layer.cornerRadius = 16
        icon.translatesAutoresizingMaskIntoConstraints = false

        let title = UILabel()
        title.text = "卡密验证"
        title.textColor = .white
        title.font = .boldSystemFont(ofSize: 22)
        title.textAlignment = .center
        title.translatesAutoresizingMaskIntoConstraints = false

        machineLabel.text = "机器码: " + MachineCode.current()
        machineLabel.textColor = UIColor(red: 0.3, green: 0.9, blue: 0.7, alpha: 1.0)
        machineLabel.font = .monospacedSystemFont(ofSize: 12, weight: .regular)
        machineLabel.textAlignment = .center
        machineLabel.numberOfLines = 0
        machineLabel.translatesAutoresizingMaskIntoConstraints = false

        cardField.placeholder = "请输入卡密"
        cardField.autocorrectionType = .no
        cardField.autocapitalizationType = .none
        cardField.backgroundColor = UIColor(red: 0.02, green: 0.02, blue: 0.03, alpha: 1.0)
        cardField.textColor = .white
        cardField.layer.borderColor = UIColor(red: 0.2, green: 0.2, blue: 0.25, alpha: 1.0).cgColor
        cardField.layer.borderWidth = 1
        cardField.layer.cornerRadius = 12
        cardField.leftView = UIView(frame: CGRect(x: 0, y: 0, width: 12, height: 0))
        cardField.leftViewMode = .always
        cardField.translatesAutoresizingMaskIntoConstraints = false

        verifyButton.setTitle("立即验证", for: .normal)
        verifyButton.setTitleColor(.white, for: .normal)
        verifyButton.backgroundColor = UIColor(red: 0.1, green: 0.65, blue: 0.5, alpha: 1.0)
        verifyButton.layer.cornerRadius = 12
        verifyButton.translatesAutoresizingMaskIntoConstraints = false
        verifyButton.addTarget(self, action: #selector(verifyTapped), for: .touchUpInside)

        statusLabel.text = ""
        statusLabel.textColor = UIColor(red: 0.95, green: 0.4, blue: 0.4, alpha: 1.0)
        statusLabel.font = .systemFont(ofSize: 13)
        statusLabel.textAlignment = .center
        statusLabel.numberOfLines = 0
        statusLabel.translatesAutoresizingMaskIntoConstraints = false

        view.addSubview(icon)
        view.addSubview(title)
        view.addSubview(machineLabel)
        view.addSubview(cardField)
        view.addSubview(verifyButton)
        view.addSubview(statusLabel)

        NSLayoutConstraint.activate([
            icon.topAnchor.constraint(equalTo: view.safeAreaLayoutGuide.topAnchor, constant: 80),
            icon.centerXAnchor.constraint(equalTo: view.centerXAnchor),
            icon.widthAnchor.constraint(equalToConstant: 64),
            icon.heightAnchor.constraint(equalToConstant: 64),

            title.topAnchor.constraint(equalTo: icon.bottomAnchor, constant: 16),
            title.centerXAnchor.constraint(equalTo: view.centerXAnchor),

            machineLabel.topAnchor.constraint(equalTo: title.bottomAnchor, constant: 12),
            machineLabel.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: 24),
            machineLabel.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -24),

            cardField.topAnchor.constraint(equalTo: machineLabel.bottomAnchor, constant: 20),
            cardField.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: 32),
            cardField.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -32),
            cardField.heightAnchor.constraint(equalToConstant: 46),

            verifyButton.topAnchor.constraint(equalTo: cardField.bottomAnchor, constant: 16),
            verifyButton.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: 32),
            verifyButton.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -32),
            verifyButton.heightAnchor.constraint(equalToConstant: 46),

            statusLabel.topAnchor.constraint(equalTo: verifyButton.bottomAnchor, constant: 16),
            statusLabel.leadingAnchor.constraint(equalTo: view.leadingAnchor, constant: 24),
            statusLabel.trailingAnchor.constraint(equalTo: view.trailingAnchor, constant: -24),
        ])
    }

    @objc private func verifyTapped() {
        let card = cardField.text?.trimmingCharacters(in: .whitespacesAndNewlines) ?? ""
        guard !card.isEmpty else {
            statusLabel.text = "请输入卡密"
            return
        }
        verifyButton.isEnabled = false
        verifyButton.setTitle("验证中...", for: .normal)

        // 离线验卡，立刻返回（绝不卡住）
        let machine = MachineCode.current()
        let result = KamiCrypto.verify(card: card, machine: machine)

        if result.ok {
            KamiUI.isVerified = true
            KamiUI.savedCard = card
            KamiUI.savedExpires = Double(result.expiresAt ?? 0)
            KamiUI.savedTypeName = result.typeName
            statusLabel.textColor = UIColor(red: 0.3, green: 0.9, blue: 0.7, alpha: 1.0)
            statusLabel.text = "激活成功！\(result.typeName ?? "")"
            DispatchQueue.main.asyncAfter(deadline: .now() + 0.5) {
                KamiUI.shared.hide()
            }
        } else {
            statusLabel.text = result.error ?? "验证失败"
            verifyButton.isEnabled = true
            verifyButton.setTitle("立即验证", for: .normal)
        }
    }
}