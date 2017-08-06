for /f "delims=\" %%a in ('dir /b /a-d /o-d "../../protofile/"') do ..\protoc -I=../../protofile --lua_out=../lua_src --plugin=protoc-gen-lua="protoc-gen-lua.bat" ../../protofile/%%a

pause