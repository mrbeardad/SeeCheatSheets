# 组件基础
* 传统做法将HTML(语义)、CSS(样式)、JS(动态控制前两者)分开，React将HTML与JS结合封装到一个组件中
* 利用JSX可更简洁直观的在JS中插入HTML，同时在HTML中调用JS表达式、组件(function|class)
* CSR由客户端执行JS代码生成HTML，SSR由服务端执行JS代码生成HTML

```js
class MyComponent extends React.Component {   // 组件名必须大写字母开头
    constructor(props) {
        super(props);       // 初始化props(只读)
        this.state = {};    // 初始化state(可写)
        this.handleChange = this.handleChange.bind(this); // 如此调用该方法时保证this上下文指向该类实例
    }

    handleChange(event) {
        this.setState({name: event.target.name});   // 使用setState异步增量修改代替直接手动直接修改
        this.setState((state, porps) => ({          // 当更新依赖上次更新结果时应该传入修改函数
            name: event.target.name
        }));
    }

    render() {   // render方法时必要的，若返回null则组织渲染
        return ( // 若需要传递额外参数则使用箭头函数包裹
            <div>
                <!-- 受控组件的value的唯一数据源为state -->
                <input type="text" name="name" value={this.state.name} onChange={this.handleChange}>
            </div>
        );
    }
}

ReactDOM.render(
    <MyComponent propsKey={propsVal} />,
    document.getElementById("root");
);
```


# 生命周期
创建一个组件后，可能会被其他父组件调用，最终被ReactDOM渲染，然后更新到浏览器DOM中，渲染成用户界面

![react lifecycle](https://projects.wojtekmaj.pl/react-lifecycle-methods-diagram/)

# 虚拟DOM
React自身维护一个虚拟DOM，每次组件更新时可能会更新虚拟DOM，利用diff算法找出更新部分再更新浏览器DOM，
而避免让浏览器重新渲染整个页面

# 技巧与优化
## 列表与key
* 为`<li>`指定属性`key=id`可优化diff算法

## 状态提升
将多个子组件中的共享数据提升到父组件中去：
* 父组件：`<Child shared={this.state.shared} />`
* 子组件：`let data = this.props.shared`

同样用类似手法提升共享数据的修改方法

## 包含组合
当父组件无法提前得知子组件时，方法如下：
```js
function Parent(props) {
    return (
        <div class="patent">
            {props.children}
        </div>
    );
}

function Child1(props) {
    return ( // <Parent元素中的元素对象作为Parent中的props.children>
        <Parent>
            <p>Hello world</p>
        </Parent>
    );
}

function Child2(props) {
    return ( // <Parent元素中的元素对象作为Parent中的props.name>
        <Parent>
            name = {
                <p>Hello world</p>
            };
        </Parent>
    );
}
```

## 代码分割
正常情况下build操作会将import的包合并到一个文件中，有时代码过于庞大则需要一点技巧来提示打包器分割代码
```js
import React, { Suspense } from 'react';
import MyErrorBoundary from './MyErrorBoundary';

const OtherComponent = React.lazy(() => import('./OtherComponent'));    // 懒加载模块defult export
const AnotherComponent = React.lazy(() => import('./AnotherComponent'));

const MyComponent = () => ( // 将懒加载组件作为Suspense子组件实现优雅降级UI
  <div>
    <MyErrorBoundary>
      <Suspense fallback={<div>Loading...</div>}>
        <section>
          <OtherComponent />
          <AnotherComponent />
        </section>
      </Suspense>
    </MyErrorBoundary>
  </div>
);
```

## 错误边界
错误边界无法捕获以下场景中产生的错误：
* 事件处理
* 异步代码（例如 setTimeout 或 requestAnimationFrame 回调函数）
* 服务端渲染
* 它自身抛出来的错误（并非它的子组件）
```js
class MyErrorBoundary extends React.Component {
  constructor(props) {
    super(props);
    this.state = { hasError: false };
  }

  static getDerivedStateFromError(error) { // 更新 state 使下一次渲染能够显示降级后的 UI
    return { hasError: true };
  }

  componentDidCatch(error, errorInfo) { // 打印错误信息
    logErrorToMyService(error, errorInfo);
  }

  render() {
    if (this.state.hasError) {
      return <h1>Something went wrong.</h1>;
    }
    return this.props.children;
  }
}
```