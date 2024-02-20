# RUST

- [RUST](#rust)
  - [项目管理](#项目管理)
  - [依赖](#依赖)
  - [变量](#变量)
  - [Triats](#triats)

## 项目管理

```txt
.
├── Cargo.lock
├── Cargo.toml
├── src/
│   ├── lib.rs
│   ├── main.rs
│   └── bin/
│       ├── named-executable.rs
│       ├── another-executable.rs
│       └── multi-file-executable/
│           ├── main.rs
│           └── some_module.rs
├── benches/
│   ├── large-input.rs
│   └── multi-file-bench/
│       ├── main.rs
│       └── bench_module.rs
├── examples/
│   ├── simple.rs
│   └── multi-file-example/
│       ├── main.rs
│       └── ex_module.rs
└── tests/
    ├── some-integration-tests.rs
    └── multi-file-test/
        ├── main.rs
        └── test_module.rs
```

通过修改 Cargo.toml 来调整依赖

```txt
1.2.3  :=  >=1.2.3, <2.0.0
1.2    :=  >=1.2.0, <2.0.0
1      :=  >=1.0.0, <2.0.0
0.2.3  :=  >=0.2.3, <0.3.0
0.2    :=  >=0.2.0, <0.3.0
0.0.3  :=  >=0.0.3, <0.0.4
0.0    :=  >=0.0.0, <0.1.0
0      :=  >=0.0.0, <1.0.0

^1.2.3 :=  ==1.2.3

~1.2.3 :=  >=1.2.3, <1.3.0
~1.2   :=  >=1.2.0, <1.3.0
~1     :=  >=1.0.0, <2.0.0

*      :=  >=0.0.0
1.*    :=  >=1.0.0, <2.0.0
1.2.*  :=  >=1.2.0, <1.3.0
```

```sh
# 新建项目
cargo new pkg_name

# 构建项目，自动下载依赖
cargo build [--release]

# 运行程序，自动构建项目
cargo run [--bin name | --example name]

# 测试程序，自动构建项目
cargo test [test_name]

# 更新依赖
cargo update [-p pkg]

# 下载、编译并安装二进制可执行文件
cargo install pkg
```

## 依赖

- 包内部的依赖由`mod`语句导入
- 包外部的依赖由`Cargo.toml`导入
- 一个包可以包含至多一个 lib crate(`src/lib.rs`)和任意数量 bin crate(`src/main.rs`, `src/bin/name.rs`)

## 变量

- `mut`对于变量，指示其非引用字段可更改
- `mut`对于引用，指示其引用的变量可更改
- `mut`引用可转换为`immut`引用
- 一个变量只能同时存在一个`mut`引用或若干个`immut`引用之一，且当变量被借用时无法对其赋值
- 编译期借用检查器保证引用在使用时必须有效（主要是从外部获取的引用）
- 引用的目的：用于函数的参数或返回值
  - 不可变引用：避免拷贝或移动
  - 可变引用：修改原值

## Triats

- Sized: 类型大小编译期可知，所有类型参数默认绑定 Sized，可以用`?Sized`语法取消默认绑定（对于`[i32]`, `dyn Base`）
- Copy: 标志该类型为平凡拷贝，否则在赋值、传参等上下文中进行移动
- Clone: 实现`clone`方法来显示拷贝实例（浅拷贝或深拷贝），Rust 隐式拷贝操作仅支持拷贝栈结构，且未实现 Copy 的类型无法隐式拷贝而只能移动
- Drop: 实现`drop`方法来代替析构函数，销毁变量时递归调用
- Deref/DerefMut: 实现`deref`方法来隐式解引用、隐式实现目标类型方法、强制转换引用类型
- Fn: 标志该函数可重复调用而不改变状态（无副作用）
- FnMut: 标志该函数可重复调用但可能改变状态（有副作用）
- FnOnce: 标志该函数只能调用一次
- Sync: 标志该类型可在多线程中安全共享（针对一个所有者和多个引用）
- Send: 标志该类型可在多线程中安全移动所有权（针对多个所有者）
