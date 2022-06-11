# ReScript

## 依赖模块

- `.res`文件即模块，也可使用关键字`module`
- `.resi`文件即模块接口，也可用关键字`module type`
- 文件名使用大驼峰命名
- 依赖导出：`.res`默认导出所有符号，若存在对应`.resi`则仅导出其中声明符号
- 依赖导入：无需导入直接引用，符号限定与文件名

```ocaml
// 导入JS符号
@module("./student") external studentName: string = "default"
@module("path") external dirname: string => string = "dirname"
```

## 变量绑定

```ocaml
let bind1 = 1
let bind2: Type = val
```

- 变量类型：引用型强类型
- 赋值拷贝：浅拷贝（值一般不可更改）
- 生命周期：退出作用域销毁
- 作用域：块作用域外不可见

## 表达式

- 算数：`*`, `/`, `+`, `-`
- 关系：`<`, `<=`, `>`, `>=`, `==`, `!=`（递归比较）
- 逻辑：`!`, `&&`, `||`
- 赋值：`=`
- 其他：`? :`, `{}`

- 分支表达式：返回值即所选分支块作用域的最后表达式返回值（注意类型一致）

```ocaml
if condition {

} else {

}
```

- 循环表达式：限制块作用域返回值类型为 unit

```ocaml
for i in startValueInclusive to endValueInclusive {
  Js.log(i)
}
for i in startValueInclusive downto endValueInclusive {
  Js.log(i)
}
while testCondition {
  // no break, continue, return
}
```

- 异常表达式：返回值即 try 或 catch 某分支块作用域的最后表达式返回值（注意类型一致）

```ocaml
exception Excep(string)  // exception 可看做特殊的 variant
let result =
  try {
    if condition {
      getVal()
    } else if raiseResExcep {
      raise(Excep("Something goes error"))
    } else {
      Js.Exn.raiseError("Something goes error!")
    }
  } catch {
  | Excep => 0
  | Js.Exn.Error(obj) => ... // catch the JS exception
  }
```

## 内置类型

### 字符串

```res
let s = "single line string"
let m = `multiple line string with ${Interpolation}`
let j = j`same as above, but support non string $Interpolation`
let r = %re("/regex/flag")
```
