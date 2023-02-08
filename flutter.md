# Flutter

## Widgets

- Flutter 使用声明式语法编写 Widgets 树，然后生成 RenderObject 树，最终渲染成画面

- Widgets 的主要任务是负责展示状态数据与响应 UI 交互（以及提供配置数据）

  - Widgets 的状态数据来源

    - 父组件
    - 内部状态
    - UI Controller
    - context

  - Widgets 的 UI 响应逻辑来源

    - 父组件
    - 内部方法
    - UI Controller

## 窗口

## 内容

用于展示内容的 widget 拥有内在固有的尺寸大小

- 文本：`Text`
- 图标：`Icon`
- 图像：`Image`

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
- 约束：由父组件传递给子组件从而约束其尺寸大小，特殊的，
  - tight 指最大约束与最小约束相等
  - loose 指最小约束为 0 且最大约束为父组件大小
  - unlimited 指约束值为无穷大，从而可以让一些组件显示自然的大小，同时也会导致一些组件无法显示
- 尺寸：由子组件根据约束计算尺寸大小再返回给父组件

通过布局容器包裹子组件从而调整子组件位置与大小

- 控制大小：
  - `ConstranedBox`, `LimitedBox`, `UnconstrainedBox`, `OverflowBox`
  - `IntrinsicWidth`, `IntrinsicHeight`, `SizedBox`, `FractionallySizedBox`, `Container`, `AspectRatio`
  - `SizedOverflowBox`, `FittedBox`
- 控制位置：`Center`, `Align`, `Padding`, `Baseline`
- 流布局：`Wrap`
- 一维布局：`Flex`, `Row`, `Column`, `Flexible`, `Expanded`, `Spacer`, `ListView`
- 二维布局：`Table`, `GridView`
- 三维布局：`Stack`, `Positioned`

### 样式

- 局部样式一般由组件的属性控制，如`style`, `color`, `shape`等

  ```dart
  Color(0xFF42A5F5);
  Color.fromARGB(0xFF, 0x42, 0xA5, 0xF5);
  Color.fromARGB(255, 66, 165, 245);
  Color.fromRGBO(66, 165, 245, 1.0);
  Colors.blue;
  Colors.blue[200];
  ColorScheme.fromSwatch(primarySwatch: Colors.purpl);
  ColorScheme.fromSeed(seedColor: Colors.purpl);
  ```

- 全局样式一般由`MaterialApp.theme`控制

```dart
MaterialApp(
  theme: ThemeData.light(),
  // ...
)
```

### 动画

- implicit animations

  - `AnimatedFoo(foo: , duration: , curve: , child: )`
  - use `TweenAnimationBuilder(tween: , duration: , curve: , builder: )`

- explicit animations

  - `FooTransition(foo: animation, child: )`
  - use `AnimatedBuilder(animation: , builder: )`

```dart
class Foo extends StatefulWidget {
  const Foo({ super.key, required this.duration, required this.tween, curve = Curves.linear });

  final Duration duration;
  final Tween<Double> tween;
  final Curve curve;

  @override
  State<Foo> createState() => _FooState();
}

class _FooState extends State<Foo> with SingleTickerProviderStateMixin {
  late AnimationController _controller;
  late Animation<Double> _animation;

  @override
  void initState() {
    super.initState();
    _controller = AnimationController(
      vsync: this, // the SingleTickerProviderStateMixin
      duration: widget.duration,
    );
    _animation = widget.tween.animation(_controller).chain(widget.curve);
  }

  @override
  void didUpdateWidget(Foo oldWidget) {
    super.didUpdateWidget(oldWidget);
    _controller.duration = widget.duration;
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Container(
      color: Colors.red,
      width: _animation.value,
      height: _animation.value,
    ); // ...
  }
}
```

动画三要素：`Duration`, `Tween`, `Curve`

- `AnimationController`：维护并控制动画状态与数据，在渲染新帧时更新
  - `Duration`由用户指定
  - `Tween`默认`Tween<Double>(begin: 0.0, end: 1.0)`
  - `Curve`默认`Curves.linear`

## 输入

- 鼠标事件：`Listener`, `MouseRegion`

- 手势检测：`GestureDetector`

- 键盘焦点：`Focus`, `FocusScope`

- 快捷键：`Shortcuts`, `Intent`, `Actions`

```dart
@override
Widget build(BuildContext context) {
  return Shortcuts(
    shortcuts: <ShortcutActivator, Intent>{
      LogicalKeySet(LogicalKeyboardKey.control, LogicalKeyboardKey.keyA): SelectAllIntent(),
      SingleActivator(LogicalKeyboardKey.keyC, control: true): CopyIntent(),
      CharacterActivator('?'): HelpIntent(),
    },
    child: Actions(
      actions: <Type, Action<Intent>>{
        SelectAllIntent: SelectAllAction(model),
        CopyIntent: CopyAction(model),
        HelpIntent: HelpAction(model),
      },
      child: Builder(
        builder: (context) => TextButton(
          child: const Text('SELECT ALL'),
          onPressed: Actions.handler<SelectAllIntent>(
            context,
            SelectAllIntent(),
          ),
        ),
      ),
    ),
  );
}
```
