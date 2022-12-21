# 目录

## 依赖模块

```dart
// Importing core libraries
import 'dart:math';

// Importing libraries from external packages
import 'package:test/test.dart';

// Importing files
import 'path/to/my_other_file.dart';
```

## 变量与常量

```dart

```

## 语句

- **异常**

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

(arg) => expression;
```

## 面向对象

### 封装

```dart
class Point {
  double x = 0;
  double y = 0;

  Point(double x, double y) {
    // See initializing formal parameters for a better way
    // to initialize instance variables.
    this.x = x;
    this.y = y;
  }

  // Sets the x and y instance variables
  // before the constructor body runs.
  Point(this.x, this.y);

  // Named constructor
  Point.origin()
      : x = xOrigin,
        y = yOrigin;
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
