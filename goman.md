# 字符处理
## 字符串
```go
import "strings"
import "bytes"

// 比较
func Compare(a, b string) int
func EqualFold(s, t string) bool    // 忽略Unicode大小写
// 搜索
func Count(s, substr string) int
func Contains(s, substr string) bool
func ContainsRune(s string, r rune) bool
func ContainsAny(s, chars string) bool
func Index(s, substr string) int
func IndexByte(s string, c byte) int
func IndexRune(s string, r rune) int
func IndexFunc(s string, f func(rune) bool) int
func IndexAny(s, chars string) int
func LastIndex(s, substr string) int
func LastIndexByte(s string, c byte) int
func LastIndexAny(s, chars string) int
func LastIndexFunc(s string, f func(rune) bool) int
func HasPrefix(s, prefix string) bool
func HasSuffix(s, suffix string) bool
// 切分，忽略空白符
func Fields(s string) []string
func FieldsFunc(s string, f func(rune) bool) []string
// 切分
func Split(s, sep string) []string
func SplitN(s, sep string, n int) []string      // 返回切片长度最大为N
func SplitAfter(s, sep string) []string
func SplitAfterN(s, sep string, n int) []string
// 连接
func Join(elems []string, sep string) string
// 重复
func Repeat(s string, count int) string
// 替换
func Replace(s, old, new string, n int) string
func ReplaceAll(s, old, new string) string
func ToValidUTF8(s, replacement string) string  // 将错误的字节替换为replacement
func Map(mapping func(rune) rune, s string) string
// 大小写转换
func Title(s string) string
func ToTitle(s string) string
func ToTitleSpecial(c unicode.SpecialCase, s string) string
func ToLower(s string) string
func ToLowerSpecial(c unicode.SpecialCase, s string) string
func ToUpper(s string) string
func ToUpperSpecial(c unicode.SpecialCase, s string) string
// 删除
func Trim(s, cutset string) string
func TrimSpace(s string) string
func TrimFunc(s string, f func(rune) bool) string
func TrimLeft(s, cutset string) string
func TrimLeftFunc(s string, f func(rune) bool) string
func TrimRight(s, cutset string) string
func TrimRightFunc(s string, f func(rune) bool) string
func TrimPrefix(s, prefix string) string
func TrimSuffix(s, suffix string) string
```

## 正则表达式
```go
import "regexp"
// 构造Regexp
func Compile(expr string) (*Regexp, error)
func MustCompile(str string) *Regexp
func QuoteMeta(s string) string

// 查看
func (re *Regexp) String() string
func (re *Regexp) NumSubexp() int
func (re *Regexp) SubexpNames() []string
func (re *Regexp) SubexpIndex(name string) int

// 模式匹配：默认匹配到第一个就结束，搜索文本（参数）与匹配（返回）类型为byte[]，
// All      表示返回切片，元素为多次匹配
// Submatch 表示返回切片，元素为子匹配
// Index    表示用int[]代替byte[]返回
// String   表示用string代替byte[]参数与返回
// Submatch与Index结合只会使返回值增加一维而非两维
func (re *Regexp) Find(All)?(String)?(Submatch)?(Index)?
func (re *Regexp) Match(b []byte) bool              // 匹配子串，全匹配利用^$
func (re *Regexp) MatchString(s string) bool
func (re *Regexp) MatchReader(r io.RuneReader) bool

// 模式替换
// 将src中匹配到的match按fmt替换后追加到dst中，返回新dst
func (re *Regexp) Expand(dst []byte, fmt []byte, src []byte, match []int) []byte
func (re *Regexp) ExpandString(dst []byte, fmt string, src string, match []int) []byte
func (re *Regexp) ReplaceAll(Func|Literal)?(String)?

// 切分
func (re *Regexp) Split(s string, n int) []string
```

## 格式化
<!-- entry begin: go fmt -->
```go
import . "fmt"

// 若两个操作数均非字符串，则中间添加空格
func Print(a ...interface{}) (n int, err error)
func Fprint(w io.Writer, a ...interface{}) (n int, err error)
func Sprint(a ...interface{}) string
// 始终在两操作数中间添加空格，并在末尾添加换行
func Println(a ...interface{}) (n int, err error)
func Fprintln(w io.Writer, a ...interface{}) (n int, err error)
func Sprintln(a ...interface{}) string
// 按格式化字符操作
func Printf(format string, a ...interface{}) (n int, err error)
func Fprintf(w io.Writer, format string, a ...interface{}) (n int, err error)
func Sprintf(format string, a ...interface{}) string
func Errorf(format string, a ...interface{}) error
// 忽略空白符，扫描直到参数读取完毕或EOF
func Scan(a ...interface{}) (n int, err error)
func Fscan(r io.Reader, a ...interface{}) (n int, err error)
func Sscan(str string, a ...interface{}) (n int, err error)
// 忽略空白符，扫描直到参数读取完毕或NL或EOF，若读取完毕时行尾仍有非空白符则算错误
func Scanln(a ...interface{}) (n int, err error)
func Fscanln(r io.Reader, a ...interface{}) (n int, err error)
func Sscanln(str string, a ...interface{}) (n int, err error)
// fmt中的'\n'匹配`\s*\n`，与'\n'相邻的'\s'匹配'\s*'，不与'\n'相邻的'\s'匹配`\s+`，其它字符都必须精准匹配
// 除%c外，其他操作都忽略开头空白符
func Scanf(format string, a ...interface{}) (n int, err error)
func Fscanf(r io.Reader, format string, a ...interface{}) (n int, err error)
func Sscanf(str string, format string, a ...interface{}) (n int, err error)

// +对于结构会打印其字段名，#对于结构会打印其类型与字段名
// 宽度与精度可替换为 [索引]* 从而用参数值替换
"%对齐 符号 # 0 宽度.精度 [索引] 类型"
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
| %e %E | 打印科学计数法e或E   | 浮点数               |
| %f %F | 打印定点数           | 浮点数               |
| %g %G | 打印智能选择         | 浮点数               |
| %c    | 打印码点对应字符     | byte、rune           |
| %U    | 打印码点值           | byte、rune           |
| %s    | 打印字符串           | string               |
| %q    | 打印引用后的字符串   | string               |
| %p    | 打印十六进制指针值   | 指针                 |
<!-- entry end -->

# 单元测试
## 断言

## 模拟

## 打桩

# Web框架：Gin
* 定义struct用来读/写时与表中行记录对接
    * 字段名`SnakeCases` => 列名`snake_cases`
    * 类型：
        * `整数     <=> 整数`
        * `浮点数   <=> 浮点数`
        * `string   <=> 字符`
        * `[]byte   <=> 字节`
        * `time.Time<=> 日期时间`
        * `string   <=> SQL类型`
    * 标签：
        * `gorm:"column:name"`指定字段对应列名
        * `gorm:"not null"`指定字段对应列名
        * `gorm:"embedded;embeddedPrefix:prefix_"`提升嵌入字段的字段
