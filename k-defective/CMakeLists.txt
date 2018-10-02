# cmake 最低版本要求
cmake_minimum_required (VERSION 2.6)

# 项目名称
project (KDefective)

# 调试开关
option (NDEBUG "disable the debug" OFF)
if (NDEBUG)
	set (CMAKE_CXX_FLAGS "-O2 ${CMAKE_CXX_FLAGS}")
else (NDEBUG)
	set (CMAKE_CXX_FLAGS "-ggdb ${CMAKE_CXX_FLAGS}")
endif (NDEBUG)

# 搜集根目录下的所有源文件
aux_source_directory (. source_list)

# 编译生成可执行文件
add_executable (KDefective ${source_list})

