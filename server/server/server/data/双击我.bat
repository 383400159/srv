xls2lua.exe -s xls data
::gen code def start
@echo off
if exist data\client rd /s /q data\client
if exist "../global_script" (
	lua51.exe gen_define.lua
	lua51.exe gen_protocol.lua
)
@echo on
::gen code def end

pause

