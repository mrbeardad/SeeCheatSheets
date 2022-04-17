# Design Pattern

- [Design Pattern](#design-pattern)
  - [设计原则](#设计原则)
  - [创建型](#创建型)
    - [简单工厂模式(Simple Factory)](#简单工厂模式simple-factory)
    - [工厂方法模式(Factory Method)](#工厂方法模式factory-method)
    - [抽象工厂模式(Abstract Factory)](#抽象工厂模式abstract-factory)
    - [原型模式(Prototype)](#原型模式prototype)
    - [建造者模式(Builder)](#建造者模式builder)
    - [单例模式(Singleton)](#单例模式singleton)
  - [结构型](#结构型)
    - [组合模式(Composite)](#组合模式composite)
    - [表观模式(Facade)](#表观模式facade)
    - [代理模式(Proxy)](#代理模式proxy)
    - [桥接模式(Bridge)](#桥接模式bridge)
    - [适配器模式(Adapter)](#适配器模式adapter)
    - [装饰器模式(Decorator)](#装饰器模式decorator)
    - [享元模式(Flyweight)](#享元模式flyweight)
  - [行为型](#行为型)
    - [解释器模式(Interpreter)](#解释器模式interpreter)
    - [迭代器模式(Iterator)](#迭代器模式iterator)
    - [备忘录模式(Memento)](#备忘录模式memento)
    - [命令模式(Command)](#命令模式command)
    - [责任链模式(Chain of Responsibility)](#责任链模式chain-of-responsibility)
    - [访问者模式(Visitor)](#访问者模式visitor)
    - [观察者模式(Observer)](#观察者模式observer)
    - [中介者模式(Mediator)](#中介者模式mediator)
    - [状态模式(State)](#状态模式state)
    - [策略模式(Strategy)](#策略模式strategy)
    - [模板方法模式(Template Method)](#模板方法模式template-method)
  - [分层模式](#分层模式)

## 设计原则

- 依赖倒置原则：高层模块与底层模块都应面向稳定的接口编程
- 接口隔离原则：类的接口粒度要足够细，避免客户依赖他们不用的方法
- 单一职责原则：一个类应该仅有一个引起它变化的原因
- 开放关闭原则：类模块应该是可扩展的，但是不可修改
  - **隔离性**：指修改或替换原有旧实现对调用方的影响程度
  - **扩展性**：指用户对模块行为的定制与扩展程度

![DesignePattern](images/DesignPattern.jpg "转自https://www.cnblogs.com/snifferhu/p/9152943.html")

## 创建型

### 简单工厂模式(Simple Factory)

![SimpleFactory](images/SimpleFactory.jpg)

- 将对象的创建与使用分离，面向接口编程，避免将具体类型硬编码到客户代码中，从而在未来扩展更换时丧失兼容性
- 用户层扩展更换时只需修改静态工厂函数的参数（可通过配置文件动态设置）
- 缺点：扩展产品需要修改静态工厂函数的源码

### 工厂方法模式(Factory Method)

![FactoryMethod](images/FactoryMethod.jpg)

- 将对象的创建与使用分离，面向接口编程，避免将具体类型硬编码到客户代码中，从而在未来扩展更换时丧失兼容性
- 用户层扩展更换时只需要修改一处创建代码
- 缺点：会产生较多的类

### 抽象工厂模式(Abstract Factory)

![AbstractFactory](images/AbatractFactory.jpg)

- 将对象的创建与使用分离，面向接口编程，避免将具体类型硬编码到客户代码中，从而在未来扩展更换时丧失兼容性
- 用户层扩展更换时只需要修改一处创建代码
- 统一创建同一产品族的多个对象
  - 工厂方法模式实现：可以隐藏具体产品类
  - 原型模式实现：动态配置工厂且减少类的创建

### 原型模式(Prototype)

![Prototype](images/Prototype.gif)

- 将对象的创建与使用分离，面向接口编程，从而在未来扩展更换时丧失兼容性
- 用户层扩展更换时只需要修改一处创建代码
- 缺点：具体类型暴露给用户从而无法保证客户代码的正确的使用原型

### 建造者模式(Builder)

![Builder](images/Builder.jpg)

- 对象的创建非常复杂，需要复杂的算法来组装各个部件，将组装算法提取出来，让各部件独立变化
- 用户层扩展更换时只需要修改一处创建代码

### 单例模式(Singleton)

![Singleton](images/Singleton.jpg)

- 限制一个类只需有一个对象实例，从而减少开销

## 结构型

### 组合模式(Composite)

![Composite](images/Composite.png)

- 将对象组合成树形结构以表示“部分-整体”的层次结构，使得客户不用区分单个对象与组合对象

### 表观模式(Facade)

![Facade](images/Facade.jpg)

- 添加一层新的接口，将多个复杂的子系统封装起来

### 代理模式(Proxy)

![Proxy](images/Proxy.jpg)

- 通过代理对象去掉客户不能看到的内容或者添加客户需要的额外服务
- 主体类完成关键功能
- 常见的代理模式类型：
  - 远程代理：延迟加载
  - 虚拟代理：延迟创建
  - COW 代理：写时复制
  - 缓存代理：缓存操作结果用于共享
  - 保护代理：权限区分
  - 同步代理：并发同步
  - 智能引用代理：引用计数

### 桥接模式(Bridge)

![Bridge](images/Bridge.jpg)

- 在初期，设计两个接口，用于将抽象部分（业务功能）与实现部分（平台实现）分离，使其在两个维度上独立变化

### 适配器模式(Adapter)

![Adapter](images/Adapter.jpg)
![Adapter](images/Adapter_classModel.jpg)

- 在后期，已存在两个接口，将它们
- 优先选择类的组合，以下考虑类的继承：
  - 需要覆盖 virtual 函数实现
  - 需要访问 protected 成员
  - 需要空基类优化(EBO)

### 装饰器模式(Decorator)

![Decorator](images/Decorator.jpg)

- 一种代替继承，可为“对象”动态扩展添加的功能的技术，或者动态进行功能的组合

### 享元模式(Flyweight)

![Flyweight](images/Flyweight.jpg)

- 将对象化分为内部状态与外部状态，内部状态由享元工厂产出，外部状态由客户代码提供

## 行为型

### 解释器模式(Interpreter)

![interpreter](images/interpreter.jpg)

- 只有满足“业务规则频繁变化，且类似的结构不断重复出现， 并且容易抽象为语法规则的问题”才适合使用 Interpreter 模式
- 通过利用组合模式与重载操作符，就相当与借用编译器的语法解析器

### 迭代器模式(Iterator)

![Iterator](images/iterator.jpeg)

- C++自带，不多解释

### 备忘录模式(Memento)

![Memento](images/memento.png)

- 在外部存储内部状态以届时恢复

### 命令模式(Command)

![Command](images/command.png)

- 将请求的调用与实现分离，从而可以实现：
  - 请求排队
  - 请求日志
  - 撤销与恢复(结合 Memento)
  - 宏命令(结合 Composite)

### 责任链模式(Chain of Responsibility)

![chain_of_responsibility](images/chain_of_responsibility.png)

- 运行时赋予多个对象处理请求的机会（但最终只有一个负责处理），从而使请求的发送者与接收者解耦
- 可设计成混合类

### 访问者模式(Visitor)

![Visitor](images/visitor.png)

- 适用于对象结构包含很多对象且有不同的接口，或者对象中包含很多不相关的操作
- 不修改类体系而进行接口（功能）扩展
- 利用双分派技术使具体操作内容取决于 visitor 和 element 的类型
- 缺点：添加元素时需要修改所有 visitor 对象

### 观察者模式(Observer)

![Observer](images/Obeserver.jpg)

- 定义对象间的多对一的依赖关系

### 中介者模式(Mediator)

![Mediator](images/Mediator.jpg)

- 利用中介对象来隔离一系列对象的复杂交互，使各对象无需显示相互引用
  > C++中中介者持有 shared_ptr，同事类持有 weak_ptr

### 状态模式(State)

![State](images/state.png)

- 内部状态改变时改变其行为

### 策略模式(Strategy)

![Strategy](images/Strategy.jpg)

- 将算法独立出来可替换、复用

### 模板方法模式(Template Method)

![Templte](images/template.png)

- 基类定义算法的骨架（各个步骤方法的默认实现），子类选择性覆盖之

## 分层模式

常见的分层架构模式

1. 三层模型：用户表示层、业务逻辑层、数据层。
   > 用户表现层：负责解析请求报文，校验请求数据，控制会话响应；
   > 业务层逻辑：封装复杂业务逻辑供用户表现层使用
   > 数据层：定义数据库模式表结构，并提供读写接口
2. 客户端-服务器模型
3. 多层结构的技术组成模型：表现层、中间层、数据层。
4. 网络系统常用三层结构：核心层、汇聚层和接入层。
5. RUP 典型分层方法：应用层、专业业务层、中间件层、系统软件层。
6. 基于 Java 的 B/S 模式系统结构：浏览器端、服务器端、请求接收层、请求处理层。
7. 某六层结构：功能层（用户界面）、模块层、组装层（软件总线）、服务层（数据处理）、数据层、核心层。
