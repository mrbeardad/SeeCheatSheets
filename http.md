# HTTP（超文本传输协议）

- [HTTP（超文本传输协议）](#http超文本传输协议)
  - [报文](#报文)
    - [请求报文](#请求报文)
    - [响应报文](#响应报文)
    - [资源](#资源)
    - [方法](#方法)
    - [状态](#状态)
  - [连接](#连接)
  - [缓存](#缓存)
  - [重定向](#重定向)
  - [代理](#代理)
  - [会话](#会话)
  - [认证](#认证)
  - [安全](#安全)
    - [Cookie](#cookie)
    - [HTTPS](#https)
    - [限制浏览器访问](#限制浏览器访问)

## 报文

编码：utf8（不存在大小端问题）

### 请求报文

```txt
方法 资源 协议版本
请求首部: 值
通用首部: 值
实体首部: 值

主体
```

Body 大致可分为两类：

- 单资源：由一个单文件组，该类型主体由两个首部定义： `Content-Type` 和 `Content-Length`
- 多资源：由多部分主体组成，每一部分包含不同的信息位。通常是和 HTML Forms 连系在一起。

### 响应报文

```txt
协议版本 状态码 状态描述
响应首部: 值
通用首部: 值
实体首部: 值

主体
```

Body 大致可分为三类：

- 单资源：由已知长度的单个文件组成，该类型主体由两个首部定义：`Content-Type` 和 `Content-Length`
- 单资源：由未知长度的单个文件组成，通过定义首部 `Transfer-Encoding:chunked` 使用 chunks 编码。
- 多资源：由多部分主体组成，每部分包含不同的信息段，这是比较少见的。

### 资源

**资源可能是静态文件，也可能是请求后台服务产生的动态数据。**

- URL

```txt
http://www.example.com:80/path/to/index.html?key1=value1&key2=value2#anchor
^------^--------------^--^------------------^-----------------------^------^
scheme    domain      port      path                query           fragment
```

- 片段`#anchor`不随 http 传输，仅用于浏览器定位页面片段
- 相对 URL
- 安全字符与非安全字符的转义

### 方法

| 方法    | 描述           | 请求包含主体 | 成功响应包含主体 | 幂等 | 安全 | 缓存                                      |
| ------- | -------------- | ------------ | ---------------- | ---- | ---- | ----------------------------------------- |
| HEAD    | 获取响应状态码 | no           | no               | yes  | yes  | yes                                       |
| GET     | 获取资源       | no           | yes              | yes  | yes  | yes                                       |
| PUT     | 替换资源       | yes          | no               | yes  | no   | no                                        |
| PATCH   | 修改资源       | yes          | no               | no   | no   | no                                        |
| POST    | 创建资源       | yes          | yes              | no   | no   | Only if freshness information is included |
| DELETE  | 删除资源       | no           | no               | yes  | no   | no                                        |
| OPTION  | 预检请求       | no           | yes              | yes  | yes  | no                                        |
| CONNECT | 建立隧道       | no           | yes              | no   | no   | no                                        |
| TRACE   | 路径测试       | no           | no               | yes  | no   | no                                        |

### 状态

| 状态码 | 状态描述                      | 备注           |
| ------ | ----------------------------- | -------------- |
| 200    | OK                            | 请求成功       |
| 301    | Moved Permanently             | 永久重定向     |
| 302    | Found                         | 临时重定向     |
| 304    | Not Modified                  | 用于缓存校验   |
| 400    | Bad Request                   | 客户端请求错误 |
| 401    | Unauthorized                  | 未验证         |
| 403    | Forbidden                     | 未授权         |
| 404    | Not Found                     | 资源不存在     |
| 407    | Proxy Authentication Required | 代理未验证     |
| 500    | Internal Server Error         | 服务器处理错误 |
| 502    | Bad Gateway                   | 上游服务器出错 |

## 连接

- HTTP/1.0（短连接）：
  - 管理方便，一次连接只进行一次请求-响应
  - TCP 连接建立与断开的开销，以及慢启动导致的高延迟
- HTTP/1.1（长连接）：
  - 节省 TCP 连接的建立与关闭的开销，并减少慢启动
  - 一次连接中可进行多次串行的”请求-响应“
  - 静态资源数据长度由`Content-Length`指明，动态资源数据长度通过分块传输编码判断
  - 服务端需要为每个客户端连接保留资源，一般存在保活机制来探测无效连接并关闭之
  - ~~管道化连接选项~~（不稳健）：请求发起不必等待前一个请求的响应结束，但响应时必须按请求顺序
- HTTP/2：
  - 首部压缩
  - 二进制帧
  - 数据流
  - 多路复用
  - 服务器推送
- HTTP/3：
  - 利用 UPD+QUIC 代替 TCP，解决重传等待

## 缓存

| Cache-Control 响应 | 备注                                                                        |
| ------------------ | --------------------------------------------------------------------------- |
| `private`          | 仅限私有缓存，即浏览器本地缓存，一般需要显示指定                            |
| `public`           | 允许共享缓存，即反向代理缓存，当响应具有`Authorization`的请求时需要显示指定 |
| `max-age`          | 指定缓存从源服务器产生（`Date`）开始计算的保持 fresh 的流逝时间             |
| `s-maxage`         | 同上，特定于共享缓存（隐式`public`）                                        |
| `no-cache`         | 每次复用缓存都必须校验                                                      |
| `must-revalidate`  | 即使与服务器断开连接，也必须校验缓存不能直接复用                            |
| `proxy-revalidate` | 同上，特定于共享缓存                                                        |
| `no-store`         | 禁用缓存                                                                    |
| `no-transform`     | 指明不可转换响应内容，一些中介会修改响应内容比如降低图片分辨率              |
| `immutable`        | 指定缓存在 fresh 期间绝不更改而忽略客户端强制校验缓存，一般用于缓存破坏模式 |

| Cache-Control 请求 | 备注                                                              |
| ------------------ | ----------------------------------------------------------------- |
| `no-cache`         | 强制缓存校验                                                      |
| `max-age`          | 指定覆盖`max-age`，超过该限制则不能复用该缓存，一般勇士浏览器刷新 |
| `no-transform`     | 指明不可转换响应内容，一些中介会修改响应内容比如降低图片分辨率    |

| 其他首部            | 类型 | 备注                                                        |
| ------------------- | ---- | ----------------------------------------------------------- |
| `Expires`           | 响应 | 指定缓存保持 fresh 的时间点而非流逝时间（不建议）           |
| `Vary`              | 响应 | 加上 Vary 指定的首部作为缓存标志定位，而非仅用 URL 定位缓存 |
| `Last-Modified`     | 响应 | 上次修改时间                                                |
| `ETag`              | 响应 | 用于缓存校验，一般为哈希值                                  |
| `If-Modified-Since` | 请求 | 缓存校验，对应`Last-Modified`                               |
| `If-None-Match`     | 请求 | 缓存校验，对应`Etag`，优先于上条                            |

缓存机制综述：

- 缓存分为 private 缓存与 public 缓存
- 缓存状态有 fresh 与 stale，根据`max-age`与`Date`或`Age`确定
- fresh 缓存可直接重用，stale 缓存需要校验后才能重用
- 当与源服务器断开连接时允许使用 stale 缓存
- 启发式缓存：未指定`Cache-Control`也可进行缓存，一般`max-age`=(`Last-Modified`-`Date`) / 10
- 缓存折叠：缓存代理同时接收多个相同请求时，只向源服务器发送一个请求，除非指定`Cache-Control: private`
- 缓存破坏：给静态资源名添加版本号（如哈希等），修改后 URL 不同导致缓存失效

## 重定向

## 代理

## 会话

## 认证

## 安全

### Cookie

响应首部设置`Set-Cookie:`来记录会话信息

- id=guid;
- Expires=Wed, 21 Oct 2015 07:28:00 GMT;
  > 默认会话期 cookie，指定过期时间
- Max-Age=3600;
  > 默认会话期 cookie，指定过期秒数
- Domain=example.com;
  > 默认为响应端 host 且不包含子域名，显式给值则包含子域名
- Path=/doc;
  > 允许匹配子路径
- Secure;
  > 仅能通过 https 访问
- HttpOnly;
  > 无法被 js 脚本访问
- SameSite=;
  - None 表示允许跨站请求发送 cookie
  - Lax 表示仅当导航到 URL 才发送 cookie
  - Strict 表示仅访问相同站点时发送 cookie

**Same-Origin**：若两 url 的 schema, host, port 均相同则为同域，否则为跨域

**Same-Site**：若两 url 减去有效一级域名后的二级域名相同则为同站，否则为跨站

### HTTPS

**HTTPS**
使用 HTTP 协议加载动态资源(如 javascript)会被浏览器阻止

**HSTS(HTTP Strict Transport Security)**
响应首部设置`Strict-Transport-Security:`以指示浏览器重定向至 https

- max-age: 指定秒数内均重定向访问至 https
- includeSubDomains: 重定向包括子域名
- preload: 加入浏览器 HSTS List 从而省略第一次报文用于指示重定向 https

**HPKP(HTTP Public Key Pinning)**
响应首部设置`Public-Key-Pins:`来让浏览器使用固定公钥访问网站

- max-age: 指定秒数内使用固定公钥
- includeSubDomains: 包括子域名

### 限制浏览器访问

**预防 CSRF(Cross-site request forgeries)**：

- 检测 Origin/Referer
- 验证码
- anti-CSRF tokens
- Cookie SameSite
- 浏览器本身会限制跨域访问（如 fetch()无法读取跨域报文内容）
- `Access-Control-Allow-Origin: *`允许浏览器跨域访问

**嵌套框架限制**：
站点可以通过确保网站没有被嵌入到别人的站点里面，从而避免 clickjacking 攻击

- X-Frame-Options: deny
- X-Frame-Options: sameorigin
- X-Frame-Options: allow-from https://example.com/

**内容安全策略 CSP**：
响应首部设置`Content-Security-Policy`提供策略来限制浏览器哪些资源仅能从哪些地方获取，启用 CSP 会使浏览器禁用内联 js 与 css

**内容类型限制**：
相应首部设置`X-Content-Type-Options: nosiniff`使下面两种情况的请求将被阻止：

- 请求类型是"style" 但是 MIME 类型不是 "text/css"，
- 请求类型是"script" 但是 MIME 类型不是 JavaScript MIME 类型。

**Referer 首部限制**：
响应头设置`Referer-Policy:`限制浏览器如何设置 Referer

- no-referer: never send the Referer header
- same-origin: send referrer, but only on requests to the same origin
- strict-origin: send referrer to all origins, but only the URL sans path (e.g. https://example.com/)
- strict-origin-when-cross-origin: send full referrer on same origin, URL sans path on foreign origin
