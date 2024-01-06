cmake_minimum_required(VERSION 3.28)

#---------------------------------------------------------------------------------------
# Compiler config
#---------------------------------------------------------------------------------------
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Modules
    VERSION 0.0.1
    LANGUAGES CXX)

set(CMAKE_CONFIGURATION_TYPES "Release;Debug" CACHE STRING "" FORCE)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_cmake")

include(GNUInstallDirs)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    message(FATAL_ERROR "CMAKE_CXX_SCAN_FOR_MODULES not supported yet")
else()
    set(CMAKE_CXX_SCAN_FOR_MODULES 1)
endif()

if(WIN32)
    add_compile_options(/experimental:module) # needed for STL modules
endif()

if(PROJECT_IS_TOP_LEVEL)
    option(TEST_INSTALLED_VERSION "Test exported cmake config set" NO)
    enable_testing()
endif()

if(NOT TEST_INSTALLED_VERSION)

#
# Single file module
#
add_executable(SingleFileModule)
target_sources(SingleFileModule
    PRIVATE
        SingleFileModule/main.cpp
    PRIVATE
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        SingleFileModule/mod.ixx
)

set_property(TARGET SingleFileModule PROPERTY CXX_STANDARD 20)
add_test(NAME SingleFileModule COMMAND SingleFileModule)

#
# Two files module
#
add_executable(TwoFileModule)
target_sources(TwoFileModule
    PRIVATE
        TwoFileModule/main.cpp
        TwoFileModule/mod.cpp
    PRIVATE
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        TwoFileModule/mod.ixx
)

set_property(TARGET TwoFileModule PROPERTY CXX_STANDARD 20)
add_test(NAME TwoFileModule COMMAND TwoFileModule)

#
# Partition module
#

add_executable(PartitionModule)
target_sources(PartitionModule
    PRIVATE
        PartitionModule/main.cpp
        PartitionModule/mod.func.cpp
        PartitionModule/mod.class.cpp
    PRIVATE
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        PartitionModule/mod.ixx
        PartitionModule/mod.class.ixx
        PartitionModule/mod.func.ixx
)

set_property(TARGET PartitionModule PROPERTY CXX_STANDARD 20)
add_test(NAME PartitionModule COMMAND PartitionModule)

#
# Conflicts - Two modules including same headers (iostream)
#

add_executable(Conflicts)
target_sources(Conflicts
    PRIVATE
        Conflicts/main.cpp
    PRIVATE
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        Conflicts/mod.ixx
        Conflicts/mod2.ixx
)

set_property(TARGET Conflicts PROPERTY CXX_STANDARD 20)
add_test(NAME Conflicts COMMAND Conflicts)

#
# SubModules
#

add_executable(SubModules)
target_sources(SubModules
    PRIVATE
        SubModules/main.cpp
        SubModules/mod/sub1/mod.sub1.class.cpp # Partitions od mod.sub1 module/submodule
        SubModules/mod/sub1/mod.sub1.func.cpp   #
    PRIVATE
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
        SubModules/mod/mod.ixx # PMI for module mod
        SubModules/mod/sub1/mod.sub1.ixx # PMI for submodule mod.sub1
        SubModules/mod/sub1/mod.sub1.class.ixx
        SubModules/mod/sub1/mod.sub1.func.ixx
        SubModules/mod/sub2/mod.sub2.ixx # PMI for submodule mod.sub2
)

set_property(TARGET SubModules PROPERTY CXX_STANDARD 20)
#set_property(TARGET SubModules PROPERTY FOLDER "Submodule")
add_test(NAME SubModules COMMAND SubModules)

#
# Module as dll
#

add_library(DllModule SHARED)
target_sources(DllModule
    PRIVATE
        DllModule/mod.class.cpp
        DllModule/mod.func.cpp
    PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES
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
    PUBLIC
    FILE_SET cxx_modules_dll TYPE CXX_MODULES FILES
        ${ixx_files}
)
set_property(TARGET ExternalDllModule PROPERTY CXX_STANDARD 20)
target_compile_definitions(ExternalDllModule
        PRIVATE
            "DLLMODULE_EXPORT=__declspec(dllexport)"
        INTERFACE
            "DLLMODULE_EXPORT=__declspec(dllimport)"
    )
set_property(TARGET ExternalDllModule PROPERTY FOLDER "Dll")

#
# install the ExternalDllModule
#

install(TARGETS ExternalDllModule EXPORT ExternalDllModule_export
    FILE_SET cxx_modules_dll DESTINATION lib/cxx/miu
)
#XXX install(FILES ${ixx_files} TYPE INCLUDE) # there is no standard destination now so we pick include
install(EXPORT ExternalDllModule_export NAMESPACE ExternalDllModule::
    DESTINATION lib/cmake/ExternalDllModule FILE ExternalDllModuleConfig.cmake
)

endif() # NOT TEST_INSTALLED_VERSION

#
# 3rd party dll module consumption
#

if(NOT TARGET ExternalDllModule)
    find_package(ExternalDllModule REQUIRED)

    add_executable(ExternalDllModuleApp)
    target_sources(ExternalDllModuleApp
        PRIVATE
            DllModule/main.cpp
    )

    target_link_libraries(ExternalDllModuleApp PRIVATE ExternalDllModule::ExternalDllModule)
    set_property(TARGET ExternalDllModuleApp PROPERTY CXX_STANDARD 20)
    set_property(TARGET ExternalDllModuleApp PROPERTY FOLDER "Dll")
    add_test(NAME ExternalDllModuleApp COMMAND ExternalDllModuleApp)
endif()

SET_PROPERTY(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SingleFileModule)
