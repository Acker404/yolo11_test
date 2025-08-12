@echo off
chcp 65001 >nul
echo ================================
echo    Qt YOLO 完整部署打包工具
echo ================================
echo.

REM 設定路徑變數
set "SOURCE_DIR=%~dp0"
set "BUILD_DIR=%SOURCE_DIR%x64\Release"
set "DEPLOY_DIR=%SOURCE_DIR%PortableTest"
set "QT_DIR=C:\Qt\6.7.0\msvc2022_64"
set "OPENCV_DIR=C:\opencv\build\x64\vc16"

echo 正在檢查來源檔案...
if not exist "%BUILD_DIR%\Qt_yolo_1.exe" (
    echo 錯誤：找不到 Qt_yolo_1.exe
    echo 請先編譯 Release 版本
    pause
    exit /b 1
)

echo 清理舊的部署目錄...
if exist "%DEPLOY_DIR%" (
    rmdir /s /q "%DEPLOY_DIR%"
)

echo 建立部署目錄結構...
mkdir "%DEPLOY_DIR%"
mkdir "%DEPLOY_DIR%\platforms"
mkdir "%DEPLOY_DIR%\imageformats"
mkdir "%DEPLOY_DIR%\iconengines"
mkdir "%DEPLOY_DIR%\styles"
mkdir "%DEPLOY_DIR%\multimedia"
mkdir "%DEPLOY_DIR%\generic"
mkdir "%DEPLOY_DIR%\networkinformation"
mkdir "%DEPLOY_DIR%\tls"
mkdir "%DEPLOY_DIR%\translations"

echo.
echo 複製主程式...
copy "%BUILD_DIR%\Qt_yolo_1.exe" "%DEPLOY_DIR%\"

echo.
echo 複製 ONNX Runtime 檔案...
copy "%SOURCE_DIR%packages\Microsoft.ML.OnnxRuntime.1.22.1\runtimes\win-x64\native\onnxruntime.dll" "%DEPLOY_DIR%\"
copy "%SOURCE_DIR%packages\Microsoft.ML.OnnxRuntime.1.22.1\runtimes\win-x64\native\onnxruntime.lib" "%DEPLOY_DIR%\"
copy "%SOURCE_DIR%packages\Microsoft.ML.OnnxRuntime.1.22.1\runtimes\win-x64\native\onnxruntime_providers_shared.dll" "%DEPLOY_DIR%\"

echo.
echo 複製 Qt6 核心檔案...
copy "%QT_DIR%\bin\Qt6Core.dll" "%DEPLOY_DIR%\"
copy "%QT_DIR%\bin\Qt6Gui.dll" "%DEPLOY_DIR%\"
copy "%QT_DIR%\bin\Qt6Widgets.dll" "%DEPLOY_DIR%\"
copy "%QT_DIR%\bin\Qt6Network.dll" "%DEPLOY_DIR%\"
copy "%QT_DIR%\bin\Qt6Multimedia.dll" "%DEPLOY_DIR%\"
copy "%QT_DIR%\bin\Qt6Svg.dll" "%DEPLOY_DIR%\"

echo.
echo 複製 Qt6 平台外掛程式...
copy "%QT_DIR%\plugins\platforms\qwindows.dll" "%DEPLOY_DIR%\platforms\"

echo.
echo 複製圖像格式外掛程式...
copy "%QT_DIR%\plugins\imageformats\*.dll" "%DEPLOY_DIR%\imageformats\"

echo.
echo 複製其他必要外掛程式...
copy "%QT_DIR%\plugins\iconengines\qsvgicon.dll" "%DEPLOY_DIR%\iconengines\"
copy "%QT_DIR%\plugins\styles\qmodernwindowsstyle.dll" "%DEPLOY_DIR%\styles\"

REM 檢查並複製多媒體外掛程式
if exist "%QT_DIR%\plugins\multimedia" (
    copy "%QT_DIR%\plugins\multimedia\*.dll" "%DEPLOY_DIR%\multimedia\"
)

REM 檢查並複製網路外掛程式
if exist "%QT_DIR%\plugins\networkinformation" (
    copy "%QT_DIR%\plugins\networkinformation\*.dll" "%DEPLOY_DIR%\networkinformation\"
)

REM 檢查並複製 TLS 外掛程式
if exist "%QT_DIR%\plugins\tls" (
    copy "%QT_DIR%\plugins\tls\*.dll" "%DEPLOY_DIR%\tls\"
)

REM 檢查並複製通用外掛程式
if exist "%QT_DIR%\plugins\generic" (
    copy "%QT_DIR%\plugins\generic\*.dll" "%DEPLOY_DIR%\generic\"
)

echo.
echo 複製翻譯檔案...
copy "%QT_DIR%\translations\qt_*.qm" "%DEPLOY_DIR%\translations\"

echo.
echo 複製 OpenCV 檔案...
if exist "%OPENCV_DIR%\bin\opencv_world4110.dll" (
    copy "%OPENCV_DIR%\bin\opencv_world4110.dll" "%DEPLOY_DIR%\"
) else (
    echo 警告：找不到 OpenCV DLL，請手動添加
)

echo.
echo 複製 Visual C++ Runtime...
REM 嘗試複製常見的 VC++ Runtime 檔案
copy "%WINDIR%\System32\msvcp140.dll" "%DEPLOY_DIR%\" >nul 2>&1
copy "%WINDIR%\System32\vcruntime140.dll" "%DEPLOY_DIR%\" >nul 2>&1
copy "%WINDIR%\System32\vcruntime140_1.dll" "%DEPLOY_DIR%\" >nul 2>&1

echo.
echo 複製 DirectX 檔案...
copy "%WINDIR%\System32\D3Dcompiler_47.dll" "%DEPLOY_DIR%\" >nul 2>&1
copy "%WINDIR%\System32\opengl32.dll" "%DEPLOY_DIR%\" >nul 2>&1

REM 檢查並複製軟體 OpenGL
if exist "%QT_DIR%\bin\opengl32sw.dll" (
    copy "%QT_DIR%\bin\opengl32sw.dll" "%DEPLOY_DIR%\"
)

echo.
echo 複製 FFmpeg 檔案（如果存在）...
copy "%BUILD_DIR%\avcodec-61.dll" "%DEPLOY_DIR%\" >nul 2>&1
copy "%BUILD_DIR%\avformat-61.dll" "%DEPLOY_DIR%\" >nul 2>&1
copy "%BUILD_DIR%\avutil-59.dll" "%DEPLOY_DIR%\" >nul 2>&1
copy "%BUILD_DIR%\swresample-5.dll" "%DEPLOY_DIR%\" >nul 2>&1
copy "%BUILD_DIR%\swscale-8.dll" "%DEPLOY_DIR%\" >nul 2>&1

echo.
echo 使用 windeployqt 自動偵測依賴項...
if exist "%QT_DIR%\bin\windeployqt.exe" (
    "%QT_DIR%\bin\windeployqt.exe" --release --no-translations --no-system-d3d-compiler --no-opengl-sw "%DEPLOY_DIR%\Qt_yolo_1.exe"
    echo windeployqt 完成
) else (
    echo 警告：找不到 windeployqt.exe
)

echo.
echo 使用 Dependency Walker 風格檢查（如果可用）...
REM 使用 dumpbin 檢查依賴項
if exist "%VS140COMNTOOLS%..\..\VC\bin\dumpbin.exe" (
    echo 正在分析 DLL 依賴項...
    "%VS140COMNTOOLS%..\..\VC\bin\dumpbin.exe" /dependents "%DEPLOY_DIR%\Qt_yolo_1.exe" > "%DEPLOY_DIR%\dependencies.txt"
)

echo.
echo 建立部署資訊檔案...
echo Qt YOLO 應用程式部署包 > "%DEPLOY_DIR%\DEPLOYMENT_INFO.txt"
echo 建立日期：%DATE% %TIME% >> "%DEPLOY_DIR%\DEPLOYMENT_INFO.txt"
echo. >> "%DEPLOY_DIR%\DEPLOYMENT_INFO.txt"
echo 主要元件： >> "%DEPLOY_DIR%\DEPLOYMENT_INFO.txt"
echo - Qt 6.x Framework >> "%DEPLOY_DIR%\DEPLOYMENT_INFO.txt"
echo - ONNX Runtime 1.22.1 >> "%DEPLOY_DIR%\DEPLOYMENT_INFO.txt"
echo - OpenCV 4.11.0 >> "%DEPLOY_DIR%\DEPLOYMENT_INFO.txt"
echo - Visual C++ Runtime >> "%DEPLOY_DIR%\DEPLOYMENT_INFO.txt"
echo. >> "%DEPLOY_DIR%\DEPLOYMENT_INFO.txt"
echo 系統需求： >> "%DEPLOY_DIR%\DEPLOYMENT_INFO.txt"
echo - Windows 10/11 x64 >> "%DEPLOY_DIR%\DEPLOYMENT_INFO.txt"
echo - Visual C++ 2019-2022 Redistributable >> "%DEPLOY_DIR%\DEPLOYMENT_INFO.txt"
echo - DirectX 11 >> "%DEPLOY_DIR%\DEPLOYMENT_INFO.txt"

echo.
echo 建立檔案清單...
dir "%DEPLOY_DIR%" /s /b > "%DEPLOY_DIR%\file_list.txt"

echo.
echo 複製除錯和診斷工具...
call :CREATE_DEBUG_TOOLS

echo.
echo ================================
echo 部署打包完成！
echo 部署目錄：%DEPLOY_DIR%
echo.
echo 下一步：
echo 1. 將整個 PortableTest 資料夾複製到目標電腦
echo 2. 執行 check_system.bat 檢查環境
echo 3. 執行 debug_start.bat 測試程式
echo ================================
pause
goto :EOF

:CREATE_DEBUG_TOOLS
echo 建立診斷工具...

REM 建立系統檢查腳本
(
echo @echo off
echo chcp 65001 ^>nul
echo echo ================================
echo echo    系統環境檢查工具
echo echo ================================
echo echo.
echo echo 檢查作業系統...
echo systeminfo ^| findstr /C:"OS Name" /C:"OS Version" /C:"System Type"
echo echo.
echo echo 檢查 Visual C++ Runtime...
echo reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" ^>nul 2^>^&1 ^&^& echo VC++ 2015-2022 x64: 已安裝 ^|^| echo VC++ 2015-2022 x64: 未安裝
echo echo.
echo echo 檢查 DirectX...
echo dxdiag /whql:off /t dxdiag_output.txt
echo findstr /C:"DirectX Version" dxdiag_output.txt
echo del dxdiag_output.txt ^>nul 2^>^&1
echo echo.
echo echo 檢查檔案完整性...
echo if exist Qt_yolo_1.exe ^(echo 主程式: 存在^) else ^(echo 主程式: 缺失^)
echo if exist onnxruntime.dll ^(echo ONNX Runtime: 存在^) else ^(echo ONNX Runtime: 缺失^)
echo if exist Qt6Core.dll ^(echo Qt6Core: 存在^) else ^(echo Qt6Core: 缺失^)
echo if exist platforms\qwindows.dll ^(echo Qt平台外掛: 存在^) else ^(echo Qt平台外掛: 缺失^)
echo echo.
echo echo 檢查權限...
echo echo ^> test_write.tmp
echo if exist test_write.tmp ^(echo 寫入權限: 正常 ^& del test_write.tmp^) else ^(echo 寫入權限: 異常^)
echo echo.
echo pause
) > "%DEPLOY_DIR%\check_system.bat"

REM 建立除錯啟動腳本
(
echo @echo off
echo chcp 65001 ^>nul
echo echo ================================
echo echo    除錯模式啟動
echo echo ================================
echo echo.
echo echo 設定除錯環境變數...
echo set QT_DEBUG_PLUGINS=1
echo set QT_LOGGING_RULES=*.debug=true
echo echo.
echo echo 啟動程式 ^(除錯模式^)...
echo echo 如果程式無法啟動，請記錄以下錯誤訊息：
echo echo.
echo Qt_yolo_1.exe
echo echo.
echo echo 程式已結束，按任意鍵查看系統事件...
echo pause ^>nul
echo echo.
echo echo 檢查 Windows 事件記錄...
echo powershell -Command "Get-WinEvent -FilterHashtable @{LogName='Application'; StartTime=(Get-Date^).AddMinutes(-5^)} | Where-Object {$_.LevelDisplayName -eq 'Error'} | Select-Object TimeCreated, Id, LevelDisplayName, Message | Format-Table -AutoSize"
echo echo.
echo pause
) > "%DEPLOY_DIR%\debug_start.bat"

REM 建立正常啟動腳本
(
echo @echo off
echo start "" Qt_yolo_1.exe
) > "%DEPLOY_DIR%\start.bat"

REM 建立 README 檔案
(
echo Qt YOLO 應用程式使用說明
echo ========================
echo.
echo 檔案說明：
echo - Qt_yolo_1.exe      : 主程式
echo - start.bat          : 正常啟動程式
echo - debug_start.bat    : 除錯模式啟動
echo - check_system.bat   : 系統環境檢查
echo.
echo 安裝步驟：
echo 1. 解壓縮所有檔案到任意資料夾
echo 2. 執行 check_system.bat 檢查環境
echo 3. 如果環境正常，執行 start.bat 啟動程式
echo 4. 如果程式無法啟動，執行 debug_start.bat 除錯
echo.
echo 常見問題：
echo 1. 缺少 VC++ Runtime
echo    解決方法：安裝 Microsoft Visual C++ 2019-2022 Redistributable ^(x64^)
echo.
echo 2. DirectX 版本過舊
echo    解決方法：更新 DirectX 到最新版本
echo.
echo 3. 防毒軟體阻擋
echo    解決方法：將程式資料夾加入防毒軟體白名單
echo.
echo 4. 權限不足
echo    解決方法：以管理員身分執行
echo.
echo 技術支援：請提供 debug_start.bat 的輸出結果
) > "%DEPLOY_DIR%\README.txt"

echo 診斷工具建立完成
goto :EOF