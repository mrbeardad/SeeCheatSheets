# 目录
<!-- vim-markdown-toc GFM -->

- [C++ Style](#c-style)
  - [名字](#名字)
  - [注释](#注释)
  - [括号](#括号)
  - [顺序](#顺序)
  - [空白符](#空白符)

<!-- vim-markdown-toc -->

# C++ Style
部分风格参考自[HosseinYousefi/CompetitiveCPPManifesto](https://github.com/HosseinYousefi/CompetitiveCPPManifesto)

## 名字
| 内容                                   | 规范                                   |
|----------------------------------------|----------------------------------------|
| 类名、类型别名、模板类型参数、静态变量 | 大驼峰拼写法`ExampleName`              |
| 动态变量                               | 小驼峰拼写法`exampleName`              |
| 类的数据成员                           | 带`_`后缀`exampleName_` `ExampleName_` |
| 命名空间、函数名称                     | 全部小写和下划线间隔`example_name`     |
| 宏、常量、模板非类型参数               | 全部大写和下划线间隔`EXAMPLE_NAME`     |

可用于命名标识符的一些通用前后缀：
* 位置：`prev`，`next`，`lhs`，`rhs`，`head`，`tail`，`mid`，`begin`，`end`，`last`，`this`
* 时间：`new`，`old`，`orig`，`before`，`after`，`cur`
* 循环：`pos`，`idx`
* 计数：`size`，`len`，`width`，`height`，`depth`，`num`，`nr`，`cnter`
* 序数：`1st`，`2nd`，`3th`
* bool：`is`，`not`，`and`，`or`，`any`，`all`，`none`，`has`
* 介词：`in`，`on`，`at`，`of`，`2`，`4`
* 类型：`int`，`char`，`str`，`strm`，`ptr2`，`itr`
* 用途：`ret`，`ans`，`val`，`need`，`tmp`，`deal`，`src`，`tag`

## 注释
* 块注释`/* comment */`：
    * 源文件标题
    * 注释大片代码
* 单行注释`// comment ` ：代码步骤解释
    * 尾后注释：解释该行代码
    * 行前注释：解释接下来一段代码

一般使用单行注释，这样可以用块注释来注释大片代码，因为`/* */`内是可以嵌套`//`的，但是不能嵌套`/* */`。
除了注释外，还可以使用宏开关与`if constexpr`语句实现类似效果。

## 括号
* 命名空间、类、函数的花括号下一行开端
* 控制流语句块、多行lambda的花括号同一行开端
* 复合表达式中，注意括号的使用以突出显示优先级

## 顺序
* 命名空间：
    1. 类型别名与类的声明
    2. 变量声明
    3. 函数声明
```cpp
class Demo
{
    /* 成员类型 */
    /* 成员对象 */
public:
    /* 析构函数 */
    /* 普通构造函数 */
    /* copy & move */
    /* operator */
    /* 成员函数 */
    /* 静态成员对象 */
    /* 静态成员函数 */
private:
    /* public段有需要private的移到此处 */
};
```

## 空白符
* 预处理分支指令层级缩进时如下
```c
#ifndef TEST
#define TEST
#   ifndef NDEBUG
/* ... */
#   endif // NDEBUG
#endif  // TEST
```
* 命名空间内成员无需缩进
* 尖括号`<>`只与右边间隔空格，如`template<typename T>`
* 嵌套模板参数的右括号`>`间隔空格，如`vector<vector<int> >`
* 模板参数包`...`只与右边隔空格，如`<typename... T>`与`(T&&... args)`

* 函数有关键字template、static、inline、constexpr应该分行写
* 函数名与参数列表不间隔空格
    ```cpp
        template<typename T>
        inline
        int test_func(T&& t);
    ```
* operator与重载符号间不留空格，合在一起当作函数名并遵守上述规则
    ```cpp
        int operator()(int i);
        int operator""_s(unsigned long long i);
        // 非重载符号的例外
        operator bool();
        operator new();
    ```
* 函数默认实参的等号两边都不隔空格`void f(int i=1)`
* 用4个`<space>`代替`<tab>`
* 指针与引用声明中，`*`与`&`应该与类型放在一起，从而引用和指针应该单独声明，如`int& r; double* p;`
* 控制流语句关键字与括号隔空格，控制流语句的括号需内部空格，如`if ( get_bool() )`
* else(catch)跟在if(try)语句块末端花括号后面
    ```cpp
        if ( get_bool() ) {
            //statement
        } else {
            //statement
        }
    ```
* 一元运算符只隔一边空格，二元两边空格，三元全隔，`*x`、`x + y`、`x < y ? x : y`
* 逗号与分号只与右边隔空格，`func(a, b, c)` `for ( init; condition; iterate )` `return 0;`
* 标签的冒号只与右边隔空格，其他地方两边都隔空格
* lambda函数体语句与控制流语句内容超过1句时应该分行
* 若本该在一行的内容过长，要有节奏的断行。
* 断行时二元运算符（除了逗号）断到次行开端。
* 断行发生在括号中时，右括号单独一行
* 函数体、类作用域、命名空间的头尾不留空行
* 各作用域和功能模块之间一般需要隔空行（特别是准备初始条件的代码与正式操作的代码）
* 各函数定义、类定义、命名空间之间隔两行
* 文件末尾加一空行

