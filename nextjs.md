# Next.js

## 初始化

```sh
pnpx create-next-app next-app
npm install -S redux react-redux @reduxjs/toolkit \
  swr \
  react-hook-form \
  @mui/material @mui/icons-material @mui/system @emotion/react @emotion/styled
```

## 目录结构

```txt
next-app/
├─ public/              编译后位于顶层目录的资源
│  ├─ favicon.ico
│  └─ images/...
├─ pages/               每个文件的默认导出组件对应一个页面，其相对路径就是路由 URL
│  ├─ _app.js           用于包裹所有单页面
│  ├─ index.js          目录 URL 对应的页面
│  ├─ 404.js            404 页面
│  ├─ 500.js            500 页面
│  └─ api/...
├─ components/          可复用的组件
│  └─ layout.js
├─ lib/                 可复用的业务逻辑
│  └─ fetch_data.js
└─ styles/              样式
   ├─ global.css
   └─ layout.module.css
```

## 核心特性

- 预渲染
- 客户端页面路由
- 代码拆分最小化
- 预加载与懒加载

## 预渲染

- 服务端渲染(SSR)：SEO 友好，数据最新，但服务端开销大
- 静态站点生成(SSG)（默认）：SEO 友好，数据不是最新，服务端开销小
- 客户端渲染(CSR)：SEO 不友好，数据最新，服务端开销小

### SSR

```js
// 在每次接收请求时调用，为页面组件提供 props
export async function getServerSideProps(context) {
  const res = await fetch(`https://.../data`);
  const data = await res.json();
  return { props: { data } };
}

// 用于动态编译为页面的组件
export default function Post({ data }) {
  return <Layout>{data}</Layout>;
}
```

### SSG

```js
// 用于静态站点生成(SSG)时给出可能的动态路由，用于静态编译相关页面
export async function getStaticPaths() {
  const fileNames = fs.readdirSync(postsDirectory);
  const paths = fileNames.map((fileName) => {
    return {
      params: {
        id: fileName.replace(/\.md$/, ""),
      },
    };
  });
  return {
    paths,
    fallback: false,
    // false 表示请求没给出的路由则返回 404
    // true 表示请求没给出的路由返回 fallback 页面
    // 'blocking' 表示请求没给出的路由返回 SSR 页面
  };
}

// 在每次静态编译时调用，为页面组件提供 props。开发模式每次请求都被调用
export async function getStaticProps({ params }) {
  const postData = getPostData(params.id);
  return {
    props: {
      postData,
    },
    revalidate: 10, // In seconds. 定时增量编译
  };
}

// 用于静态编译为页面的组件
export default function Post({ postData }) {
  return (
    <Layout>
      {postData.title}
      <br />
      {postData.id}
      <br />
      {postData.date}
    </Layout>
  );
}
```

### CSR

```js
import useSWR from "swr";

const fetcher = (...args) => fetch(...args).then((res) => res.json());

function Profile() {
  const { data, error } = useSWR("/api/profile-data", fetcher);

  if (error) return <div>Failed to load</div>;
  if (!data) return <div>Loading...</div>;

  return (
    <div>
      <h1>{data.name}</h1>
      <p>{data.bio}</p>
    </div>
  );
}
```

## 路由

### API

```js
export default function handler(req, res) {
  res.status(200).json({ text: "Hello" });
}
```
