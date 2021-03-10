# cmake命令
<!-- entry begin: cmake -->
```sh
# cmake命令
cmake "[options]" "<path-to-source>"
cmake "[options]" "<path-to-existing-build>"
cmake "[options]" -S "<path-to-source>" -B "<path-to-build>"

# 一般步骤
cd build_dir
cmake source_dir
cmake --build .
cmake --install .

# 选项
-D <var>=<val>
-G <generator-name>

--build   <build_dir> [<options>] [-- <build-tool-options>]
    -j <jobs>            # 指定线程数
    -t <target>          # 执行指定makefile目标
    --clean-first        # build前先执行clean

--install <build_dir> [<options>]
    --prefix <path>      # 覆盖CMAKE_INSTALL_PREFIX
    --component <compon> # 仅安装目标组件
```
<!-- entry end -->

# CMakeLists.txt基础命令
```cmake
# CMake版本要求
cmake_minimum_required(VERSION 3.10)

# 工程项目名称
project(project_name
    VERSION <major>[.<minor>[.<patch>[.<tweak>]]]
    DESCRIPTION "project description string"
    HOMEPAGE_URL "URL string")

# 注意：target的文件名不能加路径

# 可执行文件(target)
add_executable(exe_target
    main.cpp)

# 别名(target)
add_library(namespace::lib_target ALIAS
    lib_target)

# 静态库(target)
add_library(lib_target STATIC
    src/lib_a.cpp)

# 动态库(target)
add_library(lib_target SHARED
    src/lib_so.cpp)

# HeaderOnly库(target)
add_library(lib_target INTERFACE)
    # 该target添加头文件时也只能指定INTERFACE

# 自定义脚本运行
add_custom_target(target
    [ALL] # 表示该target应该加入default target
    cmd args...
    [COMMAND cmd args...]...)
add_dependencies(target
     [<target-dependency>]...)
```

# CMakeLists.txt编译参数
```cmake
# INTERFACE :表示只用于链接到该target的其他target
# PUBLIC    :表示用于target和链接到它的其他target
# PRIVATE   :表示只用于target

# 头文件
target_include_directories(target
    PUBLIC
    include_dir)

# 链接库
target_link_libraries(target
    PRIVATE
    lib_target)

# 标准版本
target_compile_features(target
    PRIVATE
    cxx_feature_name
    cxx_std_11)

# 编译参数
target_compile_options(target
    [BEFORE] # 表示将参数加在前面而非缺省的最后
    PRIVATE
    -lpthread -ldl -lutil -fcoroutines)
add_compile_options(-opt)

# 宏定义
target_compile_definitions(target
    PRIVATE
    MACRO=val)
add_compile_definitions(MACRO=val)

```

# CMakeLists.txt配置变量
```cmake
# 相当于在此处插入并执行子目录下的CMakeLists.txt脚本
add_subdirectory(dir_name)

# 构建期配置文件，关联cmake宏与cpp宏
configure_file(include/ver.h.in  ${PROJECT_BINARY_DIR}/ver.hpp)
    # 将文件中的 `#cmakedefine MACRO` 替换为 `#define MACRO @MACRO@`
    # 将文件中的 `@CMAKE_VAR@` 替换为cmake项目变量 `CMAKE_VAR` 的值
    # 注意使用 target_include_directories(target ${PROJECT_BINARY_DIR})

# 设置可开关的选项
option(OPTION "description" [OFF|ON])
    # 与用普通cmake变量来控制cpp宏的区别在于：
    # 1. 在GUI程序中显示该选项
    # 2. 定义为CACHE变量

# 在当前函数作用域或目录作用域中设置变量
set(<variable> <value>...
    [PARENT_SCOPE])     # 该选项表示为上级目录或上级函数作用域设置变量，而并非为当前作用域设置，返回后生效
    # 若有多个变量会存储为列表变量
    # 若<value>为空相当于unset(<variable>)

# 设置缓冲作用域的变量，其生命周期可跨多次cmake命令行调用
set(<variable> <value>...
    CACHE <type> <doc-string> # <type>包括：BOOL、FILEPATH、PATH、STRING、INTERNAL（GUI不对外显示的字符串，隐式FORCE）
    [FORCE])    # 表示强制覆盖已存在的缓存变量
    # 设置缓存变量成功后会删除同名的普通变量

# 设置环境变量
set($ENV{VAR} <value>)

# 读取文件内容到变量
file(READ <filename> <variable>
    [OFFSET <offset>]
    [LIMIT <max-bytes>]
    [HEX])

# 读取文件每行内容形成列表
file(STRINGS <filename> <variable> [<options>...])
    # <options>包括
    # LENGTH_MAXIMUM <max-len>
    # LENGTH_MINIMUM <min-len>
    # LIMIT_COUNT    <max-line>
    # REGEX          <regex>

# 计算哈希值
file(<HASH> <filename> <variable>)
    # 支持的哈希算法有：MD5、SHA1、SHA256、SHA512、SHA3_256、SHA3_512

# 下载文件
file(DOWNLOAD <URL> [<file>] [<options>...])
    # 不指定<file>则不下载而只判断<URL>是否存在
    # INACTIVITY_TIMEOUT <seconds>
    # TIMEOUT <seconds>
    # SHOW_PROGRESS
    # STATUS <variable>             # 两元素的列表变量，首元素为数字表示错误码，次元素为错误字符串
    # EXPECTED_HASH ALGO=<value>    # ALGO为哈希算法

# 打印字符串
message( [<mode>] <message-string>... )
    # FATAL_ERROR       # 停止进程与构建
    # SEND_ERROR        # 继续进程但不构建
    # WARNING           # 继续进程
```
**关于cmake脚本中的变量**：
* 变量类型均为字符串（必要时使用""来转义），一些命令会自己将字符串解析为其它类型
* 使用未定义的变量相当于使用空字符串
* 引用普通变量`${VAR}`
* 引用缓存变量`$CACHE{VAR}`
* 引用环境变量`$ENV{VAR}`
* 变量搜索会依照作用域向上进行搜索，最终还会搜索缓存变量


# 第三方库依赖
```cmake
# 需要在CMAKE_MODULE_PATH中存在FindBoost.cmake文件
find_package(Boost
    [version]                       # 指定最小版本，如1.75
    [EXACT]                         # 表示指定的版本是精准版本
    [QUIET]                         # 搜索失败直接退出
    [REQUIRED]                      # 搜索失败报错
    [COMPONENTS <libs>...]          # 必要组件，e.g. "date_time" for "libboost_date_time"
    [OPTIONAL_COMPONENTS <libs>...])# 可选组件，e.g. "date_time" for "libboost_date_time"

# 执行搜索后一般会设置如下变量
# Boost_FOUND            - True if headers and requested libraries were found
# Boost_INCLUDE_DIRS     - Boost include directories
# Boost_LIBRARY_DIRS     - Link directories for Boost libraries
# Boost_LIBRARIES        - Boost component libraries to be linked
# Boost_<C>_FOUND        - True if component <C> was found (<C> is upper-case)
# Boost_<C>_LIBRARY      - Libraries to link for component <C> (may include target_link_libraries debug/optimized keywords)
```

# 安装
```cmake
install(TARGETS   <target>... [...])
install(FILES     <file>...   [...])
install(DIRECTORY <dir>...    [...])
# DESTINATION       <path>      指定安装目录的绝对或相对地址
# PERMISSIONS       <perm>..    包括{OWNER|GROUP|WORLD}_{READ|WRITE|EXECUTE}, SETUID, SETGID
# CONFIGURATIONS    <conf>      在指定配置下才生效
# COMPONENT         <comp>      指定属于哪个安装组件，如runtime、development等
```

# 内建变量
| 变量名                | 含义                         |
|-----------------------|------------------------------|
| PROJECT_NAME          | 当前项目名称                 |
| CMAKE_PROJECT_NAME    | 顶级项目名称                 |
| PROJECT_VERSION       | 当前项目版本号               |
| CMAKE_PROJECT_VERSION | 顶级项目版本号               |
| name_VERSION          | 指定项目版本号               |
| PROJECT_SOURCE_DIR    | 当前项目源码树               |
| CMAKE_SOURCE_DIR      | 顶级项目源码树               |
| name_SOURCE_DIR       | 指定项目源码树               |
| PROJECT_BINARY_DIR    | 当前项目构建目录             |
| CMAKE_BINARY_DIR      | 顶级项目构建目录             |
| name_BINARY_DIR       | 指定项目构建目录             |
| CMAKE_BUILD_TYPE      | 构建类型（见下）             |
| CMAKE_CXX_COMPILER    | 编译平台                     |
| CMAKE_CXX_FLAGS       | 编译参数                     |
| CMAKE_CXX_STANDARD    | C++语言标准版本              |
| CMAKE_MODULE_PATH     | 默认/usr/share/cmake/Modules |
| CMAKE_INSTALL_PREFIX  | 安装路径前缀                 |

| 构建类型                | 编译参数          |
|-------------------------|-------------------|
| Release                 | `-O3 -DNDEBUG`    |
| MinSizeRel              | `-Os -DNDEBUG`    |
| RelWithDebInfo(default) | `-O2 -g -DNDEBUG` |
| Debug                   | `-g`              |
