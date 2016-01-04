#
# Watcom multi-executable makefile for REGEX library
#
# Adopted from makefile build system by M. Nieuwenhoven
# by Michael Greene

# There are currently 6 targets: Debug and release for
# 32 bit OS/2, Windows and Linux386

# Use current dir as root of the project
PROJ = $+ $(%cwd) $-

default: .SYMBOLIC
    @echo Valid targets:
    @echo   "wmake [<build><platform>] [full] [clean]"
    @echo     build:
    @echo       D - DEBUG    R - RELEASE
    @echo     platform:
    @echo       O - OS/2-ECS  L - Linux
    @echo       W - Windows
    @echo     Example: (OS/2 Debug build)
    @echo       wmake DO
    @echo     full   - build everything
    @echo     clean  - remove build files

full : .SYMBOLIC
    @set clean=
    %make DO              # OS/2
    %make DW              # Windows
    %make DL              # Linux
    %make RO              # OS/2
    %make RW              # Windows
    %make RL              # Linux

DO : .SYMBOLIC
    @set PROJ   = $(PROJ)
    @set TARGET = os2
    @set MODE   = D
    @%make buildtarg

DW : .SYMBOLIC
    @set PROJ   = $(PROJ)
    @set TARGET = nt
    @set MODE   = D
    @%make buildtarg

DL : .SYMBOLIC
    @set PROJ   = $(PROJ)
    @set TARGET = linux
    @set MODE   = D
    @%make buildtarg

RO : .SYMBOLIC
    @set PROJ   = $(PROJ)
    @set TARGET = os2
    @set MODE   = R
    @%make buildtarg

RW : .SYMBOLIC
    @set PROJ   = $(PROJ)
    @set TARGET = nt
    @set MODE   = R
    @%make buildtarg

RL : .SYMBOLIC
    @set PROJ   = $(PROJ)
    @set TARGET = linux
    @set MODE   = R
    @%make buildtarg


# Internal procedure to build the individual modules. The following macros must
# have been set: D (value D or R), O (D, O, W, L)

buildtarg : .procedure
    -@cd $(%PROJ)
    %write  .\build\maketmp.inc TARGET = $(%TARGET)
    %append .\build\maketmp.inc MODE   = $(%MODE)
    -@mkdir $(%TARGET)
    -@cd build
    wmake -h -a -f makefile.mif
    -@cd ..
    @echo Done.


#
# Clean all directories. Call each target with the 'clean' target
# because clean knowledge is there where the build knowledge is.
#
clean : .SYMBOLIC
    -@rm os2\r*
    -@rd os2
    -@rm nt\r*
    -@rd nt
    -@rm linux\r*
    -@rd linux
    -@cd build
    -@wmake -h -f makefile.mif clean
    @echo Done.

