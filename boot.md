# 系统启动
## GPT

## UEFI
**流程** ：
1. 上电自检（加载固件）
2. 读取硬件信息并检查
3. 读取并加载硬盘ESP中的boot-loader(GRUB)
> 需要掌握：
> * 进入UEFI BIOS界面
> * 为UEFI BIOS界面加密
> * 设置开机启动项
> * 了解安全启动模式

## GRUB
**流程** ：
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
> 需要掌握：
> * 了解选单制作
> * 为GRUB界面加密
> * 修改选单中内核参数以进行系统救援


## Kernel
**流程** ：
1. 挂载initramfs模拟的`/ (根目录设备)`
    1. 加载Modules
    2. 启动init程序(systemd)
    3. 启动initrd.target

2. 挂载硬盘中真正`/ (根目录设备)`
3. 释放initramfs
4. 准备启动磁盘上的default.target

> 需要掌握：
> * 内核的抽象模型与功能
## Systemd
**流程** ：
1. 启动default.target并递归检查依赖

2. sysinit.target：
    * 特殊文件系统的挂载与启动
        > 如：LVM, RAID
    * 启动plymouthd
        > 开机动画
    * 启动systemd-journald
        > 系统日志
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
4. multi-user.target
5. graphical.target
> 需要掌握：
> * 掌握Systemd-Unit的基本配置
> * systemctl工具管理daemon
> * journalctl工具查看日志


## Login
* 启动登录程序
    > 文本界面通过logind  
    > 图形界面通过GDM之类的display-manager
* 通过PAM模块进行认证
    > 通过/etc/pam.d/${daemon-name}进行配置
* 开启会话，获取shell

