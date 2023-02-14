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

| 伪选择器        | 示例           | 备注                   | 权重 |
| --------------- | -------------- | ---------------------- | ---- |
| Pseudo classes  | `:hover`       | 指定选定元素的特定状态 | 010  |
| Pseudo elements | `::first-line` | 指定选定元素的特定部分 | 001  |

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

**继承**：当子元素的某些属性没有指定值，默认会继承父元素的属性值，而哪些属性会被继承很大程度上取决于常识

| 控制继承属性值 | 备注                                       |
| -------------- | ------------------------------------------ |
| `inherit`      | 继承父元素的值                             |
| `initial`      | 使用最原始的标准默认值                     |
| `unset`        | 继承属性同`inherit`，非继承属性同`initial` |
| `revert`       | 使用浏览器默认 CSS 中的值                  |
| `revert-layer` | 使用上一层的值                             |

## 长度单位

- 物理像素：屏幕上的一个栅格，物理像素的实际大小与屏幕分辨率和尺寸有关
- 逻辑像素：逻辑像素的实际大小会随着缩放程度变化，默认情况为适配不同 DPI 屏幕，一个逻辑像素大小为 1/96 英寸

| 绝对单位 | 名称         | 等价换算            |
| -------- | ------------ | ------------------- |
| cm       | 厘米         | 1cm = 96px/2.54     |
| mm       | 毫米         | 1mm = 1/10th of 1cm |
| Q        | 四分之一毫米 | 1Q = 1/40th of 1cm  |
| in       | 英寸         | 1in = 2.54cm = 96px |
| pc       | 十二点活字   | 1pc = 1/16th of 1in |
| pt       | 点           | 1pt = 1/72th of 1in |
| px       | CSS pixel    | 1px = 1/96th of 1in |

| 相对单位 | 相对于                                                                          |
| -------- | ------------------------------------------------------------------------------- |
| em       | 对于 font-size 等排版属性是相对父元素字体大小，对于其他属性是相对本元素字体大小 |
| rem      | 相对根元素的字体大小                                                            |
| vw       | 视窗宽度的 1%                                                                   |
| vh       | 视窗高度的 1%                                                                   |
| vmin     | 视窗较小边的 1%                                                                 |
| vmax     | 视图较大边的 1%                                                                 |

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

> - 宽度与高度均指逻辑方向上的内容宽度与内容高度
> - 盒子模型的基础大小等于子元素盒子堆积排列后的挤占总空间的大小
> - 盒子模型的最终大小依布局方式进行改变

- `block`
  - padding 和 margin 会挤占空间
  - 宽度考虑`width:`
  - 高度考虑`height:`
- `inline-block`：
  - padding 和 margin 会挤占空间
  - 宽度考虑`width:`
  - 高度考虑`height:`
- `inline`
  - 水平方向的 padding 和 margin 会挤占空间，而垂直方向不会
  - 宽度忽略`width:`
  - 高度忽略`height:`
- **特殊的**：
  - 初始元素`<html>`的宽度固定为`100vw`，高度固定为`100vh`
  - 根元素`<body>`的宽度默认为`100vw`，最小高度默认为`100vh`

```css
/* box-sizing */
box-sizing: content-box; /* width/height 表示 content width/height（默认） */
box-sizing: border-box; /* width/height 表示 border width/height */

/* 宽度：width, min-width, max-width, inline-size */
width: auto; /* 各布局的默认宽度 */
width: <length>;
width: <percentage>;
width: max-content; /* 子元素全部在同一行中堆积总宽度 */
width: min-content; /* 子元素中最大宽度 */
width: fit-content(<length-or-percentage>); /* clamp(arg, min-content, max-content) */

/* 高度： height, min-height, max-height, block-size */
height: auto; /* 各布局默认高度 */
height: <length>;
height: <percentage>;

/* 内边距：padding */
padding: <length>;
padding: <percentage>;
/* vertical | horizontal */
padding: 5% 10%;
/* top | horizontal | bottom */
padding: 1em 2em 2em;
/* top | right | bottom | left */
padding: 5px 1em 0 2em;

/* 边界：border */
/* style */
border: solid;
/* width | style */
border: 2px dotted;
/* style | color */
border: outset #f33;
/* width | style | color */
border: medium dashed green;

/* 外边距：margin */
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

Normal Flow 布局是一种垂直排列布局，时默认的布局方式。

| 属性                        | 值                                          | 备注                                        |
| --------------------------- | ------------------------------------------- | ------------------------------------------- |
| `display: block`            | `block`                                     | 在开头和结尾换行，默认宽度为父元素宽度 100% |
| `display: inline`           | `inline`,`inline-block`                     | 与其他元素进行堆积排列                      |
| `write-mode: horizontal-tb` | `horizontal-tb`,`vertical-rl`,`vertical-lr` | 堆积排列方向                                |

### Flex 布局

![flex](images/flex_terms.png)

Flex 布局是一维布局，它的目标注重于灵活地伸缩空间与对齐内容。

- flex items 默认主轴方向尺寸即盒子模型基础宽度，并考虑伸缩因子来分配扩展剩余空间和分配缩小溢出空间
- flex items 默认交叉轴方向尺寸为父元素高度 100%

| flex container 属性        | 值                                                                             | 备注                                           |
| -------------------------- | ------------------------------------------------------------------------------ | ---------------------------------------------- |
| `display: flex;`           | `flex`                                                                         | 指定元素内部展示为弹性布局，外部展示为`block`  |
| `display: inline-flex;`    | `inline-flex`                                                                  | 指定元素内部展示为弹性布局，外部展示为`inline` |
| `flex-flow: row nowrap;`   |                                                                                | 组合`flex-direction`与`flex-wrap`              |
| `flex-direction: row;`     | `row`,`row-reverse`,`column`,`column-reverse`                                  | 指定内部弹性布局主轴方向                       |
| `flex-wrap: nowrap;`       | `wrap`,`nowrap`,`wrap-reverse`                                                 | 控制当主轴溢出时是否回绕到下行                 |
| `justify-content: normal;` | `flex-start`,`flex-end`,`center`,`space-between`,`space-around`,`space-evenly` | 控制内部 item 在主轴方向上如何排列             |
| `align-items: normal;`     | `flex-start`,`flex-end`,`center`                                               | 控制内部 item 在交叉轴方向上如何排列           |

| flex item 属性          | 值                  | 备注                                                               |
| ----------------------- | ------------------- | ------------------------------------------------------------------ |
| `justify-self: normal;` | 同`justify-content` | 控制该 item 在主轴方向上如何排列                                   |
| `align-self: normal;`   | 同`align-items`     | 控制该 item 在交叉轴方向上如何排列                                 |
| `flex`                  |                     | 组合`flex-grow`、`flex-shrink`和`flex-basis`                       |
| `flex-grow: 0;`         | `<number>`          | 指定 item 在主轴方向上的伸展因子，用于分配剩余尺寸                 |
| `flex-shrink: 1;`       | `<number>`          | 指定 item 在主轴方向的缩小因子，用于在主轴溢出时分配需要缩减的尺寸 |
| `flex-basis: auto;`     | 类似于`width`       | 指定 item 在主轴方向上的尺寸，优先级高于`width`/`height`           |
| `order: 0;`             | `<number>`          | 设置展示顺序，序号越大越靠后，同序号按元素顺序                     |

### Grid 布局

![grid](images/grid.png)

Grid 是二维布局，它的目标注重于整体规划区域并定位元素。

- 列轨道的宽度默认取 grid items 中盒子模型宽度最大者，默认只有一条列轨道
- 行轨道的高度默认取 grid items 中盒子模型高度最大者，默认隐式创建行轨道

| grid container 属性     | 值                        | 备注                                           |
| ----------------------- | ------------------------- | ---------------------------------------------- |
| `display: grid`         | `grid`                    | 指定元素内部展示为网格布局，外部展示为`block`  |
| `display: inline-grid`  | `inline-grid`             | 指定元素内部展示为网格布局，外部展示为`inline` |
| `grid-template-columns` | 见轨道尺寸值              | 指定列轨道模板                                 |
| `grid-template-rows`    | 见轨道尺寸值              | 指定行轨道模板                                 |
| `grid-auto-rows`        | 见轨道尺寸值              | 指定隐式创建的行轨道的高度                     |
| `grid-auto-columns`     | 见轨道尺寸值              | 指定隐式创建的列轨道的宽度                     |
| `grid-template-area`    | `"h h" "n m" "n f"`       | 为网格划分区域并指定名称                       |
| `gap`                   |                           | 组合`row-gap`与`col-gap`                       |
| `row-gap`               | `<length>`,`<percentage>` | 行轨道间距                                     |
| `col-gap`               | `<length>`,`<percentage>` | 列轨道间距                                     |

| 轨道尺寸值                                           | 备注                                             |
| ---------------------------------------------------- | ------------------------------------------------ |
| `<length>`                                           | 长度                                             |
| `<percentage>`                                       | 父元素百分比                                     |
| `<flex>`                                             | 单位`fr`表示伸缩因子                             |
| `none`                                               | 所有 grid item 尺寸由`grid-auto-columns`属性决定 |
| `max-content`                                        | 取该列所有 items 中最大的 max-content 值         |
| `min-content`                                        | 取该列所有 items 中最大的 min-content 值         |
| `minmax(min, max)`                                   | 定义尺寸范围                                     |
| `auto`                                               | 类似`minmax(min-content, max-content)`           |
| `fit-content(<length-or-percentage>)`                | 等同`clamp(arg, auto, max-content)`              |
| `repeat(<N> or auto-fill or auto-fit, <track-list>)` | 指定重复 N 次轨道模板，或自动填充                |

| grid item 属性           | 值               | 备注                            |
| ------------------------ | ---------------- | ------------------------------- |
| `grid-column: auto`      | `n`,`b/e`,`b/-1` | 指定 grid item 占据的网格空间   |
| `grid-row: auto`         | `n`,`b/e`,`b/-1` | 指定 grid item 占据的网格空间   |
| `grid-area: <area-name>` |                  | 指定该 grid item 占据的区域名称 |

### 响应式布局

- 利用布局：
  - NormalFlow(`block`&`inline`)
  - Flex(`flex`)
  - Grid(`auto-fill`&`grid-template-area`)
- 利用媒体查询：`@media screen and (min-width: 800px) {...}`
- 利用相对单位：`font-size: calc(1.5rem + 3vw);`

## 定位

| 属性值                        | 备注                                                                                                |
| ----------------------------- | --------------------------------------------------------------------------------------------------- |
| `position: static`            | 默认位置                                                                                            |
| `position: relative`          | 相对默认位置，保持原来布局中占用空间                                                                |
| `position: absolute`          | 相对最近的设置了`position`的祖先元素位置，若都未设置则相对`<html>`，脱离原来布局                    |
| `position: fixed`             | 相对浏览器视窗位置，脱离原来布局                                                                    |
| `position: sticky`            | 视窗经过该元素后再相对视窗位置，保持原来布局中占用空间。祖先元素不能设置`overflow: visibable`以外值 |
| `z-index`                     | 默认为 0，当 z 轴坐标相等且重合时，靠后元素覆盖靠前元素                                             |
| `top`,`bottom`,`left`,`right` | 设置该元素 margin 相对于上,下,左,右 border 的间隔距离                                               |

## 隐藏

- `display: none`：将该元素从布局中删除（不占用空间）
- `visibility: hidden`：将该元素设置为完全透明（仍会占用空间），不会被聚焦
- `visibility: visibale`：设置该元素可见，即使其父元素为`visibility: hidden`
- `visibility: collapse`：
  - 对于表格中行与列，作用同`display: none`
  - 对于 flex item，作用同`display: none`
  - 对于其他元素，作用同`visibility: hidden`

## 溢出

为什么会溢出？

- 限制了`width`而其`min-contetn`太大
- 限制了`height`而子元素堆积总高度太大

| 溢出处理            | 备注                 |
| ------------------- | -------------------- |
| `overflow: visible` | 可见（默认）         |
| `overflow: hidden`  | 隐藏                 |
| `overflow: scroll`  | 滚动条               |
| `overflow: auto`    | 自动隐藏或显示滚动条 |

| 图像溢出处理             | 备注                             |
| ------------------------ | -------------------------------- |
| `object-fit: none`       | 保持其原有的尺寸                 |
| `object-fit: contain`    | 缩放并保持长宽比，保证包含全图   |
| `object-fit: cover`      | 缩放并保持长宽比，必要时进行裁剪 |
| `object-fit: fill`       | 缩放且必要时拉伸，保证包含全图   |
| `object-fit: scale-down` | 取 none 与 contain 中的较小者    |
| `max-width: 100%`        | 仅缩小并保持长宽比               |

> `object-fit`需要设置`width`生效

## 动画

1. 先指定动画的基础属性

   ```css
   /* @keyframes duration | easing-function | delay | iteration-count | direction | fill-mode | play-state | name */
   animation: 3s ease-in 1s 2 reverse both paused slidein;

   /* @keyframes name | duration | easing-function | delay */
   animation: slidein 3s linear 1s;

   /* @keyframes name | duration */
   animation: slidein 3s;
   ```

2. 再指定动画的关键帧

   ```css
   @keyframes slidein {
     from {
     }
     50% {
     }
     to {
     }
   }
   ```
