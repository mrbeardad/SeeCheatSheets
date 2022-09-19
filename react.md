# React

## 系统构建

### 初始化

```sh
pnpx create-react-app react-app && cd react-app

eslint --init

pnpm add redux react-redux @reduxjs/toolkit \
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

### 环境变量

- js 中调用：`process.env.NODE_ENV`
- public/html 中调用：`<a href="%PUBLIC_URL%">%REACT_APP_WEBSITE%</p>`

| 变量          | 值                                                                                                    |
| ------------- | ----------------------------------------------------------------------------------------------------- |
| `NODE_ENV`    | `development`, `test`, `production`                                                                   |
| `PUBLIC_URL`  | public 目录中的文件资源在编译构建后的路径前缀，通过修改*package.json*中的`homepage`可修改该环境变量值 |
| `REACT_APP_*` | react app 环境变量                                                                                    |

## 核心思想

### 组件树

- 每个组件负责生成一段 HTML，组件们被根组件一层层递归包含并最终构建完整 VDOM
- 每次 render 更新 VDOM 并将 diff 更新于浏览器 DOM 而非完全重新渲染

```js
function MyComponent(props) {
  const [state, setState] = useState(props.initVal);
  return (
    <>
      <botton onClick={(e)=>{e.preventDefault();}}>
        Click Here
      </button>
      : {state}
      {props.children}
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
- 用户定义组件必须首字母大写，且必须返回一个 JSX 表达式（可以用`<>...</>`封装多个表达式）

### [生命周期](https://projects.wojtekmaj.pl/react-lifecycle-methods-diagram/)

1. 当组件首次被调用时，挂载并渲染组件；
2. 当异步调用组件的`setState()`且更改了`state`，则尝试重新渲染整个子组件树
   - 利用`React.PureComponent`与`React.memo()`会在重新渲染前浅比较`props`的每个子属性，若无变化则无需重新渲染
3. 当组件不再被调用时卸载子组件。

> **严格模式下的开发模式中，setState()等生命周期函数可能会被连续调用两次！**

```js
// useState() （基础Hook）用于获取/更新组件状态
const [state, setState] = useState(initialState);
const [state, setState] = useState(() => lazyInit());
setState(newState); // 全量替换旧状态
setState((prevState) => prevState + 1); // 依赖旧值进行更新
```

```js
// useEffect() （基础Hook）用于每次渲染后执行副作用（组件应该是纯函数，所有副作用都应通过 useEffect 执行
useEffect(() => {
  const subscription = props.source.subscribe();
  // 返回的清理函数会在组件下一次调用 effect 前以及卸载前执行
  return () => {
    subscription.unsubscribe();
  };
  // 第二参数传递 memo 数组，表示仅当 memo 更改时调用 effect，[]空数组表示仅在首次渲染后调用
}, [memo]);
```

```js
// useMemo() 用于缓存计算结果数据
const memoizedValue = useMemo(() => computeExpensiveValue(a, b), [a, b]);

// useCallback() 用于缓存临时闭包函数
const memoizedCallback = useCallback(() => {
  doSomething(a, b);
}, [a, b]);
```

Hook 就是 JavaScript 函数，但是使用它们会有两个额外的规则：

- 只能在 React 的函数组件和自定义 Hook 中调用 Hook。不要在其他 JavaScript 函数中调用。
- 只能在函数最外层调用 Hook。不要在循环、条件判断或者子函数中调用。

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
// features/counterSlice.js
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

## Router

```js
import ReactDOM from "react-dom";
import { BrowserRouter, Routes, Route } from "react-router-dom";

ReactDOM.createRoot(document.getElementById("root")).render(
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
    </Routes>
  </BrowserRouter>
);
```

- `<BrowserRouter>`：负责利用 history API 管理浏览器当前地址栏状态 URL
- `<Routes>`：负责当地址栏更改时重新渲染路径匹配且优先级最高的一条路由上的所有组件，由嵌套`<Route>`形成的路由组件树的某条分支共同参与路径匹配。父路由组件需要返回`<Outlet/>`才能渲染子路由组件
- `<Route path="pathname" element={<Component/>}/>`：路由组件，路径匹配支持`static`, `:dynamic`, `global/*`
  - `index`：当仅匹配父路由组件时，将该路由组件也加入渲染
  - `caseSensitive`：大小写敏感
- `<Link to="pathname">`：路由链接
- `<NavLink to="pathname">`：路由链接，若`pathname`匹配当前 URL 则会添加属性`class="active"`
  - `end`表示精准匹配，而非默认的前缀匹配
  - `className`与`style`：可以接受函数`({isActive})=>{}`来条件设置 class 与 style

Hooks

```js
// useParams
<Route path=":userId" element={<ProfilePage />} />;

let { userId } = useParams();

// useLocation
let location = useLocation();

interface Location {
  pathname: string;
  search: string;
  hash: string;
  state: unknown;
  key: string;
}

// useNavigate
let navigate = useNavigate();

navigate(-1); // 后退 history
navigate("../success", { replace: true }); // 追加或替换 history

// useSearchParams
let [searchParams, setSearchParams] = useSearchParams();
setSearchParams(params);
```

## Form

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

## Others

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
