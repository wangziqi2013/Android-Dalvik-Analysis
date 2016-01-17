# Android-Dalvik-Analysis (ADA)

I implemented an Android Dex (Dalvik Executable) format reader and disassembler in C++. The code is super fast, and runs stably on several testing Dex files. Currently this tools has two parts, a dex file reader and a bytecode decoder (disassembler). In the next section they will be introduced briefly.

Dex Reader
==========
Background
----------
Dex is a short-hand for "Dalvik Executable" which features the Dalvik virtual machine deployed on numerous Android devices. Though recently overshadowed by the next generation Android platform, Ant, Dalvik application still plays an important role in Android world, and thus remains valuable to be analyzed for reverse engineering, software certification or privacy consistency checking.

Dex file is a direct translation from compiled JAVA bytecode, which is then packed into an APK package together with resource and configuration files for release and deployment. Due to its nature of being close to JAVA bytecode, Dex file actually shares many of the design decisions and layout properties of a JAVA class file. To be precise, an APK package is no more than a special version of ZIP archive which could be read by Zip utilities on Windows and Linux, while the compiled Dex file always being at the root of an APK with the name "classes.dex". Therefore, the first step to open an Android application for analysis is to extract the Dex file from APK package.

Bytecode Disassembler
=====================
