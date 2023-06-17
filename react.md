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
  - 子元素，如`<tag>{name}</tag>`，合法类型包括 null、undefined、Boolean、Number、String、Array、JSX，其中前三者不会被渲染

## Component

- 应用启动：调用根组件递归构造 VDOM 树，然后再由 VDOM 树构造 DOM 树，最后浏览器根据 DOM 树渲染首帧画面
- 重新渲染：当组件关联的状态发生状态转移时，会重新调用组件构造新的 VDOM 子树，并对比新旧来更新对应 VDOM，然后在更新对应 DOM，最后更新画面

组件视图仅与组件状态有关，即 $View=f(Status)$，当状态发生变化时（状态转移）视图也会发生变化（重新渲染）

- 状态
  - 组件内部状态
  - 组件属性
  - 组件共享状态
  - UI 输入：特殊的 IO 事件，如用户表单输入
  - IO 事件：通常携带了环境状态用来对 Widget 进行状态转移，如平台交互、UI 交互、文件、网络、IPC 等
- 状态转移：事件是状态转移的唯一来源，`setState`是状态转移的唯一手段，状态转移通常由库代码自动执行（如`useMedia`）或你手动执行（如`onClick`）

保持组件简单，用 Hooks 提供状态数据和状态转移逻辑

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
<input ref={ref} />;
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

- 服务端组件：（默认）完全在服务端运行并渲染，用来保护敏感信息、更快访问后端数据、减少客户端代码

  - 可以直接嵌套客户端组件
  - 可以传递属性给客户端组件，但因为需要先将属性序列化，所以不能传递函数
  - 无法使用 Hooks 和浏览器 API，意味着其无法进行状态转移
  - 服务端组件之间共享状态可通过单例，且相同的`fetch`结果会自动共享而无序担心多余开销
  - 服务端组件支持`async`，阻塞时可由`<Suspense>`处理

- 客户端组件：（`"use client"`）发送给客户端运行并渲染，建议仅在服务端组件无法满足需要时使用

  - 只能间接嵌套服务端组件（通过`props.children`），因为客户端组件导入的组件都会自动转换为客户端组件
  - 无法传递属性给服务端属性，因为无法为`props.children`设置属性

- 渲染：服务端收到网页请求后，服务端组件代码在服务端运行并负责生成 ReactNode（可能被缓存），客户端组件代码发送给客户端运行并负责接受服务端传来的 ReactNode 然后重建完整的 React 树

  - 服务端是否缓存服务端组件结果取决于：1、fetch 是否缓存；2、是否需要动态信息（如 url、cookie 等）

- 预渲染：编译构建时预先运行一遍组件渲染流程来生成初始 HTML，代替原来完全由客户端代码运行后生成，可以提高首屏渲染速度
