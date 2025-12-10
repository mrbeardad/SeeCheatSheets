# JavaScript

- [JavaScript](#javascript)
  - [依赖模块](#依赖模块)
  - [变量与常量](#变量与常量)
  - [表达式](#表达式)
  - [语句](#语句)
  - [函数](#函数)
  - [面向对象](#面向对象)
    - [封装](#封装)
    - [继承](#继承)
  - [内置类型](#内置类型)
    - [布尔](#布尔)
    - [数字](#数字)
    - [大数](#大数)
    - [字符串](#字符串)
    - [日期时间](#日期时间)
    - [数组](#数组)
    - [对象](#对象)
    - [类库](#类库)
      - [Promise](#promise)
      - [Node Stream](#node-stream)

## 依赖模块

```js
import * as modulename from "path/to/module.js";
import { Foo, Bar } from "path/to/module.js";
import defaultExport from "path/to/module.js";
import "path/to/module.js";

export const SOME_CONSTANT = ...
export function someHelpfulFunction()
export class Foo { }
export default ...
```

- 语义版本号
  - `^`前缀表示冻结第一个不为零的版本号
  - `~`前缀表示冻结主版本与次版本号

## 变量与常量

```js
// 若不进行初始化则初始值为 undefined
let foo = new Bar(args); // 构造
let foo = initializer; // 拷贝
let foo = new Bar(other); // 转换

// 仅限制常量名无法再重新绑定引用（赋值）
const CONST_VALUE = value;
```

- 变量类型：弱类型、引用型
- 生命周期：声明时构造，不再引用后被 GC 回收
- 拷贝控制：浅拷贝

## 表达式

| Operator type          | Individual operators                         |
| ---------------------- | -------------------------------------------- |
| member                 | . ?. []                                      |
| call / create instance | () new                                       |
| negation/increment     | ! ~ - + ++ -- typeof void delete             |
| multiply/divide        | \* / % \*\*                                  |
| addition/subtraction   | + -                                          |
| bitwise shift          | << >> >>>                                    |
| relational             | < <= > >= in instanceof                      |
| equality               | == != === !==                                |
| bitwise-and            | &                                            |
| bitwise-xor            | ^                                            |
| bitwise-or             | \|                                           |
| logical-and            | &&                                           |
| logical-or             | \|\|                                         |
| Nullish Coalescing     | ??                                           |
| conditional            | ?:                                           |
| assignment             | = += -= \*= /= %= <<= >>= >>>= &= ^= \|= ??= |
| comma                  | ,                                            |

- 关系比较会隐式进行类型转换，`===`与`!==`会对类型敏感
- object 仅支持相等性引用比较
- `>>`算术右移，`>>>`逻辑右移
- 字符串支持`+`连接，与数字连接时会将数字转换为字符串
- `&&`返回最后一个转换为 false 的表达式的值，全 true 则返回最后一个表达式的值
- `||`返回最后一个转换为 true 的表达式的值，全 false 则返回最后一个表达式的值

## 语句

- 分支

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

- 循环

```js
block:
{
    outerLoop:
    for (initializer; condition; iterator) {
        forInLoop:
        for (const keyOrIndex in objectOrIterable) {
            forOfLoop:
            for (const element of iterable) {
                break forinLoop;    // break跳出forinLoop对应的循环体
                break block;        // break还可用于跳出块作用域
                continue outerLoop; // continue仅用于跳过该次循环迭代
            }
        }
    }
}
```

- 异常

```js
try {
  throw new Error("error message");
} catch (identifier) {
  return false;
} finally {
  return true; // catch语句中的return或throw会被finally中的return覆盖
}
```

## 函数

```js
function addMethod(x, y = 10, ...z) {
  // arguments，默认实参，变参数组
  return x + y + sum(...z);
}

let lambda = (x, y) => {
  x + y;
}; // 单参时可省略圆括号，单return语句可省略花括号与return；做闭包时可访问外部this
```

## 面向对象

### 封装

```js
// 定义构造器
function Person(name, age) {
  this.name = name;
  this.age = age;
}

// 将方法定义在原型中避免每个实例都创建一个函数副本
Person.prototype.greeting = () => console.log("hello.");

// 创建实例，原型在各实例间共享
let inst = new Person("xhc", 21);
```

this 的值根据调用上下文而定：

- 在方法中，this 表示该方法所属的对象；
- 在函数中，this 表示全局对象（strict 模式下为 undefined）；
- 如果单独使用，this 表示全局对象；
- 类似 call() 和 apply() 方法可以将 this 引用到任何对象。

new 关键字会进行如下的操作：

1. 创建一个空的简单 JavaScript 对象（即{}）；
2. 为步骤 1 新创建的对象添加属性**proto**，将该属性链接至构造函数的原型对象(prototype 属性) ；
3. 将步骤 1 新创建的对象作为 this 的上下文 ；
4. 如果该函数没有返回对象，则返回 this。

### 继承

```js
function Parent(name) {
  this.name = name;
  this.colors = ["red", "blue", "green"];
}

Parent.prototype.getName = function () {
  console.log(this.name);
};

function Child(name, age) {
  Parent.call(this, name);
  this.age = age;
}
// 不直接引用Parent.prototype是防止修改Child的prototype时影响到Patent
Child.prototype = Object.create(Parent.prototype);
Child.constructor = Child;

Child.prototype.getName = function () {
  console.log(this.name, this.age);
};
```

**注**：JS 中继承关系为对象关系而非类关系，可动态变化

## 内置类型

- 基本数据类型
  - undefined
  - null
  - boolean
  - number
  - bigint
  - string
  - symbol
- Object 类型

| 原始值              | 转换为布尔值 | 转换为数字 | 转换为字符串      |
| ------------------- | ------------ | ---------- | ----------------- |
| undefined           | false        | NaN        | "undefined"       |
| null                | false        | 0          | "null"            |
| false               | false        | 0          | "false"           |
| true                | true         | 1          | "true"            |
| 0                   | false        | 0          | "0"               |
| 1                   | true         | 1          | "1"               |
| NaN                 | false        | NaN        | "NaN"             |
| Infinity            | true         | Infinity   | "Infinity"        |
| -Infinity           | true         | -Infinity  | "-Infinity"       |
| ""                  | false        | 0          | ""                |
| " "                 | true         | 0          | " "               |
| " 20 "              | true         | 20         | " 20 "            |
| "Runoob"            | true         | NaN        | "Runoob"          |
| [ ]                 | true         | 0          | ""                |
| [20]                | true         | 20         | "20"              |
| [10,20]             | true         | NaN        | "10,20"           |
| ["Runoob"]          | true         | NaN        | "Runoob"          |
| ["Runoob","Google"] | true         | NaN        | "Runoob,Google"   |
| { }                 | true         | NaN        | "[object Object]" |
| function(){}        | true         | NaN        | "function(){}"    |

### 布尔

```js
let bool = false && true;

// Instance
bool.toString();
```

### 数字

```js
let int = 0b1111 + 0o777 + 99 + 0xff;
let float = 2.5 + 2e9 + 2.5e9;
let pinf = +Infinity;
let ninf = -Infinity;
let nan = NaN;

// Static
Number.MAX_VALUE;
Number.MIN_VALUE;
Number.EPSILON;
Number.MIN_SAFE_INTEGER;
Number.MAX_SAFE_INTEGER;
Number.isInteger(number);
Number.isFinite(number);
Number.isNaN(number);
Number.isSafeInteger(number);
Number.isSafeInteger(number);
Number.parseFloat(string);
Number.parseInt(string, [radix]);

// Instance
number.toExponential([fraction]); // fraction指定小数部分位数，默认精确
number.toFixed([fraction]); // fraction指定小数部分位数，默认为零
number.toPrecision([precision]); // precision指定有效位数，默认精确
number.toString([radix]); // radix指定基数，默认为10
```

### 大数

```js
let bigint = 0b1111 + 0o777 + 99 + 0xff;

// Static
BigInt.asIntN(bits, bigint);
BigInt.asUintN(bits, bigint);

// Instance
bigint.toString();
```

### 字符串

```js
let string = 'single "quote"' + "double 'quote'" + `
raw string with ${expr}
`;

let regexp = /pattern/flag

// Static
String.fromCodePoint()

// Instance
string.length
string.charAt(idx)                // 返回string[idx]，识别 utf8
string.charCodeAt(idx)            // 返回string[idx]对应码点
string.slice(start, end)          // 支持负数下标
string.substring(start, end)

string.includes(search[, from])   // 默认from为0
string.indexOf(search[, from])    // 默认from为0
string.lastIndexOf(search[, from])// 默认from为尾部
string.search(regexp)             // 返回下标，不完全匹配
string.match(regexp)              // 返回数组，不完全匹配
string.startsWith(search[, pos])
string.endsWith(search[, length])

string.repeat(count)
string.concat(...strings)
string.split([regexp [, limit]]) // 默认返回数组[string]，regex为""时切分每个字符

string.replace(regexp, replace)  // $`, $', $&, $1, $<name>
string.toLowerCase()
string.toUpperCase()
String.padStart(targetLength [, padString])
String.padEnd(targetLength [, padString])
string.trim()
string.trimStart()
string.trimEnd()

JSON.parse(string)
JSON.stringify(object)
```

| flag | 说明                                      |
| ---- | ----------------------------------------- |
| g    | 多次匹配                                  |
| i    | 忽略大小写                                |
| m    | 多行搜索                                  |
| s    | 允许`.`匹配换行符                         |
| d    | 为子匹配生成索引                          |
| y    | 从该 regex 记录的上次匹配位置开始这次匹配 |

### 日期时间

```js
let now = new Date();
let unix = new Date(unix_ms);
let utc = new Date("2006-01-02T15:04:05+08:00");
let datetime = new Date(year, monthIndex [, day [, hours [, minutes [, seconds [, milliseconds]]]]]);

date.getFullYear()      //Returns the year
date.getMonth()         //Returns the month (from 0-11)
date.getDate()          //Returns the day of the month (from 1-31)
date.getDay()           //Returns the day of the week (from 0-6)
date.getHours()         //Returns the hour (from 0-23)
date.getMinutes()       //Returns the minutes (from 0-59)
date.getSeconds()       //Returns the seconds (from 0-59)
date.getMilliseconds()  //Returns the milliseconds (from 0-999)

date.getTime()          //Returns the number of milliseconds since midnight Jan 1 1970, and a specified date
date.getTimezoneOffset()//Returns the time difference between UTC time and local time, in minutes

date.getUTCFullYear()   //Returns the year, according to universal time
date.getUTCMonth()      //Returns the month, according to universal time (from 0-11)
date.getUTCDate()       //Returns the day of the month, according to universal time (from 1-31)
date.getUTCDay()        //Returns the day of the week, according to universal time (from 0-6)
date.getUTCHours()      //Returns the hour, according to universal time (from 0-23)
date.getUTCMinutes()    //Returns the minutes, according to universal time (from 0-59)
date.getUTCSeconds()    //Returns the seconds, according to universal time (from 0-59)
date.getUTCMilliseconds()//Returns the milliseconds, according to universal time (from 0-999)

date.setFullYear()      //Sets the year of a date object
date.setMonth()         //Sets the month of a date object
date.setDate()          //Sets the day of the month of a date object
date.setHours()         //Sets the hour of a date object
date.setMinutes()       //Set the minutes of a date object
date.setSeconds()       //Sets the seconds of a date object
date.setMilliseconds()  //Sets the milliseconds of a date object
date.setTime()          //Sets a date to a specified number of milliseconds after/before January 1, 1970

date.setUTCFullYear()   //Sets the year of a date object, according to universal time
date.setUTCMonth()      //Sets the month of a date object, according to universal time
date.setUTCDate()       //Sets the day of the month of a date object, according to universal time
date.setUTCHours()      //Sets the hour of a date object, according to universal time
date.setUTCMinutes()    //Set the minutes of a date object, according to universal time
date.setUTCSeconds()    //Set the seconds of a date object, according to universal time
date.setUTCMilliseconds()//Sets the milliseconds of a date object, according to universal time

date.toString()         //Converts a Date object to a string
date.toDateString()     //Converts the date portion of a Date object into a readable string
date.toTimeString()     //Converts the time portion of a Date object to a string
date.toUTCString()      //Converts a Date object to a string, according to universal time
date.toISOString()      //Returns the date as a string, using the ISO standard
date.toJSON()           //Returns the date as a string, formatted as a JSON date
date.toLocaleDateString()//Returns the date portion of a Date object as a string, using locale conventions
date.toLocaleTimeString()//Returns the time portion of a Date object as a string, using locale conventions
date.toLocaleString()   //Converts a Date object to a string, using locale conventions
```

### 数组

```js
let array = [...elements]

let extend = [begin, ...array, end] // 展开
let [e0, , e2, ...rest] = array     // 解构

// 注意：若 idx 不是自然数，则被当做 Object 键
array[idx]              // 若idx超出范围则返回undefined
array[idx] = value      // 若idx超出范围则填充undefined
array.length
array.at(index)         // 相对array[index]额外支持负数下标
array.slice(start, end) // 支持负数下标
array.push(...items)    // 尾部添加
array.pop()             // 尾部删除
array.unshift(...items) // 头部添加
array.shift()           // 头部删除

array.includes(search[, from]])
array.indexOf(search[, from]])
array.lastIndexOf(search[, from])
array.find(function(curValue[, curIndex[, array]]));      // 返回元素
array.findIndex(function(curValue[, curIndex, [array]])); // 返回下标
array.forEach(function(curValue[, curIndex[, array]]));
array.every(function(curValue[, curIndex[, array]]));
array.some(function(curValue[, curIndex[, array]]));
array.keys()
array.entries()

array.reduce(function(total[, curValue[, curIndex]]), initVal);
array.reduceRight(function(total[, curValue[, curIndex]]), initVal);

array.sort(compareFunction)
array.reverse()

array.join(deliminator = ',')
array.concat(...arrays)

array.copyWithin(pos[, start[, end]])
array.flat(depth=1)
array.fill(value[, start[, end]])
array.filter(function(curValue[, curIndex, array]]));
array.map(function(curValue, curIndex=, array=), thisValue=);
array.splice(index, count_to_remove, ...addElements)
```

### 对象

```js
// 构造：key 类型仅限 String 与 Symbol，其他类型都会隐式转换为 String
let obj = {
  key_name: "canonical",
  "key-name": "non-canonical",
  1: "id",
  value,
  method() {}
}

let extend = {key: 1, ...obj, key: 2}     // 扩展
let {key, ["key"]: alias, ...rest} = obj  // 解构
function f({key = defaultValue})          // 解构实参

obj.key_name     // 成员式
obj['key-name']  // 下标式
obj.key = value
delete obj.key

Object.is(lhs, rhs) // 与 === 区别在于，is(NaN, NaN)与is(-0, +0)均为true
Object.keys(obj)
Object.entries(obj)
Object.fromEntries(iterable);
Object.assign(target, ...sources)
```

### 类库

#### Promise

1. `new Promise((resolve, reject)=> { })` executor 函数在构造 Promise 时同步执行，内部抛异常被视作 reject
2. `then(()=> { })` then/catch/finally callback 内部抛异常被视作 reject
3. `resolve()` resolve/reject 内部异步执行 then/catch/finally callback
4. `async () => { }` async 函数内部直到第一次await 之前的代码被视作 executor
5. `await p` await 之间的代码被视作 then/catch/finally callbak
6. 默认设置下 `--unhandled-rejections=throw`，unhandled reject 在一下情况会触发 `uncaughtException` 而非 `unhandledRejection`
   1. 未监听 `unhandledRejection`
   2. when a rejection happens during the command line entry point's ES module static loading phase
7. 监听 `uncaughtException` 会覆盖默认行为—— exit

#### Node Stream

- Reading Mode
  - `readable.readableFlowing === null`
    - Init Mode，Buffer 中无数据，也不会自动读取数据
  - `readable.readableFlowing === false`
    - Pause Mode，自动读取数据填满 Buffer，使用 `readable.read()` 读取数据
    - 状态转移：
      - `'readable'`
      - `readable.pause()`
      - `readable.unpipe()`，无法从 Init Mode 转换
  - `readable.readableFlowing === true`
    - Flowing Mode，自动读取数据填满 Buffer，通过 `'data'` 事件读取数据
    - 状态转移：
      - `'data'`，移除 listener 不会变为 Flowing mode
      - `readable.pipe()`
      - `readable.resume()`

> Flowing Mode 转为 Pause Mode 并不稳定

- [`Readable`](https://nodejs.org/docs/latest/api/stream.html#implementing-a-readable-stream)
  - `read(size)`：Pause Mode 中读取小于等于 `size` 的数据或 `null`
  - `_read()`：实现读取数据，通常调用 `push()`
  - `push()`：将数据放入缓冲区
    - `push(buffer)`
    - `push(string, encoding)`，encoding 默认 `'utf-8'`
    - `push(null)` 表示 `'end'`
  - `encoding`：默认读取 Buffer，若指定 `encoding` 则读取 String（将 Buffer 按 encoding 编码）
  
```js
function stringToStream(text) {
  let sent = false;

  return new Readable({
    read() {
      if (!sent) {
        this.push(text); // push the string
        sent = true;
      } else {
        this.push(null); // signal end of stream
      }
    },
  });
}
```

- [`Writable`](https://nodejs.org/docs/latest/api/stream.html#implementing-a-writable-stream)
  - `write()`：
    - 和 `readable.push()` 类似
    - 返回 `false` 表示缓冲区达到了 `highWaterMark`，需要等下次 `'drain'` 再写入
    - `highWaterMark` 限制字节数、字符数或对象数
