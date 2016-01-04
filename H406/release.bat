@echo off
cd /d %~dp0/Game/

:: ファイルがあったら起動する
if exist ../Release/Game.exe (
    goto start
)

:: なかったらビルド
"%VS120COMNTOOLS%..\IDE\devenv" /rebuild release /out %~dp0/release/build.log %~dp0/H406.sln

:start
start ../Release/Game.exe