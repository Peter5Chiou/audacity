# Audacity 編譯進度轉移摘要

## 1. 工作目標
在 Windows 11 環境下成功編譯並安裝 Audacity 專案。

## 2. 目前進度
- [x] **環境審核**：確認已安裝 Git, CMake, Ninja 以及 MSVC 2022 編譯器。
- [x] **Qt 安裝**：已安裝 Qt 6.10.3 至 `F:\qt`，並包含必要模組（Qt 5 Compatibility, Network Authorization, Shader Tools, State Machines）。
- [x] **子模組更新**：已執行 `git submodule update --init --recursive` 完成所有相依元件拉取。
- [x] **專案配置**：使用 `audacity-release` 預設配置完成 CMake 配置。

## 3. 目前遇到的困難
- **編譯報錯**：在編譯 `effects_nyquist_tests` 時出現 `fatal error C1083: Cannot open include file: 'wx/dir.h'`。
- **原因分析**：雖然 `wx/dir.h` 檔案存在於 `_deps/wxwidgets` 中，但 CMake 在配置測試目標時未正確將其路徑加入編譯器的搜尋路徑中。

## 4. 採取的解決方法
- **停用單元測試**：由於目標是編譯主程式而非測試集，決定停用所有測試目標以繞過報錯。
- **清理快取**：刪除 `build/audacity-release/` 下的 `CMakeCache.txt`、`CMakeFiles/`、`build.ninja` 及 `.ninja_deps` 以確保配置完全重新生成。
- **重新配置**：使用以下參數重新執行 CMake 配置，強制關閉所有測試選項：
    - `-DMUSE_ENABLE_UNIT_TESTS=OFF`
    - `-DAU_BUILD_EFFECTS_TESTS=OFF`
    - `-DAU_BUILD_APPSHELL_TESTS=OFF`
    - `-DAU_BUILD_PLAYBACK_TESTS=OFF`
    - `-DAU_BUILD_PROJECT_TESTS=OFF`
    - `-DAU_BUILD_RECORD_TESTS=OFF`
    - `-DAU_BUILD_SHARED_TESTS=OFF`
    - `-DAU_BUILD_TRACKEDIT_TESTS=OFF`
    - `-DAU_BUILD_UICOMPONENTS_TESTS=OFF`

---
**後續執行指令：**
```powershell
cmd /c "call VScompilerEnv.bat && cmake --build build/audacity-release --parallel 8"
```
