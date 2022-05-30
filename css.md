# CSS

## 选择器

| 基本选择器 | 语法                     | 权重 |
| ---------- | ------------------------ | ---- |
| Universal  | `*`                      | 000  |
| Element    | `elementname`            | 001  |
| Attribute  | `[attr]` `[attr@=value]` | 010  |
| Class      | `.classname`             | 010  |
| ID         | `#idname`                | 100  |

| 复合选择器       | 语法                    | 权重 |
| ---------------- | ----------------------- | ---- |
| Descendant       | `selector1 selector2`   | 0    |
| Child            | `selector1 > selector2` | 0    |
| General sibling  | `selector1 ~ selector2` | 0    |
| Adjacent sibling | `selector1 + selector2` | 0    |

| 伪选择器        | 示例            | 备注                   | 权重 |
| --------------- | --------------- | ---------------------- | ---- |
| Pseudo classes  | `button:hover`  | 指定选定元素的特定状态 | 010  |
| Pseudo elements | `p::first-line` | 指定选定元素的特定部分 | 001  |

## 层叠与继承

**层叠**：同一属性存在多个值时，先选择优先级高的样式来源，再选择优先级最高的值

- 样式来源：

  1. `!important` in user style sheets
  2. `!important` in author style sheets
  3. normal in user style sheets
  4. normal in author style sheets
  5. browser default style sheets

- 同源优先级：
  1. 特异性：根据选择器权重计算优先级，权重计算无进位
  2. 声明顺序：特异性优先级相同则靠后的声明优先

**继承**：当子元素的某些属性没有指定值，默认会继承父元素的某些属性，而哪些属性会被继承很大程度上取决于常识

| 控制继承属性值 | 备注                                       |
| -------------- | ------------------------------------------ |
| `inherit`      | 继承父元素的值                             |
| `initial`      | 使用最原始的标准默认值                     |
| `unset`        | 继承属性同`inherit`，非继承属性同`initial` |
| `revert`       | 使用浏览器默认 CSS 中的值                  |
| `revert-layer` | 使用上一层的值                             |

## 布局

### 盒子模型

```txt
┌─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─  ─  ─┐
  margin      ↱ border
│   ┌─────────────────────┐   │
    │ padding             │
│   │  ┌  ─ ─ ─ ─ ─ ─  ┐  │   │
    │       content       │
│   │  └  ─ ─ ─ ─ ─ ─  ┘  │   │
    │                     │
│   └─────────────────────┘   │

└─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─┘
```

- `block`
  - padding 和 margin 会推挤空间
  - 宽度：由 content 宽度决定，考虑`width:`。特殊的，根元素`<body>`的宽度固定我`100vw`
  - 高度：单行高度取子元素中 inline's content, block's border, inline-block's margin 高度中最大值，考虑`height:`。特殊的，根元素`<body>`的最小高度为`100vh`
- `inline`
  - 水平方向的 padding 和 margin 会推挤空间，而垂直方向不会
  - 宽度：由 content 宽度决定，忽略`width:`
  - 高度：由 content 高度决定，忽略`height:`
- `inline-block`：
  - padding 和 margin 会推挤空间
  - 宽度：由 content 宽度决定，考虑`width:`
  - 高度：由 content 高度决定，考虑`height:`

```css
/* box-sizing */
box-sizing: content-box; /* width/height 表示 content width/height（默认） */
box-sizing: border-box; /* width/height 表示 border width/height */

/* width, min-width, max-width, height, min-height, max-height */
width: auto; /* （默认）浏览器自动计算盒子模型的宽与高 */
width: <length>;
width: <percentage>;
width: max-content;
width: min-content;
width: fit-content(<length-percentage>);

/* padding */
padding: <length>;
padding: <percentage>;
/* vertical | horizontal */
padding: 5% auto;
/* top | horizontal | bottom */
padding: 1em auto 2em;
/* top | right | bottom | left */
padding: 2px 1em 0 auto;

/* margin: 注意外边距折叠 */
margin: <length>;
margin: <percentage>;
margin: auto; /* 浏览器自动设置值来使其居中 */
/* vertical | horizontal */
margin: 5% auto;
/* top | horizontal | bottom */
margin: 1em auto 2em;
/* top | right | bottom | left */
margin: 2px 1em 0 auto;
```

### 正常流布局

- `display: block;`
  - 另起一行，自动设置`margin`以单独一行
- `display: inline;`
  - 不会另起一行，与其他 ineline/inline-block 元素排在同一行，若空间不足则溢出到下行
- `display: inline-block;`：
  - 不会另起一行，与其他 ineline/inline-block 元素排在同一行，若空间不足则溢出到下行

### 弹性布局

![flex](https://developer.mozilla.org/en-US/docs/Learn/CSS/CSS_layout/Flexbox/flex_terms.png)

Flex 布局是**一维布局**，它可以随空间大小自由伸缩；默认条件下，

- flex items 主轴方向尺寸为`auto`（即由浏览器计算 content 尺寸）
- flex items 交叉轴方向尺寸为 flex container 在该方向上的 100%尺寸

| flex container 属性        | 值                                                                                            | 备注                                 |
| -------------------------- | --------------------------------------------------------------------------------------------- | ------------------------------------ |
| `display: flex;`           | `flex`                                                                                        | 指定元素内部展示为弹性布局           |
| `flex-flow: row nowrap;`   |                                                                                               | 组合`flex-direction`与`flex-wrap`    |
| `flex-direction: row;`     | `row`,`row-reverse`,`column`,`column-reverse`                                                 | 指定内部弹性布局主轴方向             |
| `flex-wrap: nowrap;`       | `wrap`,`nowrap`,`wrap-reverse`                                                                | 控制当主轴溢出时是否回绕到下行       |
| `justify-content: normal;` | `flex-start`,`flex-end`,`left`,`right`,`center`,`space-between`,`space-around`,`space-evenly` | 控制内部 item 在主轴方向上如何排列   |
| `align-items: normal;`     | `flex-start`,`flex-end`,`center`                                                              | 控制内部 item 在交叉轴方向上如何排列 |

| flex item 属性          | 值                                                      | 备注                                                               |
| ----------------------- | ------------------------------------------------------- | ------------------------------------------------------------------ |
| `justify-self: normal;` | 同`justify-content`                                     | 控制该 item 在主轴方向上如何排列                                   |
| `align-self: normal;`   | 同`align-items`                                         | 控制该 item 在交叉轴方向上如何排列                                 |
| `flex`                  |                                                         | 组合`flex-grow`、`flex-shrink`和`flex-basis`                       |
| `flex-grow: 0;`         | `<number>`                                              | 指定 item 在主轴方向上的生长因子，用于分配剩余尺寸                 |
| `flex-shrink: 1;`       | `<number>`                                              | 指定 item 在主轴方向的缩小因子，用于在主轴溢出时分配需要缩减的尺寸 |
| `flex-basis: auto;`     | `<length>`,`auto`,`content`,`max-content`,`min-content` | 指定 item 在主轴方向上的尺寸，优先级高于`width:`/`height:`         |
| `order: 0;`             | `<number>`                                              | 设置展示顺序，序号越大越靠后，同序号按元素顺序                     |

### 网格布局

![grid](https://developer.mozilla.org/en-US/docs/Learn/CSS/CSS_layout/Grids/grid.png)

Grid 布局将页面划分为一个个格子，每个格子间通常存在间隔(Gutters)，默认情况下只有一个格子。

| grid container 属性 | 值     | 备注                       |
| ------------------- | ------ | -------------------------- |
| `display: grid`     | `grid` | 指定元素内部展示为网格布局 |

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

## 数值与单位

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
| lh   | 元素的 line-height                                                                            |
| vw   | 视窗宽度的 1%                                                                                 |
| vh   | 视窗高度的 1%                                                                                 |
| vmin | 视窗较小尺寸的 1%                                                                             |
| vmax | 视图大尺寸的 1%                                                                               |

**百分比**：
在许多情况下，百分比与长度的处理方法是一样的。
百分比的问题在于，它们总是相对于其他值设置的。
例如，如果将元素的字体大小设置为百分比，那么它将是元素父元素字体大小的百分比。
如果使用百分比作为宽度值，那么它将是父值宽度的百分比。

## 定位

- `position: static`静态：默认位置
- `position: relative`相对：相对默认位置进行调整
- `position: absolute`绝对：将元素抽出正常布局流，类似新图层（`z-index: 1;`决定上层显示）
- `position: fixed`固定：相对浏览器视窗固定
- `position: sticky`粘性：当出现在浏览器视窗后有静态定位变为固定定位

## 溢出处理

**什么时候会溢出**：父元素设置了固定大小，而子元素过于庞大

The `overflow` property has the following values:

- visible - Default. The overflow is not clipped. The content renders outside the element's box
- hidden - The overflow is clipped, and the rest of the content will be invisible
- scroll - The overflow is clipped, and a scrollbar is added to see the rest of the content
- auto - Similar to scroll, but it adds scrollbars only when necessary

- 图像溢出
  - `height: 100%; width: 100%;`
  - `object-fit: contain`: 被替换的内容将被缩放，以在填充元素的内容框时保持其宽高比。 整个对象在填充盒子的同时保留其长宽比，因此如果宽高比与框的宽高比不匹配，该对象将被添加“黑边”。
  - `object-fit: cover`: 被替换的内容在保持其宽高比的同时填充元素的整个内容框。如果对象的宽高比与内容框不相匹配，该对象将被剪裁以适应内容框。
  - `object-fit: fill`: 被替换的内容正好填充元素的内容框。整个对象将完全填充此框。如果对象的宽高比与内容框不相匹配，那么该对象将被拉伸以适应内容框。
  - `object-fit: none`: 被替换的内容将保持其原有的尺寸。
  - `object-fit: scale-down`: 内容的尺寸与 none 或 contain 中的一个相同，取决于它们两个之间谁得到的对象尺寸会更小一些。
