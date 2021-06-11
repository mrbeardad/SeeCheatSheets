# 系统交互
## 命令行参数
```go
import "flag"

```

# 字符处理
## 格式化
```go
import . "fmt"

Print(args...)
Sprint(args...)
Fprint(args...)
Println(args...)
Sprintln(args...)
Fprintln(args...)
Printf(string, args...)
Sprintf(string, args...)
Fprintf(string, args...)

"%符号 # 0 宽度.精度 [索引] 类型"   // 宽度与精度可替换为 [索引]* 从而用参数值替换
```
| 格式    | 描述                         |
|---------|------------------------------|
| print() | 打印底层数据结构             |
| %v      | 打印值                       |
| %T      | 打印类型                     |
| %t      | 布尔值                       |
| %b      | 二进制                       |
| %o      | 八进制                       |
| %O      | 八进制带前缀0o               |
| %x      | 十六进制小写字母，包括string |
| %X      | 十六进制大写字母，包括string |
| %e %G   | 科学计数法e E                |
| %f %F   | 定点数                       |
| %g %G   | 智能选择%f或%g %F或%G        |
| %c      | byte与rune对应字符           |
| %U      | byte与rune对应码点           |
| %s      | 字符串                       |
| %q      | 引用字符串                   |
| %p      | 十六进制带前缀0x指针         |

