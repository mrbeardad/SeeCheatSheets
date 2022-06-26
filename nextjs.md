# Next.js

## 系统构建

### 初始化

```sh
pnpx create-next-app next-app
pnpm install -S redux react-redux @reduxjs/toolkit \
  swr \
  react-hook-form \
  normalize.css @mui/material @mui/icons-material @mui/system @emotion/react @emotion/styled
```

### 目录结构

```txt
next-app/
├─ public/              编译后位于顶层目录的资源
│  └─ favicon.ico
├─ pages/               每个文件的默认导出组件对应一个页面，其相对路径就是路由 URL
│  ├─ _document.js      用于更改全局标签<html>,<head>,<body>
│  ├─ _app.js           用于设置全局布局的页面 wrap 组件
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

```js
// pages/_document.js

import Document, { Html, Head, Main, NextScript } from "next/document";

class MyDocument extends Document {
  render() {
    return (
      <Html>
        <Head>
          <link
            href="https://fonts.googleapis.com/css2?family=Inter&display=optional"
            rel="stylesheet"
          />
        </Head>
        <body>
          <Main />
          <NextScript />
        </body>
      </Html>
    );
  }
}

export default MyDocument;
```

```js
// pages/_app.js

export default function App({ Component, pageProps }) {
  // Use the layout defined at the page level, if available
  const getLayout = Component.getLayout || ((page) => page);

  return getLayout(<Component {...pageProps} />);
}
```

### 环境变量

环境变量配置文件：

> $(NODE_ENV): development, production, test

1. `process.env`
2. .env.$(NODE_ENV).local
3. **.env.local** (Not checked when NODE_ENV is test.)
4. .env.$(NODE_ENV)
5. .env

```conf
# 仅用于服务端
HOSTNAME=localhost
PORT=8080
HOST=http://$HOSTNAME:$PORT

# NEXT_PUBLIC_ 开头变量可用于浏览器
NEXT_PUBLIC_ANALYTICS_ID=abcdefghijk
```

## 核心特性

### 预渲染

- 静态站点生成(SSG)：SEO 友好，编译时就获取数据
- 服务端渲染(SSR)：SEO 友好，由服务端获取最新数据
- 客户端渲染(CSR)：SEO 不友好，由客户端获取最新数据

#### SSG

```js
// 用于静态站点生成(SSG)时给出可能的动态路由，用于静态编译相关页面
// https://nextjs.org/docs/api-reference/data-fetching/get-server-side-props
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
// https://nextjs.org/docs/api-reference/data-fetching/get-static-props
export async function getStaticProps(context) {
  const postData = getPostData(context.params.id);
  return {
    props: {
      postData,
    },
    revalidate: 10, // In seconds. 增量静态再生(ISR)
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

#### SSR

```js
// 在每次接收请求时调用，为页面组件提供 props
// https://nextjs.org/docs/api-reference/data-fetching/get-server-side-props
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

#### CSR

```js
import useSWR from "swr";

const fetcher = (...args) => fetch(...args).then((res) => res.json());

// 预渲染无需外部数据的部分，外部数据的获取通过 useEffect 作为副作用在首页渲染后执行
export default function Profile() {
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

### 客户端路由

#### pages

按优先级顺序匹配路由，经过代码拆分后每个页面仅需加载首页渲染需要的代码，其他代码延迟加载。

- `pages/blog.js` → `/blog` or `/blog/`
- `pages/blog/index.js` → `/blog` or `/blog/`
- `pages/blog/[slug].js` → `/blog/:slug` (`/blog/hello-world`)
- `pages/[username]/settings.js` → `/:username/settings` (`/foo/settings`)
- `pages/post/[...all].js` → `/post/*` (`/post/2020/id/title`)

#### next/link

#### next/router

#### api

```js
export default function handler(req, res) {
  res.status(200).json({ text: "Hello" });
}
```

### 图像优化

- 自动提供多分辨率版本并转换为现代化图像格式
- 避免累计布局编译
- 默认懒加载

```js
import Image from "next/image";
import mountains from "../public/mountains.jpg";

const Intrinsic = () => (
  <div>
    <h1>Image Component With Layout Intrinsic</h1>
    <Image
      alt="Mountains"
      src={mountains}
      layout="intrinsic"
      width={700}
      height={475}
    />
  </div>
);

export default Intrinsic;
```

- `src`
  - 静态导入图像
  - 本地相对 URL
  - 远程绝对 URL（需要配置 domain）
- `width`&`height`
  - 静态导入图像可自动获取宽高
  - 对于`fixed`&`intrinsic`会影响渲染宽高
  - 对于`responsive`&`fill`仅影响渲染宽高比
- `layout`
  - `"fixed"`：宽度固定为`width`
  - `"intrinsic"`：最大宽度为`min(width, 100vw)`
  - `"responsive"`：宽度为`100wv`，父元素需要设置`display: block`
  - `"fill"`：配合`object-fit`使用，父元素需要设置`position: relative`
- `quality`：图像质量，1-100(default 75)
- `priority`：预加载代替懒加载
- `placeholder`：加载图像时的占位图，`empty`(default)或`blur`
- `blurDataURL`：当`placeholder="blur"`时提供占位图 URL
  - 静态导入图像可自动生成
  - [Demo the shimmer effect with blurDataURL prop](https://image-component.nextjs.gallery/shimmer)
  - [Demo the color effect with blurDataURL prop](https://image-component.nextjs.gallery/color)
