# Flutter

## Widgets

- Flutter 使用声明式语法编写 Widgets 树，然后计算出 RenderObject 树，最终渲染成画面

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

- 子节点属性名通常为

  - `child`: 单个子组件
  - `children`: 多个子组件
  - `builder`: 回调函数的形式通常用于
    - 必要时才构建以提高性能
    - 父组件为子组件提供额外状态数据
    - 为子组件产生新的 context

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

- 局部样式一般由组件的属性控制，如`style`, `color`等
- 全局样式一般由`MaterialApp.theme`控制

```dart
MaterialApp(
  theme: ThemeData(
    colorScheme: ColorScheme.fromSwatch().copyWith(secondary: Colors.green),
    textTheme: const TextTheme(bodyMedium: TextStyle(color: Colors.purple)),
  ),
  // ...
)
```

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
  - use `AnimatedBuilder(animation: , builder: )`
  - subclass `AnimatedWidget(listenable: animation)`

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
