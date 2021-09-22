# 变量
```js
/* 全局变量 */
globalVariable = value;

/* deprecated: var声明的变量作用域于整个函数作用域中，包括声明语句之前的部分（undefined） */
var functionVariable = value;

/* let声明的变量作用域仅限于块作用域内 */
let blcokVariable;           // 声明变量，初始值为undefined
let blockVariable = value;   // 声明并初始化变量

/* 声明常量作用域仅限于块作用域内，但无法限制更改数组与对象 */
const constant = value;
```
**注**：JS中整个全局作用域本身就是个巨大的对象(object)

# 表达式
| Operator type          | Individual operators                    |
| ---------------------- | --------------------------------------- |
| member                 | . []                                    |
| call / create instance | () new                                  |
| negation/increment     | ! ~ - + ++ -- typeof void delete        |
| multiply/divide        | * / % **                                |
| addition/subtraction   | + -                                     |
| bitwise shift          | << >> >>>                               |
| relational             | < <= > >= in instanceof                 |
| equality               | == != === !==                           |
| bitwise-and            | &                                       |
| bitwise-xor            | ^                                       |
| bitwise-or             | \|                                      |
| logical-and            | &&                                      |
| logical-or             | \|\|                                    |
| conditional            | ?:                                      |
| assignment             | = += -= *= /= %= <<= >>= >>>= &= ^= \|= |
| comma                  | ,                                       |

**注**：
* `== !=`允许先进行类型转换再比较值，`=== !==`比较包括值和类型
* `>>`算术右移，`>>>`逻辑右移
* 字符串支持`+`连接，与数字连接时会将数字转换为字符串


# 语句
## 分支
```js
if (condition) {

} else if (condition) {

} else {

}

switch (expression) {
    case label:
        break;
    default:
        break;
}
```

## 循环
```js
block:
{
    outerLoop:
    for (initializer; condition; iterator) {
        forinLoop:
        for (const idxOrKey in arrayOrObject) {
            forofLoop:
            for (const element of arrayOrObject) {
                break forinLoop;    // break跳出forinLoop对应的循环体
                break block;        // break还可用于跳出块作用域
                continue outerLoop; // continue仅用于跳过该次循环迭代
            }
        }
    }
}
```

## 异常
```js
try {
    throw;
} catch (identifier) {

} finally {
    return true; // catch语句中的return或throw会被finally中的return覆盖
}
```

# 函数
```js
function AddMethod(x, y = 10) {   // 默认实参
    return (x, y) => x + y; // 函数体多语句需要用花括号包裹
}
```
**注**
* JS中实参列表不必匹配形参列表，参数可多可少，少了则对应形参为`undefined`，多了可通过数组类型的`arguments`访问

# 面向对象
## 封装
```js
// 定义构造器
function Person(name, age) {
    this.name = name;
    this.age = age;
}
Person.prototype.greeting = () => console.log("hello.");

// 创建实例，原型在各实例间共享
let inst = new Person("xhc", 21);
```
this 的值根据调用上下文而定：
* 在方法中，this 表示该方法所属的对象；
* 在函数中，this 表示全局对象（strict模式下为undefined）；
* 如果单独使用，this 表示全局对象；
* 类似 call() 和 apply() 方法可以将 this 引用到任何对象。

new 关键字会进行如下的操作：
1. 创建一个空的简单JavaScript对象（即{}）；
2. 为步骤1新创建的对象添加属性__proto__，将该属性链接至构造函数的原型对象 ；
3. 将步骤1新创建的对象作为this的上下文 ；
4. 如果该函数没有返回对象，则返回this。

## 继承
```js
/* 学你马的垃圾OOJS，直接用框架吧 */
```
* 用你马的null/undefined
* 用你马的this
* 用你马的prototype
* 用你马的var
* 分你马的==/===
* 加你马的;

## 多态

# 类型
* 值类型
    * boolean
    * number
    * string
* 引用类型
    * object
    * array
    * function
* 单例值
    * undefined：特殊类型单例
    * null：object类型单例
    * symbol

## 布尔
```js
let bool1 = true;
let bool0 = false;
```

## 数字
```js
let int = 1;
let float = 2.5;
let inf = Infinity;
let nan = NaN;
```

## 字符串
```js
let str1 = 'single quote'
let str2 = "double quote"
```

## 数组


## 对象
```js
let obj = {
    name: 'xhc',
    age: 21,
} // obj本质就是key类型为string的hashmap
obj.name    // 若key的字符串内容符合标识符规范则可用该语法
obj['age']  // 否则只能使用方括号下标法
```

## 类型转换

| 原始值              | 转换为数字 | 转换为字符串      | 转换为布尔值 |
| ------------------- | ---------- | ----------------- | ------------ |
| false               | 0          | "false"           | false        |
| true                | 1          | "true"            | true         |
| 0                   | 0          | "0"               | false        |
| 1                   | 1          | "1"               | true         |
| NaN                 | NaN        | "NaN"             | false        |
| Infinity            | Infinity   | "Infinity"        | true         |
| -Infinity           | -Infinity  | "-Infinity"       | true         |
| "0"                 | 0          | "0"               | true         |
| "1"                 | 1          | "1"               | true         |
| ""                  | 0          | ""                | false        |
| " "                 | 0          | " "               | true         |
| " 20 "              | 20         | " 20 "            | true         |
| "Runoob"            | NaN        | "Runoob"          | true         |
| [ ]                 | 0          | ""                | true         |
| [20]                | 20         | "20"              | true         |
| [10,20]             | NaN        | "10,20"           | true         |
| ["Runoob"]          | NaN        | "Runoob"          | true         |
| ["Runoob","Google"] | NaN        | "Runoob,Google"   | true         |
| { }                 | NaN        | "[object Object]" | true         |
| function(){}        | NaN        | "function(){}"    | true         |
| null                | 0          | "null"            | false        |
| undefined           | NaN        | "undefined"       | false        |