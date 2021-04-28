<!-- entry begin: cpp  预处理 -->
# 预处理
* 头文件保护
* 宏开关
<!-- entry end -->

<!-- entry begin: cpp  泛型 -->
# 泛型编程
* 抽离
    * 模板类型无关
    * 算数类型
    * 指针类型
<!-- entry end -->

<!-- entry begin: cpp  函数 -->
# 函数设计
```
GFM
    ? W
        ? tempT&&
        : const tempT&
    : !CWD
        ? T
        : W
            ? T& -> T&&
            : cT& -> T&  -> T&&
// 注：如果函数内部必定会copy一次形参，则可直接使用传值参数(T)
```
* 修饰
    * this
    * const
    * noexcept
    * inline
    * constexpr
    * auto return
    * =delete

* 使用`temp T&&`时注意使用`std::decay_t<T>`与`std::forward<T>(t)`
* 使用`T&&`与`T`时注意使用`std::move(t)`
* 异步/延迟调用的函数注意引用参数的**生命周期**!
<!-- entry end -->

<!-- entry begin: cpp  类设计 -->
# 类的设计
* 取消友元
* 数据成员
    * handle
    * private
    * 顺序  & 对齐
    * const & 引用
    * static ? reference-return : inline
* 构造
    * default?
    * explicit?
    * non-inline
    * never-call-virtual
* 析构
    * virutal  & definition
    * noexcept & .destroy()
    * non-inline
    * never-call-virtual
* copy? & move?
* operator
    * 单成
    * 算赋
    * 前后
    * this
    * 构造转换 > operator转换
    * explicit bool 1
<!-- entry end -->

<!-- entry begin: cpp 类间关系 -->
# 类间关系
* **is-a**：public继承
    * 抽象分化
    * 混合类
* **has-a**：复合
* **impl-of**：复合或private继承
    * virtual
    * protect
    * EBO

* pure virtual      ：无默认定义
* non-pure virtual  ：提供默认定义
* non-virtual       ：提供强制定义
<!-- entry end -->

<!-- entry begin: cpp  初始化 -->
# 初始化
* 引用：
    * 可读性别名
    * 传引用参数
* 指针
    * 需要NULL
    * 更改指向
    * 注意，任何时候使用裸指针意味着持有者（接收者）无需在意其生命周期
* 形式
    > 内置类型统一使用`T t{}`
    * `auto  t = initializer`
    * `auto& t = initializer`
    * `T t{args}`
    * `T t(args)`
* 延后立初
* 循环声明
<!-- entry end -->

<!-- entry begin: cpp  并发 -->
# 并发
* atomic
* volatile
* mutable
<!-- entry end -->

<!-- entry begin: cpp  STL -->
# STL
* 优先使用可读性更好的重载`operator`
* Stream在循环中使用一定不要忘记调用`clear()`
* Stream仅持有StreamBuf的指针而非完整对象，不保证其生命周期
* Container使用`emplace`函数代替`push|insert`函数
* Container引用元素时，若下标为非常量，则应使用`at()`代替`operator[]()`
* Container增删元素时，注意range-based-for、引用、指针、迭代器的有效性
<!-- entry end -->

