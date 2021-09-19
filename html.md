```html
<!DOCTYPE html>
<html lang="en-US">

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
    <meta name="description" content="Description of the page less than 150 characters">
    <title>Page Title less than 55 characters</title>
    <link rel="icon" type="image/png" href="https://example.com/favicon-32x32.png">

    <link rel="dns-prefetch" href="//example.com/">
    <link rel="preconnect" href="https://www.example.com/">
    <link rel="prefetch" href="https://www.example.com/">
    <link rel="subresource" href="styles/styles.css">
    <link rel="prerender" href="https://example.com/">
    <link rel="preload" href="image.png" as="image">

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
    <style></style>

    <script type="text/javascript" src="scripts/main.js"></script>
    <script></script>
    <noscript>No JS alternative text</noscript>
</head>

<body>
    <!-- 网页架构 -->
    <header> </header>

    <nav> </nav>

    <main>
        <article>
            <section>
            </section>
        </article>

        <aside>
        </aside>
    </main>

    <footer> </footer>

    <!-- 标题 -->

    <h1>Header1</h1>
    <h2>Header2</h2>
    <h3>Header3</h3>
    <h4>Header4</h4>
    <h5>Header5</h5>
    <h6>Header6</h6>

    <!-- 块级或行级语义元素 -->

    <p>
        simple paragraph<br>
        <b>bold</b> and <strong>strong</strong><br>
        <i>italic</i> and <em>emphasize</em><br>
        <del>delete</del> <ins>instead</ins><br>
        L<sup><small>A</small></sup>T<sub><small>E</small></sub>X<br>
        <abbr title="abbreviations">abbr</abbr><br>
        <kbd>keyboard</kbd><br>
        <code>code</code><br>
        <mark>mark</mark><br>
        <q>quote</q><br>
        <var>variable</var><br>
        <samp>sample</samp><br>
        <cite>title of work</cite><br>
        <time datetime="2021-9-10">17:36</time><br>
        <label for="progress">Progress:</label>
        <progress id="progress" value="22" max="100"></progress><br>
        <label for="meter">Meter:</label>
        <meter id="disk_c" value="2" min="0" max="10">2 out of 10</meter><br>
        <address>
            Chengdu, Sichuan, China
        </address>
        <blockquote>
            blockquote
        </blockquote>
        <pre>
            predefined text that
            remain blank character
        </pre>
    </p>

    <!-- 列表 -->

    <ul>
        <li>unordered list item 1</li>
        <li>unordered list item 2</li>
    </ul>
    <ol>
        <li>ordered list item 1</li>
        <li>ordered list item 2</li>
    </ol>

    <!-- 表格 -->

    <table>
        <caption>Table test</caption>
        <colgroup>
            <col span="1" style="background-color: blue;">
            <col span="2" style="background-color: red;">
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
                <th scope="row" rowspan="2">row 1/2, col 1</td>
                <td>row 1, col 2</td>
                <td>row 1, col 3</td>
            </tr>
            <tr>
                <td colspan="2">row 2, col 2/3</td>
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

    <!-- 链接、图片、视频、音频 -->

    <div><span id="link"></span></div>
    <a href="#link" target="_blank">Link to relative URL</a><br>

    <a href="https://github.com/mrbeardad/SeeCheatSheets/blob/master/html.md#link" target="self">Link to absolute URL</a><br>

    <img src="images/image.png" alt="alternative content"><br>

    <img srcset="elva-fairy-320w.jpg 320w, elva-fairy-480w.jpg 480w, elva-fairy-800w.jpg 800w"
        sizes="(max-width: 320px) 280px, (max-width: 480px) 440px, 800px"
        src="elva-fairy-800w.jpg" alt="Elva dressed as a fairy">

    <figure>
        <img src="images/image.png" alt="alternative content">
        <figcaption>Fig.1 - The Caption of the Figure.</figcaption>
    </figure>

    <img src="https://www.runoob.com/try/demo_source/planets.gif" alt="Planets"  usemap="#planetmap" width="145" height="126">
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

    <!-- 嵌入子页面框架 -->

    <iframe src="template.html" width="800" height="500" sandbox="allow-forms"></iframe>
</body>

</html>
```