# 系统构建

## 项目初始化
```sh
npx create-react-app react-app
cd react-app
npm install -S eslint eslint-plugin-unused-imports htmlhint
npx install-peerdeps --dev eslint-config-airbnb
npm install -S redux react-redux @reduxjs/toolkit immutable redux-immutable \
  react-router-dom react-hook-form \
  @mui/material @mui/icons-material @mui/system @emotion/react @emotion/styled
```

## 目录结构
```txt
my-app/
  README.md
  node_modules/
  package.json
  public/       // index.html导入的文件，不会被编译处理
    index.html    /* page template */
    favicon.ico
    manifest.json
    robots.txt
  src/          // index.js导入的文件，需要编译处理
    index.js      /* entry point */
    App.js
```

# 组件基础

## 核心思想
* 传统做法将HTML（语义）、CSS（样式）、JS（动态控制前两者）分开，而React将三者结合封装到JSX组件中（利用JSX可更简洁直观的在JS中插入HTML，同时在HTML中调用JS表达式、组件，CSS则通过导入JSX的方式）
* 组件被一层层的嵌套导入，形成一个组件树，最终生成完整的html然后被渲染成网页。动态组件通常会注册事件处理函数，当执行时更新了组件内容(props, state)时，会触发react render来更新生成的html
* 通过JSX导入src目录下的js、css、image、font等文件的优点
  * 尽量合并文件从而减少网络IO
  * 缺失的文件导致编译错误而非给用户显示404
  * 编译结果文件名包含hash值从而防止客户端浏览器缓存旧网页
  
## 声明语法
```js
import React from 'react';
import ReactDOM from 'react-dom';

// 类声明形式的组件，函数声明形式的组件类似render方法
class ParentComponent extends React.Component {   // 组件名必须大写字母开头
  constructor(props) {
    super(props);       // 初始化props(只读)，注意如何传递props
    this.state = {};    // 初始化state(可写)，注意如何修改state
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

* JSX组件的编译需要导入`react`以引用`React.createElement`
* 用户定义组件必须首字母大写
* 注意`props`的只读属性以及如何传递
* 注意`state`的可写属性以及如何修改
* JSX 中的 JavaScript 表达式将会被计算为字符串、React组件元素或者是列表
* false, null, undefined, and true 是合法的子元素，但它们并不会被渲染。


# 生命周期
## 核心思想
* React自身维护一个**虚拟DOM**，每次组件触发react render后，会利用diff算法检查虚拟DOM与浏览器DOM的区别，从而仅修改部分浏览器DOM而非完全替换导致重新渲染整个页面
* 每个React组件相当于一个状态机：
  1. 当注册的浏览器事件处理函数调用时，一般会调用setState()
  2. 当调用setState()时，re-render该组件
  3. 当修改了子组件的props时，re-render该子组件
  4. 当修改了子组件的类型时，卸载旧组件并挂载新组件render
> 详情见[react lifecycle](https://projects.wojtekmaj.pl/react-lifecycle-methods-diagram/)
  
## Hooks
Hook 就是 JavaScript 函数，但是使用它们会有两个额外的规则：
* 只能在函数最外层调用 Hook。不要在循环、条件判断或者子函数中调用。
* 只能在 React 的函数组件和自定义Hook中调用 Hook。不要在其他 JavaScript 函数中调用。

### useState
```jsx
const [state, setState] = useState(initialState);
const [state, setState] = useState(() => someExpensiveComputation(props));
setState(newState);                   // 全量更新而非增量更新
setState(prevState => prevState + 1); // 依赖旧值进行更新
```
* useState在初始渲染期间，返回的状态 (state) 与传入的第一个参数 (initialState) 值相同。
* useState在后续的重新渲染中，返回的第一个值将始终是更新后最新的 state。
* 调用setState将跳过子组件的渲染及 effect 的执行。（React 使用 Object.is 比较算法 来比较 state），但可能仍需要在跳过渲染前渲染该组件。

### useEffect
```jsx
useEffect(() => {
  // 执行副作用
  const subscription = props.source.subscribe();
  // 清除订阅
  return () => { subscription.unsubscribe(); };
});
```
* `useEffect`默认 effect 将在每轮渲染结束后执行，`useLayoutEffect`则在DOM更新后且在浏览器渲染前同步调用
* 可传递 effect 所依赖的值数组而在只有某些值改变的时候 才执行，传递`[]`表示只在初次渲染时调用。
* 返回的清除函数会在组件卸载前执行，如果组件多次渲染，则在执行下一个 effect 之前，上一个 effect 就已被清除。

### useContext
```jsx
const ThemeContext = React.createContext(themes.light);

function App() {
  return (
    <ThemeContext.Provider value={themes.dark}>
      <Toolbar />
    </ThemeContext.Provider>
  );
}

function Toolbar() {
  const theme = useContext(ThemeContext);
  /* ... */
}
```

### useRef
```jsx
const MyInput = React.forwardRef((props, ref) => (
  <input ref={ref} type="text" className="FancyButton" />
));

function TextInputWithFocusButton() {
  const inputEl = useRef(null); // inputEl.current最终指向了<input />
  const onButtonClick = () => {
    // `current` 指向已挂载到 DOM 上的文本输入元素
    inputEl.current.focus();
  };
  return (
    <>
      <MyInput ref={inputEl} />
      <button onClick={onButtonClick}>Focus the input</button>
    </>
  );
}
```

### useCallback
```jsx
const memoizedCallback = useCallback(
  () => { doSomething(a, b); },
  [a, b],
);
```
* memoizedCallback只在依赖项更新时才会调用

### useMemo
```jsx
const memoizedValue = useMemo(
  () => computeExpensiveValue(a, b),
  [a, b]
);
```
* memoizedValue只在依赖项更新时才重新计算


# 技巧方法
## 减少渲染
* `<il key=id>`：为`<il>`指定key属性可优化diff算法
* `class Component extends React.PureComponent`：默认shouldComponentUpdate()浅比较props与state
* `const Component = React.memo(ComponentCore)`：同上
* `import {List, Set, Map, OrderedSet, OrderedMap, fromJS, is} from 'immutable-js'`
  > 该库提供了一些容器集合类，这些类是不可变类型，即任何试图修改其内容的操作都会返回一个新对象； 使用了结构共享技术，返回的新对象会尽量与原对象共享子对象引用节点，同时做深度值比较也更快速； 特别的，当修改操作后的值并未变化时，直接返回原对象引用


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

| 变量          | 值                                                                                                   |
| ------------- | ---------------------------------------------------------------------------------------------------- |
| `NODE_ENV`    | development, test, production                                                                        |
| `PUBLIC_URL`  | public目录中的文件资源在编译构建后的路径前缀，通过修改*package.json*中的`homepage`可修改该环境变量值 |
| `REACT_APP_*` | react app环境变量                                                                                    |


# 第三方库
## 状态管理
```js
// tree.js
import { fromJS } from 'immutable';
import { createAction } from '@reduxjs/toolkit';

const initState = fromJS({
  files: {},
});

const setFilesActionType = 'tree/setFiles';
const setContentActionType = 'tree/setFileContent';
export const setFileAction = createAction(setFilesActionType);
export const setContentAction = createAction(setContentActionType);

export function treeReducer(state = initState, { type, payload }) {
  switch (type) {
    case setFilesActionType:
      return state.set('files', fromJS(payload.files));
    case setContentActionType:
      return state.setIn(['files', payload.path, 'content'], payload.content);
    default:
      return state;
  }
}

// store.js
import { createStore } from 'redux';
import { combineReducers } from 'redux-immutable';
import { themeReducer } from './theme';
import { treeReducer } from './tree';

const store = createStore(combineReducers({
  theme: themeReducer,
  tree: treeReducer,
}));

export default store;

// App.js
export default function App() {
  const dispatch = useDispatch();
  const files = useSelector((state) => state.getIn(['tree', 'files']));
  ...
}
```
为实现父子组件双向通讯，需将state提升至全局，利用redux来管理该全局状态
* Store：存储State的容器
* State：集成所有应用中组件的state
* Action：提供操作State的元数据
* Reducer：注册于Store而根据(state, action)来实际操作State
* Listener：当State改变时调用Listener

## 网页路由
```js
// 原理：<BrowserRouter> creates a history, puts the initial location in to state, and subscribes to the URL.
ReactDOM.render(
  <BrowserRouter>
    <Routes>
      <Route path="/" element={<App />}>
        <Route index element={<Home />} />
        <Route path="teams" element={<Teams />}>
          <Route path=":teamId" element={<Team />} />
          <Route path="new" element={<NewTeamForm />} />
          <Route index element={<LeagueStandings />} />
        </Route>
      </Route>
      <Route element={<PageLayout />}>
        <Route path="/privacy" element={<Privacy />} />
        <Route path="/tos" element={<Tos />} />
      </Route>
      <Route path="contact-us" element={<Contact />} />
    </Routes>
  </BrowserRouter>,
  document.getElementById("root")
);
```
* `<Link>`or`<NavLink>`：跳转链接
  * `to="pathname"`：目标路径名
  * `replace`：替换而非添加到history stack中

* `<Routes>`：路由配置树
  * 从所有路由策略路线中匹配最优策略分支
  * 渲染整个匹配分支需要父级element使用`<Outlet />`代替`props.children`）

* `<Route>`：路由策略
  * `path="pathname"`：自动添加前缀为父级path，支持`/static`、`/:param`、`/global/*`；若无该属性则作为布局路由不参与匹配（单其子路由会参与，若子路由匹配则渲染该布局路由）
  * `index`：当恰好完全匹配父级路由path时，该条路由作为父级`<Outlet />`
  * `caseSensitive`
  * `element={<Component />}`
  
* Hooks
  * useParams()
  * useSearchParams()
  * useLocation()
  ```jsx
  {
    key: 'default',
    pathname: '/somewhere'
    search: '?some=search-string',
    hash: '#howdy',
    state: null,
  }
  ```

## 表单控制
```js
import { useForm, Controller } from "react-hook-form";
import Input from "@material-ui/core/Input";

const App = () => {
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
  } = useForm();

  // 订阅change事件，用户每次输入都会触发render与validation
  const onSubmit = (data, event) => console.log(data);
  const onError = (errors, event) => console.log(errors);

  return (
    <form onSubmit={handleSubmit(onSubmit, onError)}>
      <Controller
        name="firstName"
        control={control}
        defaultValue=""
        render={({ field }) => <Input {...field} />}
      />
      <Controller
        name="iceCreamType"
        control={control}
        render={({ field }) => <Select 
          {...field} 
          options={[
            { value: "chocolate", label: "Chocolate" },
            { value: "strawberry", label: "Strawberry" },
            { value: "vanilla", label: "Vanilla" }
          ]} 
        />}
      />
      <input type="submit" />
    </form>
  );
};
```

## UI
主题：配色、物件形状、图片、图标、字体、动画
### 布局
* Box: 默认block布局，用于包裹其他组件来使用sx属性
* Container: 默认block布局，用于限宽居中布局
* Grid: 默认flex布局，用于二维响应式布局
* Stack: 默认flex布局，用于一维响应式布局