# CMake基础命令
```sh
# 版本要求
cmake_minimum_required(VERSION 3.0)

# 工程名字
project(project_name)

# 设置变量
set(SOURCES src/main.cpp)
file(GLOB SOURCES "src/*.cpp")

# 打印消息
message([FATAL_ERROR] "notification")

# 别名
add_library(namespace::library ALIAS
    library_name
)

# 静态库(target)
add_library(library_name STATIC
    src/lib_a.cpp
)

# 动态库(target)
add_library(library_name SHARED
    src/lib_so.cpp
)

# HeaderOnly库(target)
add_library(library_name INTERFACE)

# 可执行文件(target)
add_executable(executable_name main.cpp)

# PRIVATE   :只用于target
# PUBLIC    :用于target和链接到它的其他target
# 一般编译库时使用target_include_directories(... PUBLIC ...)，
# 其他target的编译只需target_link_libraries()即可同时指定`-I`与`-l`

# 头文件
target_include_directories(target
    PUBLIC
        include/static
        include/shared
        include/installing
)

# 链接库
target_link_libraries(target
    PRIVATE
        library_name
)

# 编译参数
    # 指定编译器
    cmake -DCMAKE_CXX_COMPILER=g++

    # 指定构建类型
    cmake -DCMAKE_BUILD_TYPE=Release

    # 宏定义
    target_compile_definitions(target
        PRIVATE
            EX3
    )

    # 标准版本
    target_compile_features(target
        PUBLIC
            cxx_feature_name
    )

# 子项目可以使用父项目在添加该子项目之前作用域的所有数据，于是可以与其他子项目交互
add_subdirectory(dir_name)
```

# 构建类型
* Release       ：`-O3 -DNDEBUG`
* Debug         ：`-g`
* MinSizeRel    ：`-Os -DNDEBUG`
* RelWithDebInfo：`-O2 -g -DNDEBUG`

# 搜索第三方库
```sh
    # 需要在CMAKE_MODULE_PATH中存在FindBoost.cmake文件
    find_package(Boost
        [version] [EXACT]               # Minimum or EXACT version e.g. 1.67.0
        [REQUIRED]                      # Fail with error if Boost is not found
        [COMPONENTS <libs>...]          # Boost libraries by their canonical name. e.g. "date_time" for "libboost_date_time"
        [OPTIONAL_COMPONENTS <libs>...] # Optional Boost libraries by their canonical name. e.g. "date_time" for "libboost_date_time"
    )

    # 执行搜索后一般会设置如下变量
    # Boost_FOUND            - True if headers and requested libraries were found
    # Boost_INCLUDE_DIRS     - Boost include directories
    # Boost_LIBRARY_DIRS     - Link directories for Boost libraries
    # Boost_LIBRARIES        - Boost component libraries to be linked
    # Boost_<C>_FOUND        - True if component <C> was found (<C> is upper-case)
    # Boost_<C>_LIBRARY      - Libraries to link for component <C> (may include target_link_libraries debug/optimized keywords)
```

# conf期编程
```sh
# 搜索文件 path.h.in 并将其中的字符串字面值替换为cmake项目变量值后，安装到${PROJECT_BINARY_DIR}/ver.h
configure_file(ver.h.in ${PROJECT_BINARY_DIR}/ver.h)
```

# 安装
## Linux
```sh
install(TARGETS targets
    DESTINATION bin
)
install(TARGETS targets
    LIBRARY DESTINATION lib
)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ 
    DESTINATION include
)
install (FILES cmake-examples.conf
    DESTINATION etc
)
```

## Windows
```sh
install (TARGETS cmake_examples_inst
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
```

# 变量
## 内建变量
| 变量名               | 含义                                              |
|----------------------|---------------------------------------------------|
| PROJECT_NAME         | Set by the current project()                      |
| CMAKE_PROJECT_NAME   | Set by the top level project()                    |
| PROJECT_SOURCE_DIR   | The source directory of current project           |
| CMAKE_SOURCE_DIR     | The source directory of the top level project     |
| name_SOURCE_DIR      | The source directory of the project called "name" |
| PROJECT_BINARY_DIR   | The build directory of current project            |
| CMAKE_BINARY_DIR     | The build directory of the top level project      |
| name_BINARY_DIR      | The build directory of the project called "name"  |
| CMAKE_CXX_COMPILER   |                                                   |
| CMAKE_BUILD_TYPE     |                                                   |
| CMAKE_CXX_FLAGS      |                                                   |
| CMAKE_CXX_STANDARD   |                                                   |
| CMAKE_MODULE_PATH    | /usr/share/cmake/Modules                          |
| CMAKE_INSTALL_PREFIX | related to install()                              |

## normal变量与cache变量的区别
* cache变量会存储在CMake缓存文件中
* 两种类型的同名变量课同时存在，但normal的存在会掩盖cache
* 设置cache变量时需要该变量不存在在cache中
>

同名normal与cache变量的作用：  
提供默认值给父项目与子项目，而两个项目中又可以设置normal同名变量来掩盖默认值

## 设置cache变量
`set(<variable> <value> [[CACHE <type> <docstring> [FORCE]] | PARENT_SCOPE])`
* 若`<value>`为空则同于unset()
* 若指定了CACHE则需要指定`<type>`与`<docstring>`（给GUI用）
    * FILEPATH
    * PATH
    * STRING
    * BOOL
    * INTERNAL：对用户隐藏，隐式FORCE

* FORCE：强制修改cache值，即使该变量已存在于cache中
* PARENT_SCOPE：This command will set the value of a variable into the parent directory or calling function (whichever is applicable to the case at hand). 
* ENV：`set(ENV{PATH} /home/username)`
