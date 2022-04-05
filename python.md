# 项目构建
```txt
pkg_src/
├── __init__.py
├── main.py
├── modA.py
├── modB.py
└── modC/py
```

# 包与模块
* 在一个目录下创建名为`__init__.py`的文件即可形成一个package
* 导入包前会执行`__init__.py`并将其中符号导入包命名空间
```python
from package.module import name

import package.module       # 可将其完全当做module来编写，见下
from package import module  # 从指定的package中导入module
from package import *       # 导入包 __init__.py 中由 类型为list[str]的全局变量 __all__ 中指定的模块

import module1, module2                             # 导入模块，隔离其符号表
import module1 as alias1, module2 as alias2         # 导入模块，并使用模块别名
from module import name1, name2                     # 导入模块中指定符号
from module import name1 as alias1, name2 as alias2 # 导入模块中指定符号，并使用符号别名
from module import *                                # 导入模块中所有符号(deprecated)
```
* 当模块做脚本执行时`__name__ == '__main__'`
* 包与模块搜索路径：
    1. 执行脚本所在目录（若未指定脚本则搜索当前工作目录）
    2. `sys.path`中指定路径


# 变量
* 变量类型可动态变化，类型、类名都可动态获取
* 第一次给变量赋值即算变量声明
* 函数内局部变量作用域一直延续到声明所在函数退出
* `nonlocal`指明声明的符号绑定到闭包外同名符号
* `global`指明声明的符号绑定到同名全局符号

# 表达式
| 运算符            | 描述             |
| ----------------- | ---------------- |
| `.` `()` `[]`     | 成员、函数、下标 |
| `**`              | 指数             |
| `~` `+` `-`       | 单目             |
| `*` `/` `//` `%`  | 乘除             |
| `+` `-`           | 加减             |
| `>>` `<<`         | 位移             |
| `&`               | 位与             |
| `^`               | 位异或           |
| `\|`              | 位或             |
| `<=` `<` `>` `>=` | 大小关系         |
| `==` `!=`         | 等值关系         |
| `is`  `is not`    | 身份运算符       |
| `in` `not in`     | 成员运算符       |
| `not`             | 逻辑非           |
| `and`             | 逻辑与           |
| `or`              | 逻辑或           |

* 赋值为语句而非表达式
    * 连续赋值`a = b = c`的意思是`a = c; b = c`而非`b = c; a = b`
    * 多元赋值`a, b = b, a`中，先计算等号右边产生副本，再将副本赋值给左边
    * 解包赋值`a, b = itr`赋值itr中的元素给a, b，itr中元素数量必须刚好
* 支持连续比较`a > b > c`即`a > b and b > c`
* 浮点数支持`%`运算符

# 语句
注意：空语句体一定要使用`pass`语句

* 分支
```python
if cond:
    pass
elif cond:
    pass
else:
    pass

# 条件表达式
x if cond else y
```


* 循环
```python
while cond:
    break
else:   # 若由break跳出循环则不执行else
    pass

for key, value in Dict: # 范围可动态变化
    for elem in Seq:
        for val1, val2 in Iter:
            continue
else:   # 若由break跳出循环则不执行else
    pass
```


* 异常
```python
# 异常捕获机制存在多态类型甄别，异常类型都应派生子ExceptionBase
try:
    raise Exception('error0')

except Exception:
    raise Exception('error1')   # 引发的异常连成一条异常链，由属性 __context__ 连接

except Exception as excep:
    raise Exception('error2') from Exception('__cause__')   # from后的异常作为from前异常的直接原因而赋值到其属性 __cause__

except:
    raise

else: # 若未捕获到异常则执行else段
    pass

finally: # 无论异常是否产生、是否被捕获都会执行该段，若异常未被捕获则执行完该段后重新抛出
    pass


# 相当当于C++ unique_ptr，一般用于管理file对象
with Expr as inst:
    pass
```

# 函数
```python
##################################################
# 函数定义
##################################################
def f0(arg1, arg2, arg3):
    """Docstring starts with upper letter and ends with dot.

    If contains multi line, the second line should be blank.
    """
    pass

##################################################
# 类型注解
##################################################
def f1(arg1: int, arg2: list[int or str], arg3: dict[str: int]) -> tuple[bool, int, str]:
    # 仅做注释而不强制限制
    pass

##################################################
# 默认实参
##################################################
def f2(arg1, arg2=0, arg3=[]):
    # 存在默认实参的形参必须在无默认实参的形参之前
    # 函数默认值只会在定义时构造一次，后续调用不会清空其状态
    # 意味着若默认实参为 mutable类型，则其状态可跨越多次调用
    pass

##################################################
# 位置参数与键值参数
##################################################
def f3(pos1, pos2, /, pos_or_kwd, *, kwd1, kwd2):
    # -----------    ----------     ----------
    #   |             |                  |
    #   |        Positional or keyword   |
    #   |                                - Keyword only
    #    -- Positional only
    pass

f3(a1, a2, a3, kwd1=a4, kwd2=a5)
f3(a1, a2, pos_or_kwd=a3, kwd1=a4, kwd2=a5)

##################################################
# 可变参数
##################################################
def f4(arg1, *tupleArg, **dictArg):
    # tupleArg相当于类型为tuple的形参，存储多余的位置参数；
    # dictArg相当于类型为dict 的形参，存储多余的键值对参数；
    # tupleArg必须在dictArg之前
    pass

f4(arg1, *Tuple, *Dict, **Dict) # *Tuple解包做位置参数，*Dict也解包其key做位置参数；**Dict解包做键值参数
f4(*Tuple)                      # 即使解包对象是动态长度，也无需一定对应函数的变参位置

##################################################
# lambda闭包
##################################################
lambda arg1, arg2: retExpr
```

# 类
* python不存在真正的封装，所有属性均可动态增删改查
* 自定义类型也是引用语义

## 封装
```python
class Base:
    """Docstring starts with upper letter and ends with dot.

    If contains multi line, the second line should be blank.
    """
# 定义静态属性，该属性在所有实例间共享
    PublicMember_ = []
    _PrivateMember_ = []

# __init__方法在构造实例时自动调用，相当于构造函数
    def __init__(self, *args):
        # 为self实例添加属性
        self.bool_ = args[0]
        self.len_  = args[1]
        self.str_  = args[2]

# 所有方法的第一个实参都是类实例self
    def __nonzero__(self):  # 转换bool值时尝试调用
        return self.bool_

    def __len__(self):      # 转换bool值时尝试调用
        return self.len_

    def __str__(self):      # 转换str值时调用
        return self.str_
```

## 继承
```python
class Derived(Base, Base2):
    def __init__(self, *args):
        Base1.__init__()
        Base2.__init__()
        self.mem = args
```

## 多态
* python中变量类型本身即可动态变化，动态绑定目的为复用


# 数值类型
## 布尔值
* 字面值`True`或`False`
* 字面值`None`视作`False`
* 任何数值类型的`0`视作`False`
* 任何容器类型为空视作`False`
* 自定义类型默认视作`True`


<!-- entry begin: python int -->
## 整数
* **支持任意大小的整数**
```python
# 构造
0b10    # 二进制
0o17    # 八进制
19      # 十进制
0x1F    # 十六进制
10_000  # 支持用_来分割数字
int(Int)
int(Float)
int(Str)

# 底层表示
Int.bit_length()            # 返回表示该整数所需最少的二进制位数

Int.to_bytes(length,        # 返回整数的前length个字节
    byteorder,              # 大端'big'或小段'little'
    *, signed=False)        # 是否使用补码

Int.from_bytes(bytes,
    byteorder,
    *, signed=False)
```
<!-- entry end -->

<!-- entry begin: python float -->
## 浮点数
* **浮点数一般使用IEEE双精度浮点数，存在精度问题**
```python
# 构造
1.1         # 整数部分或小数部分之一可省略
1e1         # 小数点或指数部分之一可省略，基数为10
float(Int)
float(Float)
float(Str)  # 支持Inf、Nan

# 整数转换
Float.is_integer()          # 能不丢失精度得转换为整数
Float.as_integer_ratio()    # 返回一对整数（分子/分母）
```
<!-- entry end -->

<!-- entry begin: python fraction -->
## 分数
```python
from fractions import Fraction

Fraction(num=0, den=1)
Fraction(Int)
Fraction(Float)
Fraction(Decimal)
Fraction(Str)       # 格式："[sign] numerator [/ denominator]"
```
<!-- entry end -->

<!-- entry begin: python decimal -->
## 高精度数字
* **利用字符串存储浮点值**
```python
from decimal import Decimal

# 构造
Decimal(Int)
Decimal(Float)
Decimal(Str)        # 支持Inf与Nan

# 上下文设置
getcontext()        # 取得当前上下文。修改其数据属性.prec即可修改精度。精度只用于计算过程中，而不作用于存储时
setcontext(context) # 设置当前上下文
DefaultContext      # 精度28，舍入ROUND_HALF_EVEN，启用陷阱Overflow、InvalidOperation、DivisionByZero
BasicContext        # 精度9，舍入ROUND_HALF_UP，启动所有陷阱
ExtendedContext     # 精度9，舍入ROUND_HALF_EVEN，关闭所有陷阱
```
<!-- entry end -->


<!-- entry begin: python algorithm -->
# 容器类型
```python
len(itr)                        # 返回元素数量
enumerate(itr)                  # 返回元素为(idx, val)的迭代器
range(N)                        # 返回元素范围[0, N)的迭代器
range(begin, end, step=1)       # 返回元素范围[begin, end)且步长为step的迭代器

all(itr)                        # 元素是否全为True
any(itr)                        # 元素是否存在True
sum(itr, /, start=0)            # 返回从第start个元素开始的和
max(itr, [*, key, default])     # 返回范围中最大值。范围为空返回default
min(itr, [*, key, default])     # 返回范围中最小值。范围为空返回default
max(arg1, arg2, *args[, key])   # 返回范围中最大值。key(arg)返回元素比较键
min(arg1, arg2, *args[, key])   # 返回范围中最小值。key(arg)返回元素比较键

sorted(itr, [*, key, reverse]   # 返回范围包含排序后的元素
reversed(itr)                   # 返回范围包含逆序后的元素
filter(func, itr)               # 返回范围包含func(elem)为True的元素
map(func, itr)                  # 返回范围包含func(elem)返回值
zip(*itr)                       # 返回各范围元素组合后的迭代器
```
<!-- entry end -->
* python中容器类型底层数据结构均为指针，指向“栈结构”，从而封装了内存重新分配（扩容与缩容）

<!-- entry begin: python string -->
## 字符串
* 字符串为只读类型，底层类型即码点数组，可转换为`byte`
```python
# 构造
'可直接包含""的字符串'
"可直接包含''的字符串"
R"可以直接包含\的字符串"
F"可以使用{format syntax}到字符串"

"""多行
字符串"""

'''
可用\
转义行尾换行符
'''

"多行字符串"
"忽略之间空白符"
"自动连接"

Str = str([obj])
Str = Str1 + Str2
Str = Str * num

# 访问与搜索
Str[begin:[end[:step]]]     # 支持负数-n表示倒数第n个元素下标
subs in Str
Str.startswith(preffix)
Str.endswith(suffix)
Str.find(subs)              # 若未搜索到目标则返回-1
Str.rfind(subs)
Str.index(subs[, i[, j]])   # 若未搜索到目标则引发异常
Str.rindex(subs)
Str.count(subs)

# 字符类
Str.isalpha()       # Unicode Letter
Str.isdecimal()     # Unicode Decimal
Str.isdigit()       # Unicode Decimal,Digit
Str.isnumeric()     # Unicode Decimal,Digit,Numeric
Str.isalnum()       # 若以上四个方法均返回True则返回True
Str.isspace()       # Unicode Seperator,Space
Str.isascii()       # ASCII
Str.isprintable()   # 是否可打印
Str.isidentifier()  # 是否为保留标识符
Str.istitle()       # 单词开头字符若区分大小写则为大写，其余字符若区分大小写则为小写
Str.isupper()       # 至少一个区分大小写字符且为大写
Str.islower()       # 至少一个区分大小写字符且为小写

# 修改：注意修改操作并非修改原字符串（字符串为immutable），而是返回新到修改值
Str.title()
Str.upper()
Str.lower()
Str.swapcase()
Str.center(width[, fill])
Str.ljust(width[, fill])
Str.rjust(width[, fill])
Str.zfill(width)                # 用零填充左边至长度为width（若width小于len(Str)则忽略），若第一个字符为+或-则在其后添加0
Str.replace(old, new [, count]) # 默认替换所有old为new
Str.encode(encoding="utf-8")    # 返回byte，Byte.decode(encoding="utf-8")返回str
Str.expandtabs(tabsize=8)

Str.lstrip([chars])             # 移除开头出现在chars中的字符，默认空白符
Str.rstrip([chars])             # 移除结尾出现在chars中的字符，默认空白符
Str.strip([chars])              # 移除两边出现在chars中的字符，默认空白符
Str.removeprefix(p)
Str.removesuffix(s)

Str.join(itr)                   # itr元素类型必须为str，连接符就是Str
Str.partition(sep)              # 返回三元组（之前、分隔符、之后）
Str.rpartition(sep)             # 从右边开始搜索sep
Str.split(sep=None              # 默认sep为空白符
    maxsplit=-1)
Str.rsplit(sep=None             # 从右边开始分割
    maxsplit=-1)
Str.splitlines(keepends=False)
```


<!-- entry begin: python tuple -->
## 元组
* 元组为只读类型
```python
# 构造
Tuple = tuple([itr])
Tuple = ()
Tuple = (e0, )      # 括号在一定情况下可省略
Tuple = (e0, e1)    # 括号在一定情况下可省略
Tuple = Tuple1 + Tuple2
Tuple = Tuple * num

# 访问与搜索
Tuple[beg:[end[:step]]] # 支持负数-n表示倒数第n个元素下标
elem in Tuple
Tuple.count(v)
Tuple.index(v[, i[, j]])
```
<!-- entry end -->

<!-- entry begin: python list heap -->
## 列表
```python
# 构造
List = list([itr])
List = []
List = [e0, e1]
List = [result for x in itr if condition]
List = List1 + List2
List = List * num

# 访问与搜索
List[beg:[end[:step]]]  # 支持负数-n表示倒数第n个元素下标
elem in List
List.count(v)
List.index(v[, i[, j]])

# 修改
List[i] = v
List[i:j] = itr
List[i:j:k] = itr
List.reverse()
List.insert(i, v)   # List[i:i] = e
List.append(v)      # List[len:len] = [e]
List.extend(itr)    # List[len:len] = itr
List.pop([i])       # 若不存在则引发ValueError
List.remove(v)
del s[i:j]
del s[i:j:k]
List.clear()
```
<!-- entry end -->


<!-- entry begin: python set -->
## 集合
```python
# 构造
Set = set([itr])
Set = {ex, ey, ez}
Set = {result for x in itr if condition}
Set = Set1 + Set2
Set = Set * num

# 访问与搜索
ele in Set
Set.copy()

# 修改
Set.add(v)
Set.discard(v)
Set.remove(v)   # 若不存在则引发KeyError
Set.clear()
Set.update(itr)

# 集合运算
isdisjoint(Set)
Set <   Other
Set <=  Other
Set >   Other
Set >=  Other
Set |   Other
Set |=  Other
Set &   Other
Set &=  Other
Set -   Other
Set -=  Other
Set ^   Other
Set ^=  Other
```
**不可hash类型包括：list, set, dict**
<!-- entry end -->
<!-- entry begin: python dict -->
## 字典
```python
# 构造
Dict = dict([itr])      # itr元素为键值对元组
Dict = {}
Dict = {key: value}
Dict = {key: val for x in itr if condition}

# 访问与搜索
Dict.items()            # 键值对范围
Dict.keys()             # 键范围
Dict.values()           # 值范围

key in Dict
Dict[key]               # 若key不存在则仅在赋值时自动创建
Dict.get(key[, default])# 不自动创建
Dict.pop(key[, default])
Dict.clear()
Dict.update(itr)
Dict |  Other           # 合并两字典，Other的键值优先
Dict |= Other           # 合并两字典，Other的键值优先
```
<!-- entry end -->


# 内建函数
## 语言特性
```python
dir(obj)                    # 返回obj的符号表
type(obj)                   # 返回obj的类型
id(obj)                     # 返回对象标识值
callable(obj)               # 返回bool表示obj是否可调用
isinstance(obj, class)      # 若obj为class的实例或其子类的实例则为True
issubclass(subclass, class) # 若subclass为class的子类则为True
super()                     # 返回当前类的父类（基类）部分
locals()                    # 返回局部符号表，可用于判断变量是否存在
globals()                   # 返回全局符号表，可用于判断变量是否存在
```


## 数值处理
```python
abs(v)                  # 返回绝对值
divmod(a, b)            # 返回商和余数
pow(base, exp[, mod])
round(num, ndigits=0)   # 化简num到小数点后ndigit位（向偶舍入）
```

## 字符处理
```python
ascii(obj)  # 返回对象打印字符串，非ascii则用\x \u \U转义
ord(c)      # 返回字符c对应Unicode码点
chr(i)      # 返回Unicode码点i对应的字符
bin(i)      # 返回整数对应二进制字符串（`0b`开头）
oct(i)      # 返回整数对应八进制字符串（`0o`开头）
hex(i)      # 返回整数对应十六进制字符串（`0x`开头）
repr(obj)   # 返回对象字符串
eval(expr)  # 动态解析单个表达式并返回结果
exec(code)  # 动态解析代码片段并仅产生其副作用而返回None
```

## 输入输出
```python
input(prompt='')            # 遇换行停止

print(*obj, sep=' ', end='\n')

open(file,                  # 返回file可用于for-in语句，每次循环处理一行（包含换行符）
    mode='r',               # 只读r、截断w、追加a；读写+、二进制b、文本t、文件已存在则失败x
    buffering=-1,           # 关闭缓冲0（仅二进制）、行缓冲1（仅文本）、指定缓冲区大小>1
    encoding=None)          # 编码为系统默认编码
```
