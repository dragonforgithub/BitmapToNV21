#
# Copyright (C)  The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License);
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.6.0)

# 给工程定个名称
project(arcsoft_bitmap_to_nv21)
enable_language(C ASM CXX)

#输出编译信息
set(CMAKE_VERBOSE_MAKEFILE ON)


# 设置编译器和相关FLAGS
set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -std=gnu++11")           # c++11
set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -g")                     # 调试信息
set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -Wall")                  # 开启所有警告
set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -v")                     # 查看调用关系(invocation)

set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -O0" )               # 调试包不优化
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG " )   # release包优化

# 定义宏
add_definitions(-D_DEBUG)
add_definitions(-D_ANDROID_)
message(WARNING "++++++TEST++++++++")

# 添加依赖库及头文件
include_directories(${CMAKE_CURRENT_LIST_DIR}/prebuilts/inc)
include_directories(${CMAKE_CURRENT_LIST_DIR}/prebuilts/inc/common)
include_directories(${CMAKE_CURRENT_LIST_DIR}/prebuilts/inc/ImageBase)
include_directories(${CMAKE_CURRENT_LIST_DIR}/ImageUtils)
include_directories(${CMAKE_CURRENT_LIST_DIR}/utils)

set(LIB_BASE_DIR ${CMAKE_CURRENT_LIST_DIR}/prebuilts/lib/${ANDROID_ABI})
set(LIB_IMAGE_DIR ${CMAKE_CURRENT_LIST_DIR}/prebuilts/lib/${ANDROID_ABI}/ImageBase)


    # 虹软平台库
    set(ARCSOFT_BASE mpkernel mputility mpbase mpstream)

    foreach(lib ${ARCSOFT_BASE})
        add_library(${lib} STATIC IMPORTED)
        set_property(TARGET ${lib} PROPERTY IMPORTED_LOCATION ${LIB_BASE_DIR}/lib${lib}.a)
    endforeach()

    # 虹软解码库
    set(ARCSOFT_IMAGE_CODEC mtutils amimgcodec adkutils amcm)
    list(APPEND ARCSOFT_IMAGE_CODEC ampostprocess)
    list(APPEND ARCSOFT_IMAGE_CODEC mtjpgex  mtpng)
    list(APPEND ARCSOFT_IMAGE_CODEC mtzlib mtexif)

    foreach(lib ${ARCSOFT_IMAGE_CODEC})
        add_library(${lib} STATIC IMPORTED)
        set_property(TARGET ${lib} PROPERTY IMPORTED_LOCATION ${LIB_IMAGE_DIR}/lib${lib}.a)
    endforeach()

# 添加源文件的头文件搜索路径
    # < 递归目录树 >
    macro(RecurseDirectories result currDir)
      file(GLOB_RECURSE children  LIST_DIRECTORIES true RELATIVE ${currDir} ${currDir}/*)
      set(dirlist ${currDir})
      foreach(child ${children})
        if(IS_DIRECTORY ${currDir}/${child})
          list(APPEND dirlist ${currDir}/${child})
        endif()
      endforeach()
      set(${result} ${dirlist})
    endmacro()

    RecurseDirectories(SUB_DIRS ${CMAKE_CURRENT_LIST_DIR}/src)

    # 添加所有的子目录(头文件目录)
    include_directories(${SUB_DIRS})

# 查找当前目录下的所有源文件
set(CURR_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
file(GLOB_RECURSE SOURCE_FILES ${CURR_DIR} ${CURR_DIR}/*.cpp ${CURR_DIR}/*.c)


# 排除某个文件
#list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/glm/detail/glm.cpp)
#list(REMOVE_ITEM SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/glm/detail/dummy.cpp)



#定义生成.so的动态库
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})


message(WARNING "===[SOURCE_FILES:]===" ${SOURCE_FILES})

# 定义链接时的依赖
target_link_libraries(${PROJECT_NAME} android log jnigraphics
    ${ARCSOFT_IMAGE_CODEC} ${ARCSOFT_BASE}
)
