# QT6

## 构建

```cmake
# 至少需要 c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 导入 qt cmake 资源包
find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_executable(mainwin WIN32 mainwin.cpp)
target_link_libraries(mainwin PRIVATE Qt6::Widgets)

# 添加资源文件，如UI文件(.ui)、图像、翻译文件(.ts)、
qt_add_resources(mainwin images
    PREFIX /images
    LANG zh_CN
    FILES image1.png image2.png)
```

```cpp
QIcon(":/images/image1.png");
```

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

```cpp
class MyClass : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Priority priority READ priority WRITE setPriority NOTIFY priorityChanged)

public:
    enum Priority { High, Low, VeryHigh, VeryLow };
    Q_ENUM(Priority)

    MyClass(QObject *parent = nullptr);
    ~MyClass();

    void setPriority(Priority priority)
    {
        m_priority = priority;
        emit priorityChanged(priority);
    }
    Priority priority() const
    { return m_priority; }

signals:
    void priorityChanged(Priority);

private:
    Priority m_priority;
};
```

- View 与用户交互：event()
- View 与设计师交互：Q_PROPERTY()
- View 与 View 交互：eventFilter()
- View 与 Model 交互：signal & slots
- Model 与 Model 交互
