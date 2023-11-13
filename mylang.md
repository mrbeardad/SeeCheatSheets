# MyLang

- [MyLang](#mylang)
  - [依赖](#依赖)
  - [变量](#变量)
  - [枚举](#枚举)
  - [运算符](#运算符)
  - [控制流](#控制流)
  - [函数](#函数)
  - [面向对象](#面向对象)
    - [封装](#封装)

## 依赖

- 程序入口：`src/main.rs` 或 `src/bin/*.rs` 中的 main 函数
- 依赖单元：一个源文件（项目内部依赖才需手动导入，外部依赖由 Cargo.toml 导入）
- 依赖导入：符号限定于模块路径
- 依赖导出：`pub`声明
- ~~依赖初始化~~
- 依赖管理：cargo

```rust
pub mod fileordir;

use create::fileordir::foo;
use fileordir::{self, foo, bar};
use super::*;
```

## 变量

- 变量类型：引用型强类型
- 作用域：块作用域`{}`
- 所有权：一个实例可以有多个所有者
- 生命周期：引用计数为零时销毁

```rust
let foo = other;
let mut foo = other;

let foo: Type = other;
let mut foo: Type = other;
```

## 枚举

```rust
enum Enum {
  ZERO = 0,
  ONE,
  TWO,
}

let e: Enum = Enum::ZERO;
```

## 运算符

- 单后：`::`, `.`, `[]`, `()`
- 单前：`*`, `+`, `-`
- 算数：`**`, `*`, `/`, `%`, `+`, `-`
- 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`, `is`
- 逻辑：`not`, `and`, `or`
- 赋值：`=`
- 其他：`!`, `?`

## 控制流

- 分支

  ```rust
  // branches
  if condition {
    // ...
  } else {
    // ...
  }

  // two branches for single expression
  if let pattern = expr {
    // ...
  } else {
    // ...
  }

  // multiple branches for single expression
  match expr {
    pattern => {...},
    pattern => expr,
    _ => expr
  }
  ```

- 循环

  ```rust
  // infinite loop
  loop {
    // ...
  }

  // loop with condition
  while condition {
    // ...
  }

  // loop with pattern
  while let pattern = expr {
    // ...
  }

  // loop for iterator
  for elem in iterator {
    // ...
  }

  // loop label
  label:
  continue;
  continue label;
  break;
  break label;
  break expr;
  ```

## 函数

- 默认实参
- ~~函数重载~~
- 泛型
- ~~参数包~~

```rust
fn return_nothing() {
  ()
}

fn normal_function(
    a1: int,
    a2: uint = 1,
    {
      a3: mut str,
      a4: str = "default",
    }) -> char {
  // ...
}

let closure_v1 = (x) { x + 1 };
let closure_v2 = (x: int) -> int { x + 1 };
```

## 面向对象

### 封装

- 访问控制：`pub`
- 构造控制：一般通过工厂函数构造实例
- 析构控制：`Drop`
- 拷贝控制：`Clone`
- 比较操作：`Eq`, `Ord`, `PartialEq`, `PartialOrd`（比较字段）
- 语言扩展：通过 trait 实现
