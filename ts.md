# TypeScript

## 依赖模块

```ts
import * as foo from "...";
import { Foo, Bar } from "...";
import defaultExport from "...";
import "...";

export const SOME_CONSTANT = ...
export function someHelpfulFunction()
export class Foo { }
export default ... // Deprecated
```

## 基本类型

### 布尔

```ts
let foo = false;
let bar = true;

bool.toString();
bool.valueOf();
```

## 数字

```ts
let foo = 0b1111 + 0777 + 999 + 0xff;
let bar = 2.5 + 2 + 0.5 + 2e9 + 2.5e9;
let pinf = +Infinity;
let ninf = -Infinity;
let nan = NaN;

Number.MAX_VALUE;
Number.MIN_VALUE;
Number.NaN;
Number.NEGATIVE_INFINITY;
Number.POSITIVE_INFINITY;
Number.EPSILON;
Number.MIN_SAFE_INTEGER;
Number.MAX_SAFE_INTEGER;
Number.parseFloat(string);
Number.parseInt(string);
Number.isFinite(value);
Number.isInteger(value);
Number.isNaN(value);
Number.isSafeInteger(value);

number.toExponential(fractionDigits);
number.toFixed(digits);
number.toPrecision(precision);
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
| u    | unicode 模式                                          |
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
eval();
encodeURI();
decodeURI();
encodeURIComponent();
decodeURIComponent();
```
