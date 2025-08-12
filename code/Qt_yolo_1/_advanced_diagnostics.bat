@echo off
chcp 65001 >nul
setlocal enabledelayedexpansion

echo ================================
echo     進階診斷與修復工具
echo ================================
echo.

REM 建立診斷報告檔案
set "REPORT_FILE=diagnostic_report_%DATE:~0,4%%DATE:~5,2%%DATE:~8,2%_%TIME:~0,2%%TIME:~3,2%.txt"
set "REPORT_FILE=!REPORT_FILE: =0!"

echo 正在建立診斷報告: %REPORT_FILE%
echo Qt YOLO 應用程式診斷報告 > "%REPORT_FILE%"
echo 生成時間: %DATE% %TIME% >> "%REPORT_FILE%"
echo ================================ >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"

echo 1. 系統資訊檢查...
echo [系統資訊] >> "%REPORT_FILE%"
systeminfo | findstr /C:"OS Name" /C:"OS Version" /C:"System Type" /C:"Total Physical Memory" >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"

echo 2. 處理器資訊...
echo [處理器資訊] >> "%REPORT_FILE%"
wmic cpu get name,maxclockspeed,numberofcores,numberoflogicalprocessors /format:list | findstr /v "^$" >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"

echo 3. 記憶體資訊...
echo [記憶體資訊] >> "%REPORT_FILE%"
wmic OS get TotalVisibleMemorySize,FreePhysicalMemory /format:list | findstr /v "^$" >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"

echo 4. 顯示卡資訊...
echo [顯示卡資訊] >> "%REPORT_FILE%"
wmic path win32_VideoController get name,adapterram,driverversion /format:list | findstr /v "^$" >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"

echo 5. 已安裝程式檢查...
echo [Visual C++ Runtime 檢查] >> "%REPORT_FILE%"
reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" >>"%REPORT_FILE%" 2>&1
reg query "HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\VisualStudio\14.0\VC\Runtimes\x86" >>"%REPORT_FILE%" 2>&1
echo. >> "%REPORT_FILE%"

echo 6. DirectX 資訊...
echo [DirectX 資訊] >> "%REPORT_FILE%"
dxdiag /whql:off /t temp_dx.txt
findstr /C:"DirectX Version" /C:"Display Memory" /C:"Driver Model" temp_dx.txt >> "%REPORT_FILE%"
del temp_dx.txt >nul 2>&1
echo. >> "%REPORT_FILE%"

echo 7. 網路連線檢查...
echo [網路連線] >> "%REPORT_FILE%"
ping -n 1 8.8.8.8 >nul 2>&1 && echo 網路連線: 正常 >> "%REPORT_FILE%" || echo 網路連線: 異常 >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"

echo 8. 防毒軟體檢查...
echo [防毒軟體] >> "%REPORT_FILE%"
wmic /namespace:\\root\securitycenter2 path antivirusproduct get displayname,productstate /format:list | findstr /v "^$" >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"

echo 9. Windows Defender 狀態...
echo [Windows Defender] >> "%REPORT_FILE%"
powershell -Command "Get-MpComputerStatus | Select-Object AntivirusEnabled,RealTimeProtectionEnabled,IoavProtectionEnabled | Format-List" >> "%REPORT_FILE%" 2>&1
echo. >> "%REPORT_FILE%"

echo 10. 檔案關聯檢查...
echo [檔案關聯] >> "%REPORT_FILE%"
assoc .exe >> "%REPORT_FILE%" 2>&1
ftype exefile >> "%REPORT_FILE%" 2>&1
echo. >> "%REPORT_FILE%"

echo 11. 環境變數檢查...
echo [重要環境變數] >> "%REPORT_FILE%"
echo PATH=%PATH% >> "%REPORT_FILE%"
echo TEMP=%TEMP% >> "%REPORT_FILE%"
echo TMP=%TMP% >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"

echo 12. 使用者權限檢查...
echo [使用者權限] >> "%REPORT_FILE%"
whoami /groups | findstr /C:"管理" /C:"Administrators" >nul 2>&1 && echo 管理員權限: 是 >> "%REPORT_FILE%" || echo 管理員權限: 否 >> "%REPORT_FILE%"
echo 當前使用者: >> "%REPORT_FILE%"
whoami >> "%REPORT_FILE%"
echo. >> "%REPORT_FILE%"

echo 13. 檔案完整性檢查...
echo [檔案完整性檢查] >> "%REPORT_FILE%"
if exist Qt_yolo_1.exe (
    echo Qt_yolo_1.exe: 存在 >> "%REPORT_FILE%"
    for %%F in (Qt_yolo_1.exe) do echo 檔案大小: %%~zF bytes >> "%REPORT_FILE%"
    powershell -Command "Get-FileHash Qt_yolo_1.exe -Algorithm SHA256 | Select-Object Hash | Format-List" >> "%REPORT_FILE%" 2>&1
) else (
    echo Qt_yolo_1.exe: 缺失 [嚴重錯誤] >> "%REPORT_FILE%"
)

REM 檢查關鍵 DLL
set "CRITICAL_DLLS=onnxruntime.dll onnxruntime_providers_shared.dll Qt6Core.dll Qt6Gui.dll Qt6Widgets.dll"
for %%D in (%CRITICAL_DLLS%) do (
    if exist %%D (
        echo %%D: 存在 >> "%REPORT_FILE%"
    ) else (
        echo %%D: 缺失 [錯誤] >> "%REPORT_FILE%"
    )
)

REM 檢查外掛程式目錄
if exist platforms\qwindows.dll (
    echo platforms\qwindows.dll: 存在 >> "%REPORT_FILE%"
) else (
    echo platforms\qwindows.dll: 缺失 [嚴重錯誤] >> "%REPORT_FILE%"
)

echo. >> "%REPORT_FILE%"

echo 14. DLL 依賴項分析...
if exist Qt_yolo_1.exe (
    echo [DLL 依賴項分析] >> "%REPORT_FILE%"
    REM 使用 PowerShell 分析 PE 檔案
    powershell -Command "
    try {
        Add-Type -AssemblyName System.Drawing
        $bytes = [System.IO.File]::ReadAllBytes('Qt_yolo_1.exe')
        if ($bytes.Length -gt 1024) {
            'PE 檔案讀取成功，大小: ' + $bytes.Length + ' bytes'
        }
    } catch {
        'PE 檔案分析失敗: ' + $_.Exception.Message
    }" >> "%REPORT_FILE%" 2>&1
    echo. >> "%REPORT_FILE%"
)

echo 15. 埠占用檢查...
echo [埠占用檢查] >> "%REPORT_FILE%"
netstat -an | findstr :80 | findstr LISTENING >> "%REPORT_FILE%" 2>&1
echo. >> "%REPORT_FILE%"

echo 16. 事件日誌檢查...
echo [最近應用程式錯誤] >> "%REPORT_FILE%"
powershell -Command "
try {
    Get-WinEvent -FilterHashtable @{LogName='Application'; Level=2; StartTime=(Get-Date).AddHours(-1)} -MaxEvents 10 -ErrorAction SilentlyContinue | 
    Select-Object TimeCreated, Id, LevelDisplayName, ProviderName, Message | 
    Format-Table -AutoSize
} catch {
    '無法讀取事件日誌或無相關錯誤'
}" >> "%REPORT_FILE%" 2>&1
echo. >> "%REPORT_FILE%"

echo 17. 建立修復建議...
echo [修復建議] >> "%REPORT_FILE%"

REM 檢查常見問題並給出建議
set "SUGGESTIONS=0"

if not exist Qt_yolo_1.exe (
    echo - 主程式檔案缺失，請重新解壓縮完整的應用程式包 >> "%REPORT_FILE%"
    set /a "SUGGESTIONS+=1"
)

if not exist onnxruntime.dll (
    echo - ONNX Runtime 缺失，可能需要重新部署或安裝特定版本 >> "%REPORT_FILE%"
    set /a "SUGGESTIONS+=1"
)

if not exist platforms\qwindows.dll (
    echo - Qt 平台外掛程式缺失，程式無法建立視窗介面 >> "%REPORT_FILE%"
    set /a "SUGGESTIONS+=1"
)

reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64" >nul 2>&1 || (
    echo - 建議安裝 Microsoft Visual C++ 2015-2022 Redistributable ^(x64^) >> "%REPORT_FILE%"
    set /a "SUGGESTIONS+=1"
)

whoami /groups | findstr /C:"管理" /C:"Administrators" >nul 2>&1 || (
    echo - 建議以管理員身分執行程式 >> "%REPORT_FILE%"
    set /a "SUGGESTIONS+=1"
)

if !SUGGESTIONS! EQU 0 (
    echo - 未發現明顯問題，建議檢查防毒軟體設定或系統相容性 >> "%REPORT_FILE%"
)

echo. >> "%REPORT_FILE%"
echo ================================ >> "%REPORT_FILE%"
echo 診斷完成時間: %DATE% %TIME% >> "%REPORT_FILE%"

echo.
echo ================================
echo 診斷完成！
echo 報告儲存為：%REPORT_FILE%
echo ================================
echo.
echo 發現的問題數量：!SUGGESTIONS!
echo.

if !SUGGESTIONS! GTR 0 (
    echo 建議的修復動作：
    echo 1. 查看診斷報告中的 [修復建議] 部分
    echo 2. 執行 3_auto_repair.bat 進行自動修復
    echo 3. 如果問題仍然存在，請將診斷報告發送給技術支援
) else (
    echo 系統檢查正常，建議：
    echo 1. 檢查防毒軟體是否阻擋程式執行
    echo 2. 嘗試以管理員身分執行
    echo 3. 暫時停用即時保護後測試
)

echo.
echo 按任意鍵檢視完整診斷報告...
pause >nul
notepad "%REPORT_FILE%"