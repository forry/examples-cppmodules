# examples-cppmodules
C++20 modules examples.

## How to
You need at least CMake 3.15. It was developed under MSVC 2022 but it should be possible to run it unde MSVC 2019 too. You need to have installed the experimental STL modules (or change the source codes where they imports).

Most examples are self explanatory and ready to use after the generation of the build system. For the External Dll example you need to run the install first (**don't forget to change the install prefix on windows if you don't like Program Files as a destination**). Then run the configure/generate again. The cmake should find the installed package on it's own and then ExternalDllModuleApp target should show under dll source group.

## Basics

### Single File Module
Simplest example of the module done as a single file exporting a single function.

### Two File Module
Same module as in the Single File Module example but divided into a primary module interface and implementation file.

### Partition Module
Simple partition example. Module consists of two partitions :SomeClassPart and :func. Both having the implementation and interface separated. Then they are exported in the primary module interface (mod.ixx as usual).

### Conflicts
Just to show that two modules including the same headers can coexists together.

## DLL Modules
These are under Dll source group.

### DLL Module
Extends the Partition Module example by compiling the module into a shared library (DllModule target). The DllModuleApp target then links it from the same source tree.

### External DLL Module
Same as DLL Module but now the ExternalDllModule needs to be built and installed first. Then the CMake should be used to find it as an external package. When it is found it will configure ExternalDllModuleApp that will link against it and use it as a 3rd party library. This part is done semi manually since there is not yet support for modules from CMake (3.24.1), although file sets are in the master branch of CMake if someone wants to give it a try. 
