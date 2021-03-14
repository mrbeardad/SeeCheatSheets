**Python中所有实际数据（包括字面量）都是对内存中对象的引用，`a = b`代表的不是将b赋值到a上，而是将a引用指向b引用的对象！** 

**immutable对象的值不可更改，只能新建；mutable的对象则可通过一些方法修改原值而非新建对象** 


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
from package import *               # 导入package中由__all__定义的模块
```
* 在一个目录下创建名为`__init__.py`的文件即可形成一个package
* 包搜索路径：
    1. 脚本所在目录（若未指定脚本则搜索当前工作目录）
    2. `sys.path`中指定路径

# 函数
```python
# 函数定义
def f(arg1, arg2, arg3):
    """Docstring starts with upper letter and ends with dot.

    If contains multi line, the second line should be blank.
    """
    return None

# 默认实参
val = 0
def f1(arg1, arg2=[], arg3=val):
    # 存在默认实参的形参必须在无默认实参的形参之前
    # 函数默认值val会在定义时就被值捕获，且函数默认值只会在定义时构造一次，后续调用不会清空其状态
    pass

# 变参：tuple与dict
def f2(*tupleArg, **dictArg, mustKeyword=None):
    # *tuple_arg存储多余的位置参数，**dict_arg存储多余的键值对参数，前者必须在后者前面
    pass

# 位置参数与键值参数
def f3(pos1, pos2, /, pos_or_kwd, *, kwd1, kwd2):
    # -----------    ----------     ----------
    #   |             |                  |
    #   |        Positional or keyword   |
    #   |                                - Keyword only
    #    -- Positional only
    pass

# 类型注解
def f4(arg1: Type1, arg2: Type2 = type2()) -> RetType:
    pass

# 函数调用
f(0, arg3=1)   # 0为位置参数，arg3=1为键值对参数（arg3必须存在于函数定义的参数列表中），前者必须在后者前面

dict_arg = {'arg1': 1, 'arg2': 2, 'arg3': 3}
f(**dict_arg)

list_arg = [0, 1, 2]
f(*list_arg)
```


# 类
```python
# python不存在真正的封装
class MyClass(Base1, Base2):
    """Docstring."""

    # 数据属性无需声明，在第一次赋值时产生
    member_ = []    # 该mutable数据属性对象在所有实例间共享
    __mem = member_ # 名称改写，__mem被解释器替换为_classname__mem，从而尽量避免与子类中的名称冲突

    # 方法属性的第一个参数为self
    def __init__(self, *args):  # __init__方法再构造实例时自动调用
        self.member_ = args     # 定义一个实例的数据属性
```


# 数值类型
## 布尔值
* 字面值`True`或`False`
* 字面值`None`视作`False`
* 任何数值类型的`0`视作`False`
* 任何`空`的序列和多项集视作`False`


## 整数
* 支持任意大小的整数
* 二进制`0b10`、八进制`0o10`、十进制`10`、十六进制`0x10`
```python
int.bit_length()            # 返回表示该整数所需最少的二进制位数

int.to_bytes(length,        # 返回整数的前length个字节
    byteorder,              # 大端'big'或小段'little'
    *, signed=False)        # 是否使用补码

int.from_bytes(bytes,
    byteorder,
    *, signed=False)
```


## 浮点数
* 浮点数`1.`、`.5`、`1.5`、`1.e2`、`.5e2`、`1.5e2`、`1e2`
* 整数与浮点数混合运算结果为浮点数
```python
float.as_integer_ratio()    # 返回一对整数（分子/分母）
float.is_integer()          # 能不丢失精度得转换为整数
```


# 序列类型
* 存在：`key in Seq`    `key not in Seq`
* 连接：`seq1 + seq2`
* 重复：`n * seq`
* 索引：`Seq[0]`、`Seq[-1]`、`Dict[key]`
* 切片：`Seq[begin:end[:step]]`

> mutable序列

* s[i] = x
* s[i:j] = t
* s[i:j:k] = t
* s.append(x)
* s.copy()
* s.extend(t)或s += t
* s.insert(i, x)
* s.reverse()
* s.pop([i])
* s.remove(x)
* s.clear()
* del s[i:j]
* del s[i:j:k]

## 字符串
* 字符串：
    * `'可直接包含"" '`
    * `"可直接包含'' "`
    * `R"raw string"`
    * `F"format string"`
    * 多行字符串
        > 利用`\`可以转义删除行尾的换行符
    ```python
    """multiline
    string"""

    '''
    multiline
    string
    '''
    ```

```python
str.count(sub)
str.encode(encoding="utf-8")
str.startswith(preffix)
str.endswith(suffix)
str.expandtabs(tabsize=8)
str.find(sub)
str.rfind(sub)
str.index(sub)      # 触发ValueError
str.rindex(sub)     # 触发ValueError

str.isalpha()       # Unicode Letter
str.isdecimal()     # Unicode Decimal
str.isdigit()       # Unicode Decimal,Digit
str.isnumeric()     # Unicode Decimal,Digit,Numeric
str.isalnum()       # 若以上四个方法均返回True则返回True
str.isspace()       # Unicode Seperator,Space
str.isascii()       # ASCII
str.isprintable()   # 是否可打印
str.isidentifier()  # 是否为保留标识符
str.istitle()       # 单词开头字符若区分大小写则为大写，其余字符若区分大小写则为小写
str.isupper()       # 至少一个区分大小写字符且为大写
str.islower()       # 至少一个区分大小写字符且为小写
str.title()
str.upper()
str.lower()
str.swapcase()

str.lstrip([chars]) # 移除开头出现在chars中的字符，默认空白符
str.rstrip([chars]) # 移除结尾出现在chars中的字符，默认空白符
str.strip([chars])  # 移除两边出现在chars中的字符，默认空白符
str.removeprefix(p)
str.removesuffix(s)
str.replace(old, new
    [, count])      # 默认替换所有old为new

str.join(itr)       # itr元素必须是str
str.partition(sep)  # 返回三元组（之前、分隔符、之后）
str.rpartition(sep) # 最后一次出现
str.split(sep=None  # 默认sep为空白符，最多分隔maxsplit
    maxsplit=-1)
str.rsplit(sep=None # 从右边开始分割
    maxsplit=-1)
str.splitlines(keepends=False)
```


## 元组
```python
# 元组虽也属于immutable对象，但其元素可以时mutable的，注意这点！
tuple0 = ()
tuple1 = (t1, t2)
tuple2 = t1,
tuple3 = t1, t2
t1, t2, t3 = sequence
(t1, t2, t3) = sequence
```


## 列表
```python
list0 = []
list1 = [resultExp for x in List if condition] # resultList.appen(resltExp)
```


## 集合
```python
set0 = set()
set1 = {0, 1.0, '2'}
set2 = {resultExp for x in List if condition} # resultList.appen(resltExp)

set.add(e)
set.remove(e)   # 若不存在则引发KeyError
set.discard(e)
set.pop()       # 弹出任一元素，为空则引发KeyError
set.clear()
```
* 支持集合论运算：
    `<` `<=` `>` `>=` `|` `&` `-` `^` `|=` `&=` `-=` `^=`


## 字典
```python
dict0 = {}
dict1 = {'str': v1, 0: v2}  # 键为不可变类型，如整数、浮点数、字符串、元组
{keyExp: valueExp for x in List if condition} # resultList.appen(keyExp, valueExp)

dict.get(key[, default])
dict.items()
dict.keys()
dict.values()
dict1 | dict2
```


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


# 表达式
```python
nonlocal var    # 指明变量var属于上层作用域
global var      # 指明变量var属于全局作用域
lambda arg1, arg2: arg1 + arg2
```


# 语句
## 分支
```python
if condition1:
    pass
elif condition2:
    pass
else:
    pass
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
    # 抛出异常链，并将新异常挂到inst后面。from None 表示丢弃之前的旧链
    raise Exception('error2') from inst
# 捕获异常
except Exception:
    # 同上，抛出异常链，并将新异常挂到捕获实例后面
    raise Exception('error3')
# 捕获所有类型的异常
except:
    # 重新抛出捕获的异常
    raise
# 若未捕获到异常则执行else段
else:
    pass
# 无论异常是否被捕获都会执行该段，若异常未被捕获则执行完该段后重新抛出
finally:
    pass
```
* 用户定义的异常类应该派生自`Exception`

# 标准库
## 序列算法
```python
all(itr)                    # 元素全为True
any(itr)                    # 元素存在True
len(seq)                    # 返回元素数量

zip(*itr)                   # 返回链接各组元素后的迭代器
filter(func, itr)           # 过滤出func(e)为True的元素，返回迭代器
map(func, itr)              # 将每个元素经由func处理后装入返回的迭代器中
reversed(Seq)               # 返回反向的迭代器
sorted(itr, *,              # 返回已排序列表
    key=None, reverse=False)# key返回元素比较键，reverse表示反向排序
sum(itr, /, start=0)        # 从第start个元素开始求和

max(itr, *[, key, default]) # 返回范围中最大值。default为默认值（当范围为空时）
max(arg1, arg2, *args[,key])# 返回范围中最大值。key返回元素比较键
min(itr, *[, key, default]) # 返回范围中最小值。default为默认值（当范围为空时）
min(arg1, arg2, *args[,key])# 返回范围中最小值。key返回元素比较键

range(N)                    # 返回列表[0, 1, ..., N-1]
range(B, E, S=1)            # 返回列表[B, B+S, ..., E-S]
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
```

## 语言特性
```python
callable(obj)               # 返回bool表示obj是否可调用
dir(obj)                    # 返回obj的符号表
type(obj)                   # 返回obj的类型
id(obj)                     # 返回对象标识值
isinstance(obj, class)      # 若obj为class的实例或其子类的实例则为True
issubclass(subclass, class) # 若subclass为class的子类则为True
super()                     # 返回当前类的父类（基类）部分
```

## 类型处理
```python
class bool([x])
class int([x])
class float([x])
class str(obj)
class tuple([itr])
class list([itr])
class set([itr])
```

## 输入输出
```python
input([prompt])
print(*obj,
    sep=' ', end='\n'
    file=sys.stdout,
    flush=False)
open(file,                  # 打开失败返回OSError
    mode='r',               # 只读'r'、读写'+'、截断'w'、追加'a'、排它'x'、二进制'b'、文本't'）
    buffering=-1,           # 关闭缓冲0（仅二进制）、行缓冲1（仅文本）、指定缓冲区大小>1
    encoding=None,
    errors=None,
    newline=None,
    closefd=True,
    opener=None)
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

import re
re.findall(regex, string)   # 返回匹配子串的列表
re.sub(regex, repl, string) # 替换，语法类似sed
```


