**Nginx的功能在于路由转发与负载均衡**
# 路由转发
* 模拟一台**server**，通过**listen**本地地址与**server_name**进行匹配
* 实现路由策略，通过**location**进行匹配
* 对请求包进行操作：
  * 接受并响应静态资源，或转发动态资源请求到后端服务，或拒绝请求
  * 协议转换
  * 修改报文内容
  * 缓存资源

```nginx
server {
  # 该虚拟主机监听地址与端口
  listen (address[:port]|port) [default_server] [ssl] [http2 | spdy];

  # listen到报文时匹配主机名（可能是ip或domain），支持通配符与正则表达式
  #1. 准确匹配server_name
  #2. 通配符在开始时匹配server_name成功
  #3. 通配符在结尾时匹配server_name成功
  #4. 正则表达式匹配server_name成功
  #5. 同一优先级则匹配第一个出现的
  server_name example.com ...;
    
  # 指定查找的根目录
  root /path/to/root
  # 当访问目录时内部重定向（重新匹配location）到的默认文件（URL尾部为/）
  index index.html

  # uri匹配，并进行路由操作，支持通配符与正则表达式
  # 正常匹配规则：
  # 先匹配标准uri，记录匹配度最高的一项；
  # 再匹配正则uri，记录第一个匹配后停止，使用该正则uri项；
  # 若正则uri未匹配成功，则使用标准uri匹配度最高项。
  # =：用于标准uri前，严格精准匹配，若匹配成功则停止
  # ^~：用于标准uri前，若该location匹配度在标准匹配中最高则直接使用该项而不进行正则匹配
  # ~：用于表示uri包含正则表达式，并且区分大小写。
  # ~*：用于表示uri包含正则表达式，并且不区分大小写
  location [=|~|~*|^~] uri {
    ...
  }
}
```


# 负载均衡
  * 为upstream指定一组服务器
  * 选择负载均衡策略：轮询、加权轮询、ip hash、url hash、热备
  * 将匹配到的请求包通过负载均衡算法转发给upstream中的一个
  
```nginx
# file: sites-enabled/default

# 负载均衡配置
upstream jh.w3cschool.cn {
    server 192.168.80.121:80 weight=3;
    server 192.168.80.122:80 weight=2;
    server 192.168.80.123:80 weight=3;

    #nginx的upstream目前支持4种方式的分配
    #1、轮询（默认）
    #每个请求按时间顺序逐一分配到不同的后端服务器，如果后端服务器down掉，能自动剔除。
    #2、weight
    #指定轮询几率，weight和访问比率成正比，用于后端服务器性能不均的情况。
    #例如：
    #upstream bakend {
    #    server 192.168.0.14 weight=10;
    #    server 192.168.0.15 weight=10;
    #}
    #2、ip_hash
    #每个请求按访问ip的hash结果分配，这样每个访客固定访问一个后端服务器，可以解决session的问题。
    #例如：
    #upstream bakend {
    #    ip_hash;
    #    server 192.168.0.14:88;
    #    server 192.168.0.15:80;
    #}
    #3、fair（第三方）
    #按后端服务器的响应时间来分配请求，响应时间短的优先分配。
    #upstream backend {
    #    server server1;
    #    server server2;
    #    fair;
    #}
    #4、url_hash（第三方）
    #按访问url的hash结果来分配请求，使每个url定向到同一个后端服务器，后端服务器为缓存时比较有效。
    #例：在upstream中加入hash语句，server语句中不能写入weight等其他的参数，hash_method是使用的hash算法
    #upstream backend {
    #    server squid1:3128;
    #    server squid2:3128;
    #    hash $request_uri;
    #    hash_method crc32;
    #}

    #每个设备的状态设置为:
    #1.down表示单前的server暂时不参与负载
    #2.weight为weight越大，负载的权重就越大。
    #3.max_fails：允许请求失败的次数默认为1.当超过最大次数时，返回proxy_next_upstream模块定义的错误
    #4.fail_timeout:max_fails次失败后，暂停的时间。
    #5.backup： 其它所有的非backup机器down或者忙的时候，请求backup机器。所以这台机器压力会最轻。

    #nginx支持同时设置多组的负载均衡，用来给不用的server来使用。
    #client_body_in_file_only设置为On 可以讲client post过来的数据记录到文件中用来做debug
    #client_body_temp_path设置记录文件的目录 可以设置最多3层目录
    #location对URL进行匹配.可以进行重定向或者进行新的代理 负载均衡
}

# 路由转发配置
server {
  # listen的本地地址与端口，并可指定协议
	listen 443 ssl http2 default_server;
	listen [::]:443 ssl http2 default_server;
  # hostname匹配
  server_name _;
  # ssl卸载
	ssl_certificate /srv/certificate/1_beardad.top_bundle.crt;
	ssl_certificate_key /srv/certificate/2_beardad.top.key;
  ssl_password_file /srv/certificate/password.txt;

  # 网站根目录
	root /srv/home-page;
  # 默认请求文件
	index index.html;
  
  location = / {
    # 匹配到location后到root中查询对应文件并响应前端
  }
  
  location /file {
    try_files $uri $uri/index.html =404;
  }

	location / {
		include /etc/nginx/proxy_params;
	  proxy_pass http://127.0.0.1:3000;
  }
}
```

# 例
```nginx
# file: nginx.conf

user www-data;
worker_processes auto;
pid /run/nginx.pid;

events {
    worker_connections 768;
    multi_accept off; # 开启后每个work进程一直轮询处理连接，而非利用kernel多路复用的事件驱动
}

http {
    # Basic Settings
    sendfile on;
    tcp_nopush on;
    tcp_nodelay on;
    keepalive_timeout 65;
    types_hash_max_size 2048;
    # server_tokens off;
    # server_names_hash_bucket_size 64;
    # server_name_in_redirect off;
    include /etc/nginx/mime.types;
    default_type application/octet-stream;

    # SSL Settings
    ssl_protocols TLSv1 TLSv1.1 TLSv1.2 TLSv1.3; # Dropping SSLv3, ref: POODLE
    ssl_prefer_server_ciphers on;

    # Logging Settings
    access_log /var/log/nginx/access.log;
    error_log /var/log/nginx/error.log;

    # Gzip Settings
    gzip on;
    # gzip_vary on;
    # gzip_proxied any;
    # gzip_comp_level 6;
    # gzip_buffers 16 8k;
    # gzip_http_version 1.1;
    # gzip_types text/plain text/css application/json application/javascript text/xml application/xml application/xml+rss text/javascript;

    # Virtual Host Configs
    include /etc/nginx/conf.d/*.conf;
    include /etc/nginx/sites-enabled/*;
}
```