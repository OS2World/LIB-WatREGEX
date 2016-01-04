

                REGEX, Release 1.0 for Open Watcom

SUMMARY
=======
This is the regular expression library originally by Henry Spencer.
Searching on the internet one can find several versions of regex
libraries floating around including variants of this Spencer code
and improved libraries such as TCL and PCRE.

I settled on this version of the regex source because I believe it is
used in FreeBSD. Some fixes/updates have been added by Gary Houston
<http://arglist.com/regex/>. Gary added the GNU configure scripts which
for Open Watcom is not necessary. Also, I found a newer version that
has wide character support and it will compile. However, it requires a
number of additional BSD files and I wanted to keep this simple.

I have compiled and run the tests OS/2-ECS and Win2000/XP. I have also
compiled on OS/2 and successfully run the tests on Linux. Any dos version
is untested.

Michael Greene <greenemk@cox.net>


HISTORY
=======
July 2006 - First attempt at putting in multi-platform make system.


ISSUES:
=======
Running "re -er tests" with a debug version of regex yields assert( )
failures, but a release version passes. At this point I am not sure if
this is a real problem and it is related to using REG_BACKR option. This
option is not POSIX so I am not looking into it, as if I could figure it
out.


BUILDING
========
The makefile system is a multi-platform system. Thanks to Mat Nieuwenhoven
for providing a good makefile example. In the watcom directory is the main
makefile which accepts the following options:

   RO  Release - OS/2-ECS
   RW  Release - WinNT/2000/XP
   RL  Release - Linux

   DO  Debug - OS/2-ECS
   DW  Debug - WinNT/2000/XP
   DL  Debug - Linux

For example:

   wmake RO
      build an OS/2 Release version

Other options:

   wmake full
      build all platform libraries (release and debug)

   wmake
      help screen to display build options

   wmake clean
      clean-up the entire mess

The lib and header will be copied to target specific directories.
File naming:

 re.exe / re.elf  -  release re test executable
 regex.lib        -  release regex library

 re_d.exe / re_d.exe - debug version of test executable linked
                       with regex_d.lib
 regex_d.lib         - debug version of regex.lib

 regex.h - header

Build options such as machine type and optimization can be changed
build\makefile.mif.

Additionally, "wmake runtests" will execute the re program with the
following options:

    re tests
    re -el tests
    re -er tests




