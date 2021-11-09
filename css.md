# 选择器

| 选择器         | 示例                | 学习CSS的教程  |
| -------------- | ------------------- | -------------- |
| 通配选择器     | `* {  }`            | 通配选择器     |
| 类型选择器     | `h1 {  }`           | 类型选择器     |
| 类选择器       | `.box {  }`         | 类选择器       |
| ID选择器       | `#unique { }`       | ID选择器       |
| 标签属性选择器 | `a[title] {  }`     | 标签属性选择器 |
| 伪类选择器     | `p:first-child { }` | 伪类           |
| 伪元素选择器   | `p::first-line { }` | 伪元素         |
| 后代选择器     | `article p`         | 后代运算符     |
| 子代选择器     | `article > p`       | 子代选择器     |
| 相邻兄弟选择器 | `h1 + p`            | 相邻兄弟       |
| 通用兄弟选择器 | `h1 ~ p`            | 通用兄弟       |

# 层叠与继承
* 层叠：优先者覆盖
    * `!important`
    * 选择器优先级
    * 资源顺序

* 继承：子元素继承父元素的某些属性
    * `all: inherit`
    * `all: revert`

# 数值与单位
| 单位 | 名称         | 等价换算            |
| ---- | ------------ | ------------------- |
| cm   | 厘米         | 1cm = 96px/2.54     |
| mm   | 毫米         | 1mm = 1/10th of 1cm |
| Q    | 四分之一毫米 | 1Q = 1/40th of 1cm  |
| in   | 英寸         | 1in = 2.54cm = 96px |
| pc   | 十二点活字   | 1pc = 1/16th of 1in |
| pt   | 点           | 1pt = 1/72th of 1in |
| px   | 像素         | 1px = 1/96th of 1in |

| 单位 | 相对于                                                                                        |
| ---- | --------------------------------------------------------------------------------------------- |
| em   | 在 font-size 中使用是相对于父元素的字体大小，在其他属性中使用是相对于自身的字体大小，如 width |
| rem  | 根元素的字体大小                                                                              |
| ex   | 字符“x”的高度                                                                                 |
| ch   | 数字“0”的宽度                                                                                 |
| lh   | 元素的line-height                                                                             |
| vw   | 视窗宽度的1%                                                                                  |
| vh   | 视窗高度的1%                                                                                  |
| vmin | 视窗较小尺寸的1%                                                                              |
| vmax | 视图大尺寸的1%                                                                                |

**百分比**：
在许多情况下，百分比与长度的处理方法是一样的。
百分比的问题在于，它们总是相对于其他值设置的。
例如，如果将元素的字体大小设置为百分比，那么它将是元素父元素字体大小的百分比。
如果使用百分比作为宽度值，那么它将是父值宽度的百分比。

# 布局
## 基础
* 盒模型
    * margin: 外边距折叠，一般用于调整布局展示
    * border: 替代模式设置长宽时的边界，一般用于显示边界框
    * padding: 内边距，一般用于调整内容对齐
    * content: 标准模式设置长宽时的边界，展示的主体内容

* 正常布局流：浏览器默认的盒模型堆积模式
    > 高度的产生主要因为`<br>`与`wrap`
    * `display: inline`
      * 高度：同content高度，无法手动指定
      * 宽度：同content宽度，无法手动指定
    * `display: inline-block`
      * 高度：同content高度，可手动指定
      * 宽度：同content宽度，可手动指定
    * `display: block`
      * 高度：inline子元素content高度、block子元素border高度、inline-block子元素margin高度中最高者
      * 宽度：父元素宽度100%

* 排列方向：block inline
    * `writing-mode: horizontal-tb`
      * Top-to-bottom block flow direction. Sentences run horizontally.
    * `writing-mode: vertical-rl`
      * Right-to-left block flow direction. Sentences run vertically.
    * `writing-mode: vertical-lr`
      * Left-to-right block flow direction. Sentences run vertically.
    * `inline-size`&`block-size`

## 进阶
* Float布局
    * `float: left`: 现在仅用于插入环绕型图片

![flex](https://developer.mozilla.org/en-US/docs/Learn/CSS/CSS_layout/Flexbox/flex_terms.png)
* Flex布局
    * 默认所有item高度被拉伸为其中最高者同等高度
```css
.container {
    display: flex; /* 指定该元素为flex容器，其子元素为flex项，弹性紧凑均布地按flex方向排列（默认方向row） */
    flex-flow: column wrap; /* 设置flex容器主轴方向，并设置允许回绕下行避免溢出 */
    align-items: center; /* 控制flex项在交叉轴上的位置 */
    justify-content: space-around; /* 控制flex项在主轴上的位置 */
}
.item {
    flex: 1 200px; /* 设置flex项的最小主轴方向长度为200px，并且剩余宽度按比例分配（交叉轴方向的宽度——即高度，与最长的flex项一致） */
}
```

* Grid布局
```css
.container {
    display: grid;
    grid-template-columns: repeat(3, 1fr 2fr);
    grid-auto-rows: minmax(100px, auto);
    grid-gap: 20px;
}
.item {
  grid-column: 1 / 3;
  grid-row: 1;
}
```

* 响应式布局
```css
@media (min-width: 1200px) {
    h1 {
        font-size: 4rem;
    }
}

h1 {
    font-size: calc(1.5rem + 3vw);
}
```

# 定位
* `position: static`静态：默认位置
* `position: relative`相对：相对默认位置进行调整
* `position: absolute`绝对：将元素抽出正常布局流，类似新图层（`z-index: 1;`决定上层显示）
* `position: fixed`固定：相对浏览器视窗固定
* `position: sticky`粘性：当出现在浏览器视窗后有静态定位变为固定定位

# 溢出处理
**什么时候会溢出**：父元素设置了固定大小，而子元素过于庞大

The `overflow` property has the following values:
* visible - Default. The overflow is not clipped. The content renders outside the element's box
* hidden - The overflow is clipped, and the rest of the content will be invisible
* scroll - The overflow is clipped, and a scrollbar is added to see the rest of the content
* auto - Similar to scroll, but it adds scrollbars only when necessary

* 图像溢出
    * `height: 100%; width: 100%;`
    * `object-fit: contain`: 被替换的内容将被缩放，以在填充元素的内容框时保持其宽高比。 整个对象在填充盒子的同时保留其长宽比，因此如果宽高比与框的宽高比不匹配，该对象将被添加“黑边”。
    * `object-fit: cover`: 被替换的内容在保持其宽高比的同时填充元素的整个内容框。如果对象的宽高比与内容框不相匹配，该对象将被剪裁以适应内容框。
    * `object-fit: fill`: 被替换的内容正好填充元素的内容框。整个对象将完全填充此框。如果对象的宽高比与内容框不相匹配，那么该对象将被拉伸以适应内容框。
    * `object-fit: none`: 被替换的内容将保持其原有的尺寸。
    * `object-fit: scale-down`: 内容的尺寸与 none 或 contain 中的一个相同，取决于它们两个之间谁得到的对象尺寸会更小一些。
