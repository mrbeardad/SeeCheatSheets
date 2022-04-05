# 命令行参数
<!-- entry begin: cmake -->
```sh
# 一般步骤
cd build_dir
cmake [options] source_dir
cmake --build .
cmake --install .

# 选项
-D CMAKE_VARIABLE=value

--build build_dir [options]
    -j n                # 指定线程数
    -t target           # 构建指定target
    --clean-first       # 构建前先清理缓存

--install build_dir [options]
    --prefix=path       # 覆盖CMAKE_INSTALL_PREFIX
    --component=compon  # 仅安装目标组件
```
<!-- entry end -->

# CMakeLists.txt基础
## 添加目标
```cmake
# CMake版本要求
cmake_minimum_required(VERSION 3.10)

# 工程项目名称
project(project_name VERSION <major>[.<minor>[.<patch>[.<tweak>]]])

# 可执行文件
add_executable(target
    source...)

# 静态库
add_library(target STATIC
    source...)

# 动态库
add_library(target SHARED
    source...)

# HeaderOnly库
add_library(target INTERFACE) # 该target添加头文件时也只能指定INTERFACE

# 安装
install(TARGETS   <target>... [...])
install(FILES     <file>...   [...])
install(DIRECTORY <dir>...    [...])
# DESTINATION       <path>      指定安装目录的绝对或相对地址
# PERMISSIONS       <perm>..    包括{OWNER|GROUP|WORLD}_{READ|WRITE|EXECUTE}, SETUID, SETGID
# CONFIGURATIONS    <conf>      在指定配置下才生效
# COMPONENT         <comp>      指定属于哪个安装组件，如runtime、development等

# 自定义命令
add_custom_target(target
    [ALL] # 表示该target应该加入default target
    cmd args...
    [COMMAND command [args...]]...
    [DEPENDS [depens...]]
    [WORKING_DIRECTORY dir])
```

## 编译参数
```cmake
# INTERFACE :表示只用于链接到该target的其他target
# PUBLIC    :表示用于target和链接到它的其他target
# PRIVATE   :表示只用于target

# 头文件
target_include_directories(target
    PUBLIC include_dirs...)

# 链接库
target_link_libraries(target
    PRIVATE item...) # 目标名、库名、路径名

# 标准版本
target_compile_features(target
    PRIVATE cxx_feature_names cxx_std_11 ...)

# 编译参数
target_compile_options(target [BEFORE] # 表示将参数加在前面而非缺省的最后
    PRIVATE options...)
add_compile_options(options...)

# 宏定义
target_compile_definitions(target
    PRIVATE MACRO=val)
add_compile_definitions(MACRO=val)
```

## 脚本变量
```cmake
# 普通变量：作用于当前函数或目录
set(<variable> <value>... [PARENT_SCOPE]) # 该选项表示为上级目录或上级函数作用域设置变量，而并非为当前作用域设置，返回后生效

# 缓存变量：存储在Cmake缓存中，用于为用户提供配置。设置缓存变量成功后会删除同名的普通变量
set(<variable> <value>...
    CACHE <type>    # <type>包括：BOOL、FILEPATH、PATH、STRING、INTERNAL
    <doc-string>
    [FORCE])        # 表示强制覆盖已存在的缓存变量

option(OPTION "description" [OFF|ON])   # BOOL缓存变量

# 环境变量
set(ENV{VAR} [<value>])

# 获取文件列表
file(GLOB|GLOB_RECURSE <variable> [<globbing-expressions>...])

# 列表操作
list()

# 条件判断
if()
elseif()
else()
endif()

# 打印消息
message( [<mode>] <message-string>... )
    # FATAL_ERROR       # 停止进程与构建
    # SEND_ERROR        # 继续进程但不构建
    # WARNING           # 继续进程

# 相当于在此处插入并执行子目录下的CMakeLists.txt脚本
add_subdirectory(dir_name)

# 构建期配置文件，关联cmake宏与cpp宏
configure_file(foo.h.in foo.hpp)
    # 将文件中的 `#cmakedefine VAR ...` 替换为 `#define VAR ...`
    # 将文件中的 `@CMAKE_VAR@` 替换为cmake变量 `CMAKE_VAR` 的值
    # 注意添加 include_directories(${CMAKE_CURRENT_BINARY_DIR})
```
**关于cmake脚本中的变量**：
* 变量类型均为字符串（必要时使用""来转义），一些命令会自己将字符串解析为其它类型
* 使用未定义的变量相当于使用空字符串
* 变量搜索会依照作用域向上进行搜索，最终还会搜索缓存变量
* 引用普通变量`${VAR}`
* 引用缓存变量`$CACHE{VAR}`
* 引用环境变量`$ENV{VAR}`


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

# 内建变量
<!-- entry begin: cmake buildtype builtin-variable -->
| 变量名                   | 含义                         |
| ------------------------ | ---------------------------- |
| CMAKE_PROJECT_NAME       | 顶级项目名称                 |
| PROJECT_NAME             | 当前项目名称                 |
| CMAKE_PROJECT_VERSION    | 顶级项目版本号               |
| PROJECT_VERSION          | 当前项目版本号               |
| CMAKE_SOURCE_DIR         | 顶级项目源码树               |
| CMAKE_CURRENT_SOURCE_DIR | 当前项目源码树               |
| CMAKE_BINARY_DIR         | 顶级项目构建目录             |
| CMAKE_CURRENT_BINARY_DIR | 当前项目构建目录             |
| CMAKE_BUILD_TYPE         | 构建类型                     |
| CMAKE_MODULE_PATH        | 默认/usr/share/cmake/Modules |
| CMAKE_INSTALL_PREFIX     | 安装路径前缀                 |

| 构建类型                | 编译参数          |
| ----------------------- | ----------------- |
| Release                 | `-O3 -DNDEBUG`    |
| MinSizeRel              | `-Os -DNDEBUG`    |
| RelWithDebInfo(default) | `-O2 -g -DNDEBUG` |
| Debug                   | `-g`              |
<!-- entry end -->