@echo off
chcp 65001 >nul
echo ================================
echo    除錯模式啟動
echo ================================
echo.
echo 設定除錯環境變數...
set QT_DEBUG_PLUGINS=1
set QT_LOGGING_RULES=*.debug=true
echo.
echo 啟動程式 (除錯模式)...
echo 如果程式無法啟動，請記錄以下錯誤訊息：
echo.
Qt_yolo_1.exe
echo.
echo 程式已結束，按任意鍵查看系統事件...
pause >nul
echo.
echo 檢查 Windows 事件記錄...
powershell -Command "Get-WinEvent -FilterHashtable @{LogName='Application'; StartTime=(Get-Date^).AddMinutes(-5^)} | Where-Object {$_.LevelDisplayName -eq 'Error'} | Select-Object TimeCreated, Id, LevelDisplayName, Message | Format-Table -AutoSize"
echo.
pause
