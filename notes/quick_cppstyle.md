<!-- entry begin: cpp  一般 -->
# 一般性原则
* 头
* 预
* 无
<!-- entry end -->

<!-- entry begin: cpp  泛型 -->
# 泛型编程
* T无关
* 算数类型
* 指针类型
<!-- entry end -->

<!-- entry begin: cpp  类设计 -->
# 类的设计
* 声定顺序
* 取消友元
* 类型在前
* 数据
    * 顺序对齐
    * const &
    * private
    * handle
* 构造
    * default?
    * explicit
    * no virtual
* 析构
    * noexcept .destroy()
    * (pure) virutal definition
* copy/move
    * copy?move?
    * .clone()
* operator
    * 单成
    * 算复
    * 前后
    * this
    * 双类同向
    * bool 1
    * explicit
<!-- entry end -->

<!-- entry begin: cpp  类关系 -->
# 类间关系
* **is-a**：public继承
    * 抽象分化
    * `static<>`
* **has-a**：复合
* **is-impl**：复合或private继承
    * protect
    * virtual
    * EBO

* no-leaf pure-virtual

* pure virtual      ：无默认定义
* non-pure virtual  ：提供默认定义
* non-virtual       ：提供强制定义
<!-- entry end -->

<!-- entry begin: cpp  函数 -->
# 函数设计
* GFM ? tempT&& :
    * !CWD ? T :
        * W ? T& -> T&& :
            * cT& -> T&  -> T&&

* 修饰
    * extern
    * auto
    * this
    * noexcept
    * inline
    * =delete

* 使用类/别名简化函数类型接口
<!-- entry end -->

<!-- entry begin: cpp  初始化 -->
# 初始化
* 指针or引用
    * NULL
    * 更改指向
* 形式
    * auto& =
    * T     {}
    * T     ()
* 延后立初
* 循环声明
<!-- entry end -->

<!-- entry begin: cpp  STL -->
# STL
* .operator()
* .emplace()
* .at()
* 容器增删
    * range_based_for
    * 引用、指针、迭代器
* stream.clear()
<!-- entry end -->

<!-- entry begin: cpp  异常 -->
# 异常
* 标
* 限
* 移
* cas, up, min
<!-- entry end -->

<!-- entry begin: cpp  并发 -->
# 并发
* automic
* volatile
* mutable
<!-- entry end -->

<!-- entry begin: cpp  循环 -->
# 循环
* 多，变，顺，for-while
* 变，迭，if-continue
* 条，相，穷
* 变，操，初，once
* 尾
* 终
<!-- entry end -->

<!-- entry begin: cpp  分支 条件 -->
# 分支
* if-if
* if-else if-continue if-return if-exit
* if-elif-else  `<=>` if-else{if-else}
* for{}         `<=>` for{if-break}
* for{if{for}}  ` =>` for{if-else}
* if{for}       ` =>` for
<!-- entry end -->

<!-- entry begin: cpp  递归 -->
# 递归
* 明确功能：所有副作用，参数，返回值
* 基准情况：结束递归，并符合上述功能
* 一般进展：利用递归获取下层结果，与本层计算结合得到本层结果
* 建议：
    * 将尾递归转为循环
    * 用`stack`存储递归路径
<!-- entry end -->
