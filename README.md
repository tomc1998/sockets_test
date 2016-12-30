# sockets_test
Test of winsock (and potentially BSD in the future) socket API in C.

##Building
---
Can build by running cmake on the root directory. This will generate 2 projects which can be built separately, named 'client' and 'server'.

For an example on how to build this on windows:

```
mkdir build
cd build
cmake ..
msbuild ALL_BUILD.vcxproj
cd ..
```

Binaries for the client and server projects should be found under `build\<project>\Debug\<project>.exe` after this. For example, for the client - `build\client\Debug\client.exe`.
