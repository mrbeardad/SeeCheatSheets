# 数值处理
## 大数
```go
import "math/big"

func NewInt(x int64) *Int
func NewFloat(x float64) *Float
func ParseFloat(s string, base int, prec uint, mode RoundingMode) (f *Float, b int, err error)
func NewRat(a, b int64) *Rat
// 各大数类型提供各种方法来进行运算
```

## 数学库
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
// 双曲函数
func Cosh(x float64) float64
func Sinh(x float64) float64
func Tanh(x float64) float64
func Acosh(x float64) float64
func Asinh(x float64) float64
func Atanh(x float64) float64
// 对数函数
func Log(x float64) float64
func Log1p(x float64) float64       // ln(1+x)
func Log2(x float64) float64
func Log10(x float64) float64
// 指数函数
func Exp(x float64) float64
func Expm1(x float64) float64       // eˣ-1
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
func RoundToEven(x float64) float64 // 向偶取整（非四舍五入）
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
// 时间+时区 可唯一确定一个世界通用时，当输出时的时区改变则时间也会随之改变

// Time计算时使用monotonic clock，其他操作使用wall clock
func Now() Time
func Date(year int, month Month, day, hour, min, sec, nsec int, loc *Location) Time
func Unix(sec int64, nsec int64) Time   // 接受Unix时间戳sec*1e9+nsec
func Parse(layout, value string) (Time, error)
func ParseInLocation(layout, value string, loc *Location) (Time, error)

// 解析格式时间layout: Mon Jan 2 15:04:05 MST 2006
func (t *Time) Format(layout string) string
func (t *Time) Date() (year int, month Month, day int)
func (t *Time) Year() int
func (t *Time) Month() Month
func (t *Time) Day() int
func (t *Time) Weekday() Weekday
func (t *Time) YearDay() int
func (t *Time) Clock() (hour, min, sec int)
func (t *Time) Hour() int
func (t *Time) Minute() int
func (t *Time) Second() int
func (t *Time) Nanosecond() int

// Unix时间戳定义为1970-1-1 00:00:00 UTC开始所流逝的秒数，不受Time的时区影响
func (t *Time) Zone() (name string, offset int) // offset为相对UTC偏移秒数
func (t *Time) Unix() int64                     // 返回秒级Unix时间戳
func (t *Time) UnixNano() int64                 // 同上，但返回ns纳秒级版本
func (t *Time) UTC() Time                       // 转换为UTC时间
func (t *Time) Local() Time                     // 转换为Local时间
func (t *Time) Location() *Location
func (t *Time) In(loc *Location) Time

func (t *Time) Add(d Duration) Time
func (t *Time) AddDate(years int, months int, days int) Time
func (t *Time) Sub(u Time) Duration
func (t *Time) Truncate(d Duration) Time
func (t *Time) Before(u Time) bool
func (t *Time) After(u Time) bool
func (t *Time) Equal(u Time) bool
func (t *Time) IsZero() bool

type Month int
const (
    January Month = 1 + iota
    February
    March
    April
    May
    June
    July
    August
    September
    October
    November
    December
)
func (m Month) String() string

type Weekday int
const (
    Sunday Weekday = iota
    Monday
    Tuesday
    Wednesday
    Thursday
    Friday
    Saturday
)
func (d Weekday) String() string

// Duration单位为nanosecond
func ParseDuration(s string) (Duration, error)  // ns, us, ms, s, m, h
func Since(t Time) Duration
func Until(t Time) Duration
func (d Duration) Hours() float64
func (d Duration) Minutes() float64
func (d Duration) Seconds() float64
func (d Duration) Milliseconds() int64
func (d Duration) Microseconds() int64
func (d Duration) Nanoseconds() int64
func (d Duration) String() string
func (d Duration) Round(m Duration) Duration    // 向零取整
func (d Duration) Truncate(m Duration) Duration // 远零取整

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
// 从JSON转换时忽略stuct中不存在的字段，默认值为零值，若匹配字段的类型不匹配则出错
type Json struct {
    Bool   bool                   `json:"bool"`
    Int    int                    `json:"integer"`
    Float  float64                `json:"float"`
    String string                 `json:"string"`
    List   []string               `json:"list"`
    Obj1   map[string]interface{} `json:"obj1"`
    Obj2   nestobj                `json:"obj2"`
}
```

# 单元测试
## 断言

## 模拟

## 打桩

# 文件系统
## 文件路径
```go
import "path/filepath"

func VolumeName(path string) string                 // 返回卷名。Windows返回类似"C:"，其他返回""
func Dir(path string) string                        // 返回路径名(Clean)。"dir/file"=>"dir", "dir/"=>"dir"
func Base(path string) string                       // 返回文件名(Clean)。"dir/file"=>"file"
func Ext(path string) string                        // 返回扩展名。"f.go"=>".go"
func SplitList(path string) []string                // 返回分离出来的路径名，path中各路径名用冒号:分隔
func Split(path string) (dir, file string)          // 返回dir + file = path。"dir/file"=>"dir/"+"file"
func Rel(basepath, targpath string) (string, error) // 返回相对路径(Clean)
func Join(elem ...string) string                    // 返回合并路径(Clean)

func IsAbs(path string) bool
func IsPathSeparator(c uint8) bool
func FromSlash(path string) string                  // 转换从'/'(Clean)
func ToSlash(path string) string                    // 转换为'/'(Clean)
func Abs(path string) (string, error)               // 返回绝对路径(Clean)
func EvalSymlinks(path string) (string, error)      // 返回真实路径(Clean)
func Clean(path string) string                      // 返回干净路径(Clean)。去除"."与"..", "/.."=>"/", "/"=>"/", ""=>"/"

func Match(pattern, name string) (matched bool, err error)  // 仅语法上进行shell通配符匹配
func Glob(pattern string) (matches []string, err error)     // 返回当前目录下通配符匹配结果

func WalkDir(root string, fn fs.WalkDirFunc) error
type WalkDirFunc func(path string, d DirEntry, err error) error
// WalkDirFunc在打开文件前被调用，
// path路径前缀为root
// err若os.Lstat(path)出错则设置err，若一个目录的Readdirnames方法失败也设置err
// error返回SkipDir表示跳过该目录（或该文件的上级目录），返回其他non-nil则停止
```

## 文件信息
```go
import "os"

func Stat(name string) (FileInfo, error)
func Lstat(name string) (FileInfo, error)
func IsExist(err error) bool
func IsNotExist(err error) bool
func IsPermission(err error) bool
func SameFile(fi1, fi2 FileInfo) bool

func (s FileInfo) Name() string       // base name of the file
func (s FileInfo) Size() int64        // length in bytes for regular files; system-dependent for others
func (s FileInfo) ModTime() time.Time // modification time
func (s FileInfo) IsDir() bool        // abbreviation for Mode().IsDir()
func (s FileInfo) Mode() FileMode     // file mode bits

// FileMode包含了文件类型和权限信息，可利用位运算判断
func (m FileMode) IsDir() bool
func (m FileMode) IsRegular() bool
func (m FileMode) Perm() FileMode
func (m FileMode) String() string
func (m FileMode) Type() FileMode
```

## 文件操作
```go
import "os"

func Link(oldname, newname string) error
func Symlink(oldname, newname string) error
func Readlink(name string) (string, error)

func Mkdir(name string, perm FileMode) error
func MkdirAll(path string, perm FileMode) error
func MkdirTemp(dir, pattern string) (string, error)
func TempDir() string
func UserCacheDir() (string, error)
func UserConfigDir() (string, error)
func UserHomeDir() (string, error)

func Remove(name string) error                      // 删除普通文件或空目录
func RemoveAll(path string) error                   // 删除普通文件或目录
func Rename(oldpath, newpath string) error

func Chown(name string, uid, gid int) error
func Lchown(name string, uid, gid int) error
func Chmod(name string, mode FileMode) error
func Chtimes(name string, atime time.Time, mtime time.Time) error
```

## 输入输出
```go
import "os"

func CreateTemp(dir, pattern string) (*File, error)                 // dir为空则默认系统临时目录，pattern中最后一个*被替换为random
func Create(name string) (*File, error)                             // 不存在则创建，存在则截断
func Open(name string) (*File, error)                               // 只读模式打开
func OpenFile(name string, flag int, perm FileMode) (*File, error)  // 手动指定打开标识(os.O_*)与默认权限
func Pipe() (r *File, w *File, err error)

func (f *File) Name() string
func (f *File) Read(b []byte) (n int, err error)
func (f *File) ReadAt(b []byte, off int64) (n int, err error)
func (f *File) Write(b []byte) (n int, err error)
func (f *File) WriteAt(b []byte, off int64) (n int, err error)
func (f *File) Seek(offset int64, whence int) (ret int64, err error)// whence(os.SEEK_*)
func (f *File) Sync() error

func ReadFile(name string) ([]byte, error)
func WriteFile(name string, data []byte, perm FileMode) error
func Truncate(name string, size int64) error
```


# 进程管理
## 参数解析
```go
import "flag"

func Bool(name string, value bool, usage string) *bool
func BoolVar(p *bool, name string, value bool, usage string)
func Int(name string, value int, usage string) *int
func IntVar(p *int, name string, value int, usage string)
func Uint(name string, value uint, usage string) *uint
func UintVar(p *uint, name string, value uint, usage string)
func Float64(name string, value float64, usage string) *float64
func Float64Var(p *float64, name string, value float64, usage string)
func String(name string, value string, usage string) *string
func StringVar(p *string, name string, value string, usage string)
func Duration(name string, value time.Duration, usage string) *time.Duration
func DurationVar(p *time.Duration, name string, value time.Duration, usage string)
func Func(name, usage string, fn func(string) error)    // 如fn返回non-nil则表示解析错误
func Var(value Value, name string, usage string)
func Parse()
func Parsed() bool
func PrintDefaults()    // 手动打印参数选项提示信息到stderr

func Arg(i int) string  // 解析后剩余中第i个命令行参数
func Args() []string    // 解析后剩余的命令行参数
func NArg() int         // 解析后剩余参数个数
func NFlag() int        // 解析的选项参数个数
func Set(name, value string) error

func Visit(fn func(*Flag))      // 仅访问被设置的flag
func VisitAll(fn func(*Flag))   // 访问所有flag

type Flag struct {
    Name     string // name as it appears on command line
    Usage    string // help message
    Value    Value  // value as set
    DefValue string // default value (as text); for usage message
}

type Value interface {
    String() string
    Set(string) error
}
```

## 进程信息
```go
import "os"

func Executable() (string, error)
func Environ() []string
func Exit(code int)
func Expand(s string, mapping func(string) string) string
func ExpandEnv(s string) string

func Getenv(key string) string
func LookupEnv(key string) (string, bool)
func Setenv(key, value string) error
func Unsetenv(key string) error
func Clearenv()
func Getwd() (dir string, err error)
func Chdir(dir string) error
func Getuid() int
func Getgid() int
func Geteuid() int
func Getegid() int
func Getgroups() ([]int, error)
func Getpid() int
func Getppid() int

func Hostname() (name string, err error)
```


# 数据库
## MySQL
* 定义struct/map表示数据表
    * 结构名 => 表名
        * `SnakeCase` => `snake_cases`
        * `func (this *SnakeCase) TableName() string`
    * 字段名 => 列名
        * `SnakeCase` => `snake_case`
        * `gorm:"column:name"`
    * 类型转换
        * `整数      <=> 整数`
        * `浮点数    <=> 浮点数`
        * `string    <=> 字符`
        * `[]byte    <=> 字节`
        * `time.Time <=> 日期时间`
        * `string    <=> 任意SQL类型`
        * `struct,map<=> 单行记录`
        * `slice     <=> 多行记录`
    * 标签
        > 大多数tag用于创建表时使用，少部分会影响CRUD
        * `gorm:"column:name"`
        * `gorm:"type:sqltype"`
        * `gorm:"size:len"`
        * `gorm:"not null"`
        * `gorm:"unique"`
        * `gorm:"autoIncrement"`
        * `gorm:"default:value"`
        * `gorm:"embedded;embeddedPrefix:prefix_"`
        * `gorm:"<-"`                 // allow read and write (create and update)
        * `gorm:"<-:create"`          // allow read and create
        * `gorm:"<-:update"`          // allow read and update
        * `gorm:"<-:false"`           // readonly
        * `gorm:"->:false;<-:create"` // createonly (disabled read from db)
        * `gorm:"->:false;<-:update"` // updateonly (disabled read from db)
        * `gorm:"-"`                  // no read/write permission (ignore this field)
        * `gorm:"primaryKey"`
        * `gorm:"index:idx_member,priority:1"` // 构造索引idx_member，优先级默认为12，同优先级则根据字段先后顺序构造

```go
import "gorm.io/gorm"
import "gorm.io/driver/mysql"

func main() {
    // 连接数据库
    dsn := "user:pass@tcp(127.0.0.1:3306)/dbname?charset=utf8mb4&parseTime=True&loc=Local"
    db, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
    // 自动迁移表（创建或修改表模式，应该放在init函数里）
    db.AutoMigrate(&Record{})

    // 默认选择的列由Model指定，也可通过Select().Omit()手动指定
    db.Model(&Record{})
    db.Select("name",...)   // 手动指定选择列
    db.Omit("name",...)     // 忽略指定的已选择的列
    // 默认不进行行过滤，空字符串相当于忽略该方法调用
    db = db.Where("name = ?", "xhc")    // gorm解析?时会根据数据库类型判断是否需要加''
    db = db.Where("age <> ?", 18)       // 该Where与上条Where形成AND逻辑关系
    db = db.Where(&Record{Name:"x", age:1})
    db = db.Where("age LIKE ?", "pref%")
    db = db.Where("age BETWEEN ? AND ?", 1, 18)
    db = db.Where("age IN ?", []int{19,20})
    db = db.Or("")
    db = db.Not("")

    // 插入：选择列
    db.Create(&Record{})
    // 删除：过滤行
    db.Delete(&Record{})
    // 更新：选择列，过滤行。除非手动指定选择列，否则零值表示保持数据库原值不变
    db.Updates(&Record{})
    // 查询：选择列，过滤行
    db.Select("DISTINCT records.name AS nickname")
    db.Joins("LEFT JOIN user AS other ON records.id = user.id")
    db.Where("name = 'xhc' AND age = 18")
    db.Group("name, age").Having("COUNT(name) > 2")
    db.Order("name DESC, age")
    db.Limit(1).Offset(0)
    db.Find(&Record{})
}
```
**设计准则**
* 定义完整的与数据库模式一致的Model用于任何读写数据库的操作
* 若与其他业务代码的接口数据结构不一致则需要进行转换


# Web框架
## API注释
```go
// @Summary 接口简述
// @Description 接口详情
// @Tags 标签, 支持逗号分隔列表
// @Accept 接收MIME类型
// @Produce 生成MIME类型
// @Param 参数名 参数类型 数据类型 是否必须 "注释" 可选属性(值)
// @Success 返回码 {参数类型} 数据类型 "注释"
// @Failure 返回码 {参数类型} 数据类型 "注释"
// @Router /path/to/resource [Method]
```
* MIME类型
    * json
    * mpfd
    * xml
* 参数类型
    * path
    * query
    * header
    * body
* 数据类型
    * bool
    * int
    * number
    * string
    * array
    * object
* 属性
    * default(*)
    * enums(v1, v2)



