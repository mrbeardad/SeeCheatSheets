# 系统构建
**项目初始化**
```sh
npx create-react-app react-app
cd react-app
npm install eslint htmlhint --save
npx install-peerdeps --dev eslint-config-airbnb
npm install immutable @reduxjs/toolkit react-redux react-router-dom react-hook-form \
  @mui/material @emotion/react @emotion/styled react-motion --save
```

**目录结构**
```txt
my-app/
  README.md
  node_modules/
  package.json
  public/   // index.html导入的文件，不会被编译处理
    index.html    /* page template */
    favicon.ico   /* 网页图标 */
    manifest.json /* 安装WebApp时所需元数据 */
    robots.txt    /* 机器人排除协议 */
  src/      // index.js导入的文件，需要编译处理
    index.js      /* entry point */
    index.css
    App.css
    App.js
    App.test.js
    logo.svg
```

**核心思想**
* 传统做法将HTML（语义）、CSS（样式）、JS（动态控制前两者）分开，而React将三者结合封装到JSX组件中（利用JSX可更简洁直观的在JS中插入HTML，同时在HTML中调用JS表达式、组件，CSS则通过导入JSX的方式）
* 组件被一层层的嵌套导入，形成一个组件树，最终生成完整的html然后被渲染成网页。动态组件通常会注册事件处理函数，当执行时更新了组件内容(props, state)时，会触发react render来更新生成的html
* 通过JSX导入src目录下的js、css、image、font等文件的优点
  * 尽量合并文件从而减少网络IO
  * 缺失的文件导致编译错误而非给用户显示404
  * 编译结果文件名包含hash值从而防止客户端浏览器缓存旧网页


# 生命周期
* React自身维护一个虚拟DOM，每次组件触发react render后，会利用diff算法检查虚拟DOM与浏览器DOM的区别，从而仅修改部分浏览器DOM而非完全替换导致重新渲染整个页面
* 每个React组件相当于一个状态机：
  * 事件一（当组件类型变化时）：会卸载旧组件，挂载新组件并触发react render
  * 事件二（组件修改了props或state）：一般会触发react render
    1. 当注册的浏览器事件处理函数调用时，一般会调用setState()
    2. 当调用setState()时，re-render该组件
    3. 当修改了子组件的props时，re-render该子组件
    4. 如此递归
> 详情见[react lifecycle](https://projects.wojtekmaj.pl/react-lifecycle-methods-diagram/)）
  
**减少不必要的调用render是重要的性能优化手段**
* `<il key=id>`：为`<il>`指定key属性可优化diff算法
* `class Component extends React.PureComponent`：默认shouldComponentUpdate()浅比较props与state
* `const Component = React.memo(ComponentCore)`：同上
* `import {List, Set, Map, OrderedSet, OrderedMap, fromJS} from 'immutable-js'`
  > 该库提供了一些容器集合类，这些类是不可变类型，即任何试图修改其内容的操作都会返回一个新对象； 使用了结构共享技术，返回的新对象会尽量与原对象共享子对象引用节点，同时做深度值比较也更快速； 特别的，当修改操作后的值并未变化时，直接返回原对象引用


# 组件基础
```js
import React from 'react';
import ReactDOM from 'react-dom';

// 类声明形式的组件
class ParentComponent extends React.Component {   // 组件名必须大写字母开头
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

  render() {   // render方法是必要的，若返回null则阻止渲染
    return (
      <div>
        {this.props.children}
        <button type="button" onClick={this.hadleChange}>A</button>
        {/* deprecated: 注意传递匿名函数因每次类型都不同而导致该组件的卸载与重新加载 */}
        <button type="button" onClick={(event) => this.hadleChange(event)}>B</button>
      </div>
    );
  }
}

// 函数声明形式的组件，相当于类声明形式的render函数
function ChildComponent(props) {
  return (
    // 只能返回一个元素，若想返回多个则如下语法
    <>
      <tr>1</tr>
      <tr>2</tr>
    </>
  );
}

ReactDOM.render(
  // 声明式调用组件，通过添加属性或嵌套子元素的方法来给组件传递props属性
  <ParentComponent propsKey={propsVal}>
    <table>
      <tbody>
        <ChildComponent>
      </tbody>
    </table>
  </ParentComponent>,
  document.getElementById("root"),
);
```
* 注意`props`的只读性质以及如何传递；
* 注意`state`的可写性质以及如何修改；
* 注意`handle`方法的`.bind(this)`以及
* **Hooks**:
  * `useState(initValOrLazyFunc)`：在第一次render前
  * `useEffect(funcReturnCleaner, [memo])`：在每次render后，记忆memo若其未变更则不调用
  * `useMemo(funcGenereteVal, [memo])`：记忆memo若其未变更则不调用


# 技巧方法
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

## 环境变量
* js中调用：`process.env.NODE_ENV`
* public/html中调用：`<a href="%PUBLIC_URL%">%REACT_APP_WEBSITE%</p>`
* `NODE_ENV`：development, test, production
* `PUBLIC_URL`：public目录中的文件资源在编译构建后的路径前缀
* `REACT_APP_*`：react app环境变量


# 第三方库
## 状态管理
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

// 实践demo
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
为实现父子组件双向通讯，需将state提升至全局，利用redux来管理该全局状态
* Store：存储State的容器
  * getState
  * dispatch
  * subscribe
* State：集成所有应用中组件的state
* Action：提供操作State的元数据
* Reducer：注册于Store而根据(state, action)来实际操作State
* Listener：当State改变时调用Listener

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

## 表单控制
```js
import { useForm } from "react-hook-form";

export default function App() {
  // 创建表单控制器，提供方法统一存储、访问、操作表单数据
  const {
    handleSubmit, // 用于处理submit成功或失败
    register,     // 用于为<input>设置属性
    getValue,     // 获取表单值
    setValue,     // 修改表单值
    formState: {
      isDirty,
      dirtyFields,
      touchedFields,
      isSubmitted,
      isSubmitSuccessful,
      isSubmitting,
      submitCount,
      isValid,
      isValidating,
      errors,
    }
  } = useForm({defaultValues: {example: "default"}});

  // 订阅change事件，用户每次输入都会触发render与validation
  console.log(getValue("example"));

  const onSubmit = (data, event) => console.log(data);
  const onError = (errors, event) => console.log(errors);

  return (
    // 每次表单内容没修改都会触发render与validate
    <form onSubmit={handleSubmit(onSubmit, onError)}>
      <input defaultValue="test" {...register("example")} />
      <br />
      <input {...register("exampleRequired", { required: true })} />
      {errors.exampleRequired && <span>This field is required</span>}
      <br />
      <input type="submit" />
    </form>
  );
}

// useFormContext
import { useForm, FormProvider, useFormContext } from "react-hook-form";

export default function App() {
  const methods = useForm();
  const onSubmit = data => console.log(data);

  return (
    <FormProvider {...methods} > // pass all methods into the context
      <form onSubmit={methods.handleSubmit(onSubmit)}>
        <NestedInput />
        <input type="submit" />
      </form>
    </FormProvider>
  );
}

function NestedInput() {
  const { register } = useFormContext(); // retrieve all hook methods
  return <input {...register("test")} />;
}
```

## UI库
```js
import { Button } from '@mui/material/Button';
```
主题：配色、形状、图标、字体、动画

* Box: 用于包裹组件来调整样式
* Typography: 用于包裹内容来排版
* AppBar为flex:column+position:fixed，ToolBar为flex:raw+position:block