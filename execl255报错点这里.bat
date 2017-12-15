@echo off
for /f  %%a in ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\10.0\Access Connectivity Engine\Engines\Excel" /v TypeGuessRows 2^>nul') do set "a=%%b"
if "%a%"=="" (echo 10NO) else (echo 10YES 
							 reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\10.0\Access Connectivity Engine\Engines\Excel" /t REG_DWORD /v TypeGuessRows /d 0 /f
							 reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\10.0\Access Connectivity Engine\Engines\Lotus" /t REG_DWORD /v TypeGuessRows /d 0 /f)

for /f  %%b in ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\11.0\Access Connectivity Engine\Engines\Excel" /v TypeGuessRows 2^>nul') do set "b=%%b"
if "%b%"=="" (echo 11NO) else (echo 11YES 
							 reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\11.0\Access Connectivity Engine\Engines\Excel" /t REG_DWORD /v TypeGuessRows /d 0 /f
							 reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\11.0\Access Connectivity Engine\Engines\Lotus" /t REG_DWORD /v TypeGuessRows /d 0 /f)

for /f  %%c in ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\12.0\Access Connectivity Engine\Engines\Excel" /v TypeGuessRows 2^>nul') do set "c=%%b"
if "%c%"=="" (echo 12NO) else (echo 12YES 
							 reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\12.0\Access Connectivity Engine\Engines\Excel" /t REG_DWORD /v TypeGuessRows /d 0 /f
							 reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\12.0\Access Connectivity Engine\Engines\Lotus" /t REG_DWORD /v TypeGuessRows /d 0 /f)

for /f  %%d in ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\13.0\Access Connectivity Engine\Engines\Excel" /v TypeGuessRows 2^>nul') do set "d=%%b"
if "%d%"=="" (echo 13NO) else (echo 13YES 
							 reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\13.0\Access Connectivity Engine\Engines\Excel" /t REG_DWORD /v TypeGuessRows /d 0 /f
							 reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\13.0\Access Connectivity Engine\Engines\Lotus" /t REG_DWORD /v TypeGuessRows /d 0 /f)

for /f  %%e in ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\14.0\Access Connectivity Engine\Engines\Excel" /v TypeGuessRows 2^>nul') do set "e=%%b"
if "%e%"=="" (echo 14NO) else (echo 14YES 
						     reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\14.0\Access Connectivity Engine\Engines\Excel" /t REG_DWORD /v TypeGuessRows /d 0 /f
							 reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\14.0\Access Connectivity Engine\Engines\Lotus" /t REG_DWORD /v TypeGuessRows /d 0 /f)

for /f  %%f in ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\15.0\Access Connectivity Engine\Engines\Excel" /v TypeGuessRows 2^>nul') do set "f=%%b"
if "%f%"=="" (echo 15NO) else (echo 15YES 
	 						 reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\15.0\Access Connectivity Engine\Engines\Excel" /t REG_DWORD /v TypeGuessRows /d 0 /f
							 reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\15.0\Access Connectivity Engine\Engines\Lotus" /t REG_DWORD /v TypeGuessRows /d 0 /f)

for /f  %%g in ('reg query "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\16.0\Access Connectivity Engine\Engines\Excel" /v TypeGuessRows 2^>nul') do set "g=%%b"
if "%g%"=="" (echo 16NO) else (echo 16YES 
							 reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\16.0\Access Connectivity Engine\Engines\Excel" /t REG_DWORD /v TypeGuessRows /d 0 /f
							 reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Office\16.0\Access Connectivity Engine\Engines\Lotus" /t REG_DWORD /v TypeGuessRows /d 0 /f)

reg add HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Jet\4.0\Engines\Excel /t REG_DWORD /v TypeGuessRows /d 0 /f
reg add HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\Jet\4.0\Engines\Lotus /t REG_DWORD /v TypeGuessRows /d 0 /f
pause
