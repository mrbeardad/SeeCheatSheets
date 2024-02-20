# Qt

- [Qt](#qt)
  - [Object Model](#object-model)

## Object Model

- Meta Object System: 为 C++ 添加动态反射的功能

- Property System: 为 C++ 添加定义属性的功能

- Object Tree: 组织对象结构，并提供额外自动析构能力，即析构对象时会析构其所有子对象，并从其父对象中删除自己

- Event System: 通常用来处理 UI 事件

- Signals and Slots: 用来对象间通讯，同一线程同步调用，不同线程异步调用

```cpp
class MyClass: public QObject {
  Q_OBJECT
  Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enableChanged)

public:
  MyClass(QObject *parent = nullptr);

  bool event(QEvent* ev) override {
    if (ev->type() == QEvent::PolishRequest) {
      // overwrite handling of PolishRequest if any
      doThings();
      return true;
    } else  if (ev->type() == QEvent::Show) {
      // complement handling of Show if any
      doThings2();
      QWidget::event(ev);
      return true;
    }
    // Make sure the rest of events are handled
    return QWidget::event(ev);
  }

  bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (obj == textEdit) {
      if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        qDebug() << "Ate key press" << keyEvent->key();
        return true;
      } else {
        return false;
      }
    } else {
      // pass the event on to the parent class
      return QMainWindow::eventFilter(obj, event);
    }
  }

public slots:
  void setEnable(bool);

signals:
  void enableChanged(bool);

private:
  bool m_enabled;
}
```

```cpp
QCoreApplication* createApplication(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i) {
        if (!qstrcmp(argv[i], "-no-gui"))
            return new QCoreApplication(argc, argv);
    }
    return new QApplication(argc, argv);
}

int main(int argc, char* argv[])
{
    QScopedPointer<QCoreApplication> app(createApplication(argc, argv));

    if (qobject_cast<QApplication *>(app.data())) {
       // start GUI version...
    } else {
       // start non-GUI version...
    }

    return app->exec();
}
```
