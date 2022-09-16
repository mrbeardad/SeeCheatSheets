# QT6

## Widgets

- 无 parent 的 widgets 为窗口

## View/Model

```cpp
// 所有 Model 维护的数据都抽象成一个二维表，不论底层数据结构如何表示，其中的元素称为 Item，其类型可以是绝大多数类型，包括另一个 Model 的 Index 从而可构成层级结构
QAbstractItemModel* model = new QListModel{};

// 访问 Model 中的 Item 需要 Index 指定层级，同时还需要指定行号与列号定位其在该层级中的位置
QmodelIndex parent{}; // 若 parent 无效则指向根层级
QModelIndex index = model->index(row, column, parent);

// 读取与写入 Item 都可以指定 Qt::ItemDataRole 从而可以限制 Item 类型
QVariant value = model->data(index, Qt::DisplayRole);
QString text = value.toString();
model->setData(index, text)

// View 展示 Model 提供的数据
QAbstractItemView* view = new QListView{};
view->setModel(model);

// View 使用 QItemSelectionModel 维护用户在选择的 Items of the Model
secondTableView->setSelectionModel(firstTableView->selectionModel());
```

## Object Model

- signal and slot
- object properties
- events and events filters
- i18n string
- timer
