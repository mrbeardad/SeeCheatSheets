# 目录
<!-- vim-markdown-toc GFM -->

- [系统启动](#系统启动)
  - [MBR与GPT](#mbr与gpt)
    - [MBR](#mbr)
    - [GPT](#gpt)
  - [BIOS与UEFI](#bios与uefi)
    - [BIOS](#bios)
    - [UEFI](#uefi)
  - [grub](#grub)
    - [grub工作流程](#grub工作流程)
    - [grub配置](#grub配置)
    - [grub-Shell](#grub-shell)
    - [grub选单](#grub选单)
  - [kernel](#kernel)
    - [kernel工作流程](#kernel工作流程)
    - [kernel引导参数](#kernel引导参数)
  - [systemd](#systemd)
    - [systemd启动流程](#systemd启动流程)
    - [systemctl命令](#systemctl命令)
    - [systemd配置](#systemd配置)
    - [unit配置](#unit配置)
      - [Unit小节](#unit小节)
      - [Install小节](#install小节)
      - [Service小节](#service小节)
      - [Timer小节](#timer小节)
  - [getty与login](#getty与login)

<!-- vim-markdown-toc -->

# 系统启动

## MBR与GPT
硬盘分区格式一般有两种，老式的MBR与新式的GPT。

### MBR
> MBR章节大部分内容摘录自[鸟哥的Linux私房菜](http://linux.vbird.org/linux_basic/0130designlinux.php#partition_table)

[MBR结构](images/MBR.png)

**MBR区**  
&emsp;LBA0中的前446字节，用于存放boot-loader

**分区表项**  
&emsp;LBA0中的后64字节，最多存储4笔分区记录。其中最后一笔记录为扩展分区，其他为主分区。
扩展分区可以再细化分割出逻辑分区，而分割逻辑分区的分区表项存储再扩展分区头部。
扩展分区并非一个可使用的分区，它存在的目的即是容纳逻辑分区。

### GPT
> GPT章节大部分信息摘录自[金步国作品集](http://www.jinbuguo.com/storage/gpt.html)

![GPT表结构](images/GPT.jpg)
> 注：LBA0表示0号扇区，即第一个512B的块

**保护MBR**  
&emsp;保护MBR包含一个DOS分区表，只包含一个类型值为0xEE的分区项，
在小于2TB的磁盘上，大小为整个磁盘；在更大的磁盘上，它的大小固定为2TB。
它的作用是阻止不能识别GPT分区的磁盘工具试图对其进行分区或格式化等操作，所以该扇区被称为“保护MBR”。
实际上，EFI根本不使用这个分区表。

**GPT头部**
| 相对字节偏移量 | 字节数 | 说明                             | 备注                                                |
|----------------|--------|----------------------------------|-----------------------------------------------------|
| 00～07         | 8      | GPT头签名                        | “45 46 49 20 50 41 52 54”(ASCII码为“EFI PART”)      |
| 08～0B         | 4      | GPT版本号                        | 目前是1.0版，其值是“00 00 01 00”                    |
| 0C～0F         | 4      | GPT头的大小                      | 通常为“5C 00 00 00”(0x5C)，也就是92字节             |
| 10～13         | 4      | GPT头CRC校验和                   | 计算时把这个字段本身看做零值                        |
| 14～17         | 4      | 保留                             | 必须为“00 00 00 00”                                 |
| 18～1F         | 8      | EFI信息区(GPT头)的起始扇区号     | 通常为“01 00 00 00 00 00 00 00”，也就是LBA1         |
| 20～27         | 8      | EFI信息区(GPT头)备份位置的扇区号 | 也就是EFI区域结束扇区号。通常是整个磁盘最末一个扇区 |
| 28～2F         | 8      | GPT分区区域的起始扇区号          | 通常为“22 00 00 00 00 00 00 00”(0x22)，也即是LBA34  |
| 30～37         | 8      | GPT分区区域的结束扇区号          | 通常是倒数第34扇区                                  |
| 38～47         | 16     | 磁盘GUID                         | 全球唯一标识符,与UUID是同义词                       |
| 48～4F         | 8      | 分区表起始扇区号                 | 通常为“02 00 00 00 00 00 00 00”(0x02)，也就是LBA2   |
| 50～53         | 4      | 分区表总项数                     | 通常限定为“80 00 00 00”(0x80)，也就是128个          |
| 54～57         | 4      | 每个分区表项占用字节数           | 通常限定为“80 00 00 00”(0x80)，也就是128字节        |
| 58～5B         | 4      | 分区表CRC校验和                  |                                                     |
| 5C～*          | *      | 保留                             | 通常是全零填充                                      |

**分区表项**
| 相对字节偏移量 | 字节数 | 说明                   | 备注                           |
|----------------|--------|------------------------|--------------------------------|
| 00～0F         | 16     | 分区类型               | 用GUID表示，可用gdisk查看      |
| 10～1F         | 16     | 分区唯一标示符         | 用GUID表示                     |
| 20～27         | 8      | 分区的起始扇区         | 用LBA值表示                    |
| 28～2F         | 8      | 分区的结束扇区         | 用LBA值表示，通常是奇数        |
| 30～37         | 8      | 分区的属性标志         | 一般用于Windows，可用gdisk查看 |
| 38～7F         | 72     | UTF-16LE编码的分区名称 | 最大32个字符                   |

## BIOS与UEFI
固件规范也基本分为两种，老式的BIOS与新式的UEFI。

### BIOS
&emsp;BIOS与一般MBR配合，作为老式主板的固件。在MBR格式的硬盘分区中，除了MBR头部的引导区，每个分区头部都有一个引导区用于存放boot-loader。
BIOS加载MBR头部的boot-loader，而后者便可以选择：
* 加载操作系统镜像
* 加载其他boot-loader

### UEFI
&emsp;EFI系统分区（ESP），是一个FAT16或FAT32格式的物理分区，但是其分区标识是EF (十六进制) 而非常规的0E或0C；
因此该分区在Windows操作系统下一般是不可见的。
ESP是系统引导分区，供UEFI引导系统使用，支持EFI模式的电脑需要从ESP启动系统，EFI固件可从ESP加载EFI启动程序或者应用。

**主要规范**
* 读取GPT分区表
* 读取ESP
* 能够执行EFI可执行文件
* 添加、删除、修改启动项
    > 默认启动项位于每个ESP的/EFI/BOOT/BOOT{x64,IA32,IA64,ARM,A64}.EFI  
    > 用于那些没有指定硬盘上具体目标，而只是让固件自行搜索的启动项
* 设置启动项顺序
* 设置启动项模式（UEFI or BIOS）

**安全启动**：  
&emsp;UEFI规范规定固件可以包含一系列签名，并拒绝运行未签名或签名与固件中包含的签名不一致的 EFI 可执行文件。
这种签名机制还用于内核对加载模块的验证。

**关于Microsoft Windows**：  
* 限制了需要获得预装Windows的批量许可（即“微软认证”）的在售PC：
    * 默认启用安全启动 (Secure Boot)（服务器除外）
    * 在其信任密钥列表中包含微软的密钥
    * 启用安全启动 (Secure Boot) 时，必须禁用 BIOS 兼容模式（如果没记错的话，UEFI 规范也有此要求）
    * 支持签名黑名单

* 符合微软认证要求的 x86 计算机 还必须满足以下附加条件：
    * 允许 自然人禁用安全启动 (Secure Boot)
    * 允许 自然人启用自定义模式，以及修改固件的信任密钥列表

* 符合微软认证要求的 ARM 计算机 还必须满足以下附加条件：
    * 不允许 自然人禁用安全启动 (Secure Boot)
    * 不允许 自然人启用自定义模式，以及修改固件的信任密钥列表

## grub
### grub工作流程
1. 加载bootx64.efi
2. 定义变量`cmdpath` `prefix` `root`
3. 加载normal.mod
    > 失败则进入救援模式
4. 执行`$prefix/grub.cfg`
    > 无grub.cfg则进入命令模式
5. 进入普通模式（选单界面）
    * 按`e`进入编辑模式
    * 按`c`进入命令模式
6. 加载initramfs
7. 加载kernel

### grub配置
* /etc/default/grub：该文件中的内容会被grub-mkconfig程序转换后加入grub.conf

* /etc/grub.d/
    * 00_header     ：基础配置
    * 01_user       ：自定义环境
    * 10_linux      ：确定linux选单
    * 20_os-prober  ：确定其他OS选单
    * 40_custom     ：自定义选单

* /boot/grub/grub.conf：grub的直接配置文件

<!-- entry begin: grub-install grub-mkconfig -->
* grub-install：安装grub
    * --target=x86_64-efi
    * --efi-directory=/boot/efi
    * --bootloader-id=GRUB

* grub-mkconfig：安装grub配置
    * -o /boot/grub/grub.cfg
<!-- entry end -->

### grub-Shell
* 模块：默认自动加载command.lst与crypto.lst

* 文件命令规则
    * 分区       ：(hd0，gpt1)
    * 文件       ：(hd0，gpt1)/path/to/file
    * 扇区       ：(hd0，gpt1)0+1

* 特殊变量
    * prefix     ：grub安装目录
    * root       ：根设备，未指定设备名的文件的默认设备
    * cmdpath    ：core.image所在目录
    * superusers ：超级用户，逗号分隔

* grub命令
    * ls                    ：列出已知设备/设备中的文件/目录的内容
    * cat                   ：显示文件内容，--dos选项处理换行符
    * echo                  ：与bash用法一样
    * normal                ：执行命令脚本
    * source                ：将文件内容插入当前位置
    * configfile            ：将文件做配置加载，不会保留其设置的环境变量
    * set var=val           ：设置变量
    * export var            ：导出至环境变量，使其对configfile命令载入的配置文件可见
    * lsmod                 ：列表已加载模块
    * insmod/rmmod          ：加载/卸载模块
    * loopback dev isofile  ：建立loop设备，-d删除
    * halt/reboot           ：关机/重启

### grub选单
* menuentry
    * "title"    ：选单名
    * --class    ：选单主题样式
    * --id       ：赋值chosen，覆盖原来的"title"
    * 语句块
        * linux       ：加载内核
        * initrd      ：加载内核映像
        * boot        ：启动已加载的os或loader，选单结束时隐含
        * chianloader ：链式加载文件

* grub安全
    * 设置超级用户：`set superusers="root"`
        > 注 ：设置后只有超级用户才能修改选单
    * 设置加密密码：`password_pbkdf2 root grub.pbkdf2.sha512...`
        > 注 ：使用grub-mkpasswd-pbkdf2命令产生密码
    * 设置明文密码：`password user ...`
    * menuentry选项
        * --unrestricted    ：所有人可执行
        * --users ""        ：仅超级用户
        * --users "user"    ：仅user与超级用户

## kernel
### kernel工作流程
1. 挂载initramfs模拟的`/ (根目录设备)`
    1. 加载模块
    2. 启动init程序(systemd)
    3. 启动initrd.target

2. 挂载硬盘中真正`/ (根目录设备)`，并释放initramfs
3. 准备启动磁盘上的default.target

### kernel引导参数
<!-- entry begin: grub system rescue -->
* 修改选单内核参数为`rd.break`，并`chroot`
    > 注 ：rd.break模式下无SELinux，修改密码会导致其安全上下文失效而导致无法登陆
* 修改选单内核参数为`systemd-unit=rescu.target`
<!-- entry end -->

## systemd
### systemd启动流程
1. 寻找并启动default.target，同时递归检查依赖，一般情况依赖如下

2. sysinit.target：
    * 特殊文件系统的挂载与启动
        > 如LVM与 RAID等
    * 启动plymouthd
        > 开机动画服务
    * 启动systemd-journald
        > 系统日志服务
    * 载入额外的内核模块
        > /etc/modules-load.d/*.conf
    * 载入额外的内核参数
        > /etc/sysctl.conf 与 /etc/sysctl.d/*.conf
    * 启动乱数产生器
    * 设置终端字体
    * 启动systemd-udevd
        > 动态设备管理器
    * 加载本地文件系统与swap
        > 读取/etc/fstab

3. basic.target
    * SELlinux
    * FireWall
    * pathes.target
    * slices.target
    * sysinit.target
    * sockets.target
    * timers.target
    * getty.target

4. multi-user.target

5. graphical.target

### systemctl命令
<!-- entry begin: systemctl systemd-analyze systemd-escape -->
* systemctl
    * 信息查看
        * status
        * show
        * cat
        * edit
    * unit查看
        * --state=
        * -t                ：指定unit类型
        * -a                ：所有
        * list-units        ：已加载的
        * list-unit-files   ：所有
        * list-dependencies [--reverse]
    * 手动控制
        * start
        * stop
        * restart
        * reload
    * 开机管理
        * enable|disable    ：可使用--now同时执行start/stop
        * static            ：只能作为依赖被启动
        * mask              ：禁止启动，unmask解禁
    * 主机状态
        * suspend
        * hibernate
        * rescue
        * emergency
    * 主机target
        * get-default
        * set-default
        * isolate
    * systemd重新读取unit
        * daemon-reload

* systemd-analyze
    * blame
    * plot > plot.svg
<!-- entry end -->

### systemd配置
* unit配置目录(优先级降序)
    > * systemd无法管理手动执行启动的服务  
    > * 只有在配置目录的unit才在systemd视线里  
    > * 是否开机启动取决于满足上述的unit是否在default.target的依赖链中
    * /etc/systemd/system/
    * /run/systemd/system/
    * /lib/systemd/system/

* unit类型
    * service    ：一般服务单元
    * socket     ：监听socket以启动服务
    * timer      ：定时器用于启动服务
    * target     ：一系列unit的集合
        > 一般包括有：graphical，multi-user，basic，sysinit，rescue，emerge，shutdown，getty

### unit配置
* 模板单元：命名`foo@.service`，配置中`%i`替换为实例`foo@bar.service`中的`bar`，`%I`替换为未转移的`bar`
* 单元别名：通过在单元目录下创建符号链接即可形成单元别名
* 依赖目录：单元目录下名如`foo.service.wants/`与`foo.service.required/`的目录中的单元文件会加入`foo.service`中的`Wants=`与`Requires=`依赖中
* 额外配置：单元目录下名如`foo.service.d/`，当解析完主单元文件`foo.service`后会按字典序读取额外配置目录下的名如`*.conf`的文件
    * 选项可重复设置，后面覆盖前面
    * 对于`foo@bar.service`会优先读取`foo@bar.service.d/`，再读`foo@.service.d/`
    * 对于`foo-bar.service`会优先读取`foo-.service`，再读`foo-bar.service`，且后者中的同名文件会覆盖前者

#### Unit小节
<!-- entry begin: [Unit] -->
```ini
[Unit]
Description=            # 描述该服务的名词
Documentation=          # 该服务的详细文档。接受 http://、https://、file:、info:、man: 五种URI。若为空字符串则表示清空之前设置
After=                  # 当与列表中单元同时启动时，在其启动完成之后才启动，同时关闭时在其之前
Before=                 # 当与列表中单元同时启动时，在其启动完成之前就启动，同时关闭时在其之后
Wants=                  # 同时启动依赖单元
Requires=               # 同时启动依赖单元。若同时设置了After=依赖单元且其启动失败，则无法启动本单元。若依赖单元被显式停止，本单元连带被停止
BindsTo=                # 同时启动依赖单元。若依赖单元停止，则本单元连带被停止
Requisite=              # 只检查而不启动依赖，检查失败则fail
PartOf=                 # 跟着依赖同时关闭或重启
Conflicts=              # 与这些单元不能同时存在
```
<!-- entry end -->

#### Install小节
<!-- entry begin: [Install] -->
```ini
[Install]
WantedBy=               # enable时创建到指定的foo.service.wants/目录下的符号链接，disable时删除
RequiredBy=             # enable时创建到指定的foo.service.required/目录下的符号链接，disable时删除
Alias=                  # 更改上述创建的符号链接的文件名（别名）
Also=                   # enable与disable本单元时，也对列表中单元进行相同操作
DefaultInstance=        # 对于模板单元，设置默认实例，即直接启动该模板单元而未指定实例时的默认实例
```
<!-- entry end -->

#### Service小节
<!-- entry begin: [Service] -->
```ini
Type=
# simple    表示当fork()返回时即算启动完成
# exec      表示当exec()执行成功时才算是启动完成
# oneshot   表示当执行完成并退出后才算完成，直接从activating到inactive，应该设置RemainAfterExit
# forking   表示当父进程退出且至少有一个子进程才算完成，应该设置PIDFile=以跟踪主进程
# notify    表示当返回状态信息后才算完成
# dbus      表示当从D-Bus获得名称后算完成，需设置BusName选项
# idle      表示当所有任务完成后才启动，最多延迟5秒
User=
Group=
ExecPre=
ExecStart=
ExecPost=
ExecReload=
ExecStop=
ExecStopPost=
# 只有Type=oneshot才支持启动多个程序
# -绝对路径名，表示失败退出被视为成功，否则停止该单元
# +绝对路径名，表示使用超级权限

Restart=                # 如何重启
RestartSec=             # 重启前暂停时间，需要指定单位
# | 退出原因         | no | always | on-success | on-failure | on-abnormal | on-abort | on-watchdog |
# |------------------|----|--------|------------|------------|-------------|----------|-------------|
# | 正常退出         |    | X      | X          |            |             |          |             |
# | 退出码不为"0"    |    | X      |            | X          |             |          |             |
# | 进程被强制杀死   |    | X      |            | X          | X           | X        |             |
# | systemd 操作超时 |    | X      |            | X          | X           |          |             |
# | 看门狗超时       |    | X      |            | X          | X           |          | X           |
```
<!-- entry end -->

#### Timer小节
<!-- entry begin: [Timer] -->
```ini
OnBootSec=              # 相对内核启动
OnStartupSec=           # 相对systemd启动
OnActiveSec=            # 相对该timer.target启动
OnUnitActiveSec=        # 相对匹配service最后一次启动
OnUnitInactiveSec=      # 相对匹配service最后一次关闭
OnCalendar=             # 星期 年-月-日 时:分:秒
# 星期(可选)            Mon、Tue、Wed、Thu、Fri、Sat、Sun
# x,y                   表示指定列表
# x..y                  表示指定范围
# *                     表示指定任一
# x/N                   表示指定N间隔跳跃
# ~日                   表示指定月中倒数第几天
# 秒                    us、ms、s、m、h、d、w、M、y

AccuracySec=            设置精度，默认一分钟
Persistent=             是否操作OnCalendar不错过
WakeSystem=             是否到时唤醒系统
Unit=                   指定匹配unit，默认同名.service
```
<!-- entry end -->

## getty与login
1. 启动getty.target创建终端
2. 启动systemd-logind连接到上述终端
3. 用户输入用户名与密码，systemd-logind调用PAM进行身份验证与授权
4. systemd-logind作为超级用户进程，需要为接下来用户shell初始化安全的、基础的进程环境
5. systemd-logind调用exec加载shell，将`argv[0][0]`设置为`-`表示登录shell
6. shell读取其配置文件

