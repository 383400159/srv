@echo off

echo ---- server switch ----
echo -
echo -  1. start server
echo -  2. gen slx to data
echo -  3. gen protocol
echo -  4. export to src/Debug
echo -  5. start game
echo -  s. svn update
echo -  c. svn commit
echo -  b. build project
echo -  rb.rebuild project
echo -  other. kill all
echo -

set /p answer=input:

if "%answer%" == "1" (
	start cmd /k global_srv.exe serverconfig.xml
	start /min cmd /k game_srv.exe serverconfig.xml 0
	start /min cmd /k chat_srv.exe
	start /min cmd /k ga_srv.exe serverconfig.xml 1
	start /min cmd /k login_srv.exe
) else if "%answer%" == "2" (
	cd ../data
	Ë«»÷ÎÒ.bat
) else if "%answer%" == "3" (
	cd ../servercommon/protobuf
	@echo on
	build.bat
	@echo off
) else if "%answer%" == "4" (
	explorer %cd%
) else if "%answer%" == "5" (
	start /min cmd /k game_srv.exe serverconfig.xml 1
) else if "%answer%" == "s" (
	TortoiseProc.exe /command:update /path:../../../../../
) else if "%answer%" == "c" (
	TortoiseProc.exe /command:commit /path:../../../../../
) else if "%answer%" == "b" (
	build.bat Build
) else if "%answer%" == "rb" (
	build.bat Rebuild
) else (
	taskkill /f /im db_srv.exe
	taskkill /f /im global_srv.exe
	taskkill /f /im chat_srv.exe
	taskkill /f /im login_srv.exe
	taskkill /f /im game_srv.exe
	taskkill /f /im ga_srv.exe
	taskkill /f /im robot.exe
	taskkill /f /im cmd.exe
)
