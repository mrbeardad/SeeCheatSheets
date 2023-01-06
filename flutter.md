# flutter

## 内容

- 文本：`Text('content')`
- 图标：`Icon(Icons.favorite)`
- 图像：`Image.asset('assets/background.png')`, `Image.network('https://example.com/pic.jpg')`

## 呈现

### 布局

核心概念：

- 位置：由父组件决定子组件位置
- 约束：由父组件传递给子组件从而约束其尺寸大小，tight 指最大约束与最小约束相等，loose 指只有最大约束
- 尺寸：由子组件根据约束计算尺寸再返回给父组件

- `Container`
- `Center`
- `Align`
- `Row`, `Column`, `Flex`, `Expanded`, `Flexible`
- `ListView`, `ListTile`
- `GridView`, `GridTile`
- `Stack`
