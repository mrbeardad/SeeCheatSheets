# React

## 系统构建

### 项目初始化

```sh
pnpx create-react-app react-app && cd react-app

eslint --init

npm install -S redux react-redux @reduxjs/toolkit \
  react-router-dom \
  react-hook-form \
  @mui/material @mui/icons-material @mui/system @emotion/react @emotion/styled
```

### 目录结构

```txt
react-app/
├─ README.md
├─ package.json
├─ node_modules/
├─ public/          index.html可直接引用的文件，编译打包后位于目录顶层，使用
│  ├─ index.html    可通过 %PUBLIC_URL% 形式引用项目预定义变量
│  ├─ favicon.ico
│  ├─ robots.txt
│  └─ manifest.json
└─ src/             index.js可直接导入的文件，编译打包后位于static目录
   ├─ index.js      优点如下：
   ├─ index.css     1. 会尽量合并文件，减少网络IO
   ├─ App.js        2. 缺失文件直接编译报错，而非给用户显示404
   └─ App.css       3. 编译结果文件名包含了hash，从而利用缓存破坏机制
```

## 核心思想

- 组件：传统做法将 HTML（结构）、CSS（样式）、JS（行为）分开，而 React 将三者灵活地结合封装到组件中。每个组件返回一组 HTML，动态组件可通过注册异步回调函数来更改`props`或`state`导致触发重新渲染，并通过 Hooks 来每次获取最新的状态，从而可动态改变组件返回的 HTML；

- VDOM：组件树被一层层的递归导入最终生成 VDOM，再由此生成浏览器 DOM 并渲染网页。每当触发 render 会重新生成 VDOM，然后仅将 diff 作用于浏览器 DOM 而非完全重新渲染；

- [生命周期](https://projects.wojtekmaj.pl/react-lifecycle-methods-diagram/)：每个组件都是一个状态机：
  1. 当组件首次被调用时，挂载并渲染组件；
  2. 当组件调用`setState()`时，尝试重新渲染整个子组件树
     - 尝试重新渲染组件前会先调用`shouldComponentUpdate`，返回`true`才会重新渲染
     - `React.Component`均返回`true`
     - `React.PureComponent`返回新旧`props`（比较每个属性）与`state`浅比较(`Object.is()`)结果
  3. 当组件不再被调用时（根据子组件的类型与数量判断是否有新增或删减的组件），卸载子组件。

## Components

```js
function MyComponent(props) {
  const [state, setState] = useState(props.initVal);
  return (
    <>
      <botton onClick={(e)=>{e.preventDefault();}}>
        Click Here
      </button>
      : {state}
    </>
  )
}
```

- JSX 表达式
  - 表达式编译后即一个 object
  - 表达式范围内只有`{}`中的字符才被解释为 JS 表达式
  - 标签属性会做为组件参数`props`的子属性传递
  - 子标签元素会作为组件参数子属性`props.children`传递
- 合法子元素：null, undefined, Boolean, Number, String, Array, JSX，其中前三者不会被渲染
- 用户定义组件必须首字母大写，且必须返回一个 JSX 表达式（可以用`<>...</>`封装多个表达式

## Hooks

Hook 就是 JavaScript 函数，但是使用它们会有两个额外的规则：

- 只能在函数最外层调用 Hook。不要在循环、条件判断或者子函数中调用。
- 只能在 React 的函数组件和自定义 Hook 中调用 Hook。不要在其他 JavaScript 函数中调用。

```js
const [state, setState] = useState(initialState);
const [state, setState] = useState(() => lazyInit());
setState(newState); // 全量替换旧状态
setState((prevState) => prevState + 1); // 依赖旧值进行更新
```

```js
useEffect(() => {
  // 执行副作用
  const subscription = props.source.subscribe();
  // 返回清除函数
  return () => {
    subscription.unsubscribe();
  };
}, [a]);
```

- `useEffect`默认在每轮渲染结束后调用`effect`
- `useLayoutEffect`默认在 DOM 更新后且在浏览器渲染前同步调用`effect`
- 第二参数传递数组，表示仅当数组中值更改时才调用，`[]`表示只在初次渲染时调用
- 返回的清除函数会在组件卸载前与下一次调用`effect`执行

## Redux

```js
// index.js
import React from "react";
import ReactDOM from "react-dom";
import { Provider } from "react-redux";
import { store } from "./features/store.js";
import { App } from "./App.js";

ReactDOM.createRoot(document.getElementById("root")).render(
  <Provider store={store}>
    <App />
  </Provider>
);
```

```js
// App.js
import React from "react";
import { useDispatch, useSelector } from "react-redux";
import { increment } from "./features/counterSlice.js";

const selectApp = (state) => state.getIn(["app", "count"]);

export function App() {
  const dispatch = useDispatch();
  const count = useSelector(selectApp);
  const onClick = () => dispatch(increment());
  return <button onClick={onClick}>You've clicked {count} times.</button>;
}
```

```js
// features/counter/counterSlice.js
import { createSlice } from "@reduxjs/toolkit";

export const counterSlice = createSlice({
  name: "counter",
  initialState: {
    value: 0,
  },
  reducers: {
    increment: (state) => {
      // Redux Toolkit allows us to write "mutating" logic in reducers. It
      // doesn't actually mutate the state because it uses the Immer library,
      // which detects changes to a "draft state" and produces a brand new
      // immutable state based off those changes
      state.value += 1;
    },
    decrement: (state) => {
      state.value -= 1;
    },
    incrementByAmount: (state, action) => {
      state.value += action.payload;
    },
  },
});

// Action creators are generated for each case reducer function
export const { increment, decrement, incrementByAmount } = counterSlice.actions;
export const counterReducer = counterSlice.reducer;
```

```js
// app/store.js
import { configureStore } from "@reduxjs/toolkit";
import { counterReducer } from "../features/counter/counterSlice.js";

export default configureStore({
  reducer: {
    counter: counterReducer,
  },
});
```

为实现父子组件双向通讯，需将 state 提升至全局，利用 redux 来管理该全局状态

- Store：存储 State 的容器
- State：集成一个组件的所有状态
- Action：提供操作 State 的元数据
- Reducer：注册于 Store 而根据(state, action)来实际操作 State
- Listener：当 State 改变时调用 Listener

## 技巧方法

### 减少渲染

- `<il key=id>`：为`<il>`指定 key 属性可优化 diff 算法
- `class Component extends React.PureComponent`：默认 shouldComponentUpdate()浅比较 props 与 state
- `const Component = React.memo(ComponentCore)`：同上
- `import {List, Set, Map, OrderedSet, OrderedMap, fromJS, is} from 'immutable-js'`
  > 该库提供了一些容器集合类，这些类是不可变类型，即任何试图修改其内容的操作都会返回一个新对象； 使用了结构共享技术，返回的新对象会尽量与原对象共享子对象引用节点，同时做深度值比较也更快速； 特别的，当修改操作后的值并未变化时，直接返回原对象引用

### 代码分割

正常情况下 build 操作会将 import 的包合并到一个文件中，有时代码过于庞大则需要一点技巧来提示打包器分割代码

```js
import React from "react";
import MyErrorBoundary from "./MyErrorBoundary";

const OtherComponent = React.lazy(() => import("./OtherComponent")); // 懒加载模块defult export
const AnotherComponent = React.lazy(() => import("./AnotherComponent"));

const MyComponent = () => (
  // 将懒加载组件作为Suspense子组件实现优雅降级UI
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

### 错误边界

错误边界无法捕获以下场景中产生的错误：

- 事件处理
- 异步代码（例如 setTimeout 或 requestAnimationFrame 回调函数）
- 服务端渲染
- 它自身抛出来的错误（并非它的子组件）

```js
class MyErrorBoundary extends React.Component {
  constructor(props) {
    super(props);
    this.state = { hasError: false };
  }

  static getDerivedStateFromError(error) {
    // 更新 state 使下一次渲染能够显示降级后的 UI
    return { hasError: true };
  }

  componentDidCatch(error, errorInfo) {
    // 打印错误信息
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

### 环境变量

- js 中调用：`process.env.NODE_ENV`
- public/html 中调用：`<a href="%PUBLIC_URL%">%REACT_APP_WEBSITE%</p>`

| 变量          | 值                                                                                                    |
| ------------- | ----------------------------------------------------------------------------------------------------- |
| `NODE_ENV`    | development, test, production                                                                         |
| `PUBLIC_URL`  | public 目录中的文件资源在编译构建后的路径前缀，通过修改*package.json*中的`homepage`可修改该环境变量值 |
| `REACT_APP_*` | react app 环境变量                                                                                    |

## 第三方库

### 网页路由

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

- `<Link>`or`<NavLink>`：跳转链接

  - `to="pathname"`：目标路径名
  - `replace`：替换而非添加到 history stack 中

- `<Routes>`：路由配置树

  - 从所有路由策略路线中匹配最优策略分支
  - 渲染整个匹配分支需要父级 element 使用`<Outlet />`代替`props.children`）

- `<Route>`：路由策略
  - `path="pathname"`：自动添加前缀为父级 path，支持`/static`、`/:param`、`/global/*`；若无该属性则作为布局路由不参与匹配（单其子路由会参与，若子路由匹配则渲染该布局路由）
  - `index`：当恰好完全匹配父级路由 path 时，该条路由作为父级`<Outlet />`
  - `caseSensitive`
  - `element={<Component />}`
- Hooks

  - useParams()
  - useSearchParams()
  - useLocation()

  ```js
  {
    key: 'default',
    pathname: '/somewhere'
    search: '?some=search-string',
    hash: '#howdy',
    state: null,
  }
  ```

### 表单控制

```js
import { useForm, Controller } from "react-hook-form";
import Input from "@material-ui/core/Input";

const App = () => {
  // 创建表单控制器，提供方法统一存储、访问、操作表单数据
  const {
    handleSubmit, // 用于处理submit成功或失败
    register, // 用于为<input>设置属性
    getValue, // 获取表单值
    setValue, // 修改表单值
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
    },
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
        render={({ field }) => (
          <Select
            {...field}
            options={[
              { value: "chocolate", label: "Chocolate" },
              { value: "strawberry", label: "Strawberry" },
              { value: "vanilla", label: "Vanilla" },
            ]}
          />
        )}
      />
      <input type="submit" />
    </form>
  );
};
```
