@echo off
chcp 65001 >nul
echo ================================
echo    系統環境檢查工具
echo ================================
echo.
echo 檢查作業系統...
systeminfo | findstr /C:"OS Name" /C:"OS Version" /C:"System Type"
echo.
echo 檢查 Visual C++ Runtime...
reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" >nul 2>&1 && echo VC++ 2015-2022 x64: 已安裝 || echo VC++ 2015-2022 x64: 未安裝
echo.
echo 檢查 DirectX...
dxdiag /whql:off /t dxdiag_output.txt
findstr /C:"DirectX Version" dxdiag_output.txt
del dxdiag_output.txt >nul 2>&1
echo.
echo 檢查檔案完整性...
if exist Qt_yolo_1.exe (echo 主程式: 存在) else (echo 主程式: 缺失)
if exist onnxruntime.dll (echo ONNX Runtime: 存在) else (echo ONNX Runtime: 缺失)
if exist Qt6Core.dll (echo Qt6Core: 存在) else (echo Qt6Core: 缺失)
if exist platforms\qwindows.dll (echo Qt平台外掛: 存在) else (echo Qt平台外掛: 缺失)
echo.
echo 檢查權限...
echo > test_write.tmp
if exist test_write.tmp (echo 寫入權限: 正常 & del test_write.tmp) else (echo 寫入權限: 異常)
echo.
pause
