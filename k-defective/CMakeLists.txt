# cmake 最低版本要求
cmake_minimum_required (VERSION 2.6)

# 项目名称
project (KDefective)

# 指定编译器
if (APPLE)
    SET (CMAKE_CXX_COMPILER "clang++")
elseif (UNIX)
    SET (CMAKE_CXX_COMPILER "g++")
endif (APPLE)

# 调试开关
option (NDEBUG "disable the debug" OFF)
if (NDEBUG)
	set (CMAKE_CXX_FLAGS "-O2 -DNDEBUG ${CMAKE_CXX_FLAGS}")
else (NDEBUG)
	set (CMAKE_CXX_FLAGS "-ggdb ${CMAKE_CXX_FLAGS}")
endif (NDEBUG)

set (CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

# 整数规划开关
option (IP_SOLVER "enable the IP Solvers" OFF)

if (IP_SOLVER)
    # enable IP Solvers in source code
    set (CMAKE_CXX_FLAGS "-DINTEGER_PROGRAMMING_SOLVER ${CMAKE_CXX_FLAGS}")
# 配置AMPL库
#SET (CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#include_directories ("${CMAKE_SOURCE_DIR}/amplapi/include/")
#link_directories ("${CMAKE_SOURCE_DIR}/amplapi/lib")
#set (CMAKE_INSTALL_RPATH "${CMAKE_SOURCE_DIR}/amplapi/lib")

# 配置glpk库
    include_directories ("${CMAKE_SOURCE_DIR}/glpk/include")
    link_directories ("${CMAKE_SOURCE_DIR}/glpk/lib")

# 配置Gurobi环境
# if (APPLE)
#     set (GUROBI_INCLUDE_DIR "/Library/gurobi811/mac64/include")
#     set (GUROBI_LIB_DIR "/Library/gurobi811/mac64/lib")
# elseif (UNIX)
#     set (GUROBI_INCLUDE_DIR "/home/yi.zhou/Gurobi_Env/gurobi811/linux64/include")
#     set (GUROBI_LIB_DIR "/home/yi.zhou/Gurobi_Env/gurobi811/linux64/lib")
# endif (APPLE)

# include_directories ("${GUROBI_INCLUDE_DIR}")
# link_directories ("${GUROBI_LIB_DIR}")

    set (CMAKE_CXX_FLAGS "-DIL_STD ${CMAKE_CXX_FLAGS}")
# setting cplex environmenet on cluster
    if (UNIX)
        include_directories ("/opt/ibm/ILOG/CPLEX_Studio129/cplex/include")
        link_directories ("/opt/ibm/ILOG/CPLEX_Studio129/cplex/lib/x86-64_linux/static_pic")
        include_directories ("/opt/ibm/ILOG/CPLEX_Studio129/concert/include")
        link_directories ("/opt/ibm/ILOG/CPLEX_Studio129/concert/lib/x86-64_linux/static_pic")
    endif (UNIX)
endif (IP_SOLVER)

# 搜集根目录下的所有源文件
aux_source_directory (. source_list)

# 编译生成可执行文件
add_executable (KDefective ${source_list})

if (IP_SOLVER)
    if (APPLE)
        target_link_libraries (KDefective glpk)
    elseif (UNIX)
        target_link_libraries (KDefective glpk ilocplex concert cplex m pthread dl)
    endif (APPLE)
endif (IP_SOLVER)

# 安装
set (CMAKE_INSTALL_PREFIX ".")
install(TARGETS KDefective
        RUNTIME DESTINATION Debug)

