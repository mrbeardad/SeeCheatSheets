# 依赖
* C++
    * 依赖管理：手动（git-submodule & cmake）
    * 依赖模块：一个头文件 + 一个实现文件（可选），初始化：`global-constructor`
    * 依赖导入：导入头文件路径，符号限定于命名空间
    * 依赖导出：头文件中所有宏与符号
* Go
    * 依赖管理：自动（go）
    * 依赖模块：目录下所有文件（不包括子目录），初始化：`func init()`
    * 依赖导入：导入目录路径，符号限定于包名
    * 依赖导出：大写字母开头的符号
* Python
    * 依赖管理：手动（pip）
    * 依赖模块：一个源文件，初始化：`__init__.py`
    * 依赖导入：导入包名.模块名，符号限定于文件名
    * 依赖导出：源文件中所有符号

# 变量
* C++
    * 变量类型：强类型
    * 生命周期：退出块作用域时销毁
    * 作用域：退出块作用域后不可见
    * 赋值拷贝：一般为值语义，指针类型为引用语义
* Go
    * 变量类型：强类型
    * 生命周期：直到引用计数为零才时销毁
    * 作用域：退出块作用域后不可见
    * 赋值拷贝：值类型为值语义，引用类型为引用语义
* Python
    * 变量类型：弱类型
    * 生命周期：直到引用计数为零时才销毁
    * 作用域：直到退出函数才不可见
    * 赋值拷贝：所有类型变量本质为指针，均为引用语义

# 表达式
> 优先级：右结合 > 单目 > 乘除 > 加减 > 关系 > 逻辑与 > 逻辑或
* Go
  * `=` `++` `--`均为语句而非表达式
* Python
  * 支持指数运算`**`与浮点数模运算`%`
  * 支持连续关系比较`a > b > c`且省略逻辑与符号

# 语句
* C++
    * 分支
        * `if (init; cond) else`
        * `switch () { case CONST: default: }`
    * 循环
        * `for (init; cond; iter)`
        * `for (auto elem : container)`
    * 异常
        * `try {}`         `func() try{}`
        * `catch (excep&)` `catch (...)`
        * `throw excep`    `throw`
* Go
    * 分支
        * `if init; cond {} else {}`
        * `switch { case cond: fallthrough }`
        * `switch init; cond { case var: }`
    * 循环
        * `for (init; cond; iter) {}`
        * `for (cond) {}`
        * `for {}`
        * `for idx, val := range container {}`
    * 延迟
        * defer
    * 并发
        * go
* Python
    * 分支
        * `if cond: elif cond: else:`
    * 循环
        * `while cond: else:`
        * `for elem in Iter: else:`
    * 异常
        * `try:`
        * `except Exception:` `except Exception as excep` `except:`
        * `raise excep` `raise excep from cause` `raise`
        * `else:`
        * `finally:`

# 函数
* C++
    * 参数修饰
    * 签名修饰
    * 默认实参
    * 重载函数
    * 泛型变参
    * 闭包
* Go
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
    * 访问控制
    * 属性与方法
    * 构造（copy&move）与析构
    * operator
* Go
    * 访问控制（通过大写首字母导出字段）
    * 属性与方法（无静态，方法区分值与指针）
* Python
    * 无访问控制
    * 属性与方法（可动态更改）
    * 构造
## 复用
* C++
    * 继承机制实现复用
* Go
    * 组合机制实现复用
* Python
    * 继承机制实现复用
## 动态绑定
* C++
    * 基类的引用或指针可接受派生类的引用或指针
* Go
    * 接口值可接受任意实现接口方法的类型实例对象
* Python
    * 变量类型本身就可动态更改

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
