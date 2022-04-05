# 依赖
* C++
    * 依赖管理：手动（git-submodule & cmake）
    * 依赖模块：一个头文件 + 一个实现文件（可选），初始化：`global-constructor`
    * 依赖导入：导入头文件路径，符号限定于命名空间
    * 依赖导出：头文件中所有宏与符号
* Go
    * 依赖管理：自动（go）
    * 依赖模块：目录下所有文件（不包括子目录），初始化：`func init()`
    * 依赖导入：导入模块/目录路径，符号限定于包名
    * 依赖导出：大写字母开头的符号
* Python
    * 依赖管理：手动（pip）
    * 依赖模块：一个源文件，初始化：`__init__.py`
    * 依赖导入：导入包名.模块名，符号限定于文件名
    * 依赖导出：源文件中所有符号

# 变量
* C++
    ```cpp
    Type var{args};                     // 构造
    auto var = initializer;             // 拷贝
    auto var = static_cast<Type>(other) // 转换
    auto [a, b] = aggregation;          // 多元
    ```
    * 变量类型：强类型
    * 生命周期：退出块作用域时销毁
    * 作用域：退出块作用域后不可见
    * 赋值拷贝：一般为值语义，指针类型为引用语义
* Go
    ```go
    val := Type{members}// 构造
    val := initializer  // 拷贝
    val := Type(other)  // 转换
    val, _ := valuelist // 多元
    ```
    * 变量类型：强类型
    * 生命周期：直到引用计数为零才时销毁
    * 作用域：退出块作用域后不可见
    * 赋值拷贝：值类型为值语义，引用类型为引用语义
* Python
    ```python
    var = Type(args)    # 构造
    var = initializer   # 拷贝
    var = Type(other)   # 转换
    a, b = iterable     # 多元
    ```
    * 变量类型：弱类型
    * 生命周期：直到引用计数为零时才销毁
    * 作用域：直到退出函数才不可见
    * 赋值拷贝：所有类型变量本质为指针，均为引用语义

# 表达式
> 优先级：成员|函数|下标 > 单目 > 乘除 > 加减 > 关系 > 逻辑与 > 逻辑或
* C++
  * `=`为表达式
  * 支持`? :`
* Go
  * `=` `++` `--`均为语句而非表达式
* Python
  * `=`为语句，`:=`为表达式
  * 支持`a > b > c` `a**e`

# 语句
* C++
    * 分支
        * if-else
        * switch-case-default-break
    * 循环
        * for
        * for-range
    * 异常
        * try-catch-throw
* Go
    * 分支
        * if-else
        * switch-case-default-fallthrough
        * switch-case:condition
        * switch-case:type
    * 循环
        * for
        * for-range
    * 延迟
        * defer
    * 并发
        * go
* Python
    * 分支
        * if-elif-else
    * 循环
        * while-else
        * for-in-else
    * 异常
        * try-except-raise-else-finally
        * with

# 函数
* C++
    * 参数修饰
    * 签名修饰
    * 默认实参
    * 重载函数
    * 泛型变参
    * 闭包
* Go
    * 指针修饰
    * 返回列表
    * 变参包
    * 闭包
* Python
    * 类型注解
    * 默认实参
    * 位置参数与键值参数
    * 位置变参包与键值变参包
    * 闭包

# 面向对象
## 封装
* C++
    * 访问控制：public, private, protected
    * 属性定义：默认实例唯一，static类唯一
    * 方法定义：默认实例相关，static类相关
    * 构造控制：构造函数
    * 析构控制：析构函数
    * 拷贝控制：拷贝与移动函数
    * 类型转换：构造函数与operator函数
* Go
    * 访问控制：首字母大写导出包外
    * 属性定义：实例唯一
    * 方法定义：实例相关
    * 构造控制：花括号构造
    * 析构控制：无
    * 拷贝控制：无
    * 类型转换：底层类型相同可转换
* Python
    * 访问控制：前缀`__`私有
    * 属性定义：默认类唯一，实例可动态增删属性
    * 方法定义：默认实例相关，实例可动态增删方法
    * 构造控制：`__init__`
    * 析构控制：无
    * 拷贝控制：无
    * 类型转换：无
## 复用
* C++
    ```cpp
    class Derived: public Base {}
    ```
* Go
    ```go
    type Derived struct {
        Base
        *Base
    }
    ```
* Python
    ```python
    class Derived(Base):
        def __init__(self):
            Base.__init__()
            self.member = []
    ```
## 动态绑定
* C++：
  * 派生类的引用或指针可转换为基类的引用或指针
  * 运行时类型：`typeid()` `dynamic_cast<>()`
  * 通过虚指针与虚表实现
* Go：
  * 方法集匹配的类型可转换为相应接口类型
  * 运行时类型：`rt[, ok] = interf.(RT)` `switch rt := interf.(type) { case RT: ... }`
  * 通过静态符号表与反射实现
* Python
  * 弱类型可直接转换为接口类型
  * 运行时类型：`type()`
  * 利用动态符号表与反射实现

# 命名
可用于命名标识符的一些通用前后缀：
* 位置：`idx`，`pos`，`prev`，`next`，`lhs`，`rhs`，`head`，`tail`，`mid`，`begin`，`end`
* 计数：`count`，`size`，`length`，`width`，`height`，`depth`
* 时间：`new`，`old`，`orig`，`cur`，`before`，`after`
* 循环：`idx`，`pos`
* 序数：`number`，`1st`，`2nd`，`3rd`
* 布尔：`is`，`not`，`any`，`all`，`none`，`has`
* 介词：`in`，`on`，`at`，`of`，`2`，`4`
* 用途：`ret`，`val`，`need`，`temp`，`deal`，`src`，`dest`

| 类型           | 规范            | 举例                          |
| -------------- | --------------- | ----------------------------- |
| 命名空间、包名 | 全部小写        | `examplename`                 |
| 宏、常量       | 全部大写+下划线 | `EXAMPLE_NAME`                |
| 类型           | 大驼峰拼写法    | `ExampleName`                 |
| 全局函数/变量  | 大驼峰拼写法    | `::ExampleName`               |
| 静态变量       | 大驼峰拼写法    | `ExampleName`                 |
| 动态变量       | 小驼峰拼写法    | `exampleName`                 |
| 类的成员变量   | 带`_`后缀       | `exampleName_`/`ExampleName_` |
| 类的成员函数   | 不带`_`后缀     | `exampleName`/`ExampleName`   |


# 总结
* C++
    * 接近底层，性能强悍
    * 实用方便且一致性高的STL库
    * 强大的泛型系统
* Go
    * 方便的依赖管理与构建系统
    * 简洁明了的语法
    * 方便的垃圾回收
    * 高效的并发模型
    * 两种的错误处理方式
    * 灵活的接口组合设计
    * 泛型缺失
    * 内置类型与函数一致性较差
    * 并未完全封装内存管理
* Python
    * 高效的开发效率
    * 内置类型与函数一致性较差
    * 完全封装了内存管理
