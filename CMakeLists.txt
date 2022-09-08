cmake_minimum_required(VERSION 3.15)
cmake_policy(VERSION 3.15)

project(Modules
    VERSION 0.0.0
    LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Release;Debug" CACHE STRING "" FORCE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_cmake")

include(GnuInstallDirs)

add_compile_options(/experimental:module) # needed for STL modules

#
# Single file module
#
add_executable(SingleFileModule)
target_sources(SingleFileModule
    PRIVATE 
        SingleFileModule/main.cpp
        SingleFileModule/mod.ixx
)

set_property(TARGET SingleFileModule PROPERTY CXX_STANDARD 20)

#
# Two files module
#
add_executable(TwoFileModule)
target_sources(TwoFileModule
    PRIVATE 
        TwoFileModule/main.cpp
        TwoFileModule/mod.ixx
        TwoFileModule/mod.cpp
)

set_property(TARGET TwoFileModule PROPERTY CXX_STANDARD 20)

#
# Partition module
#

add_executable(PartitionModule)
target_sources(PartitionModule
    PRIVATE 
        PartitionModule/main.cpp
        PartitionModule/mod.ixx
        PartitionModule/mod.class.ixx PartitionModule/mod.class.cpp
        PartitionModule/mod.func.ixx PartitionModule/mod.func.cpp
)

set_property(TARGET PartitionModule PROPERTY CXX_STANDARD 20)

#
# Two modules including same headers
#

add_executable(Conflicts)
target_sources(Conflicts
    PRIVATE 
        Conflicts/main.cpp
        Conflicts/mod.ixx
        Conflicts/mod2.ixx
)

set_property(TARGET PartitionModule PROPERTY CXX_STANDARD 20)

#
# SubModules
#

add_executable(SubModules)
target_sources(SubModules
    PRIVATE 
        SubModules/main.cpp

        SubModules/mod/mod.ixx # PMI for module mod
        SubModules/mod/sub1/mod.sub1.ixx # PMI for submodule mod.sub1
        SubModules/mod/sub1/mod.sub1.class.ixx SubModules/mod/sub1/mod.sub1.class.cpp # Partitions od mod.sub1 module/submodule
        SubModules/mod/sub1/mod.sub1.func.ixx SubModules/mod/sub1/mod.sub1.func.cpp   #
        SubModules/mod/sub2/mod.sub2.ixx # PMI for submodule mod.sub2
)

set_property(TARGET SubModules PROPERTY CXX_STANDARD 20)
#set_property(TARGET SubModules PROPERTY FOLDER "Submodule")

#
# Module as dll
#

add_library(DllModule SHARED)
target_sources(DllModule
    PRIVATE 
        DllModule/mod.class.cpp
        DllModule/mod.func.cpp
    PUBLIC
        DllModule/mod.ixx
        DllModule/mod.class.ixx
        DllModule/mod.func.ixx 
)

set_property(TARGET DllModule PROPERTY CXX_STANDARD 20)
target_compile_definitions(DllModule
        PRIVATE
            "DLLMODULE_EXPORT=__declspec(dllexport)"
        INTERFACE
            "DLLMODULE_EXPORT=__declspec(dllimport)"
    )
set_property(TARGET DllModule PROPERTY FOLDER "Dll")

add_executable(DllModuleApp)
target_sources(DllModuleApp
    PRIVATE 
        DllModule/main.cpp
)

target_link_libraries(DllModuleApp PRIVATE DllModule)
set_property(TARGET DllModuleApp PROPERTY CXX_STANDARD 20)
set_property(TARGET DllModuleApp PROPERTY FOLDER "Dll")

#
# 3rd party dll module creation
#

add_library(ExternalDllModule SHARED)
#hack before proper file_set support is added
set(ixx_files
    ExternalDllModule/mod.ixx
    ExternalDllModule/mod.class.ixx
    ExternalDllModule/mod.func.ixx  
)
target_sources(ExternalDllModule
    PRIVATE 
        ExternalDllModule/mod.class.cpp
        ExternalDllModule/mod.func.cpp
        $<BUILD_INTERFACE:${ixx_files}>
    INTERFACE
        $<INSTALL_INTERFACE:include/mod.ixx>
        $<INSTALL_INTERFACE:include/mod.class.ixx>
        $<INSTALL_INTERFACE:include/mod.func.ixx>
)
set_property(TARGET ExternalDllModule PROPERTY CXX_STANDARD 20)
target_compile_definitions(ExternalDllModule
        PRIVATE
            "DLLMODULE_EXPORT=__declspec(dllexport)"
        INTERFACE
            "DLLMODULE_EXPORT=__declspec(dllimport)"
    )
install(TARGETS ExternalDllModule EXPORT ExternalDllModule_export)
install(FILES ${ixx_files} TYPE INCLUDE) # there is no standard destination now so we pick include
install(EXPORT ExternalDllModule_export NAMESPACE ExternalDllModule::  DESTINATION lib/cmake/ExternalDllModule FILE ExternalDllModuleConfig.cmake)
set_property(TARGET ExternalDllModule PROPERTY FOLDER "Dll")

#
# 3rd party dll module consumption
# You need to install the ExternalDllModule first
#

find_package(ExternalDllModule QUIET)
if(ExternalDllModule_FOUND)
    add_executable(ExternalDllModuleApp)
    target_sources(ExternalDllModuleApp
        PRIVATE 
            DllModule/main.cpp
    )

    target_link_libraries(ExternalDllModuleApp PRIVATE ExternalDllModule::ExternalDllModule)
    set_property(TARGET ExternalDllModuleApp PROPERTY CXX_STANDARD 20)
    set_property(TARGET ExternalDllModuleApp PROPERTY FOLDER "Dll")
endif()

SET_PROPERTY(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SingleFileModule)