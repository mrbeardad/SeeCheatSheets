# 系统交互
# 字符处理
## 格式化
```go
import . "fmt"

// 若两个操作数均非字符串，则中间添加空格
Print(args...)                  (nbytes, err)
Fprint(iow, args...)            (nbytes, err)
Sprint(args...)                 string
// 始终在两操作数中间添加空格，并在末尾添加换行
Println(args...)                (nbytes, err)
Fprintln(iow, args...)          (nbytes, err)
Sprintln(args...)               string
// 按格式化字符操作
Printf(fmt, args...)            (nbytes, err)
Fprintf(iow, fmt, args...)      (nbytes, err)
Sprintf(string, fmt, args...)   string
Errorf(fmt, args...)            error
// 忽略空白符，扫描直到参数读取完毕或EOF
Scan(args...)                   (n, err)
Fscan(ior, args...)             (n, err)
Sscan(string, args...)          string
// 忽略空白符，扫描直到参数读取完毕或NL或EOF，若读取完毕时行尾仍有非空白符则算错误
Scanln(args...)                 (n, err)
Sscanln(string, args...)        string
Fscanln(ior, args...)           (n, err)
// fmt中的'\n'匹配`\s*\n`，与'\n'相邻的'\s'匹配'\s*'，不与'\n'相邻的'\s'匹配`\s+`
// 除%c外，其他操作都忽略开头空白符
Scanf(fmt, args...)             (n, err)
Sscanf(fmt, string, args...)    (n, err)
Fscanf(fmt, ior, args...)       (n, err)

"%对齐 符号 # 0 宽度.精度 [索引] 类型"
// +对于结构会打印其字段名，#对于结构会打印其类型与字段名
// 宽度与精度可替换为 [索引]* 从而用参数值替换
```
| 格式  | 描述                 | 支持类型             |
|-------|----------------------|----------------------|
| %v    | 打印值               | all                  |
| %T    | 打印类型             | all                  |
| %t    | 打印布尔值           | bool                 |
| %b    | 打印二进制           | 整数、浮点数         |
| %o    | 打印八进制           | 整数                 |
| %O    | 打印八进制带前缀0o   | 整数                 |
| %d    | 打印十进制           | 整数                 |
| %x    | 打印十六进制小写字母 | 整数、浮点数、string |
| %X    | 打印十六进制大写字母 | 整数、浮点数、string |
| %e %G | 打印科学计数法e或E   | 浮点数               |
| %f %F | 打印定点数           | 浮点数               |
| %g %G | 打印智能选择         | 浮点数               |
| %c    | 打印码点对应字符     | byte、rune           |
| %U    | 打印码点值           | byte、rune           |
| %s    | 打印字符串           | string               |
| %q    | 打印引用后的字符串   | string               |
| %p    | 打印十六进制指针值   | 指针                 |

