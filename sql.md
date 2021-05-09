# 目录
<!-- vim-markdown-toc GFM -->

- [安装与使用](#安装与使用)
- [MySQL基础](#mysql基础)
  - [系统信息](#系统信息)
  - [用户管理](#用户管理)
  - [数据库](#数据库)
  - [数据表](#数据表)
  - [数据列](#数据列)
  - [行记录](#行记录)
  - [数据类型](#数据类型)
    - [整数类型](#整数类型)
    - [实数类型](#实数类型)
    - [时间日期](#时间日期)
    - [字符类型](#字符类型)
    - [枚举类型](#枚举类型)
    - [位类型](#位类型)
  - [类型约束](#类型约束)
- [SQL基础](#sql基础)
  - [基础语法](#基础语法)
  - [查询语句](#查询语句)
  - [WHERE语句](#where语句)
  - [运算符](#运算符)
  - [函数](#函数)

<!-- vim-markdown-toc -->
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

<!--
缓冲机制：buffer pool & change buffer
索引结构：row & page & extent & segment
事    务：
    持久性（redo log与double write buffer）
    原子性（undo log）
    一致性（undo log）
    隔离性（undo log与MVCC与Lock）
-->
