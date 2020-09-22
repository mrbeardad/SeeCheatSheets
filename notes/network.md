<!-- entry begin: 网络协议栈 -->
* 网络协议栈
    * 发送：
        * 应用层调用socket接口函数，发送message
        * 传输层确认参数（协议、目的端口号、本地端口号），并封装为segment
        * 网络层利用路由表确认参数（协议、目的IP、本地IP等），分片并封装为datagram，若目的IP为本地则直接上传
            > 将目的IP与本地子网掩码进行位与运算，得到网络号，再与路由表对比，收包时同理
        * 链路层通过驱动程序，利用ARP表确认目的MAC地址，若不存在则发送ARP请求，最后封装成frame，然后发送到网卡进行信号转换并传输
    * 接收：
        * 链路层中网卡默认会将MAC地址非本机且非广播的frame丢弃，若为ARP包则本层(驱动)解决
        * 网络层确认目的IP是否为本机IP或本机网段广播，若为ICMP则本层解决
        * 传输层确认socket的协议与端口，上传给应用层
        * 应用层控制会话的挂断
<!-- entry end -->

<!-- entry begin: 交换机 -->
* 交换机
    * 将端口与frame的源MAC地址进行绑定(一个端口可绑定多个MAC地址，反之不行)
    * 若arp表中无frame的目的MAC地址则泛洪(在其他端口转发原frame)
    * 二层交换机配置有IP地址与默认路由，管理员远程管理，目的MAC地址与目的IP地址都为交换机才行
<!-- entry end -->

<!-- entry begin: 路由器 -->
* 路由器
    * 检查目的MAC地址，判断是否接收
    * 检查目的IP地址，判断接收、转发或丢弃
    * 转发时可能需要NAT
<!-- entry end -->

* 防火墙(内核钩子)
![图片来自网络](../images/netfilter.jpg)

<!-- entry begin: 虚拟网卡与虚拟网桥 -->
* 虚拟网卡与虚拟网桥
    * 物理网卡接收的包发送到虚拟网桥，其通过类似交换机原理转发包，且虚拟网桥自身有IP与MAC
    * 虚拟网卡接收到虚拟网桥转发的数据包，并开始解包流程
<!-- entry end -->

<!-- entry begin: VPN -->
* VPN
    * 与远程主机连接形成VLAN，通过建立隧道和修改路由，将某目的网络/主机的路由修改到隧道另一端
    * 数据包封装了访问请求，由VPS转发
<!-- entry end -->

<!-- entry begin: 代理 -->
* 代理
    * 正向代理   ：客户端设置代理后，所有消息发送至代理服务器并由其修改源IP后转发
    * 反向代理   ：服务器设置代理后，客户端连接代理服务器，由代理服务器分配连接到真正服务器
<!-- entry end -->



# 互联网结构
## ISP
* 接入ISP
* 区域ISP
* 第一层ISP
* IXP
* CDN

## 接入网
### 广域网
* DSL （基于电话线）
* 电缆（基于电视线）
* FTTH（光纤）
* 拨号（传统电话线）
* 卫星（卫星无线电）
* 蜂窝网络
### 局域网
* 以太网
* WiFi

## 物理媒体
* 双绞铜线      ：距离短、速度快
* 同轴电缆      ：共享媒体
* 光纤          ：稳定、高成本、海底光缆
* 陆地无线电信道：穿墙、移动用户、长距离
* 卫星无线电信道：同步卫星、近地轨道卫星

## 分组交换技术
message分片封装为packet，经switch/router转发或路由
### 时延
* 处理时延（验证、路由等计算）
* 排队时延（按队列接收并转发数据）
* 传输时延（存储转发）
* 传播时延（信号亚光速传播）
### 带宽
带宽（吞吐量）的大小主要由防火墙内网卡，及程序算法的效率决定，尤其是程序算法，
会使防火墙系统进行大量运算，通信量大打折扣。

# 协议分层
* 物理层
* 链路层
* 网络层
* 传输层
* 应用层

## 应用层
### DNS: UDP 44
* 特点
    * 一个**规范主机名**可映射多个IP
        > 并且DNS服务器支持负载分配
    * 多个主机别名可映射到同一个规范主机名
        > 规范主机名用来判断“域”以获取正确DNS服务器的IP，DNS服务器会先尝试将询问的域名转换为规范主机名
    * 递归查询与迭代查询

* 分布式层次数据库
    * 根DNS
    * 顶级DNS
    * 权威DNS
    * *中间DNS
    * *本地DNS

* 一般流程
    * 访问本地DNS（递归查询）
    * 本地DNS访问根DNS（迭代查询）
    * 本地DNS访问顶级DNS（迭代查询）
    * 本地DNS访问权威DNS（迭代查询）
    * ...
    * 本地DNS获取目标主机IP
    * 本地DNS回传主机IP

* DNS记录
    > `(Name, Value, Type ,TTL)`
    * `(DomainName, DNSHostName, NS)`
    * `(HostName, IP, A)`
    * `(HostAlias, HostName, CNAME)`
    * `(MailAlias, MailHostName, MX)`

* 插入DNS记录
    * 寻找注册登记机构[](http://www.internic.net)
    * 注册登录机构会将申请的DNS记录添加到所有顶级DNS中
```
# 13台根DNS服务器
美国VeriSign公司　2台
网络管理组织IANA(Internet Assigned Number Authority)　1台
欧洲网络管理组织RIPE-NCC(Resource IP Europeens Network Coordination Centre)　1台
美国PSINet公司　1台
美国ISI(Information Sciences Institute)　1台
美国ISC(Internet Software Consortium)　1台
美国马里兰大学(University of Maryland)　1台
美国太空总署(NASA)　1台
美国国防部　1台
美国陆军研究所　1台
挪威NORDUnet　1台
日本WIDE(Widely Integrated Distributed Environments)研究计划　1台

# 通用顶级域名
generic：
.com    - 供商业机构使用，但无限制
.info   - 供资讯性网站使用，但无限制
.net    - 原供网络服务供应商使用，现无限制
.org    - 原供不属于其他通用顶级域类别的组织使用，现无限制
.xyz    - 无限制

generic-restricted：
.biz    - 供商业使用
.name   - 供家庭及个人使用
.pro    - 供部分专业使用

sponsored：
.aero   - 供航空运输业使用
.asia   - 供亚太地区的公司、组织及个人使用
.cat    - 供加泰罗尼亚语/文化使用
.coop   - 供联合会 (cooperatives)使用
.edu    - 供教育机构使用
.gov    - 供美国政府及其属下机构使用
.int    - 供由条约而成立的国际性机构使用
.jobs   - 供求职相关网站使用
.mil    - 供美国军事机构使用
.mobi   - 供手提电话等装置网站使用
.museum - 供博物馆使用
.post   - 供邮政服务使用
.tel    - 供连接电话网络与互联网的服务使用
.travel - 供旅行社、航空公司、酒店及旅游协会等机构使用
.xxx    - 供色情网站使用
```

### HTTP: 80
* 特点
    * 无状态协议
    * 非持续连接与持续连接

* 请求报文格式  
![http请求报文格式](../images/http.png)
    * 方法：`GET` `POST` `HEAD` `PUT` `DELETE`
    * 首部行：
        * Host:
        * Connection:
        * User-agent:
        * Accept-language:
        * If-modified-since:
        * Cookie:

* 响应报文格式  
![http响应报文格式](../images/httpd.png)
    * 方法：`GET` `POST` `HEAD` `PUT` `DELETE`
    * 状态码：
        * 200：OK
        * 301：Moved Permanently
        * 400：Bad Request
        * 404：Not Found
        * 505：HTTP Version Not Supported
    * 首部行：
        * Connection:
        * Server:
        * Date:
        * Last-Modified:
        * Content-Length:
        * Content-Type:
        * Set-cookie:

* Cookie：
    * 客户端：记录网站的访问历史，以及本机在该网站上的cookie标志
    * 服务器：记录访问本网站的主机信息、cookie标志及访问记录等信息，记录状态

* Web缓存/代理：缓存http报文，平均0.2~0.7的命中率

* DASH：基于HTTP的动态适应性流
    * 视频编码为多个清晰度的版本
    * 下载时动态选择将要下载的块的版本

### SMTP: 25
* 特点
    * SMTP握手
    * 持续连接
    * ASCII7

* 一般流程
    * 发送方发送报文给SMTP服务器
    * SMTP服务器利用SMTP协议转发报文给接收方SMTP服务器（循环尝试，失败则发邮件提醒发送方）
    * 接收方连接SMTP服务器接收报文，并存入接收方邮箱
    * 接收方通过客户端与其SMTP服务器通讯获取邮件

* 客户端与邮件服务器通讯
    * POP3
    * IMAP
    * HTTP

### BitTorrent
* 特点
    * 文件分块
    * 同时下载和上传文件块

* 一般流程
    * 向追踪器注册自己
    * 追踪器随机提供一个对等方子集
    * 并行连接所有对等方子集
    * 周期性向对等方询问其已有块列表
    * 应该选择下载哪个块：最稀缺优先原则
    * 响应哪些邻居请求：疏通

## 传输层
### TCP
* 全双工连接
* 可靠数据传输
* 拥塞控制机制
