@echo off
@echo SubWCRev.exe  .\version_tamplate.lua .\data\version.lua
SubWCRev.exe .\ .\version_template.lua .\data\version.lua 2> nul || goto ver_failed
@echo Make version info successfully.
goto end

:ver_failed
@echo Make version info failed, use default file
@echo on
copy .\version_template.lua .\data\version.lua
:end

pause