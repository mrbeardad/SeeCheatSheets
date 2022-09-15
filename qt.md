# QT6

## Widgets

- 无 parent 的 widgets 为窗口

## View/Model

```cpp
// 所有 Model 维护的数据都抽象成一个二维表，其中的元素称为 Item，其类型可以是绝大多数类型，包括另一个 Model 的 Index 从而可构成层级结构
QAbstractItemModel* model = new QFileSystemModel{};

// 访问 Model 中的 Item 需要 Index 指定层级，同时还需要指定行号与列号定位其在该层级中的位置
QmodelIndex parent{}; // 若 parent 无效则指向根层级
QModelIndex index = model->index(row, column, parent);

// 指定 Qt::ItemDataRole 从而可以限制 Item 类型
QVariant value = model->data(index, Qt::DisplayRole);
QString text = value.toString();

// QItemSelectionModel 维护用户在 View 中选择的 Items of Model
secondTableView->setSelectionModel(firstTableView->selectionModel());
```

## Object Model

- signal and slot
- object properties
- events and events filters
- i18n string
- timer
