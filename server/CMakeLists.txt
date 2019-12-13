cmake_minimum_required(VERSION 3.5)
project(hashAddressMappingService)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(SOURCE_FILES main.cpp
        source/Tools/Log.hpp
        source/Tools/Log.cpp
        source/Common/FileUtils.cpp
        source/Common/FileUtils.hpp
        source/Command/HashAddressMappingCmd.cpp
        source/Command/HashAddressMappingCmd.hpp
        source/ThirdParty/CompatibleFileSystem.hpp
        source/ThirdParty/ghc-filesystem.hpp
        source/HashAddressMappingService.h
        source/HashAddressMappingService.cpp
        source/ErrCode.cpp
        source/ErrCode.h)
set(CHATGROUP_LIB_INCLUDE lib/include)
INCLUDE_DIRECTORIES(${CHATGROUP_LIB_INCLUDE})
INCLUDE_DIRECTORIES(source/)
INCLUDE_DIRECTORIES(./lib/include/Elastos.SDK.Contact)
INCLUDE_DIRECTORIES(./lib/include/Elastos.SDK.Keypair.C)
LINK_DIRECTORIES("source/")

add_executable(hashAddressMappingService ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(hashAddressMappingService pthread sqlite3
        "${PROJECT_SOURCE_DIR}/lib/x86_64/libcrystal.so"
        "${PROJECT_SOURCE_DIR}/lib/x86_64/libElastos.SDK.Contact.so"
        "${PROJECT_SOURCE_DIR}/lib/x86_64/libPeerNode.so")
