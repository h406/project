@echo off
cd /d %~dp0/Game/

:: �t�@�C������������N������
if exist ../Release/Game.exe (
    goto start
)

:: �Ȃ�������r���h
"%VS120COMNTOOLS%..\IDE\devenv" /rebuild release /out %~dp0/release/build.log %~dp0/H406.sln

:start
start ../Release/Game.exe