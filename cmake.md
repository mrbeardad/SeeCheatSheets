# CMake

- [CMake](#cmake)
  - [命令行参数](#命令行参数)
  - [CMakeLists.txt 基础](#cmakeliststxt-基础)
    - [项目目标](#项目目标)
    - [编译参数](#编译参数)
    - [脚本命令](#脚本命令)
  - [内建变量](#内建变量)

## 命令行参数

<!-- entry begin: cmake cli -->

```sh
# 配置
cmake [-D CMAKE_VARIABLE=VALUE] [-S/path/to/source_dir -B/path/to/build_dir | /path/to/source_dir]

# 构建
--build build_dir [options]
    -j n                # 指定线程数
    -t target           # 构建指定target
    --clean-first       # 构建前先清理缓存

# 安装
--install build_dir [options]
    --prefix=path       # 覆盖CMAKE_INSTALL_PREFIX
    --component=compon  # 仅安装目标组件
```

<!-- entry end -->

## CMakeLists.txt 基础

### 项目目标

```cmake
# CMake最低版本要求
cmake_minimum_required(VERSION 3.16)

# 项目名称
project(project_name
    VERSION <major>[.<minor>[.<patch>[.<tweak>]]]
    DESCRIPTION <project-description-string>)

# 可执行文件
add_executable(target [WIN32] [MACOSX_BUNDLE]
    files...)

# HeaderOnly库
add_library(target INTERFACE) # 该target添加头文件时也只能指定INTERFACE

# 静态库
add_library(target STATIC
    files...)

# 动态库
add_library(target SHARED
    files...)

# 插件模块：调用dlopen-like运行时动态加载
add_library(target MODULE
    files...)

# 自定义命令
add_custom_target(target
    [ALL]                   # 表示该target应该加入default target
    cmd1 args... [COMMAND cmd2 [args...]]...
    [WORKING_DIRECTORY dir] # 默认构建目录
    [DEPENDS [file...]]     # 3.16后若依赖文件为target副产物则将该target纳入依赖启动
    [COMMENT "doc-string"]
    VERBATIM)               # 保证命令字符正确转义，一般必选

# 安装
install(TARGETS targets...
    RUNTIME         # 可执行程序，默认DESTINATION bin
    ARCHIVE         # 静态库，默认DESTINATION lib
    LIBRARY         # 动态库，默认DESTINATION lib
    PUBLIC_HEADER   # 公共头文件，默认DESTINATION include
    PRIVATE_HEADER  # 私有头文件，默认DESTINATION include
    DESTINATION path    # 指定安装目录的绝对路径或相对CMAKE_INSTALL_PREFIX路径
    COMPONENT comp)     # 指定所属安装组件，如runtime、development等
```

### 编译参数

```cmake
# PRIVATE   :表示只用于target
# PUBLIC    :表示用于target和链接到它的其他target
# INTERFACE :表示只用于链接到该target的其他target

# 标准版本
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
target_compile_features(target
    PRIVATE cxx_std_11 ...)

# 编译参数
add_compile_options(options...)
target_compile_options(target
    PRIVATE options...)

# 宏定义
add_compile_definitions(MACRO_NAME=VAL)
target_compile_definitions(target
    PRIVATE MACRO_NAME=VAL)

# 头文件
include_directories(dirs...)
target_include_directories(target
    PUBLIC dirs...      # 要导出作为接口的头文件设为PUBLIC
    PRIVATE dirs...)    # 不导出的头文件设为PRIVATE

# 链接库：指定目标名、库名、路径名
target_link_libraries(target
    PIBLIC item...      # 若接口文件导入了该库的接口文件，则PUBLIC继承
    PRIVATE item...)    # 若接口文件未导入该库的接口文件，则PRIVATE继承
```

### 脚本命令

<!-- entry begin: cmake variable script -->

- 引用普通变量`${VAR}`，向上级作用域或目录搜索，最终搜索缓存变量
- 引用缓存变量`$CACHE{VAR}`
- 引用环境变量`$ENV{VAR}`

```cmake
# 执行子目录脚本
add_subdirectory(source_dir)

# 普通变量：作用于当前函数或目录（包括子目录），若未定义则搜索同名缓存变量
set(<variable> <value>...
    [PARENT_SCOPE]) # 该选项表示为上级目录或上级函数作用域设置变量，而并非为当前作用域设置，离开作用域后生效

# 缓存变量：持久性存储于Cmake缓存中，目的为用户提供配置选项
# 3.21之前设置缓存变量成功后会删除同名的普通变量
set(<variable> <value>... CACHE
    <type>          # BOOL、STRING、FILEPATH、PATH、STRING、INTERNAL(仅用于持久化存储变量，隐式FORCE)
    <help_text>
    [FORCE])        # 表示强制覆盖已存在的缓存变量

# 设置BOOL缓存变量，value默认为OFF，若已存在同名普通变量或缓存变量则无效
option(<variable> "<help_text>" [OFF|ON])

# 环境变量
set(ENV{<variable>} [<value>])

# 列表操作：https://cmake.org/cmake/help/latest/command/list.html
list()
    # 任何需要变量列表的命令都可以正确解析列表变量
    # 当列表变量通过configure_file()命令进行替换时，会转换为分号分割的变量列表

# 条件判断：https://cmake.org/cmake/help/latest/command/if.html
if()
elseif()
else()
endif()

# 获取文件列表：注意每次添加文件都需要重新构建CMake缓存，添加CONFIGURE_DEPENDS表示每次build前重新运行一次GLOB
file(GLOB|GLOB_RECURSE <variable> CONFIGURE_DEPENDS [<globbing-expressions>...])

# 打印消息
message([<mode>] "message text" ...)
    # <mode>:
    #  FATAL_ERROR       # 停止进程与构建
    #  SEND_ERROR        # 继续进程但不构建
    #  WARNING           # 继续进程

# 将CMAKE变量传递给源码
configure_file(foo.h.in foo.hpp @ONLY)
    # `#cmakedefine VAR ...` 替换为 `#define VAR ...` 或 `/* #undef VAR */`（取决于cmake是否定义变量VAR）
    # `#cmakedefine01 VAR` 替换为 `#define VAR 1` 或 `#define VAR 0`（取决于cmake是否定义变量VAR）
    # `@CMAKE_VAR@` 替换为 cmake变量 `${CMAKE_VAR}` 的值
    # 注意添加 include_directories(${CMAKE_CURRENT_BINARY_DIR})
```

<!-- entry end -->

## 内建变量

<!-- entry begin: cmake project buildtype builtin-variable -->

| 变量名                     | 含义                         |
| -------------------------- | ---------------------------- |
| CMAKE_PROJECT_NAME         | 顶级项目名称                 |
| PROJECT_NAME               | 当前项目名称                 |
| CMAKE_PROJECT_VERSION      | 顶级项目版本号               |
| PROJECT_VERSION            | 当前项目版本号               |
| CMAKE_SOURCE_DIR           | 顶级目录源码树               |
| CMAKE_CURRENT_SOURCE_DIR   | 当前目录源码树               |
| project_CURRENT_SOURCE_DIR | project 项目源码树           |
| CMAKE_BINARY_DIR           | 顶级目录构建目录             |
| CMAKE_CURRENT_BINARY_DIR   | 当前目录构建目录             |
| project_CURRENT_BINARY_DIR | project 项目构建目录         |
| CMAKE_BUILD_TYPE           | 构建类型                     |
| BUILD_SHARED_LIBS          | 默认构建为动态库或静态库     |
| CMAKE_MODULE_PATH          | 默认/usr/share/cmake/Modules |
| CMAKE_INSTALL_PREFIX       | 安装路径前缀                 |

| 构建类型                | 编译参数          |
| ----------------------- | ----------------- |
| Release                 | `-O3 -DNDEBUG`    |
| MinSizeRel              | `-Os -DNDEBUG`    |
| RelWithDebInfo(default) | `-O2 -g -DNDEBUG` |
| Debug                   | `-g`              |

<!-- entry end -->
