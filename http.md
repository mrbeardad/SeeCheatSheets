# HTTP（超文本传输协议）

- [HTTP（超文本传输协议）](#http超文本传输协议)
  - [报文](#报文)
    - [请求报文](#请求报文)
    - [响应报文](#响应报文)
    - [资源](#资源)
    - [方法](#方法)
    - [状态码](#状态码)
  - [连接](#连接)
  - [重定向](#重定向)
  - [缓存](#缓存)
  - [认证](#认证)
  - [安全](#安全)
    - [HTTP 严格传输安全(HSTS)](#http-严格传输安全hsts)
    - [MIME 类型嗅探限制](#mime-类型嗅探限制)
    - [嵌套框架限制](#嵌套框架限制)
    - [Cookie 安全](#cookie-安全)
    - [跨源资源共享(CORS)](#跨源资源共享cors)
    - [内容安全策略(CSP)](#内容安全策略csp)

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
- 单资源：由未知长度的单个文件组成，通过定义首部 `Transfer-Encoding: chunked` 使用 chunks 编码。
- 多资源：由多部分主体组成，每部分包含不同的信息段，这是比较少见的。

### 资源

```txt
        origin                                uri
v------------------------v-------------------------------------------------v
http://www.example.com:80/path/to/index.html?key1=value1&key2=value2#anchor
^------^--------------^--^------------------^-----------------------^------^
 scheme    host       port      path                query           fragment
```

- 片段`#anchor`不随 http 传输，仅用于浏览器定位页面片段
- 绝对 URL 与相对 URL
- 安全字符与非安全字符的转义

### 方法

- 首先根据请求与响应是否包含主体来选择方法，若无法确定则再根据幂等或安全与否再选择。

| 方法    | 描述           | 请求包含主体 | 成功响应包含主体 | 幂等 | 安全 | 缓存                                 |
| ------- | -------------- | ------------ | ---------------- | ---- | ---- | ------------------------------------ |
| GET     | 获取资源       | no           | yes              | yes  | yes  | yes                                  |
| POST    | 发送资源       | yes          | yes              | no   | no   | include `Cache-control` or `Expires` |
| PUT     | 创建或替换资源 | yes          | no               | yes  | no   | no                                   |
| PATCH   | 部分修改资源   | yes          | no               | no   | no   | no                                   |
| DELETE  | 删除资源       | no           | no               | yes  | no   | no                                   |
| HEAD    | 预查 GET       | no           | no               | yes  | yes  | yes                                  |
| OPTION  | 预检请求       | no           | yes              | yes  | yes  | no                                   |
| CONNECT | 建立隧道       | no           | yes              | no   | no   | no                                   |
| TRACE   | 路径测试       | no           | no               | yes  | no   | no                                   |

### 状态码

| 状态码 | 状态描述              | 备注           |
| ------ | --------------------- | -------------- |
| 200    | OK                    | 请求成功       |
| 400    | Bad Request           | 客户端请求错误 |
| 500    | Internal Server Error | 服务器处理错误 |
| 502    | Bad Gateway           | 上游服务器出错 |

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

## 重定向

| 状态码 | 描述               | 处理方法                               | 应用场景                                                              |
| ------ | ------------------ | -------------------------------------- | --------------------------------------------------------------------- |
| 301    | Moved Permanently  | GET 方法不变，其他方法有可能会变为 GET | 网站重构                                                              |
| 308    | Permanent Redirect | 方法和消息主体都不发生变化             | 网站重构，用于非 GET 方法                                             |
| 302    | Found              | GET 方法不变，其他方法有可能会变为 GET | 网页意外不可用                                                        |
| 307    | Temporary Redirect | 方法和消息主体都不发生变化             | 网页意外不可用，用于非 GET 方法                                       |
| 300    | Multiple Choice    | 用户手动选择 html 中列出的重定向       | `<link rel="alternate" href="https://zh.example.com/" hreflang="zh">` |
| 304    | Not Modified       | 重定向到缓存                           | 缓存校验，表明缓存仍有效                                              |

- HTTP 重定向：`Location: URL`
- HTML 重定向：`<meta http-equiv="Refresh" content="0; URL=https://example.com/">`
- JavaScript 重定向：`window.location = "https://example.com/";`

## 缓存

- 缓存类型：缓存分为 private 缓存与 public 缓存
- 缓存状态：缓存状态有 fresh 与 stale，根据`max-age`与`Date`或`Age`确定
- 缓存校验：fresh 缓存可直接重用，stale 缓存需要校验成功后才能重用
- 复用旧缓存：当与源服务器断开连接时允许使用 stale 缓存
- 启发式缓存：即使未指定`Cache-Control`也可进行缓存，一般`max-age`=(`Last-Modified`-`Date`) / 10
- 缓存折叠：缓存代理同时接收多个相同请求时，只向源服务器发送一个请求，除非指定`Cache-Control: private`
- 缓存破坏：给静态资源名添加版本号（如哈希等），修改后 URL 不同导致缓存失效

| `Cache-Control` 响应首部 | 备注（`,`逗号分隔列表）                                                     |
| ------------------------ | --------------------------------------------------------------------------- |
| `private`                | 仅限私有缓存，即浏览器本地缓存，一般需要显示指定                            |
| `public`                 | 允许共享缓存，即反向代理缓存，当响应具有`Authorization`的请求时需要显示指定 |
| `max-age=`               | 指定缓存从源服务器产生（`Date`）开始计算的保持 fresh 的流逝时间             |
| `s-maxage=`              | 同上，特定于共享缓存（隐式`public`）                                        |
| `no-cache`               | 每次复用缓存都必须校验                                                      |
| `must-revalidate`        | 即使与服务器断开连接，也必须校验缓存不能直接复用                            |
| `proxy-revalidate`       | 同上，特定于共享缓存                                                        |
| `no-transform`           | 指明不可转换响应内容，一些中介会修改响应内容比如降低图片分辨率              |
| `immutable`              | 指定缓存在 fresh 期间绝不更改而忽略客户端强制校验缓存，一般用于缓存破坏模式 |
| `no-store`               | 禁用缓存                                                                    |

| `Cache-Control` 请求首部 | 备注（`,`逗号分隔列表）                                           |
| ------------------------ | ----------------------------------------------------------------- |
| `max-age=`               | 指定覆盖`max-age`，超过该限制则不能复用该缓存，一般用于浏览器刷新 |
| `no-cache`               | 强制缓存校验                                                      |
| `no-transform`           | 指明不可转换响应内容，一些中介会修改响应内容比如降低图片分辨率    |

| 其他相关首部                 | 类型 | 备注                                                                                    |
| ---------------------------- | ---- | --------------------------------------------------------------------------------------- |
| `Date`                       | 响应 | 响应报文创建时间                                                                        |
| `Age`                        | 响应 | 响应报文在代理中已缓存的时间                                                            |
| `Last-Modified`              | 响应 | 资源上次修改时间                                                                        |
| `ETag`                       | 响应 | 用于缓存校验，一般为哈希值                                                              |
| `Expires`                    | 响应 | 指定缓存保持 fresh 的时间点而非流逝时间                                                 |
| `Vary: <header>[, <header>]` | 响应 | 加上 Vary 指定的请求首部作为缓存标志而非仅用 URL，应用场景如`Origin, Accept-Encoding`等 |
| `If-Modified-Since`          | 请求 | 缓存校验，对应`Last-Modified`                                                           |
| `If-None-Match`              | 请求 | 缓存校验，对应`Etag`，优先于上条                                                        |

| 状态码 | 描述         | 备注               |
| ------ | ------------ | ------------------ |
| 304    | Not Modified | 作为缓存校验的响应 |

## 认证

| 状态码 | 描述                          | 备注       |
| ------ | ----------------------------- | ---------- |
| 401    | Unauthorized                  | 未认证     |
| 407    | Proxy Authentication Required | 代理未认证 |
| 403    | Forbidden                     | 未授权     |
| 404    | Not Found                     | 资源不存在 |

| 响应首部                                   | 备注             |
| ------------------------------------------ | ---------------- |
| `WWW-Authenticate: <type> realm=<realm>`   | 指定认证方案     |
| `Proxy-Authenticate: <type> realm=<realm>` | 指定代理认证方案 |

| 请求首部                                    | 备注         |
| ------------------------------------------- | ------------ |
| `Authorization: <type> <credentials>`       | 认证信息     |
| `Proxy-Authorization: <type> <credentials>` | 认证代理信息 |

## 安全

### HTTP 严格传输安全(HSTS)

- 浏览器会阻止在 HTTP（非安全）协议下加载动态资源，如 JavaScript
- HSTS 表明当前站点仅允许在 HTTPS 安全协议下访问
- `about://net-internals#hsts`中查看/删除 HSTS 列表

| `Strict-Transport-Security`响应首部 | 备注（`;`分号分隔列表）              |
| ----------------------------------- | ------------------------------------ |
| `max-age=`                          | 指定 HSTS 记忆有效时间               |
| `includeSubDomains`                 | 包括子域名也应用 HSTS                |
| `preload`                           | 使用 HSTS 预加载列表，由 google 维护 |

### MIME 类型嗅探限制

| `X-Content-Type-Options`响应首部 | 备注                                                 |
| -------------------------------- | ---------------------------------------------------- |
| `nosniff`                        | 若嗅探的 MIME 类型与`Content-Type`不匹配，则阻止请求 |

### 嵌套框架限制

| `X-Frame-Options`响应首部 | 备注（默认允许跨源嵌套） |
| ------------------------- | ------------------------ |
| `DENY`                    | 禁止任何形式的嵌套       |
| `SAMEORIGIN`              | 允许同源请求嵌套         |

### Cookie 安全

> `Site`表示两 URL 的 secheme 相同且 host 中的可注册域名相同（表示网站来自同一组织）

| `Set-Cookie`响应首部 | 备注（`;`分号分隔列表）                                                                                                               |
| -------------------- | ------------------------------------------------------------------------------------------------------------------------------------- |
| `<name>=<value>`     | name 不能包含控制字符、空白符和一些符号；value 可以用双引号来转移特殊字符；若需多个 name=value 对则添加多个`Set-Cookie`               |
| `Max-Age=`           | 指定 cookie 持久化时间，默认 session cookie                                                                                           |
| `Expires=`           | 指定 cookie 持久化时间点，默认 session cookie                                                                                         |
| `Domain=`            | 指定 cookie 对应的域名且包括子域名，默认当前域名且不含子域名                                                                          |
| `Path=`              | 指定 cookie 对应的路径且包括子路径，默认匹配所有路径                                                                                  |
| `Secure`             | 指定 cookie 仅在 https 协议下使用                                                                                                     |
| `HttpOnly`           | 指定 cookie 不能被 JS 访问                                                                                                            |
| `SameSite=`          | `Strict`拒绝任何跨站请求，（默认）`Lax`表示仅额外允许导航页面（更改浏览器地址栏）的跨站请求，`None`表示允许跨站请求但必须设置`Secure` |

| `Cookie`请求首部 | 备注（`;`分号分隔列表）             |
| ---------------- | ----------------------------------- |
| `<cookie-list>`  | 浏览器访问资源时会携带匹配的 cookie |

### 跨源资源共享(CORS)

- `XMLHttpRequest` 或 Fetch APIs 发起的跨源 HTTP 请求
- `drawImage()` 将 Images/video 画面绘制到 canvas。
- WebGL 贴图
- CSS 中通过 `@font-face` 使用跨源字体资源
- CSS Shape from images
- 可使用`crossorigin`属性的元素，如：`<link>`, `<img>`等等

由以上途径发起的跨源资源请求时，浏览器会先进行`OPTION`预检，若**目标资源服务器**允许**源服务器**跨源访问该资源后再进行正常资源访问。
脚本无法获取跨源访问出错报错信息，需要打开浏览器控制台查看。

简单请求不会触发 CORS 预检，典型的限制如下：

- 仅允许使用`HEAD`、`GET`与`POST`
- 仅允许使用简单首部（其中`Content-Type`限制为`text/plain`、`multipart/form-data`与`application/x-www-form-urlencoded`）
- 等等

| `crossorigin`属性 | 备注                                                                         |
| ----------------- | ---------------------------------------------------------------------------- |
|                   | 默认不会限制 html 标签链接的跨源访问                                         |
| `""`,`anonymous`  | 允许 CORS 预检                                                               |
| `use-credentials` | 允许跨源访问携带凭证(Cookie, Authorization Headers, TLS Client Certificates) |

| 请求首部                                                        | 备注                       |
| --------------------------------------------------------------- | -------------------------- |
| `Origin: <origin>`                                              | 指明跨源请求的源 Domain    |
| `Referer: <url>`                                                | 指明跨源请求的源 URL       |
| `Access-Control-Request-Method: <method>`                       | 预检时指明方法             |
| `Access-Control-Request-Headers: <field-name>[, <field-name>]*` | 预检时指明携带的自定义首部 |

| 响应首部                                               | 备注                                                                                           |
| ------------------------------------------------------ | ---------------------------------------------------------------------------------------------- |
| `Access-Control-Allow-Origin: <origin> 或 *`           | 允许跨源请求的源服务器，若指定为`<origin>`则应该添加首部`Vary: Origin`                         |
| `Access-Control-Allow-Methods: <method>[, <method>]*`  | 允许跨源请求的方法                                                                             |
| `Access-Control-Allow-Headers: <header>[, <header>]*`  | 允许跨源请求携带的用户自定义的非简单首部                                                       |
| `Access-Control-Max-Age: <delta-seconds>`              | 允许预检结果有效的时间，期间无需再次进行相同预检                                               |
| `Access-Control-Expose-Headers: <header>[, <header>]*` | 允许`getResponseHeader`访问获取非基本首部                                                      |
| `Access-Control-Allow-Credentials: true`               | 允许跨源请求携带凭证，否则浏览器忽略响应                                                       |
| `Referrer-Policy: strict-origin-when-cross-origin`     | 浏览器默认行为，同源请求发送 origin、path 和 query，跨源请求发送仅 origin，若非 HTTPS 则不发送 |

### 内容安全策略(CSP)

| `Content-Security-Policy`响应首部        | 备注                                                           |
| ---------------------------------------- | -------------------------------------------------------------- |
| `<policy-directive>; <policy-directive>` | 限制加载的各类型资源的来源(Origin)，这默认会禁用内联 js 与 css |

也可使用 HTML`<meta>`元素设置策略：

```html
<meta
  http-equiv="Content-Security-Policy"
  content="default-src 'self'; img-src https://*; child-src 'none';"
/>
```

详细策略参见[MDN](https://developer.mozilla.org/en-US/docs/Web/HTTP/Headers/Content-Security-Policy)
