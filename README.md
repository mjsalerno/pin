#Getting Pin


- Download Pin from [here](http://software.intel.com/en-us/articles/pin-a-dynamic-binary-instrumentation-tool#GettingStarted)


#Using MemGuard

- extract the Pin tarball you downloaded from [here](http://software.intel.com/en-us/articles/pin-a-dynamic-binary-instrumentation-tool#GettingStarted)

```bash
$ tar -xvzf pin-2.13-62141-gcc.4.4.7-linux.tar.gz
```

- rename it to pin

```bash
$ mv pin-2.13-62141-gcc.4.4.7-linux pin
```

- move it to the root of the MemGuard folder

```bash
$ mv pin memguard/pin
```

- cd into src/

```bash
$ cd memguard/src/
```

- Make and test the tool.

```bash
$ ./make.sh
```

- The results will be in the obj-intel64 folder.

Or run it manually

- make

```bash
$ make PIN_ROOT=../pin
```

- Now run the MemGuard tool on a sample binary from the src directory

```bash
$ ../pin/pin.sh -t obj-intel64/memguard.so -- ./control
```
- clean

```bash
$ make PIN_ROOT=../pin clean
```

or

```bash
$ ./make.sh clean
```

# Errors
An error may occur when you run pin:
```bash
Killed
paul@paul-UX32VD:~/Git/cse409/pin/MyPinTool$ ./pin/pin -t obj-intel64/mallocmem.so -- test
Killed
E:Attach to pid 7725 failed.
E:  The Operating System configuration prevents Pin from using the default (parent) injection mode.
E:  To resolve this, either execute the following (as root):
E:  $ echo 0 > /proc/sys/kernel/yama/ptrace_scope
E:  Or use the "-injection child" option.
E:  For more information, regarding child injection, see Injection section in the Pin User Manual.
E:
```

If this happens do:

1. sudo su
2. echo 0 > /proc/sys/kernel/yama/ptrace_scope
3. exit

Run the command again

#Building Pin Tools


- Navagate to cd source/tools/ManualExamples and make all

```bash
$ cd pin/source/tools/ManualExamples
$ make all
```

- or make just one tool

```bash
$ make malloctrace.test
```

#Using Pin

```bash
$ ./pin -t source/tools/ManualExamples/obj-intel64/<pin-tool>.so  -- <prgm-to-test>
```
example

```bash
$ ./pin -t source/tools/ManualExamples/obj-intel64/malloctrace.so -- ~/Desktop/test
```

# Sources
1. [Detecting uninitialized memory read access bugs using pin](http://jbremer.org/detecting-uninitialized-memory-read-access-bugs-using-pin-a-la-valgrind/)
2. [Pintool tutorials](http://shell-storm.org/repo/Notepad/more-Pin-stuff-references.txt)
3. [Pinheads Discussion Group](http://groups.yahoo.com/neo/groups/pinheads/info)
4. [Pintool API Documentation](http://software.intel.com/sites/landingpage/pintool/docs/49306/Pin/html/group__API__REF.html)