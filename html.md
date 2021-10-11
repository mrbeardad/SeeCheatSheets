```html
<!DOCTYPE html>
<html lang="en-US">

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
    <meta name="description" content="Description of the page less than 150 characters">

    <link rel="dns-prefetch" href="//example.com/">
    <link rel="preconnect" href="https://www.example.com/">
    <link rel="prefetch" href="https://www.example.com/">
    <link rel="subresource" href="styles/styles.css">
    <link rel="prerender" href="https://example.com/">
    <link rel="preload" href="images/image.png" as="image">

    <meta property="og:type" content="website">
    <meta property="og:url" content="https://example.com/page.html">
    <meta property="og:title" content="Content Title">
    <meta property="og:image" content="https://example.com/image.jpg">
    <meta property="og:description" content="Description Here">
    <meta property="og:site_name" content="Site Name">
    <meta property="og:locale" content="en_US">
    <meta property="og:image:width" content="1200">
    <meta property="og:image:height" content="630">

    <link rel="alternate" href="https://zh.example.com/" hreflang="zh">
    <link rel="alternate" href="https://example.com/" hreflang="x-default">

    <link rel="stylesheet" type="text/css" href="styles/style.css">
    <style>
        /* */
    </style>

    <script type="text/javascript" src="scripts/main.js"></script>
    <script>
        //
    </script>

    <title>页面标题</title>
    <link rel="icon" type="image/png" href="https://example.com/favicon-32x32.png">
</head>

<body>
    <noscript>不支持JS时显示的文本</noscript>

    <!-- 网页内容分区结构 -->

    <header>
        <h1>网站一般信息：标题或标志</h1>
    </header>

    <nav>
        <ul>
            <li>导航栏链接一</li>
            <li>导航栏链接二</li>
        </ul>
    </nav>

    <main>
        <!-- article与section可根据上下文相互嵌套 -->
        <article>
            <section>
                <h2>文章标题</h2>
                <p>文章内容</p>
            </section>
        </article>

        <aside>
            <p>侧边栏或者标注框</p>
        </aside>
    </main>

    <footer>
        <p>&copy;2021-2022 版权信息</p>
        <address>
            联系方式：<br>
            <a href="mailto:mrbeardad@qq.com">mrbeardad@qq.com</a><br>
            <a href="tel:+13115552368">(311) 555-2368</a><br>
            Chengdu, Sichuan, China
        </address>
    </footer>

    <!-- 内联语义元素 -->

    普通文本
    <strong>重要</strong><br>
    <em>强调</em><br>
    <del>删除</del>
    <ins>取代</ins><br>
    <kbd>按键</kbd><br>
    <mark>标记</mark><br>
    <q>引用</q><br>
    <cite>作品名</cite><br>
    <code>代码</code><br>
    <var>变量</var><br>
    <samp>程序输出</samp><br>
    正常<sup>上标</sup><sub>下标</sub><br>
    <data value="001">枚举名</data><br>
    <time datetime="2016-01-20T19:30+01:00">20 January 2016, 8.30pm in France</time><br>
    <abbr title="完整词汇">缩写词汇</abbr><br>

    <a href="URL" target="self">链接</a><br>
    <a href="#id" target="_blank">链接</a><br>

    <label>
        进度：
        <progress id="progress" value="22" max="100"></progress><br>
    </label>

    <label>
        计量：
        <meter id="disk_c" value="2" min="0" max="10">2 out of 10</meter><br>
    </label>


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

    <blockquote>
        blockquote
    </blockquote>

    <pre>
        predefined text that
        remain all blank character
        and use monospace font
    </pre>

    <figure>
        主文中引用的图片，插图，表格，代码段等等
        <figcaption>Fig.1 - The Caption of the Figure.</figcaption>
    </figure>

    <!-- 无语义元素，用于布局与样式 -->

    <div>
        <span id="link">
            <b>粗体</b>
            <i>斜体</i>
            <u>下划线</u>
            <small>缩小</small>
        </span>
    </div>

    <!-- 表格 -->

    <table>
        <caption>Table test</caption>
        <colgroup>
            <col span="1">
            <col span="2">
        </colgroup>
        <thead>
            <tr>
                <th scope="col">header1</th>
                <th scope="col">header2</th>
                <th scope="col">header3</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <th scope="row" rowspan="2">row 1/3, col 1</th>
                <td>row 1, col 2</td>
                <td>row 1, col 3</td>
            </tr>
            <tr>
                <td colspan="2">row 2, col 2/4</td>
            </tr>
        </tbody>
        <tfoot>
            <tr>
                <td>footer1</td>
                <td>footer2</td>
                <td>footer3</td>
            </tr>
        </tfoot>
    </table>

    <!-- 图片、视频、音频 -->

    <canvas id="canvas" width="300" height="300">
        您的浏览器不支持&lt;canvas&gt;元素的浏览器或是禁用了JavaScript
    </canvas>

    <img src="images/image.png" alt="alternative content"><br>

    <img srcset="elva-fairy-320w.jpg 320w,
        elva-fairy-480w.jpg 480w,
        elva-fairy-800w.jpg 800w" sizes="(max-width: 320px) 280px,
        (max-width: 480px) 440px,
        800px" src="elva-fairy-800w.jpg" alt="Elva dressed as a fairy">

    <img src="https://www.runoob.com/try/demo_source/planets.gif" alt="Planets" usemap="#planetmap" width="145"
        height="126">
    <map name="planetmap">
        <area shape="rect" coords="0,0,82,126" href="https://www.runoob.com/images/sun.gif" alt="Sun">
        <area shape="circle" coords="90,58,3" href="https://www.runoob.com/try/demo_source/merglobe.gif" alt="Mercury">
        <area shape="circle" coords="124,58,8" href="https://www.runoob.com/images/venglobe.gif" alt="Venus">
    </map><br>

    <video autoplay loop muted playsinline>
        <source src="instead-of-GIF.webm" type="video/webm">
        <source src="instead-of-GIF.mp4" type="video/mp4">
    </video>

    <video controls poster="images/poster.png">
        <source src="example.mp4" type="video/mp4">
        <source src="example.webm" type="video/webm">
    </video>

    <audio controls>
        <source src="viper.mp3" type="audio/mp3">
        <source src="viper.ogg" type="audio/ogg">
    </audio>

    <!-- 表单-->

    <form method="GET" action="URL">
        <!-- type name id value placeholder autocomplete list readonly disable required pattern size minlength maxlength min max step -->
        <fieldset>
            <legend>User Information</legend>
            <div>
                <label>
                    Username:
                    <input type="text" name="user" id="user" placeholder="enter your name">
                </label>
            </div>
            <div>
                <label>
                    Password:
                    <input type="password" name="password" id="pswd" placeholder="enter yout password">
                </label>
            </div>
            <div>
                <label>
                    Email:
                    <input type="email" name="email" id="email" placeholder="user@example.com" multiple>
                </label>
            </div>
            <div>
                <label>
                    HomePage:
                    <input type="url" name="url" id="url" placeholder="https://example.com">
                </label>
            </div>
            <div>
                <label>
                    Telephone:
                    <input type="tel" name="tel" id="tel" placeholder="00123456789">
                </label>
            </div>
            <div>
                <label>
                    Birthday:
                    <input name="datetime" id="datetime">
                </label>
                <label>
                    Date:
                    <input type="date" name="date" id="date">
                </label>
                <label>
                    Time:
                    <input type="time" name="time" id="time">
                </label>
                <label>
                    Month:
                    <input type="month" name="month" id="month">
                </label>
                <label>
                    Week:
                    <input type="week" name="week" id="week">
                </label>
            </div>
            <div>
                <label>
                    Age:
                    <input type="number" name="age" id="age">
                </label>
            </div>
            <div>
                <label>
                    Favorite Color:
                    <input type="color" name="color" id="color">
                </label>
            </div>
            <div>
                Range:
                0 <input type="range" id="range" name="range" value="50" min="0" max="100" step="1"
                    oninput="out.value=parseInt(range.value)"> 100
                <output for="range" name="out">50</output>
            </div>
            <div>
                Gender:
                <label>
                    <input type="radio" name="gender" id="gender_male" value="male" checked>
                    Male
                </label>
                <label>
                    <input type="radio" name="gender" id="gender_famale" value="famale">
                    Famale
                </label>
            </div>
            <div>
                Assets:
                <label>
                    <input type="checkbox" name="assets" id="assets_car" value="car" checked>
                    Car
                </label>
                <label>
                    <input type="checkbox" name="assets" id="assets_house" value="house">
                    House
                </label>
            </div>
            <div>
                <label>
                    Browser:
                    <input type="text" name="browser" id="browser" placeholder="Browser" list="browsers">
                    <datalist id="browsers">
                        <option value="Internet Explorer"></option>
                        <option value="Firefox"></option>
                        <option value="Chrome"></option>
                        <option value="Opera"></option>
                        <option value="Safari"></option>
                    </datalist>
                </label>
                <label>
                    Browser:
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
                <input type="file" name="file" id="file" accept=".pdf" multiple>
            </div>
            <div>
                <textarea id="textarea" name="textarea" cols="50" rows="10" placeholder="enter text"></textarea>
            </div>
        </fieldset>

        <div>
            <label>
                Search:
                <input type="search" name="search" id="search">
            </label>
        </div>

        <div>
            Meter:
            <meter min="0" max="100" value="50" low="33" high="66" optimum="100">50</meter>
            Progress:
            <progress max="100" value="70">70%</progress>
        </div>

        <div>
            <button type="button" onclick="alert('丢雷搂母!')">Button</button>
            <button type="reset">Reset</button>
            <button type="submit">Submit</button>
            <input type="image" name="iamge" id="image" src="URL" alt="alternate">
        </div>
    </form>

    <!-- 交互元素 -->

    <details>
        <summary>简介</summary>
        <p>折叠隐藏的内容</p>
    </details>

    <!-- 嵌入子页面框架 -->

    <iframe src="template.html" width="800" height="500" sandbox="allow-forms"></iframe>

</body>

</html>
```