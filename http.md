# HTTP（超文本传输协议）

- [HTTP（超文本传输协议）](#http超文本传输协议)
  - [报文](#报文)
    - [方法](#方法)
    - [资源](#资源)
    - [首部](#首部)
    - [状态](#状态)
  - [安全](#安全)
    - [Cookie](#cookie)
    - [HTTPS](#https)
    - [限制浏览器访问](#限制浏览器访问)
  - [连接](#连接)

## 报文

请求报文格式

```txt
方法 URL 协议版本\r\n
首部:值\r\n
...
\n
主体
```

响应报文格式

```txt
协议版本 状态码 描述\r\n
首部:值\r\n
...
\n
主体
```

- 对于浏览器前端开发来说，URL,Header,Body 都是可以传递参数的地方
- 对于服务器后端开发来说，Code,Header 是可以控制浏览器行为的信息

### 方法

| 方法   | 描述           | 请求包含主体 | 成功响应包含主体 | 安全 | 幂等 | 缓存                                      |
| ------ | -------------- | ------------ | ---------------- | ---- | ---- | ----------------------------------------- |
| HEAD   | 获取响应状态码 | no           | no               | yes  | yes  | yes                                       |
| GET    | 获取资源       | no           | yes              | yes  | yes  | yes                                       |
| POST   | 创建资源       | yes          | yes              | no   | no   | Only if freshness information is included |
| PUT    | 替换资源       | yes          | no               | no   | yes  | no                                        |
| DELETE | 删除资源       | no           | no               | no   | yes  | no                                        |
| PATCH  | 修改资源       | yes          | no               | no   | no   | no                                        |

### 资源

资源可能是静态文件，也可能是请求后台服务产生的动态数据

通用资源标识符 URI：

- URN
- URL：`<scheme>://<user>:<password>@<host>:<port>/<path>;<params>?<query>&<query>#<frag>`
  - 相对 URL
  - 安全字符与非安全字符的转义

### 首部

| 通用首部 | 值  | 描述 |
| -------- | --- | ---- |

| 请求首部             | 值                  | 描述 |
| -------------------- | ------------------- | ---- |
| Authorization:       |                     |      |
| Proxy-Authorization: |                     |      |
| Cookie:              | yummy_cookie=choco; |      |

| 响应首部            | 值      | 描述 |
| ------------------- | ------- | ---- |
| WWW-Authenticate:   |         |      |
| Proxy-Authenticate: |         |      |
| Set-Cookie:         | id=val; |      |

| 实体首部 | 值  | 描述 |
| -------- | --- | ---- |

### 状态

| 状态码 | 描述                  | 备注                          |
| ------ | --------------------- | ----------------------------- |
| 200    | OK                    | 若非 HEAD 请求则响应包含 body |
| 204    | No Content            | 响应不包含 body               |
| 206    | Partial Content       | 应用于分块下载与断点续传      |
| 301    | Moved Permanently     | 永久重定向                    |
| 302    | Moved Temporarily     | 临时重定向                    |
| 304    | Not Modified          | 资源未修改而重定向缓存文件    |
| 400    | Bad Request           | 请求报文有误                  |
| 403    | Forbidden             | 禁止访问                      |
| 404    | Not Found             | 资源不存在                    |
| 500    | Internal Server Error | 服务器发生错误                |
| 501    | Not Implemented       | 请求功能未支持                |
| 502    | Bad Gateway           | 网关或代理访问后端服务器出错  |
| 503    | Service Unavailable   | 服务器忙                      |

## 安全

### Cookie

响应头部设置`Set-Cookie:`来记录会话信息

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
响应头部设置`Strict-Transport-Security:`以指示浏览器重定向至 https

- max-age: 指定秒数内均重定向访问至 https
- includeSubDomains: 重定向包括子域名
- preload: 加入浏览器 HSTS List 从而省略第一次报文用于指示重定向 https

**HPKP(HTTP Public Key Pinning)**
响应头部设置`Public-Key-Pins:`来让浏览器使用固定公钥访问网站

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
响应头部设置`Content-Security-Policy`提供策略来限制浏览器哪些资源仅能从哪些地方获取，启用 CSP 会使浏览器禁用内联 js 与 css

**内容类型限制**：
相应头部设置`X-Content-Type-Options: nosiniff`使下面两种情况的请求将被阻止：

- 请求类型是"style" 但是 MIME 类型不是 "text/css"，
- 请求类型是"script" 但是 MIME 类型不是 JavaScript MIME 类型。

**Referer 头部限制**：
响应头设置`Referer-Policy:`限制浏览器如何设置 Referer

- no-referer: never send the Referer header
- same-origin: send referrer, but only on requests to the same origin
- strict-origin: send referrer to all origins, but only the URL sans path (e.g. https://example.com/)
- strict-origin-when-cross-origin: send full referrer on same origin, URL sans path on foreign origin

## 连接

- 短连接：
  - 管理方便，一次连接只进行一次请求-响应
  - TCP 连接建立与断开的开销，以及慢启动导致的高延迟
- 长连接：
  - 节省 TCP 连接的建立与关闭的开销，并减少慢启动
  - 一次连接中可进行多次串行的请求-响应
  - 静态资源数据长度由`Content-Length`指明，动态资源数据长度通过分块传输编码判断
  - 服务端需要为每个客户端连接保留资源，服务端一般存在保活机制来探测无效连接并关闭之
- 管道化连接：
  - 请求发起不必等待前一个请求的响应结束，但响应时必须按请求顺序，从而可造成队头阻塞
- HTTP/2：
  - 头部压缩
  - 二进制帧
  - 数据流
  - 多路复用
  - 服务器推送
- HTTP/3：
  - 利用 UPD+QUIC 代替 TCP，解决重传等待
