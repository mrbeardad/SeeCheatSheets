# PowerShell

## 模块

- 程序入口：任意源文件顺序执行
- 模块结构：
  - `.psd1` 负责模块元数据和符号导出
  - `.psm1` 负责主模块代码
  - `.ps1` 拆分的脚本代码
- 模块导入
  - 模块安装路径
    - 当前用户: `$HOME\Documents\PowerShell\Modules`
    - 所以用户: `$env:ProgramFiles\PowerShell\Modules`
    - 内置模块: `$PSHOME\Modules`
  - 符号名字限定
    - 默认无限定
    - 可选限定符 `ModuleName\FunctionName`
- 模块导出
  - 在 `.psd1` 中用 `*ToExport` 字段导出（可实现懒加载）
  - 在 `.psm1` 中用 `Export-ModuleMember` 命令导出
- 模块初始化：脚本首次导入立即执行
- 依赖管理：PowerShell Gallery

```ps1
Install-Module # 下载并安装模块
Uninstall-Module

Import-Module # 手动导入模块
Remove-Module

Get-Module
```

## 变量

- 类型系统：弱类型 + 引用型
- 生命周期：第一次引用时构造，退出函数作用域后不可见，不再引用后被 GC 回收
- 作用域：

  - `Env` (环境变量)
  - `Global` (作为程序入口的脚本的 Script 作用域即 Global 作用域)
  - `Script` (脚本被直接调用时有独立的 Script 作用域，使用 `.` 加载脚本则与调用者共享作用域)
  - `Local` (Function)
  - 因为没有变量声明语法，赋值会被识别为在当前作用域新构造变量，所以无法直接修改父作用域中的变量，需要使用 `Set-Variable -Scope 1`
  - 使用 `Private` 修饰符甚至可以拒绝子作用域访问变量

- 拷贝控制：浅拷贝

```ps1
$foo = $bar
$foo = ${bar}
$foo = [int]$bar
$Private:foo = $Global:bar

New-Variable
Clear-Variable # 将变量置为 NULL
Remove-Variable # 将变量删除
Set-Variable
Get-Variable
```

- 特殊变量
  - [Automatic Variable](https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_automatic_variables): 存储 PowerShell 的状态
  - [Preference Variable](https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_preference_variables): 调整 PowerShell 的行为

## 运算符

- 成员：`::` `.` `?.` `[]` `?[]` `()`

- 算术：`++` `--` `+` `-` `*` `/` `%`

```ps1
5 / 2
[Math]::Round(5 / 2) # 四舍五入，默认行为，同 5 / 2
[Math]::Round(5 / 2, [MidpointRounding]::ToEven)
[Math]::Round(5 / 2, [MidpointRounding]::ToZero)
[Math]::Round(5 / 2, [MidpointRounding]::AwayFromZero)
[Math]::Ceiling(5 / 2)
[Math]::Floor(5 / 2)
```

- 按位：`-band` `-bor` `-bxor` `-bnot` `-shl` `-shr`

- 关系：

  - `-gt` `-lt` `-le` `-ge` `-eq` `-ne`
  - `-like` `-notlike`
  - `-match` `-notmatch` `$Matches`
  - `-contains` `-notcontains` `-in` `-notin`
  - `-is` `-isnot` `-as`
  - `? :`
  - 对于字符串，添加前缀 c 则变为大小写敏感模式，如 `-ceq`
  - 当左侧操作数是集合类型时，则返回匹配的**元素子集**而非布尔值

- 逻辑：`-and` `-or` `-xor` `-not` `!` `??`

- 连接：`|` `;` `&&` `||` `&`

- 字符操作：`-replace` `-split` `-join` `-f`

```ps1
-split <String> or <String[]>

<String> -split <Delimiter> # 默认是 '\s+'，正则表达式分组会被作为元素返回

<String> -split <Delimiter>, <Max-substrings>

<String> -split <ScriptBlock>

-join <String[]>

<String[]> -join <Delimiter>
```

> `-f` 见 [Format item syntax](https://learn.microsoft.com/en-us/dotnet/standard/base-types/composite-formatting#format-item-syntax)

- 重定向
  - `n>`
  - `n>>`
  - `n>&1`
  - n 默认为 1

| Stream # | Description        | Write Cmdlet                      |
| -------- | ------------------ | --------------------------------- |
| 1        | Success Stream     | `Write-Output`                    |
| 2        | Error Stream       | `Write-Error`                     |
| 3        | Warning Stream     | `Write-Warning`                   |
| 4        | Verbose Stream     | `Write-Verbose`                   |
| 5        | Debug Stream       | `Write-Debug`                     |
| 6        | Information Stream | `Write-Information`, `Write-Host` |
| \*       | All Streams        |                                   |

对于 PowerShell 来说存在 6 种流用于不同用途，所有 Cmdlet, Function, Script 都遵循这种规则

但对于 Native Command 只有 3 种流 stdin, stdout, stderr

当 native 与 powershell 交互时

- Success Steam 相当于 stdout
- Error Steam 相当于 stderr
- Return Value 相当于 stdin

## 控制流

```ps1
if () {

} elseif () {

} else {

}
```

```ps1
# 若表达式为集合则相当于 foreach-switch
switch [-regex | -wildcard | -exact] [-casesensitive] (<test-expression>) {
    "string" | number | variable | { <value-scriptblock> } { <action-scriptblock> }
    default { <action-scriptblock> } # optional
}
# 默认按顺序匹配所有子句
break # 退出 switch
continue # 若表达式为集合，则跳过该次迭代
$switch # 枚举器
```

```ps1
while (<condition>) {
  <statement list>
}

for (<Init>; <Condition>; <Repeat>) {
    <Statement list>
}

foreach ($<item> in $<collection>) {
  <statement list>
}
$foreach # 枚举器

ForEach-Object
            [-InputObject <PSObject>]
            [-Begin <ScriptBlock>]
            [-Process] <ScriptBlock[]>
            [-End <ScriptBlock>]
```

```ps1
try {<statement list>}
catch [[<error type>][',' <error type>]*] {<statement list>}
finally {<statement list>}

trap [[<error type>]] {<statement list>}

$? # 上条命令是否成功返回，对于 PowerShell 需要上条命令未抛异常且未写入 Error Steam，对于 Native 需要返回值为 0
$LASTEXITCODE # 上条 Native 命令的返回值
$Error # 包含最近发生的错误的数组，0 为最近的错误
$StackTrace # 最近发生错误时的栈帧
```

## 函数

```ps1
function [<scope:>]<name>
{
  param([type]$parameter1 [,[type]$parameter2])
  dynamicparam {<statement list>}
  begin {<statement list>}
  process {<statement list>}
  end {<statement list>} # 默认
  clean {<statement list>}
}
$args # 包含未被绑定的参数
$PSBoundParameters # 包含已绑定的参数
$_ # 包含当前迭代的管道输入对象
$input # 枚举器，包含所有管道输入对象
$MyInvocation # 调用者信息
```

- [`[CmdletBinding()]`](https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_functions_cmdletbindingattribute) 用于添加一些通用参数
- [`[Parameter()]`](https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_functions_advanced_parameters) 用于设置参数属性

  - ParameterSetName
  - Mandatory
  - Position
  - ValueFromPipeline
  - ValueFromPipelineByPropertyName
  - ValueFromRemainingArguments
  - HelpMessage

  ```txt
  -Name <String>

    Required?                    false
    Position?                    1
    Default value                Current directory
    Accept pipeline input?       false
    Accept wildcard characters?  false
  ```

- [`[OutputType()]`](https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_functions_outputtypeattribute) 用于设置函数返回值类型

- 任何命令默认都有返回值，函数中调用的所有命令的返回值共同构成函数的返回值，若要避免不必要的返回值
  - `Command | Out-Null`
  - `Command >$null`
  - `[void]$expr`

## 语法

PowerShell 脚本解析有三种模式，详情参见 [about_Parsing](https://learn.microsoft.com/en-us/powershell/module/microsoft.powershell.core/about/about_parsing)，
只要脚本语句可以被成功解析为表达式，则使用表达式模式，否则使用参数模式

- 表达式解析：和常见编程语言类似，只不过变量的访问需要 `$` 且操作符通常 `-` 开头

```ps1
$null

$true
$false

32
1.25

'Hello'
"World"
@"
raw
string
"@

@(1,2,3)
@{a=1;b=2;c=3}
[pscustomobject]@{name='mrbeardad';age=99;}

-not $Quiet
```

- PowerShell 参数解析：将 Tokens 分为 Command, Parameter, Value 三种类型，其中 Value 可以按部分表达式语法来解析，最终构造对象并绑定到参数上然后调用 PowerShell Cmdlet, Script, Function

- Native 参数解析：将 Tokens 分为 Command, Value 两种类型，其中 Value 可以按部分表达式语法来解析，最终构造字符串数组传递给 Native Executable

```ps1
Command -Parameter Value

Script -Arg 1, 2, 3 -- --unparsed-opt

$s = "a b"
NativeExe -Arg 1 , 2, 3 $s --% raw toekns
```

一些特殊符号

- `` ` ``: 转义字符，类似 Bash 的 `\`
- `$var` `${var}`: 访问变量
- `'` `"` `@'` `@"`: 构造字符串
- `,`: 仅在 PowerShell 参数解析中用于构造数组对象
- `@(,)`: 构造数组
- `@{;}`: 构造哈希表
- `()`: 嵌入表达式
- `$()`: 嵌入多条语句
- `&`: 调用 Command, Script, Script Block, 通常用于间接调用，若需要反射解析则应该使用 `Invoke-Expression`
- `~`: 用户目录
- `--`: 不再将 Tokens 解析为 Parameter
- `--%`: 停止解析 Tokens

## 基础命令

- 帮助

  - Get-Help -> help
  - Get-Command -> gcm
  - Get-Member -> gm

- 增删改

  - New-Item -> ni
  - Clear-Item -> cli
  - Remove-Item -> rm
  - Rename-Item -> ren
  - Move-Item -> mv
  - Copy-Item -> cp

- 输入

  - Get-Item -> gi
  - Get-ChildItem -> ls
  - ConvertFrom-Json
  - ConvertFrom-Csv
  - ConvertFrom-Xml

- 过滤

  - Select-String -> sls
  - Select-Object -> select
  - Where-Object -> where
  - Group-Object -> group
  - Sort-Object -> sort
  - Measure-Object -> measure

- 输出
  - Format-Table -> ft
  - Format-List -> fl
  - ConvertTo-Json
  - ConvertTo-Csv
  - ConvertTo-Xml

> Item 由 PSProvider 提供，比如文件系统、注册表等，见 `Get-PSProvider`
