# 设计模式

- [设计模式](#设计模式)
  - [参考资料](#参考资料)
  - [设计原则](#设计原则)
  - [创建型](#创建型)
    - [工厂方法模式(Factory Method)](#工厂方法模式factory-method)
    - [抽象工厂模式(Abstract Factory)](#抽象工厂模式abstract-factory)
    - [建造者模式(Builder)](#建造者模式builder)
    - [原型模式(Prototype)](#原型模式prototype)
    - [单例模式(Singleton)](#单例模式singleton)
  - [结构型](#结构型)
    - [享元模式(Flyweight)](#享元模式flyweight)
    - [表观模式(Facade)](#表观模式facade)
    - [组合模式(Composite)](#组合模式composite)
    - [桥接模式(Bridge)](#桥接模式bridge)
    - [代理模式(Proxy)](#代理模式proxy)
    - [适配器模式(Adapter)](#适配器模式adapter)
    - [装饰器模式(Decorator)](#装饰器模式decorator)
  - [行为型](#行为型)
    - [状态模式(State)](#状态模式state)
    - [策略模式(Strategy)](#策略模式strategy)
    - [模板方法模式(Template Method)](#模板方法模式template-method)
    - [访问者模式(Visitor)](#访问者模式visitor)
    - [解释器模式(Interpreter)](#解释器模式interpreter)
    - [迭代器模式(Iterator)](#迭代器模式iterator)
    - [备忘录模式(Memento)](#备忘录模式memento)
    - [责任链模式(Chain of Responsibility)](#责任链模式chain-of-responsibility)
    - [命令模式(Command)](#命令模式command)
    - [中介者模式(Mediator)](#中介者模式mediator)
    - [观察者模式(Observer)](#观察者模式observer)

## 参考资料

<https://design-patterns.readthedocs.io/zh_CN/latest/index.html>  
<https://www.tutorialspoint.com/design_pattern/design_pattern_overview.htm>  
<https://springframework.guru/gang-of-four-design-patterns/>
<https://www.zhihu.com/question/20275578/answer/26577791>

## 设计原则

- 面向对象：对象(Object)更准确的翻译应该叫物体，是对“世间万物”的抽象表示

  - 封装：将物体对应的特征（属性）与行为（方法）关联地放在一起（类），可以方便我们用代码来进行描述与表达；比如人的特征如身高、体重等，人的行为如跑步、游泳等；

  - 多态：相似的物体具有相似的特征与行为，但细节又各有不同，所以针对具体环境，对他们相同的不变的点进行抽象描述（接口）以忽略这些细节，如此一来那些相似但又不同的物体都可以用来表示该抽象物体；比如麻雀属于雀型目，身形矮小且会飞，企鹅属于企鹅目，身形较大但不会飞，然后它们的共同点是两足、恒温、卵生，所以抽象出了鸟纲，所有属于鸟纲的物体都可以用来表达“两足行走”这个行为；

  - ~~继承~~：继承不应该是一种思想，而只是一种技术方案，一种实现手法，一种代码复用与兼容接口的方便机制，但通常伴随着高耦合的缺点；可惜的是大多数面向对象编程语言都将多态与继承绑定到一起使用，不过一些新生语言倒是直接放弃了继承机制，如 Go（使用 interface 实现多态）、Rust（使用 trait 实现多态）

- 设计模式：构建软件系统来解决一系列复杂的问题时，通常会将复杂问题递归地拆分成更小的子问题，再针对小的问题设计简单的解决方案，最后再将方案逻辑地组织起来以解决最终问题。然而，时常因为问题过于复杂多变而不得不去修改原有的解决方案，从而导致牵一发而动全身；可以考虑一下设计原则来避免：

  - 依赖倒置：高层模块与低层模块不应该相互依赖，转而依赖他们之间约定的抽象接口

  - 接口隔离：一个接口如果做了太多事情，考虑拆分成多个更细粒度的接口让用户可以灵活地组织起来使用

  - 单一职责：一个类不要负责太多事情，将容易变化的部分抽离出去

  - 迪米特法则：尽可能选择最少最简单的关系以降低耦合

| 关系     | 抽象概念                                               | 代码实现                    |
| -------- | ------------------------------------------------------ | --------------------------- |
| A 组合 B | A 作为整体不能没有部分 B，B 作为部分不能离开整体 A     | B 作为 A 的值类型成员变量   |
| A 聚合 B | A 作为整体可以没有部分 B，B 作为部分可独立存在且可共享 | B 作为 A 的引用类型成员变量 |
| A 关联 B | A 长久的使用 B                                         | B 作为 A 的引用类型成员变量 |
| A 依赖 B | A 临时的使用 B                                         | B 作为 A 的方法的参数类型   |

## 创建型

创建型设计模式提供了一种在隐藏创建逻辑的同时创建对象的方法，而不是直接使用 new 操作符实例化对象。这使程序在决定需要为给定的用例创建哪些对象时更加灵活。

### 工厂方法模式(Factory Method)

当一个抽象类体系中存在很多复杂的具体类时，利用工厂类，将创建对象时所需要的创建逻辑与具体类型集中隐藏起来管理，从而减少用户可能的硬编码。

![FactoryMethod](images/FactoryMethod.webp)

### 抽象工厂模式(Abstract Factory)

利用抽象工厂（即工厂的工厂），它将多个工厂类组合起来，从而统一地创建一套内部相互关联的对象族。

![AbstractFactory](images/AbsatractFactory.jpg)

### 建造者模式(Builder)

将一个复杂类实例对象的创建过程分成两部分，Builder 负责可变或可选的部分（如原始数据），Director 负责不变的部分（如构建算法或数据校验）。

![Builder](images/Builder.png)

### 原型模式(Prototype)

当构造函数开销很大时（如需要网络 IO 或磁盘 IO），不必对用户的每次请求都完全创建一个新对象，转而克隆原型对象获取其副本。

实现细节：

- 注意浅拷贝与深拷贝
- 考虑写时复制技术(Copy On Write)

![Prototype](images/Prototype.webp)

### 单例模式(Singleton)

仅允许类存在一个实例，并提供全局方法访问之，一般用于“管理类”和“工厂类”等

实现细节：

- 注意线程安全性，包括创建时与更改时
- 注意需求“延迟初始化”或“提前初始化”

![Singleton](images/Singleton.jpg)

## 结构型

结构型设计模式涉及类和对象组合。继承的概念用于组合接口和组合对象以获得新的功能。

### 享元模式(Flyweight)

使用享元工厂来创建对象，将可共享复用的内部数据与不可共享的外部状态分开创建，可提高整体性能。

![Flyweight](images/Flyweight.png)

### 表观模式(Facade)

当与多个子系统的交互比较复杂时，使用表观模式封装子系统从而避免用户与子系统耦合

注意：与中介者模式区别在于，表观为对外封装解耦，中介者为对内封装解耦

![Facade](images/Facade.png)

### 组合模式(Composite)

使用组合模式来创建层状树结构，并提供统一的方法来访问非叶节点和叶节点。

![Composite](images/Composite.png)

### 桥接模式(Bridge)

使用桥接模式来让类可以在两个维度上任意变换扩展和组合

![Bridge](images/Bridge.jpg)

### 代理模式(Proxy)

使用代理对象来代替实际对象，从而控制实际对象的创建与访问。常见代理：

- 远程代理：延迟加载
- 虚拟代理：延迟创建
- COW 代理：写时复制
- 缓存代理：缓存结果
- 保护代理：权限区分
- 智能引用代理：引用计数

![Proxy](images/Proxy.jpg)

### 适配器模式(Adapter)

使用适配器来使将一个接口转换到互不兼容的另一个接口上工作。
优先选择组合，以下情况才考虑多重继承：

- 需要覆盖 virtual 函数实现
- 需要访问 protected 成员
- 需要空基类优化(EBO)

![Adapter](images/Adapter.jpg)

### 装饰器模式(Decorator)

使用装饰器装饰（组合）基础对象来动态地为其附加额外的职责。各个职责相互独立从而可相互组合，若使用继承来扩展的话会产生很多类，代码过于冗余。

![Decorator](images/Decorator.jpg)

## 行为型

行为型设计模式特别关注对象之间的通信。

### 状态模式(State)

利用更改内部抽象对象来实现状态切换。

![State](images/State.png)

### 策略模式(Strategy)

允许在运行时动态地选择一个算法族。

![Strategy](images/Strategy.jpg)

### 模板方法模式(Template Method)

将算法的框架定义为一个抽象类，允许其子类提供具体的行为覆盖之。

![Templte](images/Template.png)

### 访问者模式(Visitor)

访问者允许你定义一个新的操作，而不需要改变它所操作的元素的类。双分派技术缺点是当添加元素类时需要修改所有访问者。

![Visitor](images/Visitor.png)

### 解释器模式(Interpreter)

利用组合模式实现语法解析器，若语言支持还可利用重载操作符。满足以下条件才适用：

- 业务规则频繁变化
- 类似的结构不断重复出现
- 并且容易抽象为语法规则的问题

![Interpreter](images/Interpreter.webp)

### 迭代器模式(Iterator)

按顺序访问对象的元素，而不暴露其底层表示。

![Iterator](images/Iterator.webp)

### 备忘录模式(Memento)

在外部存储内部状态以届时恢复

![Memento](images/Memento.png)

### 责任链模式(Chain of Responsibility)

赋予多个对象处理请求的机会，最终只有一个负责处理。

![Chain of Responsibility](images/Chain_of_Responsibility.png)

### 命令模式(Command)

使用命令对象来传递请求，将请求的发送方与接收方解耦。类似制定通讯协议。

![Command](images/Command.webp)

### 中介者模式(Mediator)

利用中介对象来隔离一系列对象的复杂交互，使同事类对象之间无需显示相互引用

![Mediator](images/Mediator.jpg)

### 观察者模式(Observer)

是一种发布/订阅模式，允许多个观察者对象查看事件。

![Observer](images/Obeserver.jpg)
