; ===================================================================
; Qt_yolo_1 安裝程式產生器 - Inno Setup 6.4.3 相容版本
; ===================================================================
[Setup]
AppName=smkoing detect
AppVersion=1.0
DefaultDirName={autopf}\smkoing detect
DisableProgramGroupPage=yes
OutputDir=.
OutputBaseFilename=Qt_yolo_1_Installer
Compression=lzma2
SolidCompression=yes
ArchitecturesInstallIn64BitMode=x64
MinVersion=10.0
; --- 使用相對路徑 ---
SetupIconFile=code\Qt_yolo_1\x64\Release\icon.ico
; --- 請填寫您的真實資訊 ---
AppPublisher=您的公司或團隊名稱
AppSupportURL=https://your-website.com
AppUpdatesURL=https://your-website.com/updates
WizardStyle=modern
ShowLanguageDialog=no
SetupLogging=yes
; --- 進度相關設定 ---
UsePreviousSetupType=no
UsePreviousTasks=no

[Messages]
; --- 自訂進度訊息 (中文化) ---
SetupWindowTitle=安裝 - %1
FinishedLabel=安裝程式已成功在您的電腦上安裝 [name]。
InstallingLabel=正在安裝 %1，請稍候...
StatusExtractFiles=正在解壓縮檔案...
StatusCreateDirs=正在建立目錄...
StatusInstallFiles=正在安裝檔案...
ConfirmUninstall=您確定要將 %1 及其所有元件從您的電腦中移除嗎?

[Tasks]
Name: "desktopicon"; Description: "建立桌面捷徑"; GroupDescription: "其他任務:"; Flags: checkedonce

[Dirs]
; --- 使用此區段預先建立必要的資料夾 ---
Name: "{app}\models"
Name: "{app}\logs"

[Files]
; --- 主程式和所有依賴檔案 (使用相對路徑) ---
; 一次性複製 Release 資料夾內所有內容到安裝目錄，排除不必要的開發檔案
Source: "code\Qt_yolo_1\x64\Release\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs; Excludes: "*.pdb,*.ilk,*.vshost.*"

; --- YOLO 模型檔案 ---
Source: "model\best.onnx"; DestDir: "{app}\models"; Flags: ignoreversion

; --- VC++ Redistributable 安裝檔 ---
; 將 vc_redist.x64.exe 放在與 .iss 同層的根目錄
Source: "vc_redist.x64.exe"; DestDir: "{tmp}"; Flags: deleteafterinstall; Check: VCRedistNeedsInstall

[Icons]
Name: "{group}\smkoing detect"; Filename: "{app}\Qt_yolo_1.exe"; IconFilename: "{app}\icon.ico"
Name: "{autodesktop}\smkoing detect"; Filename: "{app}\Qt_yolo_1.exe"; IconFilename: "{app}\icon.ico"; Tasks: desktopicon

[Run]
; --- 執行 VC++ Redistributable 安裝 ---
Filename: "{tmp}\vc_redist.x64.exe"; Parameters: "/quiet /norestart"; StatusMsg: "正在安裝 Microsoft Visual C++ 運行庫..."; Check: VCRedistNeedsInstall; Flags: runhidden waituntilterminated

; --- 安裝完成後提供啟動選項 ---
Filename: "{app}\Qt_yolo_1.exe"; Description: "啟動 smkoing detect"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
; --- 解除安裝時，刪除整個應用程式資料夾，確保乾淨 ---
Type: filesandordirs; Name: "{app}"

[Code]
var
  ProgressPage: TOutputProgressWizardPage;
  ProgressPageVisible: Boolean;

// ===================================================================
// 檢查是否需要安裝 Visual C++ Redistributable
// 這是比較穩健的檢查方式，適用於 2015-2022 版本
// ===================================================================
function VCRedistNeedsInstall: Boolean;
var
  Version: Cardinal;
begin
  Result := True;
  
  // 檢查 x64 版本是否已安裝
  if RegQueryDWordValue(HKLM, 'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x64', 'Installed', Version) then
  begin
    if Version = 1 then
    begin
      Log('Visual C++ 2015-2022 Redistributable (x64) is already installed.');
      Result := False;
      Exit;
    end;
  end;
  
  // 檢查其他可能的註冊表位置
  if RegKeyExists(HKLM, 'SOFTWARE\Classes\Installer\Dependencies\Microsoft.VS.VC_RuntimeMinimumVSU_amd64,v14') then
  begin
    Log('Visual C++ Redistributable found via alternate registry key.');
    Result := False;
    Exit;
  end;
  
  Log('Visual C++ 2015-2022 Redistributable (x64) not found. Installation required.');
end;

// 初始化嚮導時建立自訂進度頁面
procedure InitializeWizard();
begin
  // 建立進度頁面
  ProgressPage := CreateOutputProgressPage('安裝進度', '正在安裝必要的組件，請稍候...');
  ProgressPageVisible := False;
  
  // 設定進度頁面的初始狀態
  ProgressPage.SetText('準備安裝環境...', '');
  ProgressPage.SetProgress(0, 100);
end;

// 當安裝步驟改變時的處理
procedure CurStepChanged(CurStep: TSetupStep);
begin
  case CurStep of
    ssInstall:
    begin
      // 顯示進度頁面
      ProgressPage.Show;
      ProgressPageVisible := True;
      try
        ProgressPage.SetText('正在解壓縮檔案...', '準備安裝程式檔案');
        ProgressPage.SetProgress(10, 100);
        Sleep(100); // 短暫延遲以顯示進度
      except
        // 處理可能的例外
      end;
    end;
    
    ssPostInstall:
    begin
      if ProgressPageVisible then
      begin
        ProgressPage.SetText('正在完成安裝...', '建立捷徑和設定檔案');
        ProgressPage.SetProgress(90, 100);
        Sleep(100);
        
        // 確保必要目錄存在
        if not DirExists(ExpandConstant('{app}\logs')) then
        begin
          ForceDirectories(ExpandConstant('{app}\logs'));
          Log('Created logs directory');
        end;
        
        ProgressPage.SetProgress(100, 100);
        Sleep(200);
        ProgressPage.Hide;
        ProgressPageVisible := False;
      end;
    end;
  end;
end;

// 檔案安裝進度更新
procedure CurInstallProgressChanged(CurProgress, MaxProgress: Integer);
var
  ProgressPercent: Integer;
begin
  if Assigned(ProgressPage) and ProgressPageVisible then
  begin
    // 計算進度百分比 (10% 到 85% 之間)
    if MaxProgress > 0 then
    begin
      ProgressPercent := 10 + Round((CurProgress * 75) / MaxProgress);
      ProgressPage.SetProgress(ProgressPercent, 100);
      
      // 根據進度顯示不同訊息
      if ProgressPercent < 30 then
        ProgressPage.SetText('正在安裝核心程式...', '複製執行檔和庫檔案')
      else if ProgressPercent < 60 then
        ProgressPage.SetText('正在安裝系統組件...', '複製 Qt 框架和相依性檔案')
      else if ProgressPercent < 85 then
        ProgressPage.SetText('正在安裝AI模型...', '複製 YOLO 模型和設定檔')
      else
        ProgressPage.SetText('即將完成安裝...', '進行最後設定');
    end;
  end;
end;