# Flutter

## Widgets

- Flutter 使用声明式语法将 Widgets 组装成树并最终渲染成可视画面
- Widgets 的主要任务是负责展示状态数据与响应 UI 交互
- Widgets 的状态数据来源
  - 父组件
  - 上下文
  - UI 交互：`Controller`
- Widgets 的 UI 响应逻辑来源
  - 父组件
  - 当前组件方法
  - `Controller`
- 子节点属性名通常为
  - `child`: 单个子组件
  - `children`: 多个子组件
  - `builder`: 回调函数的形式通常用于提高性能或者父组件为子组件提供额外状态数据

## 窗口

## 内容

用于展示内容的 widget 拥有内在固有的尺寸大小

- 文本：`Text('content')`, `SelectableText('content')`
- 图标：`Icon(Icons.favorite)`
- 图像：`Image.asset('assets/background.png')`, `Image.network('https://example.com/pic.jpg')`

```yaml
# pubspec.yaml
# app 加载 asset: Image.asset('images/background.png')
# pkg 加载 asset: Image.asset('images/background.png', package: 'my_pkg')
flutter:
  assets:
    - images/background.png
    - images/2.0x/background.png
    - icons/
```

## 展示

核心概念：

- 位置：由父组件决定子组件位置
- 约束：由父组件传递给子组件从而约束其尺寸大小，tight 指最大约束与最小约束相等，loose 指只有最大约束
- 尺寸：由子组件根据约束计算尺寸再返回给父组件

### 布局

通过布局容器包裹子组件从而调整子组件位置与大小

- `UnconstrainedBox`, `ConstranedBox`, `LimitedBox`, `SizedBox`, `FittedBox`
- `Container`, `Center`, `Align`
- `Row`, `Column`, `Flex`, `Expanded`, `Flexible`
- `ListView`, `ListTile`
- `GridView`, `GridTile`
- `Stack`
- `Navigator`, `MaterailPageRoute`

### 样式

大多由组件的属性控制，如`style`, `color`等

### 动画

```dart
final controller = AnimationController(duration: const Duration(milliseconds: 500), vsync: this);
final animation = IntTween(begin: 0, end: 255).animate(CurvedAnimation(parent: controller, curve: Curves.easeOut));
```

动画组件由三部分组成：

- `Tween`：负责在两个值间计算插值
- `Animation`：负责维护动画状态
- `AnimationController`：负责控制动画状态，在必要时才更新渲染动画

最终目的为其他组件提供状态数据`animation.value`

- implicit animations
  - `AnimatedFoo(foo: , duration: , curve: , child: )`
  - use `TweenAnimationBuilder(tween: , duration: , curve: , builder: )`
- explicit animations
  - `FooTransition(foo: animation, child: )`
  - subclass `AnimatedWidget(listenable: animation)`
  - use `AnimatedBuilder(animation: , builder: )`

## 输入

- 鼠标：`Listener`, `MouseRegion`, `GestureDetector`

- 键盘：`Focus`, `Shortcuts`, `Actions`
