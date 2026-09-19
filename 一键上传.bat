@echo off
chcp 65001 >nul
cd /d "%~dp0"

echo ================================================
echo  KamiVerify dylib 一键上传到 GitHub
echo ================================================
echo.

echo [1/4] 检查 git ...
where git >nul 2>nul
if %errorlevel% neq 0 (
    echo [错误] 未安装 git，请先到 https://git-scm.com 安装
    pause
    exit /b 1
)

echo [2/4] 提交本地 commit ...
git add -A
git commit -m "kami verify dylib: offline HMAC card verification" >nul 2>&1
if %errorlevel% neq 0 (
    echo  （无新改动或已提交，继续）
)

echo [3/4] 配置远程仓库 ...
echo.
echo  请输入你的 GitHub 仓库地址，例如：
echo    https://github.com/你的用户名/kami-verify.git
echo.
set /p REPO=  仓库地址: 

git remote remove origin 2>nul
git remote add origin %REPO%

echo.
echo [4/4] 推送到 GitHub ...
echo.
echo  提示：如果弹窗让你登录，请输 GitHub 账号；密码处请粘贴 Personal Access Token
echo  （不是 GitHub 登录密码！）
echo.
git push -u origin main 2>&1
if %errorlevel% neq 0 (
    echo.
    echo  上面如果用 main 失败，尝试 master 分支...
    git branch -M master 2>nul
    git push -u origin master 2>&1
)

echo.
echo ================================================
echo  如果推送成功，请去 GitHub 仓库的 Actions 标签页
echo  等待构建完成后，下载 KamiVerify.dylib
echo ================================================
pause