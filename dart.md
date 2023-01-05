# 目录

## 依赖模块

```dart
// Importing core libraries
import 'dart:math';

// Importing libraries from external packages
import 'package:lib1/lib1.dart';
import 'package:lib2/lib2.dart' as lib2;

// Only dart compile js supports deferred loading
import 'package:greetings/hello.dart' deferred as hello;

// Import only foo.
import 'package:lib1/lib1.dart' show foo;
// Import all names EXCEPT foo.
import 'package:lib2/lib2.dart' hide foo;

// Importing files
import 'path/to/my_other_file.dart';
```

## 变量与常量

```dart
var foo = Bar();        // 默认
var foo = Bar(args);    // 构造
var foo = initializer;  // 拷贝
var foo = other as Bar; // 转换

final map = {if (i is int) i: 'int'};         // final 不可改变变量本身，但可以改变其字段内容
const set = {if (list is List<int>) ...list}; // const 两者都不可改变
enum Color { red, green, blue }               // 支持定义拥有字段的enum类
```

## 语句

- 分支

  ```dart
  // if else
  if (condition) {
    statement;
  } else {
    statement;
  }

  // switch
  switch (comparable) {
    case constant1:
      continue fallthrough;
    fallthrough:
    case constant2:
      statement;
      break;
    default:
      statement;
  }
  ```

- 循环

  ```dart
  // for
  for (declaration; condition; expression) {
    statement;
  }

  // for in
  for (final elem in iterabal) {
    statement;
  }
  ```

- 异常

```dart
try {
  breedMoreLlamas();
} on OutOfLlamasException {
  // A specific exception
  buyMoreLlamas();
} on Exception catch (e) {
  // Anything else that is an exception
  print('Unknown exception: $e');
} catch (e) {
  // No specified type, handles all
  print('Something really unknown: $e');
} catch (e, s) {
  print('Exception details:\n $e');
  print('Stack trace:\n $s');
  rethrow;
} finally {
  // Always clean up, even if an exception is thrown.
  cleanLlamaStalls();
}
```

## 函数

```dart

String fn({required String r1, required String? r2, String o1 = "optional", String? o2}) {...}

String fn(String r1, String? r2 [String o1 = "optional", String? o2]) {...}

(arg) {return ...}

(arg) => expression
```

## 面向对象

### 封装

```dart
class MyClass {
  String _name;

  // constructor likes function but no return type
  MyClass(String name) {
    this._name = name;
  }

  // named constructor beacase no funtion overload
  MyClass.named(String name) {
    this._name = name;
  }

  // using this in constructor, a syntax sugar
  MyClass.useThis(this._name);

  // initializer lists: final fields must have values before the constructor body executes
  MyClass.initList(String name) : _name = name;

  // redirect to another constructor
  MyClass.redirecting1(String name) : this(name);
  MyClass.redirecting2(String name) : this.named(name);

  @override
  bool operator ==(Object other) => other is MyClass && other._name = name;

  String get name => name;
  set name(String value) => name = value;
}
```

## 内置类型

```dart
var b = true && false;

var i = 10 + 0xFF;

var d = 1.1 + 1.1e5;

var s = 'with " ' + "with ' " + "both with ${name}" + r"with raw without any \e and $name";
var s1 = '''
You can create
multi-line strings like this one.
''';
var s2 = """This is also a
multi-line string.""";

var list = [1, 2, 3];
var list2 = [0, ...list];

var set1 = {'fluorine', 'chlorine', 'bromine', 'iodine', 'astatine'};
var set2 = <String>{};
```
