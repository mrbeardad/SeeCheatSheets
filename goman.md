# 数值处理
## 大数
```go
import "math/big"

func NewInt(x int64) *Int
func NewFloat(x float64) *Float
func ParseFloat(s string, base int, prec uint, mode RoundingMode) (f *Float, b int, err error)
func NewRat(a, b int64) *Rat
```

## 数学
```go
import "math"

// 三角函数
func Cos(x float64) float64
func Sin(x float64) float64
func Sincos(x float64) (sin, cos float64)
func Tan(x float64) float64
func Acos(x float64) float64        // [-π/2, π/2]
func Asin(x float64) float64        // [0,    π]
func Atan(x float64) float64        // [-π/2, π/2]
func Atan2(y, x float64) float64    // [-π,   π]
// 双曲三角函数
func Cosh(x float64) float64
func Sinh(x float64) float64
func Tanh(x float64) float64
func Acosh(x float64) float64
func Asinh(x float64) float64
func Atanh(x float64) float64
// 对数函数
func Log(x float64) float64
func Log1p(x float64) float64
func Log2(x float64) float64
func Log10(x float64) float64
// 指数函数
func Exp(x float64) float64
func Expm1(x float64) float64
func Exp2(x float64) float64
func Pow10(n int) float64
func Pow(x, y float64) float64
// 幂函数
func Sqrt(x float64) float64
func Cbrt(x float64) float64
// 其他函数
func Dim(x, y float64) float64      // max(x - y, 0)
func FMA(x, y, z float64) float64   // x * y + z
func Hypot(p, q float64) float64    // √(x² + y²)
func J0(x float64) float64
func J1(x float64) float64
func Jn(n int, x float64) float64
func Y0(x float64) float64
func Y1(x float64) float64
func Yn(n int, x float64) float64
func Erf(x float64) float64
func Erfc(x float64) float64
func Erfcinv(x float64) float64
func Erfinv(x float64) float64
func Gamma(x float64) float64
func Lgamma(x float64) (lgamma float64, sign int)
// 浮点运算
func Mod(x, y float64) float64      // 商四舍五入后计算余数
func Remainder(x, y float64) float64// 商向零取整后计算余数
func Max(x, y float64) float64      // max(x, y) 若存在NAN则返回NAN
func Min(x, y float64) float64      // min(x, y) 若存在NAN则返回NAN
// 浮点取整
func Ceil(x float64) float64        // 向上取整
func Floor(x float64) float64       // 向下取整
func Trunc(x float64) float64       // 向零取整
func Round(x float64) float64       // 远零取整（四舍五入）
func RoundToEven(x float64) float64 // 向偶取整
// 符号
func Abs(x float64) float64
func Signbit(x float64) bool
func Copysign(x, y float64) float64 // 取x的值和y的符号
// 浮点编码
func Inf(sign int) float64
func IsInf(f float64, sign int) bool
func NaN() float64
func IsNaN(f float64) (is bool)
func Modf(f float64) (int float64, frac float64)
func Frexp(f float64) (frac float64, exp int)
func Ldexp(frac float64, exp int) float64
func Logb(x float64) float64
func Ilogb(x float64) int
func Nextafter(from, to float64) (r float64)
func Nextafter32(from, to float32) (r float32)
func Float32bits(f float32) uint32
func Float32frombits(b uint32) float32
func Float64bits(f float64) uint64
func Float64frombits(b uint64) float64
```

## 随机数
```go
import "math/rand"

func Seed(seed int64)       // 做种默认随机数生成器

func Int() int              // [0, max]
func Int31() int32
func Uint32() uint32
func Int63() int64
func Uint64() uint64
func Intn(n int) int        // [0, n)
func Int31n(n int32) int32
func Int63n(n int64) int64
func Float32() float32      // [0.0, 1.0)
func Float64() float64
func ExpFloat64() float64   // 指数分布
func NormFloat64() float64  // 正态分布

func Perm(n int) []int
func Read(p []byte) (n int, err error)
func Shuffle(n int, swap func(i, j int))    // i为索引[1, n]，j为随机数[0, i]
```

## 时间库
```go
import "time"

// Time计算时使用monotonic clock，其他操作使用wall clock
func Now() Time
func Unix(sec int64, nsec int64) Time
func Date(year int, month Month, day, hour, min, sec, nsec int, loc *Location) Time
func Parse(layout, value string) (Time, error)
func ParseInLocation(layout, value string, loc *Location) (Time, error)
// 解析格式时间layout: Mon Jan 2 15:04:05 MST 2006
func (t Time) Format(layout string) string
func (t Time) Date() (year int, month Month, day int)
func (t Time) Year() int
func (t Time) Month() Month
func (t Time) Day() int
func (t Time) Clock() (hour, min, sec int)
func (t Time) Hour() int
func (t Time) Minute() int
func (t Time) Second() int
func (t Time) Nanosecond() int
func (t Time) YearDay() int
func (t Time) Weekday() Weekday
func (t Time) Unix() int64
func (t Time) UnixNano() int64

func (t Time) Zone() (name string, offset int)
func (t Time) Location() *Location
func (t Time) Local() Time
func (t Time) UTC() Time
func (t Time) In(loc *Location) Time

func (t Time) Add(d Duration) Time
func (t Time) AddDate(years int, months int, days int) Time
func (t Time) Sub(u Time) Duration
func (t Time) Truncate(d Duration) Time
func (t Time) Before(u Time) bool
func (t Time) After(u Time) bool
func (t Time) Equal(u Time) bool
func (t Time) IsZero() bool

func NewTicker(d Duration) *Ticker
func (t *Ticker) Reset(d Duration)
func (t *Ticker) Stop()
func AfterFunc(d Duration, f func()) *Timer
func NewTimer(d Duration) *Timer
func (t *Timer) Reset(d Duration) bool
func (t *Timer) Stop() bool
func Sleep(d Duration)
```

# 字符处理
## 字符串
```go
import "bytes"
import "strings"

// 比较
func Compare(a, b string) int
func EqualFold(s, t string) bool    // 忽略Unicode大小写
// 查询
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
// 切分与连接
func Fields(s string) []string                  // 连续空白符切分
func FieldsFunc(s string, f func(rune) bool) []string
func Split(s, sep string) []string
func SplitN(s, sep string, n int) []string      // 返回切片长度最大为N
func SplitAfter(s, sep string) []string
func SplitAfterN(s, sep string, n int) []string
func Join(elems []string, sep string) string
func Repeat(s string, count int) string
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
// 替换
func Replace(s, old, new string, n int) string
func ReplaceAll(s, old, new string) string
func Map(mapping func(rune) rune, s string) string
func ToValidUTF8(s, replacement string) string  // 将错误的字节替换为replacement
// 大小写转换
func Title(s string) string
func ToTitle(s string) string
func ToTitleSpecial(c unicode.SpecialCase, s string) string
func ToLower(s string) string
func ToLowerSpecial(c unicode.SpecialCase, s string) string
func ToUpper(s string) string
func ToUpperSpecial(c unicode.SpecialCase, s string) string
```

## 字符类
```go
import unicode

var Categories = map[string]*RangeTable{...}
func Is(rangeTab *RangeTable, r rune) bool
func In(r rune, ranges ...*RangeTable) bool
func IsOneOf(ranges []*RangeTable, r rune) bool

func IsDigit(r rune) bool       // ASCII数字
func IsNumber(r rune) bool      // Unicode数字
func IsLetter(r rune) bool
func IsUpper(r rune) bool
func IsLower(r rune) bool
func IsTitle(r rune) bool
func IsPunct(r rune) bool
func IsMark(r rune) bool
func IsSymbol(r rune) bool
func IsSpace(r rune) bool
func IsGraphic(r rune) bool     // 包含空白符
func IsPrint(r rune) bool       // 对于空白符仅包含ASCII space
func IsControl(r rune) bool
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

// 模式替换：将src中匹配到的match按fmt替换后追加到dst中，返回新dst
func (re *Regexp) Expand(dst []byte, fmt []byte, src []byte, match []int) []byte
func (re *Regexp) ExpandString(dst []byte, fmt string, src string, match []int) []byte
func (re *Regexp) ReplaceAll(Func|Literal)?(String)?

// 切分
func (re *Regexp) Split(s string, n int) []string
```

## 格式化
```go
import "fmt"

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
<!-- entry begin: go fmt -->
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

## 序列化
### JSON
```go
import "github.com/goccy/go-json"

func Unmarshal(data []byte, v interface{}) error
func Marshal(v interface{}) ([]byte, error)
func MarshalIndent(v interface{}, prefix, indent string) ([]byte, error)
func Indent(dst *bytes.Buffer, src []byte, prefix, indent string) error
func Compact(dst *bytes.Buffer, src []byte) error
func Valid(data []byte) bool

type Json struct {
    String string            `json:"string"`
    Int    int               `json:"integer"`
    Float  float64           `json:"float"`
    Bool   bool              `json:"bool"`
    List   []string          `json:"list"`
    Obj1   map[string]string `json:"obj1"`
    Obj2   nestobj           `json:"obj2"`
}
```

# 数据结构与算法
## 堆
```go
type Interface interface {
    sort.Interface
    Push(x interface{}) // add x as element Len()
    Pop() interface{}   // remove and return element Len() - 1.
}
func Init(h Interface)
func Fix(h Interface, i int)
func Push(h Interface, x interface{})
func Pop(h Interface) interface{}
func Remove(h Interface, i int) interface{}
```


# 文件系统
## 基本IO
```go
import "os"

func Create(name string) (*File, error)                             // 不存在则创建，存在则截断
func CreateTemp(dir, pattern string) (*File, error)                 // dir为空则默认系统临时目录，pattern中最后一个*被替换为random
func Open(name string) (*File, error)                               // 只读模式打开
func OpenFile(name string, flag int, perm FileMode) (*File, error)  // 手动指定打开标识(os.O_*)与默认权限

func (f *File) Name() string
func (f *File) Read(b []byte) (n int, err error)
func (f *File) ReadAt(b []byte, off int64) (n int, err error)
func (f *File) Write(b []byte) (n int, err error)
func (f *File) WriteAt(b []byte, off int64) (n int, err error)
func (f *File) Seek(offset int64, whence int) (ret int64, err error)// whence(os.SEEK_*)
```


# 单元测试
## 断言

## 模拟

## 打桩

# ORM框架：gorm
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
