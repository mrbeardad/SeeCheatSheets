# HTML

## 基础

```html
<!DOCTYPE html>
<html lang="en-US">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover" />
    <meta name="description" content="Description of the page less than 150 characters" />
    <title>页面标题</title>
    <link rel="icon" type="image/png" href="https://example.com/favicon-32x32.png" />

    <link rel="preload" href="images/image.png" as="image" type="image/png" />
    <!-- 预加载：https://developer.mozilla.org/en-US/docs/Web/HTML/Link_types/preload -->
    <!-- 懒加载：https://developer.mozilla.org/en-US/docs/Web/Performance/Lazy_loading -->

    <meta property="og:type" content="website" />
    <meta property="og:url" content="https://example.com/page.html" />
    <meta property="og:title" content="Content Title" />
    <meta property="og:image" content="https://example.com/image.jpg" />
    <meta property="og:description" content="Description Here" />
    <meta property="og:site_name" content="Site Name" />
    <meta property="og:locale" content="en_US" />
    <meta property="og:image:width" content="1200" />
    <meta property="og:image:height" content="630" />

    <link rel="alternate" href="https://zh.example.com/" hreflang="zh" />
    <link rel="alternate" href="https://example.com/" hreflang="x-default" />

    <link rel="stylesheet" type="text/css" href="styles/style.css" />
    <style>
      /* inline css */
    </style>

    <script type="text/javascript" src="scripts/main.js"></script>
    <script>
      // inline javascript
    </script>
  </head>

  <body>
    <noscript>You need to enable JavaScript to run this app.</noscript>

    <header>
      <h1>网站标题信息</h1>
    </header>

    <nav>
      <ul>
        <li>导航栏链接</li>
      </ul>
    </nav>

    <main>
      <article>
        <!--
          article是独立自包含的，其目的是为了独立分发与复用；
          section也是独立的部分，在没有其他更适合的语义标签时使用，内部应该总是包含标题。
        -->
        <section>
          <h2>文章标题</h2>
          <p>文章段落</p>
          <aside>相关旁白</aside>
        </section>
      </article>
      <footer>
        <p>&copy; 2022 版权信息</p>
        <p>联系方式</p>
        <address>
          <a href="mailto:mrbeardad@qq.com">mrbeardad@qq.com</a><br />
          <a href="tel:+13115552368">(311) 555-2368</a><br />
          Chengdu, Sichuan, China
        </address>
      </footer>
    </main>
  </body>
</html>
```

| 通用属性    | 备注                               |
| ----------- | ---------------------------------- |
| `id`        | 元素唯一标识符，可作为`#anchor`    |
| `class`     | 元素的类（逗号分隔列表）           |
| `title`     | 元素咨询信息，通常会在提示框中展示 |
| `autofocus` | 页面加载完成后聚焦该元素           |
| `hidden`    | 指明该元素语义上已不属于当前页面   |
| `style`     | 内联样式                           |

## 语义元素

```html
<!-- 无语义元素，用于样式布局 -->

<div>
  <span>
    <b>粗体</b>
    <i>斜体</i>
    <u>下划线</u>
    <small>缩小</small>
  </span>
</div>

<!-- 内联语义元素 -->

<strong>重要</strong><br />
<em>强调</em><br />
<del>删除</del>
<ins>取代</ins><br />
<kbd>按键</kbd><br />
<mark>标记</mark><br />
<q>引用</q><br />
<cite>作品名</cite><br />
<code>代码</code><br />
<var>变量</var><br />
<samp>程序输出</samp><br />
正常<sup>上标</sup><sub>下标</sub><br />
<data value="001">枚举名</data><br />
<time datetime="2016-01-20T19:30+01:00">20 January 2016, 8.30pm in France</time><br />
<abbr title="完整词汇">缩写词汇</abbr><br />
<a href="https://example.com" target="_self">链接</a>

<details>
  <summary>折叠简介</summary>
  <p>折叠内容</p>
</details>

<!-- 块级语义元素 -->

<ul>
  <li>unordered list item</li>
</ul>

<ol>
  <li>ordered list item</li>
</ol>

<dl>
  <dt>description list title</dt>
  <dd>description list description</dd>
</dl>

<blockquote>blockquote</blockquote>

<pre>
    predefined text that
    remain all blank character
    and use monospace font
</pre>

<figure>
  主文中引用的图片，表格，代码等等
  <figcaption>Fig.1 - The Caption of the Figure.</figcaption>
</figure>

<hr />
```

| `<a>`属性`href`       | 备注                   |
| --------------------- | ---------------------- |
| `https://example.com` | 绝对 URL               |
| `//example.com`       | Scheme-relative URL    |
| `/en-US/docs/`        | Origin-relative URL    |
| `./p`                 | Directory-relative URL |
| `#anchor`             | Page Fragment          |

| `<a>`属性`target` | 备注（`browsing context` 可以是 tab, window 或 iframe） |     |
| ----------------- | ------------------------------------------------------- | --- |
| `_self`           | the current browsing context. (Default)                 |
| `_blank`          | usually a new tab                                       |
| `_parent`         | the parent browsing context                             |
| `_top`            | the topmost browsing context                            |

| `<a>`属性`download` | 备注(仅适用于 same-origin URL)                                                     |
| ------------------- | ---------------------------------------------------------------------------------- |
|                     | 没有值，则浏览器根据`Content-Disposition`首部或最后一段 URL path 确定扩展名/文件名 |
| `<filename>`        | 手动指定建议文件名                                                                 |

## 表格

```html
<table>
  <caption>
    Table with thead, tfoot, and tbody
  </caption>
  <thead>
    <tr>
      <th>Header content 1</th>
      <th>Header content 2</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Body content 1</td>
      <td>Body content 2</td>
    </tr>
  </tbody>
  <tfoot>
    <tr>
      <td>Footer content 1</td>
      <td>Footer content 2</td>
    </tr>
  </tfoot>
</table>

<hr />

<table>
  <caption>
    Table with scoping raws and columns
  </caption>
  <tbody>
    <tr>
      <th scope="col">Name</th>
      <th scope="col">HEX</th>
      <th scope="col">HSLa</th>
      <th scope="col">RGBa</th>
    </tr>
    <tr>
      <th scope="row">Teal</th>
      <td><code>#51F6F6</code></td>
      <td><code>hsla(180, 90%, 64%, 1)</code></td>
      <td><code>rgba(81, 246, 246, 1)</code></td>
    </tr>
    <tr>
      <th scope="row">Goldenrod</th>
      <td><code>#F6BC57</code></td>
      <td><code>hsla(38, 90%, 65%, 1)</code></td>
      <td><code>rgba(246, 188, 87, 1)</code></td>
    </tr>
  </tbody>
</table>

<hr />

<table>
  <caption>
    Table with colgroup and col
  </caption>
  <colgroup>
    <col />
    <col span="2" style="background-color: #0f0" />
  </colgroup>
  <tr>
    <th>Lime</th>
    <th>Lemon</th>
    <th>Orange</th>
  </tr>
  <tr>
    <td>Green</td>
    <td>Yellow</td>
    <td>Orange</td>
  </tr>
</table>

<hr />

<table>
  <caption>
    Table with rowspan and colspan
  </caption>
  <tr>
    <td>cell 1,1</td>
    <td>cell 1,2</td>
    <td>cell 1,3</td>
  </tr>
  <tr>
    <td rowspan="2" style="background-color: #0f0">cell 2,1 3,1</td>
    <td>cell 2,2</td>
    <td>cell 2,3</td>
  </tr>
  <tr>
    <td colspan="2" style="background-color: #f00">cell 3,2 3,3</td>
  </tr>
</table>
```

## 多媒体

```html
<!-- 该元素仅用于确认位置信息，然后由 JavaScript 调用 API 来绘制图形和动画-->
<canvas id="canvas" width="100" height="100">
  An alternative text describing what your canvas displays.
</canvas>

<hr />

<!--
  width, height 设置 SVG 元素的大小，
  矢量图一般具有初始的大小，要裁减或缩放矢量图则可设置视窗(viewbox)的位置与大小，
  视窗偏离坐标系可达到裁减的效果，
  视窗缩小视窗可减小画布可见范围从而放大图像，放大视窗可扩大画布可见范围从而缩小图像
-->
<svg
  width="100"
  height="100"
  viewbox="0 0 10 10"
  xmlns="http://www.w3.org/2000/svg"
  stroke="red"
  fill="grey"
  style="border: 2px solid red"
>
  <circle cx="5" cy="5" r="4" />
</svg>

<hr />

<img
  src="https://developer.mozilla.org/en-US/docs/Web/SVG/Tutorial/Positions/canvas_default_grid.png"
  alt="alternative content"
/>

<hr />

<!--按条件加载图片，img为默认-->
<picture>
  <source media="(max-width: 799px)" srcset="elva-480w-close-portrait.jpg" />
  <source media="(min-width: 800px)" srcset="elva-800w.jpg" />
  <img src="elva-800w.jpg" alt="Chris standing up holding his daughter Elva" />
</picture>

<hr />

<!--按顺序加载视频，直到第一个成功加载的视频-->
<video controls autoplay loop muted preload="auto" poster="poster.png">
  <source src="rabbit320.mp4" type="video/mp4" />
  <source src="rabbit320.webm" type="video/webm" />
  <!--添加字幕-->
  <track kind="subtitles" src="subtitles_es.vtt" srclang="es" label="Spanish" />
  <p>
    Your browser doesn't support HTML5 video. Here is a
    <a href="rabbit320.mp4">link to the video</a> instead.
  </p>
</video>

<audio controls>
  <source src="viper.mp3" type="audio/mp3" />
  <source src="viper.ogg" type="audio/ogg" />
  <p>
    Your browser doesn't support HTML5 audio. Here is a
    <a href="viper.mp3">link to the audio</a> instead.
  </p>
</audio>

<hr />

<img
  src="https://www.runoob.com/try/demo_source/planets.gif"
  alt="Planets"
  usemap="#planetmap"
  width="145"
  height="126"
/>
<map name="planetmap">
  <area shape="rect" coords="0,0,82,126" href="https://www.runoob.com/images/sun.gif" alt="Sun" />
  <area
    shape="circle"
    coords="90,58,3"
    href="https://www.runoob.com/try/demo_source/merglobe.gif"
    alt="Mercury"
  />
  <area
    shape="circle"
    coords="124,58,8"
    href="https://www.runoob.com/images/venglobe.gif"
    alt="Venus"
  /> </map
><br />
```

## 表单

```html
<dialog open>对话框</dialog>

<form method="post" action="URL">
  <fieldset>
    <legend>Input</legend>
    <div>
      <label>
        Username:
        <input type="text" name="user" placeholder="enter your username" />
      </label>
    </div>
    <div>
      <label>
        Password:
        <input type="password" name="password" placeholder="enter yout password" />
      </label>
    </div>
    <div>
      <label>
        Email:
        <!--允许空格或逗号分隔列表给出多个email，且允许为空即使指定required-->
        <input type="email" name="email" placeholder="user@example.com" multiple />
      </label>
    </div>
    <div>
      <label>
        Url:
        <input type="url" name="url" placeholder="https://example.com" />
      </label>
    </div>
    <div>
      <label>
        Telephone:
        <input type="tel" name="tel" placeholder="10086" />
      </label>
    </div>
    <div>
      <label>
        Number:
        <input type="number" name="number" />
      </label>
    </div>
    <div>
      <label>
        Search:
        <input type="search" name="search" />
      </label>
    </div>
    <div>
      <label>
        Text:
        <input type="text" name="text" placeholder="enter text" list="datalist" />
        <datalist id="datalist">
          <option value="Internet Explorer"></option>
          <option value="Firefox"></option>
          <option value="Chrome"></option>
          <option value="Opera"></option>
          <option value="Safari"></option>
        </datalist>
      </label>
    </div>
    <div>
      <textarea name="textarea" rows="10" cols="50">This is predefined value in textarea</textarea>
    </div>
  </fieldset>
  <fieldset>
    <legend>Click</legend>
    <div>
      <label>
        Datetime-local:
        <input type="datetime-local" name="datetime" value="2006-01-02T15:04:05" />
      </label>
      <label>
        Date:
        <input type="date" name="date" value="2006-01-02" />
      </label>
      <label>
        Time:
        <input type="time" name="time" value="15:04" />
      </label>
      <label>
        Month:
        <input type="month" name="month" value="2006-01" />
      </label>
      <label>
        Week:
        <input type="week" name="week" value="2006-W01" />
      </label>
    </div>
    <div>
      Radio:
      <label>
        <input type="radio" name="gender" value="male" checked />
        Male
      </label>
      <label>
        <input type="radio" name="gender" value="famale" />
        Famale
      </label>
    </div>
    <div>
      Checkbox:
      <label>
        <input type="checkbox" name="assets" value="house" checked />
        House
      </label>
      <label>
        <input type="checkbox" name="assets" value="car" checked />
        Car
      </label>
      <label>
        <input type="checkbox" name="assets" value="girlfriend" />
        Girlfriend
      </label>
    </div>
    <div>
      <label>
        Select:
        <select name="browser">
          <optgroup label="PC">
            <option value="IE">IE</option>
            <option value="Chrome">Chrome</option>
            <option value="Firefox">Firefox</option>
          </optgroup>
          <optgroup label="Mobile">
            <option selected value="UC">UC</option>
          </optgroup>
        </select>
      </label>
    </div>
    <div>
      <input type="file" name="file" accept=".doc,.docx,application/msword" multiple />
    </div>
    <div>
      <label>
        Color:
        <input type="color" name="color" value="#efff2b" />
      </label>
    </div>
    <div>
      Range: 0
      <input
        type="range"
        id="range"
        name="range"
        value="50"
        min="0"
        max="100"
        step="1"
        oninput="out.value=parseInt(range.value)"
      />
      100
      <output for="range" name="out">50</output>
    </div>
  </fieldset>

  <div>
    Meter:
    <meter min="0" max="100" value="50" low="33" high="66" optimum="100">50</meter>
  </div>
  <div>
    Progress:
    <progress max="100" value="70">70%</progress>
  </div>

  <div>
    <button type="button" onclick="alert('丢雷搂母!')">Button</button>
    <button type="reset">Reset</button>
    <button type="submit">Submit</button>
  </div>
</form>
```

| input 属性     | 适用类型                    | 备注                                 |
| -------------- | --------------------------- | ------------------------------------ |
| `name`         | all                         | 名称                                 |
| `value`        | all                         | 值                                   |
| `autocomplete` | all                         | `on`开启（默认），`off`关闭          |
| `disabled`     | all                         | 不可输入，不随表单发送               |
| `readonly`     | almost all                  | 不可修改 value                       |
| `list`         | almost all                  | 绑定一个`<datalist>`获取建议的预设值 |
| `max`          | 数字类型                    | 设置最大数字                         |
| `min`          | 数字类型                    | 设置最小数字                         |
| `maxlength`    | 文本类型                    | 设置最长文本                         |
| `minlength`    | 文本类型                    | 设置最短文本                         |
| `placeholder`  | 文本类型                    | 占位提示文本                         |
| `size`         | email,password,tel,text,url | 设置输入框长度                       |
| `pattern`      | password,text,tel           | value 必须与 pattern 匹配            |
| `required`     | almost all                  | 用户必须指定一个值                   |

## 嵌套框架

```html
<iframe src="https://www.runoob.com/html/html-tutorial.html" width="800" height="500"></iframe>
```
