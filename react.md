# React

- [React](#react)
  - [JSX](#jsx)
  - [Component](#component)
  - [Hook](#hook)
  - [Server Component](#server-component)

## JSX

```js
function Component(porps) {
  user = props.user;
  return (
    <>
      <h1>Hello World</h1>
      <img
        className="avatar"
        src={user.imageUrl}
        alt={"Photo of " + user.name}
        style={{
          width: user.imageSize,
          height: user.imageSize,
        }}
      />
      {props.children}
    </>
  );
}
```

原始 Web 应用将内容(HTML)、样式(CSS)、逻辑(JS)分开，但随着应用交互性越来越强，很多时候内容需要逻辑来决定，所以将 HTML 与 JS 混合在一起编写更加方便、灵活。

- JSX 标签名可以是 HTML 标签，其属性名有些许变动，因为 JS 变量不能包含`-`且不能为保留关键字，如`stroke-width`->`strokeWidth`、`class`->`className`
- JSX 标签名也可以是自定义组件，组件即大写首字母的函数，属性存储在参数`props`中，子元素存储在`props.children`中
- 在 JS 代码中嵌入 JSX 表达式
  - 嵌套 JSX 需要用 `()` 包裹
  - JSX 必须闭合，如`<tag></tag>`或`<tag/>`
- 在 JSX 表达式中嵌入 JS 表达式
  - 属性，如`<tag src={name}>`或`<tag {...name}>`
  - 子元素，如`<tag>{name}</tag>`，合法类型包括 undefined、null、Boolean、Number、String、Array、JSX，其中前三者不会被渲染

## Component

组件视图仅与组件状态有关，即 $View=f(Status)$。当事件触发状态转移后视图也会发生变化，即重新渲染。

- 状态数据
  - 内部状态
  - 共享状态
  - UI 输入：特殊的 IO 事件，如用户表单输入等
  - IO 事件：事件发生时通常会携带外部数据，如 UI 交互、网络通讯等
- 监听事件：通过设置组件属性，如`onClick`
- 状态转移：调用状态修改方法，如`setState`

> 保持组件简单，用 Hooks 封装状态数据和状态转移逻辑

- 应用启动：调用根组件递归构造 VDOM 树，然后再由 VDOM 树构造 DOM 树，最后浏览器根据 DOM 树渲染首帧画面
- 重新渲染：当组件关联的状态发生状态转移时，会重新调用组件构造新的 VDOM 子树，并对比新旧来更新对应 VDOM，然后在更新对应 DOM，最后更新画面

## Hook

> - Hooks 因其特殊实现，只能在组件内顶层作用域或自定义 Hooks 中调用；

- State Hooks: 为组件维护当前的状态数据，并提供状态转移的能力
  > 在严格模式下，React 将调用你的初始化函数两次，以帮助你找到意外的杂质。这是仅用于开发的行为，不会影响生产。

```js
const [state, setState] = useState(initState);
const [state, setState] = useState(() => initState);
// 显示或隐式地调用 setState 可能会触发重新渲染整个子组件数
// 前提时比较前后的状态不相等，即发生了状态转移
setState({ ...state, mod: 1 });
setState((prevState) => ({ ...prevState, mod: 1 }));
```

- Context Hooks: 方便灵活地从遥远的祖先组件获取数据

```js
const Context = createContext(0)

<Context.Provider value={state}>

ctxVal = useContext(Context)

</Context>
```

- Ref Hooks: 用来缓存一些与状态无关的数据，如 DOM

```js
// 缓存状态但不触发重绘
const inputRef = useRef(null);

// inputRef.current 引用缓存状态
const handler = () => inputRef.current.focus();

// 利用 ref 属性将该 DOM 赋值给 inputRef
<input ref={inputRef} />;
```

- Effect Hooks: 仅在发生特定的状态转移时执行逻辑

```js
useEffect(() => {
  // 在组件挂载时，和指定状态发生变化时执行
  const connection = createConnection(serverUrl, roomId);
  connection.connect();
  // 可选的返回清理函数，在下次调用主逻辑前或组件卸载前执行
  return () => {
    connection.disconnect();
  };
  // 监听的状态，为空表示仅在组件挂载时执行主逻辑，且仅在卸载时执行清理逻辑
}, [serverUrl, roomId]);
```

- Performance Hooks

```js
// useMemo() 用于缓存计算结果数据
const memoizedValue = useMemo(() => computeExpensiveValue(a, b), [a, b]);

// useCallback() 用于缓存临时闭包函数
const memoizedCallback = useCallback(() => {
  doSomething(a, b);
}, [a, b]);
```

还可以用高阶组件`memo`来减少组件重新渲染，
原理是当父组件重新渲染时会直接重新渲染整个子组件树，
使用`memo(SubWidget)`会先比较子组件每个属性是否发生变化，若均相等则不再重绘。

## Server Component

- 服务端组件

  - 优点：减小客户端代码体积、获取后端数据更快、隐藏敏感数据
  - 可以直接导入并嵌套客户端组件，反之则不能，因为服务端组件导入客户端组件则自动转为客户端组件
  - 可以传递属性给客户端组件，但无法直接传递函数，因为函数无法序列化，可以使用 Server Action 代替
  - 无法使用 Hooks 和浏览器专属 API，意味着其无状态（组件代码仅运行一次）且无交互
  - 支持`async`，通过使用`<Suspense fallback={<Loading/>}>`内嵌服务端组件可在其阻塞时渲染`fallback`从而使客户端可以异步加载服务端组件

- 缓存

| Mechanism           | What                          | Where  | Purpose                                         | Duration                        |
| ------------------- | ----------------------------- | ------ | ----------------------------------------------- | ------------------------------- |
| Request Memoization | Return values of functions    | Server | Re-use data in a React Component tree           | Per-request lifecycle           |
| Data Cache          | Fectch and Route Handler Data | Server | Store data across user requests and deployments | Persistent (can be revalidated) |
| Full Route Cache    | HTML and RSC payload          | Server | Reduce rendering cost and improve performance   | Persistent (can be revalidated) |
| Router Cache        | RSC Payload                   | Client | Reduce server requests on navigation            | User session or time-based      |

- Full Route Cache 会跟随 Data Cache 一同 Revalidate

> - 服务端将渲染结果传给客户端，客户端将两边渲染结果组合(hydrate)在本地重建完整 React VDOM
> - 客户端代码如何传递？答：通过页面 HTML 的资源链接，这些资源可上传至 CDN（需要设置`assetPrefix`）
> - 服务端渲染结果如何获取？答：硬导航（如刷新页面）通过页面 HTML 尾部追加的内联`<script>`，软导航（如路由跳转）通过 fetch
> - 服务端渲染结果的传递格式？答：JSON 序列化后的 ReactNode
