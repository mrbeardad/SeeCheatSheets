**关系型数据库的典型实现主要被调整用于执行规模小而读写频繁，或者大批量极少写访问的事务。**

<!-- entry begin: mysql init install -->
* 初始化mysql
    * `mariadb-install-db --user=mysql --basedir=/usr --datadir=/var/lib/mysql`
    > 或
    * `mysqld --initialize --user=mysql --basedir=/usr --datadir=/var/lib/mysql`

* mysql
    * -h    ：主机名
    * -P    ：端口号
    * -u    ：用户名
    * -p    ：密码
    * -D    ：数据库名
<!-- entry end -->

# SQL基础
* `--`开启注释
* SQL语句以分号`;`结束
* 可以忽略大小写

## 用户
两种表达形式：
* user@host
* user          ：默认host为`'%'`（见下）

其中host有三种类型：
* '%'           ：允许从任意主机登录
* localhost     ：只允许本机登录
* 192.168.0.1   ：只允许从指定IP的主机登录

## 库、表、域
* 数据库一般表示：
    * `<db_name>`
    * `*`
* 数据表表示：
    * `<db_name>.<tbl_name>`
    * `*.*`
    * `<tbl_name>`          ：默认db_name为当前`use`选中的数据库
    * `*`
* 域表示：
    * `<db_name>.<tbl_name>.<fd_name>`
    * `<tbl_name>.<fd_name>`
    * `<fd_name>`
    * `<fd1_name>, <fd2_name>`
    * `*`

## 数据类型
* 整数类型
    > * 支持在其后添加`(N) ZEROFILL`，指定十进制显示时的最小显示位数，不足时用0填充（不限制数值大小）
    > * 支持在其后添加`UNSIGNED`，指定为无符型整数

    | 类型      | 字节 |
    |-----------|------|
    | TINYINT   | 1    |
    | SMALLINT  | 2    |
    | MEDIUMINT | 3    |
    | INT       | 4    |
    | BIGINT    | 8    |

* 浮点类型
    > * 支持在其后添加`(N, P)`，分别指定十进制显示时的最大有效位数（限制数值大小）与小数位数精度
    > * DECIMAL是用字符串表示的高精度数值，默认的十进制显示模式为`(10, 0)`

    | 类型    | 字节 |
    |---------|------|
    | FLOAT   | 4    |
    | DOUBLE  | 8    |
    | DECIMAL | ∞    |

* 字符串类型
    > * CHAR支持在其后添加`(N)`，指定固定字节数
    > * VARCHAR支持在其后添加`(N)`，指定最大字符数
    | 类型    | 字节    | 说明                           |
    |---------|---------|--------------------------------|
    | CHAR    | 0~255   | 固定字节数，忽略输入的尾后空格 |
    | VARCHAR | 0~65535 | 可变字节数                     |
    | BLOB    | 0~65535 | 二进制数据                     |

* 时间日期
    > 表示字符串类型域日期时间类型的值，都需要使用单引号
    | 类型      | 字节 | 格式                |
    |-----------|------|---------------------|
    | DATE      | 3    | YYYY-mm-dd          |
    | TIME      | 3    | HH:MM:SS            |
    | DATETIME  | 8    | YYYY-mm-dd HH:MM:SS |
    | TIMESTAMP | 4    | YYYY-mm-dd HH:MM:SS |

## 类型约束
| 约束              | 说明         | 备注                                           |
|-------------------|--------------|------------------------------------------------|
| `NOT NULL`        | 不能为空     | PRIMARY KEY的必要条件                          |
| `UNIQUE`          | 不能重复     | PRIMARY KEY的必要条件，UNIQUE KEY的充分条件    |
| `DEFAULT <VALUE>` | 设置默认值   |                                                |
| `AUTO_INCREMENT`  | 自动增加数值 | 其后可选添加`=n`设置初始值，默认为1，每次递增1 |

# SQL语句
## 用户管理
<!-- entry begin: sql user password grant -->
```sql
-- 创建用户
CREATE USER <user_name> [IDENTIFIED BY '<passwd>'];

-- 删除用户
DROP USER <user_name>;

-- 修改密码
SET PASSWORD FOR <user_name> = PASSWORD('<passwd>');

-- 查询用户信息
SELECT user,host,password FROM mysql.user;

-- 授权用户
GRANT <pribileges_code> ON <db_name>.<tbl_name> TO <user_name>;
FLUSH PRIVILEGES;
-- 权限码包括
    -- ALL PRIVILEGES
    -- SELECT
    -- INSERT
    -- 等等...
```
<!-- entry end -->

## 数据库、数据表、视图
<!-- entry begin: sql database db table tbl view -->
```sql
-- 数据库
SHOW DATABASES;
CREATE DATABASE <db_name>;
DROP DATABASE <db_name>;

-- 数据表
SHOW TABLES [FROM <db_name>];
CREATE TABLE <tbl_name> (
    <fd_name> <type> [<const>],
    ...,
    [PRIMARY KEY (<fd_name>),]
    [CONSTRAINT <fk_name>  FOREIGN KEY (<fd_name>) REFERENCES <tbl_name>(<fd_name>),]
    [CONSTRAINT <ck_name>  CHECK (<Clause>)]
) ENGINE=innodb CHARSET=utf8mb4;
DROP TABLE <tbl_name>;
ALTER TABLE <tbl_name> RENAME TO <new_tbl_name>;

-- 视图
CREATE VIEW <view_name> AS SELECT 语句;
DROP VIEW <view_name>;
```
<!-- entry end -->

## 操作元数据
<!-- entry begin: sql table index metadata -->
* 操作元数据
```sql
-- field
SHOW COLUMNS FROM <tbl_name>;
ALTER TABLE <tbl_name> ADD <fd_name> <type> [<const>] [FIRST| AFTER <fd_name>];
ALTER TABLE <tbl_name> DROP <fd_name>;
ALTER TABLE <tbl_name> CHANGE <fd_name> <new_fd_name> <type> [<const>];

-- index
SHOW INDEX FROM <tbl_name>;
ALTER TABLE <tbl_name> ADD INDEX <idx_name>(<fd_name>);
ALTER TABLE <tbl_name> DROP INDEX <idx_name>;

-- PRIMARY KEY
    -- 目标域必须为`NOT NULL UNIQUE`
ALTER TABLE <tbl_name> ADD PRIMARY KEY (<fd_name>);
ALTER TABLE <tbl_name> DROP PRIMARY KEY;

-- FOREIGN KEY
    -- 两域类型必须相同，且被引用的域必须为`UNIQUE`
ALTER TABLE <tbl_name> ADD CONSTRAINT <fk_name> FOREIGN KEY (<fd_name>) REFERENCES <tbl_name>(<fd_name>);
ALTER TABLE <tbl_name> DROP FOREIGN KEY <fk_name>;

-- CHECK
ALTER TABLE <tbl_name> ADD CONSTRAINT <ck_name> CHECK (<Clause>);
ALTER TABLE <tbl_name> DROP CHECK <ck_name>;
```
<!-- entry end -->

## 操作行数据
<!-- entry begin: sql insert delete 事务 -->
* 操作行数据
```sql
-- 插入
INSERT INTO <tbl_name> [(<fd_name>, ...)] VALUES    -- 外键列需要指定为该键所指向的列中已存在的值以进行关联
(<value>, ...), ... ;                               -- 若类型为字符串或日期时间，则`value`必须使用单引号

-- 复制
INSERT INTO <tbl_name> (<fd_name>, ...)
SELECT 语句 ;                            -- 两语句中的<fd_name>部分两两对应

-- 删除
DELETE FROM <tbl_name>
[WHERE <Clause>] ;                      -- 默认删除所有数据

-- 修改
UPDATE <tbl_name> SET <fd_name>=<value>, ...
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

## SELECT语句
### 查询
<!-- entry begin: sql select -->
* sql查询
```sql
-- SELECT基础查询语句
SELECT <fd_name>或<call_func> [AS <fd_alias>], ... FROM <tbl_name> [<tbl_alias>], ...
[LIMIT N] [OFFSET M]
[WHERE <Clause>] ;

-- SELECT 语句会对表中每一行筛选、处理，然后放入SELECT结果中
-- SELECT 语句的结果就是一个列表`(<val1>, <val2>),(<val3>, <val4>)`
-- 子SELECT 语句作为列表可出现在WHERE和FROM后
<!-- entry end -->
```
<!-- entry end -->

<!-- entry begin: group  -->
* sql分组
```sql
SELECT <fd_name>或<call_func> [AS <fd_alias>], ... FROM <tbl_name> [<tbl_alias>], ...
GROUP BY <fd_name> [WITH ROLLUP]
[HAVING (Clause)] ;

-- 将指定域相同的行合并到一行，从而可以对该组中的行使用aggregate函数（若不分组则会将所有行压缩到一行再调用aggregate函数）
-- WITH ROOLUP会将所有合并后的行再合并成一行并添加到最后，非aggregate函数的列为NULL
-- HAVING代替WHERE，从而可以使用aggregate函数
```
<!-- entry end -->

### 排序
<!-- entry begin: sql sort -->
* sql排序
```sql
SELECT 语句
ORDER BY <fd_name> [DESC], ... ; -- 默认升序，DESC指明为降序
```
<!-- entry end -->

### 连接
<!-- entry begin: sql join -->
* sql连接
```sql
SELECT 语句
[INNER|LEFT|RIGHT] JOIN <tbl_name> [<tbl_alias>]
ON <tbl_alias>.<fd_name> = <tbl_alias>.<fd_name> ;

-- 原始的、未过滤的join表可设`ON 1`来查看
-- ON与WHERE的区别在于前者在生成临时表时，后者在生成临时表后
```
下图中的交集表示符合`ON`表达式的行，两个集合则分别表示两张表的所有行
![JOIN](https://www.runoob.com/wp-content/uploads/2019/01/sql-join.png)
<!-- entry end -->

### 合并
<!-- entry begin: sql union -->
* sql合并
```sql
-- 将两个SELECT 语句查询的字段按顺序对应地合并在一列中，字段名由第一个SELECT指出
SELECT 语句
UNION [ALL] -- ALL表示允许重复，默认删掉重复值
SELECT 语句 ;
```
<!-- entry end -->

## WHERE语句
<!-- entry begin: sql where 条件 -->
* where语句
    * `WHERE <fd_name> <OP> <value>`
    * `WHERE <fd_name> [NOT] REGEXP 'regex_pattern'`
    * `WHERE <fd_name> [NOT] BETWEEN <value> AND <value>`
    * `WHERE <fd_name> [NOT] IN (<value>, ...)`
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

