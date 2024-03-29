# Flutter

- [Flutter](#flutter)
  - [简介](#简介)
  - [窗口](#窗口)
  - [内容](#内容)
  - [呈现](#呈现)
    - [布局](#布局)
      - [Box](#box)
      - [Sliver](#sliver)
    - [样式](#样式)
    - [动画](#动画)
    - [Material Design](#material-design)
  - [交互](#交互)
  - [状态管理](#状态管理)
  - [导航路由](#导航路由)
  - [国际化](#国际化)

## 简介

具体参见<https://docs.flutter.dev/resources/architectural-overview>

- 应用启动：调用根 Widget 的 build 方法递归地构造 Element 树，再由后者构造 RenderObject 树，最后由 RenderObject 树负责渲染画面
- 重新渲染：Widget 关联的状态发生状态转移时，重新调用其 build 方法生成新的 Element 子树，并更新对应的 Element，再由后者更新对应 RenderObject，最后更新画面。一些 Widget 利用类似 `builder(context, child)` 属性来避免不必要地重新构建子树

- `StatelessWidget` 与 `StatefulWidget` 只用于组合 Element 节点
- `LeafRenderObjectWidget`、`SingleChildRenderObjectWidget` 与 `MultiChildRenderObjectWidget` 才会新建 Element 节点

- Widget 视图仅与其状态有关，即 $View=f(Status)$

- 状态数据
  - 内部状态
  - 共享状态
  - UI 输入：特殊的 IO 事件，如用户表单输入等
  - IO 事件：事件发生时通常会携带外部数据，如 UI 交互、网络通讯等
- 状态转移

  - 事件是触发状态转移的唯一来源
  - 改变属性和调用方法是进行状态转移的途径

- Widget 常见属性名：
  - `child`：单个子组件
  - `children`：子组件列表
  - `builder`：构建子组件的函数，为其提供 context 和额外参数
  - `delegation`：将内部的一些配置参数与策略方法暴露给用户来提供和实现
  - `controller`：为组件提供状态数据（可能由 UI 交互获取），并为用户提供方法来控制组件内部状态

## 窗口

- 由 Embeder（即平台 Native 代码）控制
- `package:bitsdojo_window`

## 内容

用于展示内容的 widget 拥有内在固有的尺寸大小

- 文本：`Text`
- 图标：`Icon`, `Icons`
- 图像：`Image`, `FadeInImage`
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

## 呈现

### 布局

#### Box

- 盒子布局：可看做若干矩形盒子在平面上排列、嵌套和覆盖

  - 约束：从上到下，由父组件传递给子组件从而约束其尺寸大小，其中特殊的有
    - tight 指最大约束与最小约束相等
    - loose 指最小约束为 0 且最大约束为父组件大小
    - unlimited 指约束值为无穷大，从而可以让一些组件显示自然的大小，同时也会导致一些组件无法显示
  - 尺寸：从下到上，由子组件根据约束计算尺寸大小再返回给父组件
  - 位置：父组件决定子组件位置

- Widgets：

  - 控制位置：`Center`, `Align`, `Padding`, `Baseline`
  - 控制大小：
    - 获取约束：`LayoutBuilder`
    - 更改约束：`ConstranedBox`, `LimitedBox`, `UnconstrainedBox`, `OverflowBox`
    - 指定大小（考虑约束）：`IntrinsicWidth`, `IntrinsicHeight`, `SizedBox`, `FractionallySizedBox`, `Container`, `AspectRatio`
    - 指定大小（忽略约束）：`SizedOverflowBox`, `FittedBox`
  - 流布局：`Wrap`
  - 一维布局：`Flex`, `Row`, `Column`, `Flexible`, `Expanded`, `Spacer`, `ListView`
  - 二维布局：`Table`, `GridView`
  - 三维布局：`Stack`, `Positioned`

#### Sliver

- 滚动布局：sliver 布局协议比较复杂且灵活，每个 sliver 可以根据视窗大小和滑动状态来决定自己在视窗中的位置、绘制大小等

> 由于 Box 与 Sliver 之间不能直接传递约束和大小等布局信息，所以通常使用一些封装或中介 Widget，如 ListView, GridView, CustomScrollView, RenderSliverToBoxAdapter 等

- Widgets：

  - `CustomScrollView`
  - `SliverPadding`
  - `SliverAppBar`
  - `SliverList`, `SliverFixedExtentList`, `SliverPrototypeExtentList`
  - `SliverGrid`
  - `SliverChildBuilderDelegate`, `SliverChildListDelegate`
  - `SliverToBoxAdapter`
  - `Scrollbar`, `SingleChildScrollView`, `ListView`, `ReorderableListView`, `GridView`, `PageView`, `RefreshIndicator`

### 样式

- 全局样式，`MaterialApp(theme: ThemeData.from(colorScheme: c, textTheme: t))`
- 局部样式，由组件的诸如`style`, `color`, `shape`等属性控制
- 视效组件

  - `ShaderMask`
  - `ClipRect`, `ClipRRect`, `ClipOval`, `ClipPath`
  - `DecoratedBox`, `BoxDecoration`, `LinearGradient`, `RadialGradient`, `SweepGradient`
  - `BackdropFilter`, `ImageFilter`
  - `Opacity`, `Visibility`
  - `RotatedBox`
  - `Transform`

### 动画

- implicit animations：改变属性时自动生成`Animation`来控制动画

  - `AnimatedFoo(foo: , duration: , curve: , child: )`
  - use `TweenAnimationBuilder(tween: , duration: , curve: , builder: )`

- explicit animations：由用户提供`Animation`来控制动画

  - `FooTransition(foo: animation, child: )`
  - use `AnimatedBuilder(animation: , builder: )`

- 复合多动画：组合 Widget
- 顺序多动画：`curve: Interval`

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

### Material Design

- `MaterialApp`
- `Scaffold`, `NavigationBar`, `NavigationRail`, `NavigationDrawer`, `AppBar`
- `TabBar`, `TabBarView`, `TabPageSelector`, `TabController`
- ...

## 交互

- 鼠标事件：`Listener`, `MouseRegion`, `AbsorbPointer`, `IgnorePointer`

- 高级手势：`GestureDetector`, `Draggable`, `LongPressDraggable`, `DragTarget`, `SelectionArea`, `InteractiveViewer`, `Dismissible`

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

## 状态管理

- `package:flutter_hooks`

## 导航路由

- `package:go_router`
- `package:url_launcher`

## 国际化

- `package:flutter_localization`
- `package:intl`
- 具体参见<https://docs.flutter.dev/development/accessibility-and-localization/internationalization>

```dart
import 'dart:ui'

final Locale systemLocale = window.locale;
final Locale contextLocale = Localizations.localeOf(context);
```
