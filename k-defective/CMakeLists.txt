# cmake 最低版本要求
cmake_minimum_required (VERSION 2.6)

# 项目名称
project (KDefective)

# 指定编译器
if (APPLE)
    SET (CMAKE_CXX_COMPILER "clang++")
elseif (LINUX)
    SET (CMAKE_CXX_COMPILER "g++")
endif (APPLE)

# 调试开关
option (NDEBUG "disable the debug" OFF)
if (NDEBUG)
	set (CMAKE_CXX_FLAGS "-O2 ${CMAKE_CXX_FLAGS}")
else (NDEBUG)
	set (CMAKE_CXX_FLAGS "-ggdb ${CMAKE_CXX_FLAGS}")
endif (NDEBUG)

set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

# build a static package to fit the cluster
if (UNIX)
    set (CMAKE_CXX_FLAGS "-static ${CMAKE_CXX_FLAGS}")
endif (UNIX)

# 配置AMPL库
#SET (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#include_directories ("${CMAKE_SOURCE_DIR}/amplapi/include/")
#link_directories ("${CMAKE_SOURCE_DIR}/amplapi/lib")
#set (CMAKE_INSTALL_RPATH "${CMAKE_SOURCE_DIR}/amplapi/lib")

# 配置glpk库
include_directories ("${CMAKE_SOURCE_DIR}/glpk/include")
link_directories ("${CMAKE_SOURCE_DIR}/glpk/lib")

# 配置Gurobi环境
if (APPLE)
    set (GUROBI_INCLUDE_DIR "/Library/gurobi811/mac64/include")
    set (GUROBI_LIB_DIR "/Library/gurobi811/mac64/lib")
elseif (LINUX)
    set (GUROBI_INCLUDE_DIR "/home/yi.zhou/Gurobi_Env/gurobi811/linux64/include")
    set (GUROBI_LIB_DIR "/home/yi.zhou/Gurobi_Env/gurobi811/linux64/lib")
endif (APPLE)

include_directories ("${GUROBI_INCLUDE_DIR}")
link_directories ("${GUROBI_LIB_DIR}")

# 搜集根目录下的所有源文件
aux_source_directory (. source_list)

# 编译生成可执行文件
add_executable (KDefective ${source_list})
#target_link_libraries (KDefective ampl)
target_link_libraries (KDefective glpk gurobi_c++ gurobi81)

# 安装
set (CMAKE_INSTALL_PREFIX ".")
install(TARGETS KDefective
        RUNTIME DESTINATION Debug)

