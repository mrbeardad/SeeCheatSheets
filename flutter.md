# Flutter

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

### 布局

核心概念：

- 位置：由父组件决定子组件位置
- 约束：由父组件传递给子组件从而约束其尺寸大小，tight 指最大约束与最小约束相等，loose 指只有最大约束
- 尺寸：由子组件根据约束计算尺寸再返回给父组件

---

- `UnconstrainedBox`, `ConstranedBox`, `LimitedBox`, `SizedBox`, `FittedBox`
- `Container`, `Center`, `Align`
- `Row`, `Column`, `Flex`, `Expanded`, `Flexible`
- `ListView`, `ListTile`
- `GridView`, `GridTile`
- `Stack`
- `Navigator`, `MaterailPageRoute`

### 样式

大多由组件的构造参数控制，如`style`, `color`等

### 动画

```dart
AnimationController controller = AnimationController(
    duration: const Duration(milliseconds: 500), vsync: this);
final Animation<double> curve =
    CurvedAnimation(parent: controller, curve: Curves.easeOut);
Animation<int> alpha = IntTween(begin: 0, end: 255).animate(curve);
```

- implicit animations: 单个组件的状态改变时自动触发过渡动画
  - `AnimatedFoo`
  - use `TweenAnimationBuilder`
- explicit animations
  - `FooTransition`
  - subclass `AnimatedWidget` for single widget
  - use `AnimatedBuilder` for multiple midgets

## 输入

- 鼠标：`Listener`, `MouseRegion`, `GestureDetector`

- 键盘：`Focus`, `Shortcuts`, `Actions`
