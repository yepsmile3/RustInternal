@echo off
title KamiVerify Upload
cd /d "%~dp0"

echo ================================================
echo   KamiVerify dylib upload to GitHub
echo ================================================
echo.

echo [1/4] Checking git ...
where git >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] git not found. Install from https://git-scm.com
    pause
    exit /b 1
)

echo [2/4] Commit local files ...
git add -A
git commit -m "kami verify dylib: offline HMAC card verification" >nul 2>&1
if %errorlevel% neq 0 (
    echo   (no changes, or already committed)
)

echo [3/4] Set remote repo ...
echo.
echo   Paste your GitHub repo URL, e.g.:
echo     https://github.com/YOURNAME/kami-verify.git
echo.
set /p REPO=  Repo URL: 

git remote remove origin 2>nul
git remote add origin %REPO%

echo.
echo [4/4] Push to GitHub ...
echo.
echo   NOTE: GitHub uses a Personal Access Token, NOT your login password.
echo   If a login popup appears:
echo     username = your GitHub username
echo     password = your Personal Access Token
echo.
git push -u origin main 2>&1
if %errorlevel% neq 0 (
    echo.
    echo   main branch failed, trying master branch ...
    git branch -M master 2>nul
    git push -u origin master 2>&1
)

echo.
echo ================================================
echo   If push succeeded, go to your GitHub repo ->
echo   Actions tab -> wait for build -> download artifact
echo ================================================
pause