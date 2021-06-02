# 系统构建
* 模块
    * 一系列类、函数的集合，文件系统中表现为一个文件
    * 模块名即文件名
* 包
    * 一系列模块的集合，文件系统中表现为一个目录
    * 包名即目录名
* 程序入口
    * 任何模块都可被执行，程序入口即执行文件全局起始处。不过一般还是有一个模块被当做`main`
* 构建
    * 依赖需要手动（包管理器）安装
    * 发布时，或者由用户手动安装依赖，或者由开发者将依赖一同打包
```txt
pkg/
    __init__.py
    main.py~
    modA.py
    modB.py
    modC.py
```

# 模块
```python
import module1, module2                             # 导入模块，隔离其符号表
import module1 as alias1, module2 as alias2         # 导入模块，并使用模块别名
from module import name1, name2                     # 导入模块中指定符号
from module import name1 as alias1, name2 as alias2 # 导入模块中指定符号，并使用符号别名
from module import *                                # 导入模块中所有符号(deprecated)
```
* 当模块做脚本执行时`__name__ == '__main__'`
* 模块搜索路径：
    1. 脚本所在目录（若未指定脚本则搜索当前工作目录）
    2. `sys.path`中指定路径


# 包
```python
import package.subpackage.module    # 可将其完全当做module来编写
from package import module          # 从指定的package中导入module
from package import *               # 导入__init__.py中由__all__定义的模块
```
* 在一个目录下创建名为`__init__.py`的文件即可形成一个package
* 包搜索路径：
    1. 脚本所在目录（若未指定脚本则搜索当前工作目录）
    2. `sys.path`中指定路径


# 函数
```python
##################################################
# 函数定义
##################################################
def func(arg1, arg2, arg3):
    """Docstring starts with upper letter and ends with dot.

    If contains multi line, the second line should be blank.
    """
    return None

##################################################
# 默认实参
##################################################
def f1(arg1, arg2=0, arg3=[]):
    # 存在默认实参的形参必须在无默认实参的形参之前
    # 函数默认值只会在定义时构造一次，后续调用不会清空其状态
    # 意味着若默认实参为 mutable类型，则其状态可跨越多次调用
    pass

##################################################
# 可变参数
##################################################
def f2(*tupleArg, **dictArg):
    # tuple_arg为tuple，存储多余的位置参数；
    # dict_arg 为dict ，存储多余的键值对参数；
    # tuple_arg必须在dict_arg之前
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
f2(*Tuple, **Dict)  # 将Tuple解包做位置参数；将Dict解包做键值参数；若*Dict下相当于*keys(Dict)

##################################################
# 类型注解
##################################################
def f4(arg1: int, arg2: list[int or str], arg3: dict[str: int]) -> tuple[bool, int, str]:
    # 仅做注释而不强制限制
    pass

##################################################
# lambda表达式
##################################################
lambda arg1, arg2: express

##################################################
# 函数属性
##################################################
func.__doc__
func.__name__
func.__module__
func.__defaults__
func.__kwdefaults__
func.__annotations__
```

# 类
python不存在真正的封装，所有属性均可动态增删改查，于是有了一些约定俗成的不成文法则
* `_protected_mem`，单下划线开头来暗示其为private成员，再加以装饰器以模拟private访问控制
```python
class MyClass(Base1, Base2):
    """Docstring starts with upper letter and ends with dot.

    If contains multi line, the second line should be blank.
    """
##################################################
# 定义静态属性，该属性在所有实例间共享
##################################################
    PublicMember_ = []
    _PrivateMember_ = []

##################################################
# 定义方法，方法的第一个参数为该类实例self
##################################################
    # __init__方法在构造实例时自动调用，相当于构造函数
    def __init__(self, *args):
        # 调用基类的构造函数，单继承可使用super()获取基类部分的实例
        Base1.__init__()
        Base2.__init__()

        # 为self实例添加属性
        self._privateMember_ = args[0]
        self.publicMember_ = args[1]

    # 在需要将用户自定义类的实例转换为bool值时，
    # 先尝试调用__nonzero__方法，若不存在则调用__len__，若都不存在则转换为True
    def __nonzero__(self):
        return self.bool_

    def __len__(self):
        return self.len_

    # 在需要将用户自定义类的实例转换为str时调用
    def __str__(self):
        return self.str_

##################################################
# 定义静态方法
##################################################
    @staticmethod
    def StaticMethod(args):
        pass

    # 利用装饰器定义类方法，第一个参数为该类cla
    @classmethod
    def ClassMethod(cls):
        pass

##################################################
# 增强封装性
##################################################
    # 名称改写，以两个下划线开头的 __name 会被改写为 _classname__name，从而尽量避免与子类中的名称冲突
    def __name(self):   # 改写为_MyClass__name
        __mem = []      # 改写为_MyClass__mem

    # 限制只能该类只能绑定如下属性
    __slots__ = ('mem1_', 'mem2_')

    # 利用装饰器设置只读属性或方法
    @property
    def mem(self):
        return self.mem_

    # 修改属性mem时会调用mem.setter。默认不能修改
    @mem.setter
    def mem(self, new):
        self.mem_ = new

    # 删除属性mem时会调用mem.deleter。默认不能删除
    @mem.deleter
    def mem(self):
        del self.mem_
```


# 数值类型
## 布尔值
* 字面值`True`或`False`
* 字面值`None`视作`False`
* 任何数值类型的`0`视作`False`
* 任何`空`的集合视作`False`


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
int([x])

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
* 整数与浮点数混合运算结果为浮点数
```python
# 构造
1.
.5
1.5
1.e1
.5e1
1.5e1
1e1
f = float([x])

# 其他
Float.is_integer()          # 能不丢失精度得转换为整数
Float.as_integer_ratio()    # 返回一对整数（分子/分母）
```
<!-- entry end -->

<!-- entry begin: python decimal -->
## 高精度浮点数
* **利用字符串存储浮点值** 
```python
from decimal import Decimal

# 构造
Decimal('3.14')
Decimal('0')
Decimal('-0')
Decimal('inf')
Decimal('-inf')
Decimal('nan')

# 上下文设置
getcontext()        # 取得当前上下文。修改其数据属性.prec即可修改精度。精度只用于计算过程中，而不作用于存储时
setcontext(context) # 设置当前上下文
BasicContext        # 精度9，舍入ROUND_HALF_UP，启动所有陷阱
ExtendedContext     # 精度9，舍入ROUND_HALF_EVEN，关闭所有陷阱
DefaultContext      # 精度28，舍入ROUND_HALF_EVEN，启用陷阱Overflow、InvalidOperation、DivisionByZero
```
<!-- entry end -->

<!-- entry begin: python fraction -->
## 分数
```python
from fractions import Fraction

Fraction(num=0, den=1)
Fraction(Float)
Fraction(Decimal)
Fraction(string)    # "[sign] numerator [/ denominator]"
```
<!-- entry end -->

<!-- entry begin: python sequence -->
# 容器类型
```python
all(itr)                    # 元素全为True
any(itr)                    # 元素存在True
sum(itr, /, start=0)        # 从第start个元素开始求和
max(itr, *[, key, default]) # 返回范围中最大值。default为默认值（当范围为空时）
max(arg1, arg2, *args[,key])# 返回范围中最大值。key返回元素比较键
min(itr, *[, key, default]) # 返回范围中最小值。default为默认值（当范围为空时）
min(arg1, arg2, *args[,key])# 返回范围中最小值。key返回元素比较键

len(itr)                    # 返回元素数量
enumerate(itr)              # 返回元素为（索引，值）元组的迭代器
range(N)                    # 返回列表[0, 1, ..., N-1]
range(B, E, S=1)            # 返回列表[B, B+S, ..., E-S]

zip(*itr)                   # 返回链接各组元素后的迭代器
filter(func, itr)           # 过滤出func(e)为True的元素，返回迭代器
map(func, itr)              # 将每个元素经由func处理后装入返回的迭代器中
reversed(itr)               # 返回反向的迭代器
sorted(itr, *,              # 返回已排序列表
    key=None, reverse=False)# key返回元素比较键，reverse表示反向排序
```
<!-- entry end -->

<!-- entry begin: python string -->
## 字符串
```python
# 构造
'可直接包含""的字符串'
"可直接包含''的字符串"
R"可以直接包含\的字符串"
F"可以使用{format语法}到字符串"

"""多行
字符串"""

'''
可用\
转义行尾换行符
'''

"多行字符串"
"忽略之间空白符"
"自动连接"

Str = str(obj)
Str = Str1 + Str2
Str = Str * num


# 访问与搜索
Str[0]
Str[-1]
Str[begin:end[:step]]
Str[::-1]
subStr in Str
Str.startswith(preffix)
Str.endswith(suffix)
Str.find(sub)           # find函数若未搜索到目标则静默处理，返回-1
Str.rfind(sub)
Str.index(sub[ i[,j]])  # index函数若未搜索到目标则引发异常
Str.rindex(sub)
Str.count(sub)

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
Str.replace(old, new [, count]) # 默认替换所有old为new
Str.encode(encoding="utf-8")
Str.expandtabs(tabsize=8)
Str.center(width[, fill])
Str.ljust(width[, fill])
Str.rjust(width[, fill])
Str.zfill(width)    # 用零填充左边至长度为width（若width小于len(Str)则忽略），若第一个字符为+或-则在其后添加0

Str.lstrip([chars]) # 移除开头出现在chars中的字符，默认空白符
Str.rstrip([chars]) # 移除结尾出现在chars中的字符，默认空白符
Str.strip([chars])  # 移除两边出现在chars中的字符，默认空白符
Str.removeprefix(p)
Str.removesuffix(s)

Str.join(itr)       # itr元素必须是str，连接符就是Str
Str.partition(sep)  # 返回三元组（之前、分隔符、之后）
Str.rpartition(sep) # 最后一次出现
Str.split(sep=None  # 默认sep为空白符，最多分隔maxsplit
    maxsplit=-1)
Str.rsplit(sep=None # 从右边开始分割
    maxsplit=-1)
Str.splitlines(keepends=False)
```


<!-- entry begin: python tuple -->
## 元组
* **元组虽也属于immutable对象，但其元素可以时mutable的，注意这点！**
```python
# 构造
Tuple = tuple([itr])
Tuple = ()
Tuple = (e0, )      # 括号在一定情况下可省略
Tuple = (e0, e1)    # 括号在一定情况下可省略
Tuple = e0,
Tuple = e0, e1
Tuple = Tuple1 + Tuple2
Tuple = Tuple * num

T1, T2= e0, e1
T1, T2= itr

# 访问与搜索
Tuple[0]
Tuple[-1]
Tuple[begin:end[:step]]
Tuple[::-1]
ele in Tuple
Tuple.count(e)
Tuple.index(e[,i[,j]])
```
<!-- entry end -->

<!-- entry begin: python list heap -->
## 列表
```python
# 构造
List = list([itr])
List = []
List = [e0, e1]
List = [resultExpr for x in List if condition] # resultList.appen(resltExp)
List = List1 + List2
List = List * num

# 访问与搜索
List[0]
List[-1]
List[begin:end[:step]]
List[::-1]
ele in List
List.count(e)
List.index(e[,i[,j]])

# 修改
List[i] = e
List[i:j] = itr
List[i:j:k] = itr
List.reverse()
List.insert(i, e)   # List[i:i] = e
List.append(e)      # List[len:len] = [e]
List.extend(itr)    # List[len:len] = itr
List.pop([i])       # 若不存在则引发ValueError
List.remove(e)
del s[i:j]
del s[i:j:k]
List.clear()

# 堆算法
import heapq
heapq.heapify(List)
heapq.heappush(heap, itm)
heapq.heappop(heap)
heapq.heappushpop(heap, itm)    # 将itm插入并弹出最小值（比分开操作更快速）
heapq.heapreplace(heap, itm)    # 同上，但弹出的值可能会大于压入的itm
heapq.merge(*itr, key=None, reverse=False)
# 利用堆排序算法返回小值，若n太大则应使用sorted，若n==1则应使用max或min
heapq.nlargest(n, itr, key=None)
heapq.nsmallest(n, itr, key=None)
```
<!-- entry end -->


<!-- entry begin: python set -->
## 集合
```python
# 构造
Set = set([itr])
Set = {ex, ey, ez}
Set = {resultExp for x in List if condition} # resultList.appen(resltExp)
Set = Set1 + Set2
Set = Set * num

# 访问与搜索
ele in Set
Set.copy()

# 修改
Set.add(e)
Set.discard(e)
Set.remove(e)   # 若不存在则引发KeyError
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
<!-- entry end -->

<!-- entry begin: python dict -->
## 字典
```python
# 构造
Dict = dict([itr])      # itr元素为键值对元组
Dict = {}
Dict = {key: value, k: v}  # 键为不可变类型，如数值类型、字符串、元组
Dict = {keyExp: valueExp for x in List if condition} # resultList.appen(keyExp, valueExp)

# 访问与搜索
Dict.items()            # 元素为元组的列表
Dict.keys()             # 元素为键的列表。dict作itr时其元素即dict的键
Dict.values()           # 元素为值到列表

key in Dict             # for key, value in Dict
Dict[key]               # 若key不存在则仅在赋值时自动创建
Dict.get(key[, default])# 不自动创建
Dict.pop(key[, default])
Dict.clear()
Dict.update(itr)
Dict |  Other           # 合并两字典，Other的键值优先
Dict |= Other           # 合并两字典，Other的键值优先
```
<!-- entry end -->


# 运算符
| 运算符                                                               | 描述                                       |
|----------------------------------------------------------------------|--------------------------------------------|
| `()`                                                                 | 函数调用、元组                             |
| `[]` `[:]`                                                           | 下标，切片                                 |
| `**`                                                                 | 指数（注意该二元运算符优先级高于单目运算符 |
| `~` `+` `-`                                                          | 按位取反, 正负号                           |
| `*` `/` `%` `//`                                                     | 乘，除，模（向下取整），整除（向下取整）   |
| `+` `-`                                                              | 加，减                                     |
| `>>` `<<`                                                            | 右移，左移                                 |
| `&`                                                                  | 按位与                                     |
| `^`                                                                  | 按位异或                                   |
| `\|`                                                                 | 按位或                                     |
| `<=` `<` `>` `>=`                                                    | 小于等于，小于，大于，大于等于             |
| `==` `!=`                                                            | 等于，不等于                               |
| `is`  `is not`                                                       | 身份运算符                                 |
| `in` `not in`                                                        | 成员运算符                                 |
| `not`                                                                | 逻辑非                                     |
| `and`                                                                | 逻辑与                                     |
| `or`                                                                 | 逻辑或                                     |
| `=` `+=` `-=` `*=` `/=` `%=` `//=` `**=` `&=` `\|=` `^=` `>>=` `<<=` | （复合）赋值运算符                         |

**注意**：
* 多重赋值`a = b = c`的意思是`a = c; b = c`
* 元组赋值`a, b = b, a`中，先计算等号右边形成右值，再将已固定的右值赋值给左边
* 支持连续比较`a > b > c`

# 语句
## 分支
```python
if condition1:
    pass
elif condition2:
    pass
else:
    pass

# 条件表达式
x if Condition else y
```


## 循环
```python
while condition:
    # break 
    # continue
    pass

for key, value in Dict:
    pass
else:
    # 若穷尽Dict/List元素则执行else语句，注意由break终止循环时会跳过该语句
```


## 异常
```python
# 开启异常机制
try:
    # 抛出异常
    raise Exception('error', 'message')
# 捕获异常实例并命名为inst
except Exception as inst:
    # 丢弃之前的旧异常链，并抛出新链
    raise Exception('error2') from None
# 捕获异常
except Exception:
    # 抛出异常链，并将新异常挂到捕获实例后面
    raise Exception('error3')
# 捕获所有类型的异常
except:
    # 重新抛出捕获的异常
    raise
# 若未捕获到异常则执行else段
else:
    pass
# 无论异常是否产生、是否被捕获都会执行该段，若异常未被捕获则执行完该段后重新抛出
finally:
    pass


# 相当当于C++ unique_ptr，一般用于管理file对象
with Expr as inst:
    pass
```
* 用户定义的异常类应该派生自`Exception`

# 语法杂项
* 逻辑行即一条完整的语句
* 物理行即两个换行符之间到内容
* 多个逻辑行可显示或隐式的连接成一个逻辑行
    * 显示：利用`\`转义行尾换行符
    * 隐式：`()` `[]` `{}`内可自动转义行尾换行符
* 逻辑行到缩进层级与结束利用 **单调栈** 来判断
* 利用`nonlocal`与`global`调整变量到作用域
* 变量生命周期会延长至语句局部作用域外，循环体中局部变量仍会跨越多次循环
    > （静态语言中变量需要声明，从语法上就不可能跨越多次循环）
* **Python中所有实际数据（包括字面量）都是对内存中对象的引用，`a = b`代表的不是将b赋值到a上，而是将a引用指向b引用的对象！**
* **immutable对象的值不可更改，只能新建；mutable的对象则可通过一些方法修改原值而非新建对象**
* **Python中的表达式并不行C/C++一样有严格的规律的（左右值、类型、返回值），在Python中对应操作就该使用对应语法糖** 




# 标准库
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
repr(obj)       # 返回对象字符串
eval(expr)      # 动态解析单个表达式并返回结果
exec(code)      # 动态解析代码片段并仅产生其副作用而返回None

import re
re.findall(regex, string)   # 返回匹配子串的列表
re.sub(regex, repl, string) # 替换，语法类似sed
```

## 输入输出
```python
input([prompt])             # 遇换行停止

print(*obj,
    sep=' ', end='\n'
    file=sys.stdout,
    flush=False)

open(file,                  # 返回file可用于for-in语句，每次循环处理一行（包含换行符）
    mode='r',               # 只读'r'、截断'w'、追加'a'、读写'+'、排它'x'、二进制'b'、文本't'）
    buffering=-1,           # 关闭缓冲0（仅二进制）、行缓冲1（仅文本）、指定缓冲区大小>1
    encoding=None,
    errors=None,
    newline=None,
    closefd=True,
    opener=None)

import json
json.dump(obj, File, *      # dict中的键会强制转换为字符串
    skipkeys=False          # 若键不为bool、int、float、str、None则是否跳过而避免引发异常
    indent=None             # 缩进宽度
    sort_keys=False)        # 是否对dict排序

json.dumps(obj, *           # 返回json字符串而非写入文件
    skipkeys=False          # 若键不为bool、int、float、str、None则是否跳过而避免引发异常
    indent=None             # 缩进宽度
    sort_keys=False)        # 是否对dict排序

json.load(File)
json.loads(Str)
```


## 操作系统
```python
import os
os.getcwd()
os.chdir(path)
os.system(cmd)

import shutil
shutil.copyfile(from, to)
shutil.move(from, to)

import glob
glob.glob('*.py')   # 返回文件列表

import sys
sys.argv            # 返回命令行参数列表
sys.stdin
sys.stdout
sys.stderr
```


