# 模块
```js
import defaultExport from "module-name";
import * as name from "module-name";
import { export } from "module-name";
import { export as alias } from "module-name";
import { export1 , export2 } from "module-name";
import { foo , bar } from "module-name/path/to/specific/un-exported/file";
import { export1 , export2 as alias2 , [...] } from "module-name";
import defaultExport, { export [ , [...] ] } from "module-name";
import defaultExport, * as name from "module-name";
import "module-name";

// 导出单个特性
export let name1, name2, …, nameN; // also var, const
export let name1 = …, name2 = …, …, nameN; // also var, const
export function FunctionName(){...}
export class ClassName {...}

// 导出列表
export { name1, name2, …, nameN };

// 重命名导出
export { variable1 as name1, variable2 as name2, …, nameN };

// 解构导出并重命名
export const { name1, name2: bar } = o;

// 默认导出
export default expression;
export default function (…) { … } // also class, function*
export default function name1(…) { … } // also class, function*
export { name1 as default, … };

// 导出模块合集
export * from …; // does not set the default export
export * as name1 from …; // Draft ECMAScript® 2O21
export { name1, name2, …, nameN } from …;
export { import1 as name1, import2 as name2, …, nameN } from …;
export { default } from …;
```


# 变量
```js
/* let声明的变量作用域仅限于块作用域内，若不进行初始化则初始值为undefined */
let blcokVariable = value;

/* const声明常量作用域仅限于块作用域内，但无法限制更改数组与对象 */
const constant = value;
```


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
* `&&`与`||`返回结果并不强制转换为bool
* 引用类型的`===`与`==`仅当引用同一数据时才为true


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
        forInLoop:
        for (const idxOrKey in arrayOrObject) {
            forOfLoop:
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
    throw new Error("error message");
} catch (identifier) {
    return false;
} finally {
    return true; // catch语句中的return或throw会被finally中的return覆盖
}
```


# 函数
```js
function AddMethod(x, y = 10, ...z) {   // arguments，默认实参，变参数组
    return x + y + sum(...z);
}

let lambda = (x, y) => {x + y}; // 单参时可省略圆括号，单return语句可省略花括号与return；做闭包时可访问外部this
```


# 面向对象
## 封装
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
* 在方法中，this 表示该方法所属的对象；
* 在函数中，this 表示全局对象（strict模式下为undefined）；
* 如果单独使用，this 表示全局对象；
* 类似 call() 和 apply() 方法可以将 this 引用到任何对象。

new 关键字会进行如下的操作：
1. 创建一个空的简单JavaScript对象（即{}）；
2. 为步骤1新创建的对象添加属性__proto__，将该属性链接至构造函数的原型对象(prototype属性) ；
3. 将步骤1新创建的对象作为this的上下文 ；
4. 如果该函数没有返回对象，则返回this。


## 继承
```js
function Parent (name) {
    this.name = name;
    this.colors = ['red', 'blue', 'green'];
}

Parent.prototype.getName = function () {
    console.log(this.name)
}

function Child (name, age) {
    Parent.call(this, name);
    this.age = age;
}
// 不直接引用Parent.prototype是防止修改Child的prototype时影响到Patent
Child.prototype = Object.create(Patent.prototype);
Child.constructor = Child;

Child.prototype.getName = function () {
    console.log(this.name, this.age)
}
```
**注**：JS中继承关系为对象关系而非类关系，可动态变化


## 多态


# 类型
* 基本类型：是一种既非对象也无方法的数据（不可改变）
    * null
    * undefined
    * symbol
    * boolean
    * bigint
    * number
    * string
* Object：任何 constructed 对象实例的特殊非数据结构类型；JavaScript为基础类型提供了Object包装
    * Object
    * Array
    * Set
    * Map
    * Date

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


## 布尔
```js
let bool = new Boolean(thing) // 注意布尔值的Object包裹转换为布尔值时为true
let bool0 = false
let bool1 = true

bool.toString()
bool.valueOf()
```


## 大数
```js
let bigint = new BigInt(thing)
let bigint1 = 1n

BigInt.asIntN(bits, int)
BigInt.asUintN(bits, int)

bigint.toString()
bigint.valueOf()
```


## 数字
```js
let number = new Number(thing)
let allNumberIsFloat = 0b1111 + 0777 + 999 + 0xff + 2.5 + 2. + .5 + 1e9
let pinf = +Infinity
let ninf = -Infinity
let nan = NaN

Number.MAX_VALUE
Number.MIN_VALUE
Number.NaN
Number.NEGATIVE_INFINITY
Number.POSITIVE_INFINITY
Number.EPSILON
Number.MIN_SAFE_INTEGER
Number.MAX_SAFE_INTEGER
Number.parseFloat(string)
Number.parseInt(string)
Number.isFinite(value)
Number.isInteger(value)
Number.isNaN(value)
Number.isSafeInteger(value)

number.toExponential(fractionDigits)
number.toFixed(digits)
number.toPrecision(precision)
```

## 字符串
```js
let string = new String(thing)
let str1 = 'single quote'
let str2 = "double quote"
let str3 = `${expr}, and remain black charactor`

String.fromCharCode(...charCodes)

string.length
string.charAt(idx)         // 返回string[idx]
string.charCodeAt(idx)     // 返回unicode数字

string.indexOf(searchvalue, start=0)
string.lastIndexOf(searchvalue, start=searchvalue.length-1) // start表示最后一个字符用于匹配searchvalue的beginning
string.startsWith(searchvalue, start=0)
string.endsWith(searchvalue, length=searchvalue.length)

let regex = /pattern/flag
string.search(regexp)       // 返回下标
string.match(regexp)        // 返回数组
string.replace(regexp, newvalue)

string.split(separator="regexp", limit=Infinity)
string.concat(...strings)
string.repeat(count)
string.substr(start, length)
string.substring(start, end)
string.slice(start, end)    // 支持类似python slice

string.toLowerCase()
string.toUpperCase()
string.trim()

JSON.parse(string)
JSON.stringify(object)
```

| flag | 说明                                                  |
| ---- | ----------------------------------------------------- |
| g    | 多次匹配                                              |
| i    | 忽略大小写                                            |
| m    | 多行搜索                                              |
| s    | 允许.匹配换行符                                       |
| u    | unicode模式                                           |
| y    | 执行“粘性(sticky)”搜索,匹配从目标字符串的当前位置开始 |


## 日期时间
```js
let date = new Date(); // 当前时间
let date1 = new Date(number); // Unix时间戳，单位ms
let date2 = new Date(dateString);
let date3 = new Date(year, monthIndex [, day [, hours [, minutes [, seconds [, milliseconds]]]]]);

Date.now()  // 返回当前Unix时间戳，单位ms
Date.parse(string)
Date.UTC(year,month[,date[,hrs[,min[,sec[,ms]]]]]) // 返回返回对应Unix时间戳，单位ms

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

## 数组
```js
let array = new Array(length=1)
let arr1 = [...elements]

let extend = [...array]         // 展开，还可用于函数调用
let [v1, v2, ...rest] = array   // 解构，不必完全解构数组内容

array[idx]          // 若idx超出范围则返回undefined
array[idx] = value  // 若idx超出范围则填充undefined；若idx非整数则作为属性
array.length

Array.from(object)  // object with a length property or any iterable object
Array.isArray(obj)

array.indexOf(searchElement, start=0])
array.lastIndexOf(searchElement, start=searchElement.length-1)
array.find(function(curValue, curIndex=, array=), thisValue=);
array.findIndex(function(curValue, curIndex=, array=), thisValue=);

array.forEach(function(curValue, curIndex=, array=), thisValue=);
array.filter(function(curValue, curIndex=, array=), thisValue=);
array.map(function(curValue, curIndex=, array=), thisValue=);
array.reduce(function(total, curValue=, curIndex=, array=), initialValue);
array.reduceRight(function(total, curValue=, curIndex=, array=), initialValue);
array.every(function(curValue, curIndex=, array=), thisValue=);
array.some(function(curValue, curIndex=, array=), thisValue=);

array.join(deliminator = ',')
array.concat(...arrays)
array.copyWithin(target, start, end)

array.push(...items)
array.pop()
array.unshift(...items)
array.shift()
array.splice(index, count_to_remove, addElement1, addElement2, ...)
array.slice(start_index, upto_index)
array.reverse()
array.sort(compareFunction)
array.fill(value, start, end)

array.keys()
array.entries()
```

## 集合
```js
let set = new Set()
let wset = new WeakSet()    // 对象弱引用集合，key必须为object

let extend = [...set];      // 展开，还可用于函数调用
let [v1, v2, ...rest] = set;// 解构，不必完全解构数组内容

set.size
set.has(value)
set.add(value)
set.delete(value)
set.clear()
set.keys()
set.values()
set.entries()
set.forEach(function(curValue, curIndex=, set=), thisValue=);
```

## 映射
```js
let map = new Map()
let map = new Map([[key, val], [key, val]])
let wmap = new WeakMap()    // 对象弱引用集合，key必须为object

Array.from(map) // [[key, val], [key, val]]

let extend = [...map];          // 展开，还可用于函数调用
let [pair, [key, val]] = map;   // 解构，不必完全解构数组内容

map.size
map.has(key)
map.set(key, val)
map.get(key)        // 若不存在则返回undefined
map.delete(key)

map.keys()
map.values()
map.entries()
map.forEach(function(curValue, curKey=, map=), thisValue=);
```


## 对象
```js
let obj = {
    name: 'xhc',
    age: 21,
} // obj本质就是key类型为string的hashmap

obj.name    // 若key的字符串内容符合标识符规范则可用该语法
obj['age']  // 否则只能使用方括号下标法

let extend = {...obj}       // 展开，一种copy手法
let {k1, k2, ..rest} = obj  // 解构，不必完全解构内容
function({k1, k2})          // 解构，可用于函数调用
let {keyName: val} = obj    // 解构，非规范属性名
let {['keyName']: val} = obj// 同上，先计算keyName

Object.keys(o)
Object.getOwnPropertyNames(o)
```

# 内置函数
```js
eval()
encodeURI()
decodeURI()
encodeURIComponent()
decodeURIComponent()
```