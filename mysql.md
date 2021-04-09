# 安装与使用
<!-- entry begin: mysql mysqld initialize install -->
**初始化mysql**
* `mariadb-install-db --user=mysql --basedir=/usr --datadir=/var/lib/mysql`
* `mysqld --initialize --user=mysql --basedir=/usr --datadir=/var/lib/mysql`

**连接mysql**
* `mysql -h 主机名 -P 端口号 -D 数据库名 -u 用户名 -p密码`
<!-- entry end -->

# MySQL基础
## 系统信息
<!-- entry begin: sql variable status engines charset collation names -->
```sql
-- 默认变量作用域为SESSION
SET  [SESSION|GLOBAL] var = val;
SHOW [SESSION|GLOBAL] VARAIABLES [LIKE 'pattern'];
SHOW [SESSION|GLOBAL] STATUS     [LIKE 'pattern'];
SHOW PROCESSLIST;
SHOW ENGINES    [LIKE 'pattern'];
SHOW CHARSET    [LIKE 'pattern'];
SHOW COLLATION  [LIKE 'pattern'];
SET  NAMES charset;
-- SET character_set_client     = charset;  -- 客户端使用的字符集
-- SET character_set_connection = charset;  -- 服务器处理请求时使用的字符集
-- SET character_set_results    = charset;  -- 回传给客户端时使用的字符集
```
<!-- entry end -->

## 用户管理
<!-- entry begin: sql user password grant -->
```sql
SELECT user, host, password FROM mysql.user;    -- 查询用户
CREATE USER username [IDENTIFIED BY 'passwd'];  -- 创建用户
DROP   USER username;                           -- 删除用户
SET PASSWORD FOR username = PASSWORD('passwd'); -- 修改密码
GRANT  privcode ON db_name.tbl_name TO username;-- 授权用户
FLUSH  PRIVILEGES;                              -- 刷新权限
    -- 权限码包括：
    -- ALL PRIVILEGES
    -- SELECT
    -- INSERT
    -- 等等...
```
<!-- entry end -->

## 数据库
<!-- entry begin: sql database db -->
```sql
SHOW   DATABASES [LIKE 'pattern'];          -- 查询已有数据库
SHOW   CREATE DATABASE           db_name;   -- 查询数据库详情
CREATE DATABASE  [IF NOT EXISTS] db_name;   -- 创建数据库
DROP   DATABASE  [IF EXISTS]     db_name;   -- 删除数据库
```
<!-- entry end -->

## 数据表
```sql
SHOW   TABLES [FROM db_name | LIKE 'pattern'];  -- 查询已有数据表
SHOW   CREATE TABLE tbl_name;                   -- 查询数据表详情
CREATE TABLE tbl_name (                         -- 创建数据表
    fd_name type [const],
    ...,
    [PRIMARY KEY (fd_name),]
    [KEY         (fd_name),]
    [FOREIGN KEY (fd_name) REFERENCES tbl_name(fd_name),]
    [CHECK       (clause)]
) ENGINE=engine CHARSET=charset ROW_FORMAT=format;

DROP  TABLE tbl_name;                           -- 删除数据表
ALTER TABLE tbl_name RENAME TO new_tbl_name;    -- 修改数据表

-- 视图
CREATE VIEW <view_name> AS SELECT 语句;
DROP VIEW <view_name>;

-- 导入与导出
LOAD DATA LOCAL INFILE '<file_name>' INTO TABLE tbl_name;

SELECT * FROM tbl_name INTO OUTFILE '<file_name>';
```
<!-- entry end -->

## 数据列
<!-- entry begin: sql table index metadata -->
* 操作元数据
```sql
-- field
SHOW COLUMNS FROM tbl_name;
ALTER TABLE tbl_name ADD fd_name <type> [<const>] [FIRST| AFTER fd_name];
ALTER TABLE tbl_name DROP fd_name;
ALTER TABLE tbl_name CHANGE fd_name <new_fd_name> <type> [<const>];

-- index
SHOW INDEX FROM tbl_name;
ALTER TABLE tbl_name ADD INDEX <idx_name>(fd_name);
ALTER TABLE tbl_name DROP INDEX <idx_name>;

-- PRIMARY KEY
    -- 目标域必须为`NOT NULL UNIQUE`
ALTER TABLE tbl_name ADD PRIMARY KEY (fd_name);
ALTER TABLE tbl_name DROP PRIMARY KEY;

-- FOREIGN KEY
    -- 两域类型必须相同，且被引用的域必须为`UNIQUE`
ALTER TABLE tbl_name ADD CONSTRAINT <fk_name> FOREIGN KEY (fd_name) REFERENCES tbl_name(fd_name);
ALTER TABLE tbl_name DROP FOREIGN KEY <fk_name>;

-- CHECK
ALTER TABLE tbl_name ADD CONSTRAINT <ck_name> CHECK (<Clause>);
ALTER TABLE tbl_name DROP CHECK <ck_name>;
```
<!-- entry end -->

## 行记录
<!-- entry begin: sql insert delete 事务 -->
* 操作行数据
```sql
-- 插入
INSERT INTO tbl_name [(fd_name, ...)] VALUES    -- 外键列需要指定为该键所指向的列中已存在的值以进行关联
(<value>, ...), ... ;                               -- 若类型为字符串或日期时间，则`value`必须使用单引号

-- 复制
INSERT INTO tbl_name (fd_name, ...)
SELECT 语句 ;                            -- 两语句中的fd_name部分两两对应

-- 删除
DELETE FROM tbl_name
[WHERE <Clause>] ;                      -- 默认删除所有数据

-- 修改
UPDATE tbl_name SET fd_name=<value>, ...
[WHERE <Clause>] ;                      -- 默认修改所有行

-- 事务
BEGIN;                                  -- 开启事务
...
SAVEPOINT <point>;                      -- 设置保存点
...
RELEASE SAVEPOINT <point>;              -- 删除保存点
...
ROLLBACK TO <point>;                    -- 撤销至保存点
...
COMMIT; 或 ROLLBACK;                    -- 提交事务 或 撤销此次事务
```
<!-- entry end -->
## 数据类型
### 整数类型
> * 支持在其后添加`(N)`，指定十进制显示时的最小显示位数，最大255（不影响存储与计算） **deprecated** 
* 支持在其后添加`UNSIGNED`，指定为无符型整数
* 计算时统一使用`BIGINT`类型

| 类型      | 字节 |
|-----------|------|
| TINYINT   | 1    |
| SMALLINT  | 2    |
| MEDIUMINT | 3    |
| INT       | 4    |
| BIGINT    | 8    |

### 实数类型
> * 对于FLOAT与DOUBLE，支持在其后添加`(N, P)`，分别指定十进制显示时的最大有效位数与小数位数精度（影响存储大小限制） **deprecated** 
* 对于DECIMAL，支持在其后添加`(N, P)`，分别指定十进制显示时的最大有效位数与小数位数精度（默认`(10, 0)`）
* 计算时统一使用`DOUBLE`类型

| 类型    | 字节 |
|---------|------|
| FLOAT   | 4    |
| DOUBLE  | 8    |
| DECIMAL | ∞    |

### 时间日期
* 对于TIMESTAMP
    * 存储格林尼治时间而显示服务器或客户端本地时间
    * 默认`NOT NULL`
    * 默认插入或更新时会更新第一个TIMESTAMP列
* 除DATE外，其余类型支持在其后添加`(fps)`表示秒级小数位数

| 类型      | 字节 | 格式                |
|-----------|------|---------------------|
| DATE      | 3    | YYYY-mm-dd          |
| TIME      | 3    | HH:MM:SS            |
| TIMESTAMP | 4    | YYYY-mm-dd HH:MM:SS |
| DATETIME  | 8    | YYYY-mm-dd HH:MM:SS |

### 字符类型
* 对于CHAR，支持在其后添加`(N)`，指定固定字符数（字节数最小为N）
* 对于VARCHAY，支持在其后添加`(N)`，指定最大字符数
* 对于BINARY，支持在其后添加`(N)`，指定固定字节数
* 对于VARBINAY，支持在其后添加`(N)`，指定最大字节数
* 对于TEXT族与BLOB族，排序时仅比较前`max_sort_length`字节，且可能使用外部空间存储数据而在表中存储指针

| 类型            | 长度             | 说明                                 |
|-----------------|------------------|--------------------------------------|
| CHAR            | 0~255       字符 | 固定字符数，忽略输入的尾后空格       |
| VARCHAR         | 0~65535     字节 | 可变字符数                           |
| BINARY          | 0~255       字节 | 类似CHAR，但用`\0`填充且索引时不忽略 |
| VARBINARY       | 0~65535     字节 | 类似VARCHAR                          |
| TINYTEXT        | 0~255       字符 | 大字符串数据                         |
| SMALLTEXT(TEXT) | 0~65535     字符 | 大字符串数据                         |
| MEDIUMTEXT      | 0~16777215  字符 | 大字符串数据                         |
| LONGTEXT        | 0~4294967286字符 | 大字符串数据                         |
| TINYBLOB        | 0~255       字节 | 大二进制数据                         |
| SMALLBLOB(BLOB) | 0~65535     字节 | 大二进制数据                         |
| MEDIUMBLOB      | 0~16777215  字节 | 大二进制数据                         |
| LONGBLOB        | 0~4294967286字节 | 大二进制数据                         |

### 枚举类型
* 支持在其后添加`('str1', 'str2')`来设置枚举字符
    * 对于ENUM，枚举字符映射为1个整数
    * 对于SET，枚举字符映射为1个bit
* 对于ENUM，存储、计算、索引时使用数字，显示时使用映射的字符。
特别注意排序时也是按数字大小来，所以最好映射的字符顺序与数字顺序相对应

| 类型 | 说明                       |
|------|----------------------------|
| ENUM | 数字与字符串间的双射       |
| SET  | 给每一位取个可读性高的名称 |

### 位类型
> **deprecated**
> * 对于BIT，字面量语法位`b'1010'`。根据上下文语境可转换位字符或数字
> * 对于BIT，InnoDB默认使用最小的可以完整表示位的整数表示，故无法节省空间
> * 对于BIT，支持在其后添加`(N)`表示位数


## 类型约束
| 约束             | 说明         | 备注                                           |
|------------------|--------------|------------------------------------------------|
| `NOT NULL`       | 不能为空     | PRIMARY KEY的必要条件                          |
| `UNIQUE`         | 不能重复     | PRIMARY KEY的必要条件，UNIQUE KEY的充分条件    |
| `AUTO_INCREMENT` | 自动增加数值 | 其后可选添加`=n`设置初始值，默认为1，每次递增1 |
| `DEFAULT val`    | 设置默认值   |                                                |


# SQL基础
## 基础语法
**用户名**
* user              ：默认host为`'%'`
* user@host
    * '%'           ：允许从任意主机登录
    * localhost     ：只允许本机登录
    * 192.168.0.1   ：只允许从指定IP的主机登录

**库表列名**
* 库：
    * `db_name`
    * `*`
* 表：
    * `db_name.tbl_name`
    * `*.*`
    * `tbl_name`
    * `*`
* 列：
    * `db_name.tbl_name.fd_name`
    * `tbl_name.fd_name`
    * `fd_name`
    * `*`


## 查询语句
<!-- entry begin: sql select group order -->
```sql
-- 出现tbl_name、列表`(v1,v2)`的地方，一般都可用(SELECT语句)代替
SELECT [DISTINCT] fd_name或expression [AS fd_alias], ... FROM tbl_name [tbl_alias], ...
[[INNER|LEFT|RIGHT] JOIN tbl_name [tbl_alias]]  -- 连接多表，与在上一行中指定多表区别在于此处可外连接
[ON fd_name = fd_name]                          -- 类似WHERE，但对于外连接不会过滤掉驱动表的行
[WHERE Clause]                                  -- WHERE子句，用于条件过滤
[GROUP BY fd_name                               -- 将指定列相等的列聚合为一行，查询结果中非聚合函数的列为NULL
[WITH ROLLUP]]                                  -- 会将所有聚合后的行再聚合成一行并添加到最后
[HAVING (Clause)]                               -- 类似WHERR，可使用聚合函数
[ORDER BY fd_name [DESC], ...]                  -- 按指定顺序的列进行（升序）排序，DESC指定该列按降序排列
[LIMIT N]
[OFFSET M]
UNION [ALL]                                     -- 合并两查询结果（上下连接）。ALL表示允许重复，默认删掉重复值
SELECT 语句 ;
;
```
![JOIN](https://www.runoob.com/wp-content/uploads/2019/01/sql-join.png)
<!-- entry end -->


## WHERE语句
<!-- entry begin: sql where 条件 -->
* where语句
    * `WHERE fd_name <OP> <value>`
    * `WHERE fd_name [NOT] REGEXP 'regex_pattern'`
    * `WHERE fd_name [NOT] BETWEEN <value> AND <value>`
    * `WHERE fd_name [NOT] IN (<value>, ...)`
<!-- entry end -->

## 运算符
<!-- entry begin: sql operator -->
* sql运算符
    * 逻辑：`NOT` `AND`、`OR`、`XOR`        ：可以用`()`来改变优先级
    * 算术：`+` `-` `*` `/` `%`
    * 比较：
        * `=` `!=` `<` `<=` `>` `=>`        ：若有一边为NULL则会返回false
        * `<=>`                             ：当两边相等或均为NULL返回true
    * 关系：
        * `IS [NOT] NULL <value>`
        * `[NOT] REGEXP '<pattern>'`
        * `[NOT] BETWEEN <value1> AND <value2>`
        * `[NOT] IN (SELECT 语句)`
        * `[NOT] EXISTS (SELECT 语句)`
        * `<OP> ALL (SELECT 语句)`          ：列表中所有行都符合`<OP>`
        * `<OP> ANY (SELECT 语句)`          ：列表中有一行符合`<OP>`
<!-- entry end -->

## 函数
<!-- entry begin: sql function -->
* SQL Aggregate 函数计算从完整的列中取得的所有值，返回一个单一的值

| 函数    | 描述           |
|---------|----------------|
| SUM()   | 返回总和       |
| AVG()   | 返回平均值     |
| COUNT() | 返回行数       |
| FIRST() | 返回第一个值   |
| LAST()  | 返回最后一个值 |
| MAX()   | 返回最大值     |
| MIN()   | 返回最小值     |

* 用于blob的函数

| 函数                  | 描述                 |
|-----------------------|----------------------|
| HEX()                 | 输出十六进制底层数据 |
| SUBSTRING(fd,beg,len) | 截取字节，beg从1开始 |
| CONCAT(s, s)          | 拼接两数据           |
| CONV(s,hex,dec)       | 十六进制转十进制     |
| LOAD_FILE(str)        | 返回文件内容         |

* [其它sql函数](https://www.runoob.com/mysql/mysql-functions.html)
<!-- entry end -->

# 数据库原理
## 系统架构
![mysql](images/mysql_arch.png)

* 连接与线程管理
* 语法解析与查询优化
* 存储引擎

## 存储引擎架构
![ibplru](images/innodb-architecture.png)

### 内存结构
#### 缓冲池
缓冲池是主内存中的一个区域，在InnoDB访问表和索引数据时会在其中进行 高速缓存。
缓冲池允许直接从内存访问经常使用的数据，从而加快了处理速度。
为了提高大容量读取操作的效率，缓冲池被划分为多个页面，这些页面可以潜在地容纳多行。
为了提高缓存管理的效率，缓冲池被实现为页面的链接列表。
使用最近最少使用（LRU）算法的变体，将很少使用的数据从缓存中老化掉。

![lru](images/innodb-buffer-pool-list.png)

普通LRU算法的缺点：
> 以下读取的数据的访问频率很可能很低，如果用普通LRU算法则很容易将一些真正的热点数据给驱逐掉
* 线性预读：如果顺序访问了某个区的`innodb_read_ahead_threshold`个页面，则据异步读取下一个区中全部页面到缓冲池
* 随机预读：如果已经缓存了某个区的13个连续页面，且页面处于LRU Young子列表的前1/4，则读取本区所有其他页面到缓冲池
* 全表扫描

改良版的LRU算法优点如下：
* 会将从磁盘读取的新页面插入到Old子列表头部，保护热点数据不容易被驱逐；
* 在第一次读取页面时会记录读取时间，只有在此刻之后`innodb_old_blocks_time`的读取才会将其加入到young子列表头部
* 访问yuong子列表中的热点数据时，为减少链表调整频率，仅当访问young后1/4的数据才会将其调整到young子列表头部
* 冲刷脏页时的两种途径：后台线程定时冲刷flush链表；冲刷Old子列表中的冷数据

#### 更改缓冲区
当对表执行INSERT、UPDATE和DELETE操作时，索引列的值(尤其是辅助键的值)通常是无序的，需要大量的 I/O 才能使辅助索引保持最新状态。
当相关的页面不在缓冲池中时，更改缓冲区缓存对辅助索引条目的更改，从而通过不立即从磁盘读取页面来避免昂贵的 I/O 操作。
当页面加载到缓冲池中时，缓冲的更改将合并，更新的页面随后将刷新到磁盘。
InnoDB主线程在服务器接近空闲时和slow shutdown期间合并缓冲的更改。
由于更改缓冲区功能可以减少磁盘读写操作，因此它对于受 I/O 约束的工作负载(例如，具有大量 DML 操作的应用程序，例如批量插入)最有价值。
但是，更改缓冲区占用了缓冲池的一部分，从而减少了可用于缓存数据页的内存。
如果工作集几乎适合缓冲池，或者表具有相对较少的二级索引，则禁用更改缓冲可能很有用。
如果工作数据集完全适合缓冲池，则更改缓冲不会带来额外的开销，因为它仅适用于不在缓冲池中的页面。

#### 自适应哈希索引
当注意到某些索引值被非常频繁的使用时，会在内存中基于B+Tree索引上再创建一个哈希索引；
这样就让B+Tree索引具有一些哈希索引的优点，比如快速的哈希查找。

#### 日志缓冲区
将日志冲刷到磁盘RedoLog中去；
缓冲区大小由`innodb_log_buffer_size`控制，默认16M；
冲刷方式由`innodb_flush_log_at_trx_commit`控制；
冲刷频率由`innodb_flush_log_at_timeout`控制；


### 磁盘结构
#### 表空间结构
##### 行记录结构
![row](images/mysql_row.png)

**行溢出处理**：  
行记录数据列最长为65535字节（不计算头记录，不计算隐藏列，不计算TEXT与BLOB族的类型）；
而数据页一般最大为16384字节(16KB)，且规定一页至少要放两行用户记录；
于是针对过长的VARCHAR、TEXT、BLOB类型数据进行行溢出处理。
* Compact处理方式：将部分数据存储于本页，其余部分存储于溢出页
* Dynamic处理方式：将全部数据存储于溢出页中


**InnoDB四种行格式** 
* Compact
* Redundant
    > 相对Compact，使用字段长度偏移列表
* Dynamic
    > 相对Compact，行溢出时将全部数据存储于溢出页
* Compress
    > 相对Dynamic，使用压缩算法压缩页数据


##### 数据页结构
![page](images/mysql_page.png)

| 名称               | 中文名      | 简单描述                                                         |
|--------------------|-------------|------------------------------------------------------------------|
| File Header        | 文件头部    | 页的一些通用信息（包含页类型与前后指针）                         |
| Page Header        | 页面头部    | 数据页专有的一些信息（包含空闲空间地址与垃圾回收链表）           |
| Infimum + Supremum | 最小/大记录 | 两个虚拟的行记录                                                 |
| User Records       | 用户记录    | 实际存储的行记录内容                                             |
| Free Space         | 空闲空间    | 页中尚未使用的空间                                               |
| Page Directory     | 页面目录    | 页中的某些记录的相对位置（将已排序的记录链表分组以进行二分搜索） |
| File Trailer       | 文件尾部    | 校验页是否完整                                                   |


| 页类型                  | 十六进制 | 描述                  |
|-------------------------|----------|-----------------------|
| FIL_PAGE_TYPE_ALLOCATED | 0x0000   | 最新分配，还没使用    |
| FIL_PAGE_UNDO_LOG       | 0x0002   | Undo日志页            |
| FIL_PAGE_INODE          | 0x0003   | 段信息节点            |
| FIL_PAGE_IBUF_FREE_LIST | 0x0004   | Insert Buffer空闲列表 |
| FIL_PAGE_IBUF_BITMAP    | 0x0005   | Insert Buffer位图     |
| FIL_PAGE_TYPE_SYS       | 0x0006   | 系统页                |
| FIL_PAGE_TYPE_TRX_SYS   | 0x0007   | 事务系统数据          |
| FIL_PAGE_TYPE_FSP_HDR   | 0x0008   | 表空间头部信息        |
| FIL_PAGE_TYPE_XDES      | 0x0009   | 扩展描述页            |
| FIL_PAGE_TYPE_BLOB      | 0x000A   | 溢出页                |
| FIL_PAGE_INDEX          | 0x45BF   | 索引页                |


**页分裂处理**：  
当向页中插入数据而页空间不足时，将一半的数据（按键大小顺序）移到其他页中去，并将两页连接成双向链表


**垃圾回收**：  
页面头部中存储有 **垃圾记录链表**与 **空闲空间大小**；
当删除一条记录时，先将其头记录中的`delete_mask`置位（此中间态为MVCC保留），再将其加入垃圾记录链表头部；
当插入新记录时，先判断垃圾链表头节点空间是否足够，若足够则覆盖之并更新 **垃圾记录链表**与 **空闲空间大小**，
否则直接向页面申请空间而非遍历垃圾链表；
若页面空间申请失败，则观察 **空闲空间大小**是否满足插入记录，满足则重整页面以消除碎片，
否则进行页分裂；

##### 区、段、组
* 每连续的 64个页划分为1个区(1M)，当数据量大的时候，就会以区为单位给索引分配空间来较少随机I/O；
* 每连续的256个区划分为1个组(256M)，每组的第一个区中的头几页会存储区相关元数据，第一组第一区第一页还会存储表相关元数据；
* 功能相同的“区”组成一个逻辑段，如：非叶节点段、叶节点段、回滚段、等等；
段中各区不一定连续，将功能不同的数据区别存放于不同的区中可减少随机I/O，并利用段来组织它们；
如：叶节点段、非叶节点段、回滚段等等。


存在4种类型的区：
* 空闲区
* 有剩余空间的碎片区
* 无剩余空间的碎片区
* 附属某一段的区

其中碎片区中的页是可以用于存储不同的段中的数据的；
为节省空间，初始时会利用碎片区中的页来存储不同段中的数据；
当某一段占用了32个碎片区后，就会以整个区为单位来分配空间了；

* 所有区加起来组成一个表空间，在文件系统上表现为一个文件
    * 系统表空间
    * 独立表空间（file-per-table）
    * 通用表空间（共享表空间）
    * 临时表空间
    * 撤销表空间


#### 双写缓冲区
在冲刷缓冲池中页面到磁盘适当位置前，需要先将数据拷贝一份到双写缓冲区中，
当系统崩溃时，可从双写缓冲区中找到良好的数据副本；
虽然数据写了两次，但是磁盘I/O开销并非翻倍，
数据会以较大的序列块通过一次系统调用冲刷到磁盘中；

如果没有双写缓冲区，则系统崩溃时可能仅冲刷非完整的页到磁盘，导致FileTrailer校验失败；
redo log恢复事务操作需要完整的页面，只冲刷了一半的页面是无法恢复的；

#### Redo日志
为满足事务的持久性，在事务提交会先将redo log（很小的数据量）写入磁盘，而页数据由缓冲池适时冲刷。

## 索引
### B+Tree索引
**索引构建**
1. 每创建一个B+Tree索引，便为此创建一个根节点（页面）
2. 初始时根节点（页面）为空，随后向其中插入用户记录
3. 当根节点（页面）空间不足时，先将数据复制到另一页中然后进行页分裂
    > 页分裂最终使同一层级的节点连接成一个双向链表
4. 此时根节点（页面）存储目录项，每个目录项包含键与该键所在节点（页面）
5. 每当页分裂时便会再上一层节点中创建新的目录项，当根节点（页面）空间又不足时循环步骤3


**索引策略**
* B+Tree索引
    * 优点：适用于索引列的左前缀匹配查询，范围查询，排序与分组
* 聚簇索引
    * 优点：将相关数据存储在一起可能减少磁盘I/O次数
    * 缺点：插入速度严重依赖与插入行的主键大小（插入顺序）
* 二级索引
    * 优点：相比聚簇索引更小
    * 缺点：用户记录仅存储索引列与主键列（以降低维护成本），若要获取完整用户记录需要回表查询
* 伪哈希索引
    * 优点：将字符串转换为哈希整数值从而减小索引大小且大幅加速比较过程
    * 缺点：仅可用于全键匹配查询
* 前缀索引
    * 优点：根据平均索引选择性以及最坏情况来决定前缀长度从而减小索引大小且加速比较过程
    * 缺点：仅可用于`LIKE`范围查询
* 联合索引
    * 优点：根据索引选择性、单独查询频率、等值查询概率调整索引列顺序可用于优化WHERE子句中的条件相交`AND`
    * 缺点：只能从左往右进行匹配不能跳过某些中间列，可通过使用`IN()`来部分解决
* 冗余索引
    * 优点：解决多列索引按列顺序匹配的问题
    * 缺点：引入冗余索引而增大维护成本与空间成本
* 合并索引
    > 一般情况下多条件查询只会使用一条索引然后查表返回，再使用WHRER匹配；
    > 是否进行合并取决于优化器判断其是否相比单键索引更高效；
    > 甚至是否使用二级索引也取决与优化器判断是否二级索引+回表查询比全表查询更高效。
    * 交集合并：
        > 考虑创建多列索引
        * 二级索引列全值匹配
        * 主键范围匹配
    * 并集合并：
        * 二级索引列全值匹配
        * 主键范围匹配
        * 与交集合并结果的合并
    * 并集排序：
        * 二级索引范围匹配
* 覆盖索引
    * 覆盖索引是一种指导思想，旨在使二级索引尽量覆盖需要查询与匹配的列，从而尽量避免回表查询
    * 索引分为三星级别：
        * 索引仅仅是将相关查询记录放在一起:star:
        * 索引将查询记录按查询顺序排在一起:star: :star:
        * 索引中包含了所有查询所需的全部列:star: :star: :star:


**索引类型**
* `const`       ：唯一索引的对常量的等值比较
* `eq_ref`      ：唯一索引的对非常量的等值比较
* `ref`         ：非唯一索引的对常量的等值比较
* `ref_or_null` ：同上，且非NOT NULL索引对NULL值进行匹配
* `index_merge` ：索引合并查询
* `range`       ：索引的范围匹配
* `index`       ：扫描二级索引
* `ALL`         ：扫描聚簇索引


**单表查询**
* 多索引过滤
    * 判断是否使用合并索引
    * 判断是否使用索引以及使用哪一条索引
    * 根据选取的索引进行单索引过滤，获取临时表后再使用WHERE条件过滤
* 单索引过滤
    * 判断是否直接扫描叶节点
    * 若为等值/范围匹配，则可根据索引快速圈定查询目标记录的链表范围
    * 若为多范围匹配，则可条件扫描第一个范围圈定的目标记录的链表范围
    * 根据实际情况（如查询列、多条件等）判断是否回表查询

**多表查询**
* 对驱动表和被驱动表进行单表查询过滤除结果
* 将驱动表查询结果加入join buffer中，根据连接条件对被驱动表进行扫描匹配
* 第一轮查询结果作为驱动表，再次迭代进行扫描


![trx](images/trx_cache.png)

## 事务
* 原子性：整个事务中的操作要么全部提交成功，要么全部失败回滚（Undo Log）
* 一致性：未成功提交的事务中所做的修改不会保存至数据库      （Undo Log）
* 隔离性：一个事务在最终提交前其修改对其他事务的可见性      （MVCC）
* 持久性：一旦事务提交则其所做的修改就会永久保存到数据库中  （Redo Log与Double Write Buffer）

| 隔离级别 | 并发问题   | 解决方案 |
|----------|------------|----------|
|          | 脏写       | 互斥锁   |
| 未提交读 | 脏读       | MVCC     |
| 已提交读 | 不可重复读 | MVCC     |
| 可重复读 | 幻读       | 间隙锁   |
| 串行化   |            |          |

**多版本并发控制(MVCC)**

* Undo日志
    * 为满足事务的原子性，每次修改都会记录undo log，其中包含了足够的信息来获取旧版本数据，Undo日志会连成一条版本链
    * 会有后台线程自动删除已无用的undo log；
        插入型UndoLog在事务提交后便可删除；
        更改型UndoLog在没有事务使用时才可以删除；
        在删除为`DELETE`编写的UndoLog时才会真正的删除行记录；

* ReadView
    * 目的：将事务的可见性依据时间节点顺序来建立
    * 内容：
        * `m_ids`：生成时刻系统活跃的事务列表
        * `min_trx_id`：生成时刻系统最小活跃事务ID
        * `max_trx_id`：生成时刻系统分配给下个事务的ID
        * `creator_trx_id`：生成该ReadView的事务ID，第一次执行更易操作时才分配事务ID，否则为0
    * 已提交读：每次读都重新生成ReadView，于是每次读都可读到新提交的其他事务的修改结果
    * 可重复读：只在第一次读时生成ReadView，于是每次读都只能读到生成ReadView之前就以提交的其他事务的修改结果
    * 对于二级索引，没有隐藏的事务ID列与回滚指针列：
        * 会在二级索引页面元数据中设置最大修改事务ID，
            若用ReadView确认其小于`min_trx_id`则表示该页中所有修改已提交，
            否则所有查询均需回表；

* 锁
    * 当一个事务修改一个页面时，会为该页面加互斥锁，修改完成后便会释放该锁，
    * 但是仍会对修改过的行加锁，其他事务只能读取历史适当的历史版本，试图修改该行的事务都会阻塞，直到事务提交

* INSERT
    * 判断是否存在间隙锁
    * 获取目标页互斥锁
    * 向所有索引中插入新行
    * 设置聚簇索引中记录的事务ID（当前事务ID）与回滚指针（undo log）
    * 更新二级索引页面的最大修改事务ID
    * 释放目标页互斥锁，并获取目标行的互斥锁
* DELETE
    * 获取目标页的互斥锁
    * 将所有索引中的目标行的delete_mask置位
    * 设置聚簇索引中记录的事务ID（当前事务ID）与回滚指针（undo log）
    * 更新二级索引页面的最大修改事务ID
    * 释放目标页的互斥锁，并获取目标行的互斥锁
* UPDATE
    * 获取目标页的互斥锁
    * 若不更改索引键：若修改后所有列长度不变则就地修改并生成undo log，否则完全删除原记录在执行一次INSERT
    * 若要更改索引键：执行一次DELETE再执行一次INSERT
    * 释放目标页的互斥锁，并获取目标行的互斥锁
* SELECT
    * 查询的目标行需要满足：行的事务ID要小于或等于当前事务TID，且未置位删除位标识
    * 若查询行的事务ID大于当前事务TID，则根据回滚指针指向的版本链回溯适当的历史版本
    * 若无适当历史版本则这条记录对当前事务不可见
    * 范围搜索时还会给范围加上间隙锁，避免出现幻行

<!--
缓冲机制：buffer pool & change buffer
索引结构：row & page & extent & segment
事    务：
    持久性（redo log与double write buffer）
    原子性（undo log）
    一致性（undo log）
    隔离性（undo log与MVCC与Lock）
-->
