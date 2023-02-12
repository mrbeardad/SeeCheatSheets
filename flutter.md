# Flutter

## Widgets

- Widget 只是用来保存视图配置信息的数据结构，可以频繁创建与销毁而没有性能问题

- Element 在首次创建 Widget 时对应地生成，目的为用作 Widget 缓存以节省开销

- RenderObject 在挂载 Element 时对应地生成，它负责布局、绘制等渲染任务

- Widgets、Elements 与 RenderObjects 各自组成三棵树，Elment 与 Widget 一一对应，而 RenderObject 与 Element 则不是

- 视图重绘时，会重新构建 Widget Subtree，并比较新旧 Widget 然后更新对应 Element，再由 Element 更新对应 RenderObject，一些 Widget 利用类似 `builder(context, child)` 属性来避免重新构建子树

- `StatelessWidget`与`StatefulWidget`用于组合 Element 节点

- `LeafRenderObjectWidget`、`SingleChildRenderObjectWidget`与`MultiChildRenderObjectWidget`才会新建 Element 节点

- 广义上讲，Widget 的主要任务是负责展示状态数据与响应 UI 交互

  - 状态数据来源

    - 父组件
    - 内部状态
    - controller
    - context

  - UI 响应逻辑来源

    - 父组件
    - 内部方法
    - controller

  - 常见属性名：
    - `child`：单个子组件
    - `children`：子组件列表
    - `builder`：构建子组件的函数
      - 在必要时才构建以提高性能
      - 提供额外的状态数据
      - 为子组件提供 context 而避免重构提取出子组件
    - `delegation`：将内部的一些配置参数与策略方法暴露给用户来提供和实现
    - `controller`：为组件提供状态数据（可能由 UI 交互获取），并为用户提供方法来控制组件内部状态

## 窗口

由平台 Native 代码控制

## 内容

用于展示内容的 widget 拥有内在固有的尺寸大小

- 文本：`Text`
- 图标：`Icon`
- 图像：`Image`
- 2D 绘制：`CustomPaint`

```yaml
# pubspec.yaml
# app 加载 asset: Image.asset('images/background.png')
# pkg 加载 asset: Image.asset('images/background.png', package: 'my_pkg')
flutter:
  assets:
    - images/background.png
    - images/2.0x/background.png
    - fonts/
```

## 展示

### 布局

盒子布局：

- 核心概念：

  - 位置：由父组件决定子组件位置
  - 约束：由父组件传递给子组件从而约束其尺寸大小，特殊的，
    - tight 指最大约束与最小约束相等
    - loose 指最小约束为 0 且最大约束为父组件大小
    - unlimited 指约束值为无穷大，从而可以让一些组件显示自然的大小，同时也会导致一些组件无法显示
  - 尺寸：由子组件根据约束计算尺寸大小再返回给父组件

- 布局容器：

  - 控制大小：
    - 获取约束：`LayoutBuilder`
    - 更改约束：`ConstranedBox`, `LimitedBox`, `UnconstrainedBox`, `OverflowBox`
    - 指定大小（考虑约束）：`IntrinsicWidth`, `IntrinsicHeight`, `SizedBox`, `FractionallySizedBox`, `Container`, `AspectRatio`
    - 指定大小（忽略约束）：`SizedOverflowBox`, `FittedBox`
  - 控制位置：`Center`, `Align`, `Padding`, `Baseline`
  - 流布局：`Wrap`
  - 一维布局：`Flex`, `Row`, `Column`, `Flexible`, `Expanded`, `Spacer`, `ListView`
  - 二维布局：`Table`, `GridView`
  - 三维布局：`Stack`, `Positioned`

滚动布局：

- 核心概念：
  - 滚动布局因其子组件尺寸动态确定，故使用特殊的 Sliver 布局协议
  - 利用滚动布局从而可以处理盒子布局的溢出问题
- 布局容器：
  - `CustomScrollView`
  - `SliverPadding`
  - `SliverToBoxAdapter`
  - `SliverAppBar`
  - `SliverList`, `SliverFixedExtentList`, `SliverPrototypeExtentList`
  - `SliverGrid`
  - `SliverChildBuilderDelegate`, `SliverChildListDelegate`
  - `Scrollbar`, `SingleChildScrollView`, `ListView`, `ReorderableListView`, `GridView`, `PageView`, `RefreshIndicator`

### 样式

- 全局样式，`MaterialApp(theme: ThemeData.from(colorScheme: c, textTheme: t))`
- 局部样式，由组件的诸如`style`, `color`, `shape`等属性控制
- 视效容器

  - `BackdropFilter`, `ImageFilter`
  - `Opacity`, `Visibility`
  - `ShaderMask`
  - `ClipRect`, `ClipRRect`, `ClipOval`, `ClipPath`
  - `DecoratedBox`, `BoxDecoration`, `LinearGradient`, `RadialGradient`, `SweepGradient`
  - `RotatedBox`
  - `Transform`

### 动画

- implicit animations：改变属性时自动生成`Animation`来控制动画

  - `AnimatedFoo(foo: , duration: , curve: , child: )`
  - use `TweenAnimationBuilder(tween: , duration: , curve: , builder: )`

- explicit animations：由用户提供`Animation`来控制动画

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
    _animation = widget.tween.animation(CurvedAnimation(
      parent: _controller,
      curve: widget.curve,
    ));
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
    return AnimatedBuilder(
      animation: _animation,
      builder: (context, child) => Container(
        color: Colors.red,
        width: _animation.value,
        height: _animation.value,
        child: child,
      ),
    ); // ...
  }
}
```

动画三要素（动画的本质即随时间变化而更改布局与样式）：

- `Duration`：指定动画运行时长
- `Tween`：指定样式数据的类型与变化范围
- `Curve`：应用时间-曲线函数得到不同的动画效果

`AnimationController`：维护并控制动画状态与数据，在渲染新帧时更新

- `Duration`由用户指定
- `Tween`默认`Tween<Double>(begin: 0.0, end: 1.0)`
- `Curve`默认`Curves.linear`

## 输入

- 鼠标事件：`Listener`, `MouseRegion`, `AbsorbPointer`, `IgnorePointer`

- 手势检测：`GestureDetector`, `Draggable`, `LongPressDraggable`, `DragTarget`, `SelectionArea`, `InteractiveViewer`, `Dismissible`

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

## Material Design Components

- `MaterialApp`
- `Scaffold`, `AppBar`, `Drawer`, `BottomAppBar`, `BottomNavigationBar`
- `TabBar`, `TabBarView`, `TabPageSelector`, `TabController`
- ...

## 状态管理

- `package:flutter_hooks`
- `package:hooks_riverpod`
- `package:riverpod_annotation`
- `package:freezed`

## 导航路由

- `package:go_router`
- `package:url_launcher`
