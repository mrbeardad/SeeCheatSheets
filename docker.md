# Docker

- [Docker](#docker)
  - [基础概念](#基础概念)
  - [命令行](#命令行)
    - [镜像](#镜像)
      - [基础操作](#基础操作)
      - [远程仓库](#远程仓库)
      - [制作镜像](#制作镜像)
    - [容器](#容器)
      - [基础操作](#基础操作-1)
      - [运行容器](#运行容器)
      - [文件交互](#文件交互)
  - [网络模型](#网络模型)

## 基础概念

- 仓库：在本地或远程存储并管理镜像的系统

- 镜像：相当于一个 root 文件系统

- 容器：加载镜像创建的虚拟运行环境称为容器

## 命令行

### 镜像

#### 基础操作

> 所有`IMAGE`都可扩展为`REPO/IMAGE:TAG`

- docker image
  - ls
  - inspect IMAGE
  - history IMAGE
  - tag SRC_IMAGE TAG_IMAGE
  - rm IMAGE
  - prune -a

#### 远程仓库

- docker login
- docker logout
- docker search IMAGE
- docker pull IMAGE
- docker push IMAGE

#### 制作镜像

> save/load 与 export/import 的区别在于，
> 前者会保留 IMAGE 所有信息（包括 TAG, CMD），
> 而后者只是保留容器中的文件系统。

- docker save -o file.tar IMAGE
- docker load -i file.tar

- docker export -o file.tar CONT
- docker import file|URL|- IMAGE

- docker commit CONT IMAGE

- docker build -t IMAGE

```conf
# .dockerignoe

# 语法同 .gitignore
```

```conf
# Dockerfile
FROM        IMAGE:TAG           # 指定基础镜像
LABEL       maintainer="e@mail"
ENV         PROJECT=/app        # 设置环境变量，环境变量也能在Dockerfile命令中使用
WORKDIR     /app                # 指定工作目录
SHELL       ["bash", "-u"]      # 指定构建命令使用的默认SHELL
COPY        . /app              # 将当前目录下所有文件拷贝到镜像中的`/app`目录下
RUN         cmd                 # 在基础镜像的当前layer上执行该命令以更改镜像
EXPOSE      port                # 暴露指定端口
VOLUME      /path               # 指定挂载的VOLUME
STOPSIGNAL  signal              # 指定时容器exit的signal
ONBUILD     CMD cmd             # 指定当该镜像用作FROM时执行的命令
HEALTHCHECK CMD cmd             # 周期性检测容器是否正常（cmd返回0）
ENTRYPOINT  cmd                 # 设置入口命令（使用相对/绝对项目路径），`docker run IMAGE`后的参数传给该命令，包括CMD指定的参数。该入口命令一般为脚本，记得使用exec执行真正命令以防止SIGTERM被bash捕获
CMD         ["cmd", "args"]     # 设置容器启动命令，以JSON-List形式写命令则默认调用exec而非通过shell执行
```

### 容器

#### 基础操作

- docker container

  - create
  - start [-ia]
  - run
  - rename
  - rm [-v]
  - prune
  - update

  - stop CONT
  - restart CONT
  - pause CONT
  - unpause CONT
  - wait CONT
  - kill [-sKILL] CONT
  - attach CONT
  - exec [-it] CONT

  - ls [-a]
  - logs [-f] CONT
  - inspect CONT
  - top CONT
  - diff CONT

- docker stats
- docker events

#### 运行容器

- docker create|run [OPT] IMAGE CMD ARGS

  - `-it`：终端交互
  - `-d` ：后台运行（保存 stdout）
  - `--name NAME` ：指定容器的 NAME
  - `--rm` ：退出时自动清理容器
  - `--restart` ：指定为`no|on-failure|always|unless-stopped`
  - `-e` ：指定环境变量
  - `--workdir` ：指定工作目录

  - `-c [0-1024]` ：限制 CPU 使用率
  - `--cpu-sets=0,1` ：限制 CPU 核心使用
  - `-m 500M` ：限制内存使用

  - `--device=/dev/sda` ：指定挂载设备
  - `--privileged` ：挂载所有设备

  - `-v /h_path:/d_path` ：挂载本机路径`/h_path`到容器`/d_path`，若前者不存在则自动创建
  - `-v v_name:/d_path` ：冒号前不加`/`则表示指定 docker 在`/var/lib/docker/volumes`创建并挂载 Volume`v_name`
  - `-v /docker_path` ：指定 docker`/var/lib/docker/volumes`创建并挂载 Volume，volume 名称随机
  - `--volumes-form` ：挂载某个容器挂载的 volumes

  - `--network NET` ：指定 Docker 网络，默认提供`bridge`与`host`
  - `-P` ：将所有端口随机映射到宿主机
  - `-p HPORT:CPORT` ：指定容器 IP:容器端口:宿主机端口
  - `-h HOSTNAME` ：指定容器主机名，会写入容器中的`/etc/{hostname,hosts}`
  - `--dns=IP` ：指定容器 DNS，会写入容器中的`/etc/resolv.conf`
  - `--add-host=HOST:IP` ：指定容器 hostname 解析，写入`/etc/hosts`

- docker-compose

```yaml
## docker-compose.yml
version: "3" # 指定版本
services: # 指定需要运行的容器
  mysql: # 指定名称
    depends_on:
      - other_services
    images: "mysql:5.7" # 指定使用的镜像，或使用下述build
    build: . # 指定build需要的Dockerfile所在目录
    stdin_out: true # -i
    tty: true # -t
    workding_dir: /app # --workdir
    environment: # -e
      MYSQL_ALLOW_EMPTY_PASSWORD: 1
    container_name: db # --name
    restart: "always" # 与--start一样
    devices: # --device
      - "/dev/sda:/dev/sda"
    volume: # -v
      - "/h_path:/d_path"
      - "v_name:/d_path"
      - "/d_path"
    network_mode: "none" # --network host|bridge
    ports:
      - "3306:3306"
    hostname: "laptop" # -h
    extra_hosts: # --add-host
      - "googledns:8.8.8.8"
```

#### 文件交互

> `-`表示从 stdin 读取 tarball 并解压到容器中，或将容器中的文件打包成 tarball 输出到 stdout

- docker cp [-aL] CONT:SRC_PATH DEST_PATH|-
- docker cp [-aL] SRC_PATH|- CONT:DEST_PATH

## 网络模型

考虑下列情况：

- 容器 0 中运行了 httpd，地址为`172.17.0.2:80`
- 虚拟网桥 Docker0 的地址为`172.17.0.1`
- 物理网卡 wlp2s0 的地址为`192.168.0.106`

&emsp;此时从宿主机中打开浏览器，输入地址`172.17.0.2:80`即可访问容器中的 httpd，**此时数据包走向为**：

> 忽略传输层

1. **应用层**发起连接请求，目的地址`172.17.0.2:80`
2. **网络层**进行路由抉择，发现应该通过虚拟网桥 Docker0 `172.17.0.1`直接发送数据
3. **链路层**中虚拟网桥 Docker0 的驱动程序接受该数据包并转发给容器 0 `172.17.0.2`
4. 容器 0 中 httpd 响应连接并回传数据包，目的地址为`172.17.0.1`
5. **链路层**虚拟网桥 Docker0 的驱动程序接受到容器 0 的数据包
6. **网络层**进行路由抉择，发现数据包目的地址为本机，且存在应用监听该网卡
7. **传输层**进行端口查询，发现数据包目的端口存在应用监听
8. **应用层**浏览器处理响应

若运行镜像时指定了端口映射，docker 会自动创建 netfilter 钩子实现端口转发，以保证外部主机访问宿主机中容器内的服务。
