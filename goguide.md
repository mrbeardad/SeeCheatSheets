* 面向对象设计
    * 方法接收指针类型
    * 值形式嵌套复用
    * 接口赋值为指针


* 函数设计
    * 设计接口参数实现多态
    * 使用指针参数避免拷贝或修改原值



* 优雅的使用通道
    > 编写go代码时，可在心中延用经典的多线程模型，只是goroutine在CPU密集型效率更低而在IO密集型效率更高
    * 串行化（`make(chan T)`）
    * 异步安全数据管道（`make(chan Future, bufsize)`）
    * 限制并发量（`make(chan struct{}, MAXPARALLEL)`、`maxChan <- struct{}{}; <-maxChan`）
    * 一对一异步事件信号（`make(chan struct{}, 1)`
    * 一对多异步事件信号（`close(done)`）
    * 多对一异步事件信号（`var wg sync.WaitGroup; wg.Add(N)`）
    * 多对一异步事件信号采用首例（使用try一对一）
    * 异步定时器（`<-time.After(dur)`）


* 优雅的关闭通道（通知所有接受方和发送方交流结束）
    * **只能由通道的唯一发送者关闭通道**，本质上是 **不能向closed通道send，不能关闭closed通道，否则引发panic**
    * 一个sender：sender作主动方，直接`close(dataChan)`即可
        > 应用上述“一对多异步事件信号”模型
    * 多个sender一个receiver：receiver作主动方，`close(stopChan)`通知senders都结束返回，从而`dataChan`被垃圾回收
        > 应用上述“一对多异步事件信号”模型和“select多路复用”模型
    * 多个sender多个receiver：引入中间者
        * 任一sender或receiver尝试发送一对一异步信号给中间者
        ```go
