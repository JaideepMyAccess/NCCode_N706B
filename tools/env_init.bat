@echo off
set CURDIR=%~dp0
set rar="7z.exe"
set gnu_cross_dir=%CURDIR%GNU_Arm_Toolchain
set gnu_cross_zip=%CURDIR%GNU_Arm_Toolchain.zip
set packet_tool_dir=%CURDIR%aboot
set packet_tool_zip=%CURDIR%aboot.zip

if exist %gnu_cross_dir% (
	echo ---------------GNU Arm Toolchain exist----------------
)else (
	echo ---------------uncompress GNU Arm Toolchain----------------
	%rar% x %gnu_cross_zip% -o%CURDIR%
	echo ------------uncompress GNU Arm Toolchain successfully-------------
)

if exist %packet_tool_dir% (
	echo ---------------packet tool exist----------------
)else (
	echo ---------------uncompress packet tool----------------
	%rar% x %packet_tool_zip% -o%CURDIR%
	echo ------------uncompress packet tool successfully-------------
)

set PATH=%gnu_cross_dir%;%gnu_cross_dir%\bin;%CURDIR%gnumake;%PATH%