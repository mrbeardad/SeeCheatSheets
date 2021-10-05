# 系统构建
* 项目初始化
```sh
npx create-react-app react-app
cd react-app
npx install-peerdeps --dev eslint-config-airbnb
npm install react-router-dom @reduxjs/toolkit react-redux --save
```

* 目录结构
```txt
my-app/
  README.md
  node_modules/
  package.json
  public/   /* index.html导入的文件，不会被编译处理 */
    index.html    /* page template */
    favicon.ico   /* 网页图标 */
    manifest.json /* 用于安装WebApp时的元数据 */
    robots.txt    /* 机器人排除协议 */
  src/      /* index.js导入的文件，需要编译处理 */
    index.js      /* entry point */
    index.css
    App.css
    App.js
    App.test.js
    logo.svg
```
* 传统做法将HTML(语义)、CSS(样式)、JS(动态控制前两者)分开，而React将三者结合封装到JSX组件中
* 利用JSX可更简洁直观的在JS中插入HTML，同时在HTML中调用JS表达式、组件(function|class)
* 通过JSX导入src目录下的js、css、image、font等文件的优点
  * 尽量合并文件从而减少网络IO
  * 缺失的文件导致编译错误而非给用户显示404
  * 编译结果文件名包含hash值从而防止客户端浏览器缓存旧网页


```js
import React from 'react';
import ReactDOM from 'react-dom';

class MyComponent extends React.Component {   // 组件名必须大写字母开头
    constructor(props) {
        super(props);       // 初始化props(只读)
        this.state = {};    // 初始化state(可写)
        this.handleChange = this.handleChange.bind(this); // 保证调用该方法时this上下文指向该类实例
    }

    handleChange(event) {
        this.setState({name: event.target.name});   // 使用setState异步增量修改代替直接手动直接修改
        this.setState((state, porps) => ({          // 当更新依赖上次更新结果时应该传入修改函数
            name: event.target.name
        }));
    }

    render() {   // render方法时必要的，若返回null则阻止渲染
        return (
            <div className="my" onClick={this.handleChange}>
                {this.props.children}
            </div>
        );
    }
}

ReactDOM.render(
    <MyComponent propsKey={propsVal}>
      <Child>
    </MyComponent>,
    document.getElementById("root");
);
```

# 组件基础
* `props`
  * 声明：`super(props)`, `funtion f(props)`
  * 调用：`<MyComponent propsProperty="value"> <Child /> </MyComponent>`
  * 包含：`props.children`

* `state`
  * 声明：`this.state = {}`
  * 修改：
    * `this.setState({name: event.target.name})`
    * `this.setState((state, props) => ({name: state.name + event.target.name}))`

* `handle`
  * 声明：`handle(event) {}`
  * 绑定：`this.handle = this.handle.bind(this)`

* `env`
  * js中调用：`process.env.NODE_ENV`
  * public/html中调用：`<a href="%PUBLIC_URL%">%REACT_APP_WEBSITE%</p>`
  * `NODE_ENV`：development, test, production
  * `PUBLIC_URL`：public目录中的文件资源在编译构建后的路径前缀
  * `REACT_APP_*`：react app环境变量

# 生命周期
React自身维护一个虚拟DOM，每次组件更新时可能会更新虚拟DOM，利用diff算法找出更新部分再更新浏览器DOM，而避免让浏览器重新渲染整个页面；
每个React组件都是一个状态机，状态切换时会执行对应回调函数（见[react lifecycle](https://projects.wojtekmaj.pl/react-lifecycle-methods-diagram/)）；
组件状态机的更新事件一般都是依赖浏览器事件触发的。


# 技巧与优化
## 重新渲染优化
* PureComponent
* shouldComponentUpdate()
* React.memo(MyComponent, areEqual)
* list&key：为`<li>`指定属性`key=id`可优化diff算法

## 受控组件
使表单输入值完全受控于`this.state`
```js
class MyComponent extends React.Component {
  /* ... */
  render() {
    return <input type="text" value={this.state.value} onClick={this.handle}>
  }
}
```

## 多元返回
```js
function App() {
  return (
    <>
    <tr>1</tr>
    <tr>2</tr>
    </>
  )
}
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

## 代码分割
正常情况下build操作会将import的包合并到一个文件中，有时代码过于庞大则需要一点技巧来提示打包器分割代码
```js
import React  from 'react';
import MyErrorBoundary from './MyErrorBoundary';

const OtherComponent = React.lazy(() => import('./OtherComponent'));    // 懒加载模块defult export
const AnotherComponent = React.lazy(() => import('./AnotherComponent'));

const MyComponent = () => ( // 将懒加载组件作为Suspense子组件实现优雅降级UI
  <div>
    <MyErrorBoundary>
      <React.Suspense fallback={<div>Loading...</div>}>
        <section>
          <OtherComponent />
          <AnotherComponent />
        </section>
      </React.Suspense>
    </MyErrorBoundary>
  </div>
);
```


## 网页路由
```js
import {BrowserRouter, Link, Switch, Route} from 'react-router-dom';

const ShowUI = (props) => {
  console.log(props);
  return <p>{props.match.url}</p>;
};

const App = () => (
  <BrowserRouter basename="/route">
    <ul>
      <li><Link to="/">Home</Link></li>
      <li><Link to="/fuck">Fuck</Link></li>
      <li><Link to="/you">You</Link></li>
      <li><Link to="/fuck/you">Fuck You</Link></li>
      <li><Link to="/fuck/you/man">Fuck You Man</Link></li>
    </ul>
    <Switch>
      <Route path="/" exact component={ShowUI} />
      <Route path="/fuck" component={ShowUI} />
      <Route path="/fuck/you" component={ShowUI} />
      <Route path="/fuck/you/*" component={ShowUI} />
    </Switch>
  </BrowserRouter>
);
```
* 原理：利用接口监听URL变更，阻止浏览器发送http请求，转而根据路由判断渲染各Route组件
* `<BrowserRouter>`：使用HTML5 history API实现前端路由
* `<HashRouter>`：使用URL hash实现前端路由
* `<Link>`：跳转链接
  * `to=string`：目标路径名
  * `replace=bool`：替换而非添加到history stack中
* `<NavLink>`：同上，若与当前url匹配则额外添加样式
  * `activeClassName=string`：指定匹配时的className
  * `activeStyle=object`：直接指定css样式
  * `exact=bool`：是否精准匹配
  * `strict=bool`：是否严格模式（匹配包括末尾斜杠）
* `<Switch>`：所有匹配的Route组件都会被渲染，使用Switch包装起来则只会渲染第一个匹配的Route
* `<Prompt>`：当用户离开页面时展示提示信息
  * `message=string`
  * `message=func`
  * `when=bool`
* `<Route>`：路由组件，若路由匹配则渲染该组件
  * 路径匹配
    * `path=string`：匹配路由路径，支持正则表达式以及url path parameter
    * `exact=bool`：是否精准匹配，默认只需匹配路径前缀
    * `strict=bool`：是否严格模式（匹配包括末尾斜杠）
    * `sensitive=bool`：是否大小写敏感
  * 渲染方法
    > 每个inline function都是不同类型的实例，当使用component=inline func时，更新组件时React判断旧组件被卸载了，新组件被加载了，而实际上我们只需重新尝试render。
    * `component=Component`：class or function组件
    * `render=func`：inline function组件
    * `children=func`：inline function组件，无论是否匹配都会被渲染，利用传入的match，组件可自行依据路径匹配决定哪些要渲染而哪些不渲染
  * 组件属性
    * history
    * match
      * path：即传给`<Route />`的path属性
      * url：path所匹配的url
      * params：匹配的url path paramter
      * isExact：是否精准匹配
    * location
      * pathname：当前url path
      * search：当前url query
      * hash：当前url hash

## 组件通讯
### 状态提升
* 将多个子组件中的共享数据提升到最近公共父组件中去：`this.state.date`
* 然后传入子组件：`<Child shared={this.state.date} changeShared={this.onChange} />`
* 子组件使用：`let data = this.props.shared`
* 子组件修改：`this.props.changeShared()`

### 组件提升
* 当封装大组件时，需要传递一个接口值到组件深处使用，则提升该深处的组件
```js
function Page(props) {
  const user = props.user;
  const userLink = (
    <Link href={user.permalink}>
      <Avatar user={user} size={props.avatarSize} />
    </Link>
  );
  return <PageLayout userLink={userLink} />;
}

// 现在，我们有这样的组件：
<Page user={user} avatarSize={avatarSize} />
// ... 渲染出 ...
<PageLayout userLink={...} />
// ... 渲染出 ...
<NavigationBar userLink={...} />
// ... 渲染出 ...
{props.userLink}
```

### Context
* 组件向上寻找最近的Context，这种做法会加强耦合降低复用性
* Context更新时会强制更新渲染其消费者
```js
const ThemeContext = React.createContext('light');  // 1

class App extends React.Component {
  render() {
    return (
      <ThemeContext.Provider value="dark"> // 2
        <Toolbar />
      </ThemeContext.Provider>
    );
  }
}

function Toolbar() {
  return (
    <div>
      <ThemedButton />
    </div>
  );
}

class ThemedButton extends React.Component {
  static contextType = ThemeContext; // 3

  render() {
    return <Button theme={this.context} />; // 4
  }
}
```

### Ref
```js
let ref = React.createRef();  // ref将指向了该div元素
<div ref={ref}></div>

const FancyButton = React.forwardRef((props, ref) => (
  <button ref={ref} className="FancyButton">
    {props.children}
  </button>
));

const ref = React.createRef();  // ref将指向button
<FancyButton ref={ref}>Click me!</FancyButton>;
```

### Redux
为实现父子组件双向通讯，需将state提升至全局，利用redux来管理该全局状态
* Store：存储State的容器
  * getState
  * dispatch
  * subscribe
* State：集成所有应用中组件的state
* Action：提供操作State的元数据
* Reducer：注册于Store而根据(state, action)来实际操作State
* Listener：当State改变时调用Listener
```js
// 简单demo
import {createStore} from 'redux'

const store = createStore(reducer) // 创建store

function reducer(state = {value: 0}, action) { // 实现reducer
  switch(action.type) {
    case 'counter/incremented':
      return state.value + 1
    case 'counter/decremented':
      return state.value - action.payload
  }
}


store.subscribe(() => console.log(store.getState())) // 注册监听器

store.dispatch({type: 'counter/incremented'}) // 调用reducer(state, action)
store.dispatch({type: 'counter/incremented'})
store.dispatch({type: 'counter/decremented'})

// 高级demo
import { createSlice, configureStore } from '@reduxjs/toolkit'
/*
 * 1个store对应多个stateSlice
 * 1个stateSlice对应1个reducerSlice
 * 1个reducerSlice对应多个action
*/
const counterSlice = createSlice({
  name: 'counter',
  initialState: {
    value: 0
  },
  reducers: {
    incremented: state => {
      state.value += 1  // 修改的是state副本
    },
    decremented: (state, action) => {
        state.value -= action.payload
    }
  }
})

const store = configureStore({
  reducer: {
    counter: counterSlice.reducer
  }
})

export const { incremented, decremented } = counterSlice.actions

store.subscribe(() => console.log(store.getState()))

store.dispatch(incremented()) // {value: 1}
store.dispatch(incremented()) // {value: 2}
store.dispatch(decremented()) // {value: 1}

// 实践
import { Provider, useSelector, useDispatch, useStore } from 'react-redux'
import store from './app/store'

ReactDOM.render(
  <React.StrictMode>
    <Provider store={store}>
      <App />
    </Provider>
  </React.StrictMode>,
  document.getElementById('root')
);

function App() {
  // 利用hooks访问store
  let store = useStore(); // store
  let state = useSelector(state => state.counter); // store.getState().counter
  let dispatch = useDispatch(); // store.dispatch
}
```