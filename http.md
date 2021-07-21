# 目录
<!-- vim-markdown-toc GFM -->

- [报文](#报文)
- [事务](#事务)
- [资源](#资源)
- [连接](#连接)
- [web结构组件](#web结构组件)

<!-- vim-markdown-toc -->

# 报文

请求报文格式
> 方法 URI 协议版本

![http请求报文格式](images/http.jpg)

响应报文格式
> 协议版本 状态码 描述

![http响应报文格式](images/httpd.png)


# 事务
| 方法    | 描述                                               | 是否包含主体 |
|---------|----------------------------------------------------|--------------|
| GET     | 安全且幂等的从服务器获取资源                       | 否           |
| HEAD    | 同GET但响应不包含body                              | 否           |
| POST    | 向指定资源新增或提交数据，可缓存                   | 是           |
| PUT     | 幂等地创建或替换指定资源                           | 是           |
| DELETE  | 删除指定资源                                       | 否           |
| CONNECT | 建立隧道                                           | 否           |
| TRACE   | 对可能经过代理服务器传送到服务器上去的报文进行追踪 | 否           |
| OPTIONS | 决定可以在服务器上执行哪些方法                     | 否           |

| 状态码 | 描述                  | 备注                         |
|--------|-----------------------|------------------------------|
| 200    | OK                    | 若非HEAD请求则响应包含body   |
| 204    | No Content            | 响应不包含body               |
| 206    | Partial Content       | 应用于分块下载与断点续传     |
| 301    | Moved Permanently     | 永久重定向                   |
| 302    | Moved Temporarily     | 临时重定向                   |
| 304    | Not Modified          | 资源未修改而重定向缓存文件   |
| 400    | Bad Request           | 请求报文有误                 |
| 403    | Forbidden             | 禁止访问                     |
| 404    | Not Found             | 资源不存在                   |
| 500    | Internal Server Error | 服务器发生错误               |
| 501    | Not Implemented       | 请求功能未支持               |
| 502    | Bad Gateway           | 网关或代理访问后端服务器出错 |
| 503    | Service Unavailable   | 服务器忙                     |

| 首部              | 字段值                               |
|-------------------|--------------------------------------|
| Host              | 目的主机名`google.com`               |
| User-Agent        | 浏览器类型`Mozilla/5.0`              |
| Connection        | 连接方式`close\|Keep-Alive`          |
| Accept            | 内容类型`*/*`                        |
| Accept-Charset    | 字符编码`ISO-8859-1`                 |
| Accept-Encoding   | 压缩格式`gzip`                       |
| Accept-Language   | 自然语言`zh-CN`                      |
| Content-Length    | 本次响应数据长度                     |
| Content-Type      | 本次响应数据格式                     |
| Content-Encoding  | 本次响应数据压缩方法                 |
| If-Modified-Since | 是否已变更`Wed, 9 Sep 2015 09:23:24` |
| Cookie            | 客户端存储目的域名的Cookie           |
| Reference         | 由`URL`跳转而来                      |
> 分行写时次行必须以space或tab开头


# 资源
资源可能是静态文件，也可能是从程序生成的动态数据
* URI
    * URN
    * URL：`<scheme>://<user>:<password>@<host>:<port>/<path>;<params>?<query>#<frag>`
        * 相对URL（可移植性）
        * 自动扩展URL
        * 安全字符与非安全字符的转义


# 连接
* 短连接：
    * 管理方便，一次连接只进行一次请求-响应
    * TCP连接建立与断开的开销，以及慢启动导致的高延迟
* 长连接：
    * 节省TCP连接的建立与关闭的开销，并减少慢启动
    * 一次连接中可进行多次串行的请求-响应
    * 静态资源数据长度由`Content-Length`指明，动态资源数据长度通过分块传输编码判断
    * 服务端需要为每个客户端连接保留资源，服务端一般存在保活机制来探测无效连接并关闭之
* 管道化连接：
    * 请求发起不必等待前一个请求的响应结束，但响应时必须按请求顺序，从而可造成队头阻塞
* HTTP/2：
    * 头部压缩
    * 二进制帧
    * 数据流
    * 多路复用
    * 服务器推送
* HTTP/3：
    * 利用UPD+QUIC代替TCP，解决重传等待

* 时延性能聚焦：
    * 连接建立
    * 拥塞控制（尤其慢启动）
    * 数据聚集的Nagle算法（尽量填充MSS）
    * 延迟累计确认
    * TIME_WAIT时延与端口耗尽

# web结构组件
* 缓存
* 代理
* 网关
* 隧道
* Agent代理

* Cookie：
    * 客户端：记录本机在服务器上对应的Cookie号
    * 服务器：记录客户端在本机上对应的Cookie号，以及相应的用户信息。

* Web代理/缓存：
    * 进行代理访问
    * 缓存http报文

* DASH：基于HTTP的动态适应性流  
    将视频编码为多个清晰度的版本，并将每个版本的视频切分为块，下载时动态选择将要下载的块的版本
