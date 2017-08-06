rem @echo off
for /f "delims=\" %%a in ('dir /b /a-d /o-d "../protofile/*.proto"') do protoc -I=../protofile --cpp_out=./ ../protofile/%%a 
cd build_lua
build