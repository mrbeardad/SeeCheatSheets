# protobuf

```sh
protoc -I=$SRC_DIR --cpp_out=$DST_DIR $SRC_DIR/files.proto
```

```protobuf
syntax "proto3";

package foo.bar;

import "google/protobuf/any.proto";

// 编号以 1 开头，编号作为字段标识不能重用，特别是更改协议字段的时候
message Types {
  // 保留字段，用于更改协议删除字段后防止编号被复用
  reserved 2, 15, 9 to 11;
  reserved "foo", "bar";

  bool bool = 1;
  int32 int32 = 2;
  int64 int64 = 3;
  sint32 int32 = 6;
  sint64 int64 = 7;
  uint32 uint32 = 8;
  uint64 uint64 = 9;
  sfixed32 int32 = 10;
  sfixed64 int64 = 11;
  fixed32 uint32 = 12;
  fixed64 uint64 = 13;
  string string = 14;
  bytes string = 15;

  repeated string array = 19;
  map<string, Project> map = 20; // 键类型只能是 integral or string，且 map 不能为 repeated

  google.protobuf.Any any = 16;
  oneof variant {
    int32 foo = 17;
    string bar = 18;
  }
}

// 枚举值以 0 开头，默认不能存在同值枚举名
enum Enumerations {
  option allow_alias = true;
  EAA_UNSPECIFIED = 0;
  EAA_STARTED = 1;
  EAA_RUNNING = 1;
  EAA_FINISHED = 2;
}
```

```cpp
// 使用 arana 一次性申请和释放内存可提高性能
#include <google/protobuf/arena.h>

Arena arena;

MyFeatureMessage* arena_message = google::protobuf::Arena::CreateMessage<MyFeatureMessage>(&arena);

// Numeric or Enum
pb.foo();
pb.set_foo(value);
pb.clear_foo();
// Repeated
pb.foo_size();
pb.foo(index);
pb.set_foo(index, value);
pb.add_foo(value);
pb.clear_foo();
pb.foo();
pb.mutable_foo();

// String
pb.foo();
pb.set_foo(value);
pb.mutable_foo();             // 暴露内部成员
pb.clear_foo();
pb.set_allocated_foo(&value); // 手动release_foo释放内存
pb.release_foo();
// Repeated
pb.foo_size();
pb.foo(index);
pb.set_foo(index, value);
pb.mutable_foo(index);
pb.add_foo(value);
pb.add_foo();                 // 添加并暴露元素
pb.clear_foo();
pb.foo();
pb.mutable_foo();

// Message
pb.has_foo();
pb.foo();
pb.mutable_foo();
pb.clear_foo();
pb.set_allocated_foo(&value)
pb.release_foo();
// Repeated
pb.foo_size();
pb.foo(index);
pb.mutable_foo(index);
pb.add_foo();
pb.clear_foo();
pb.foo();
pb.mutable_foo();

// Map: 类似 C++ std::map

// Any
any.Is<Type>();
any.UnpackTo(&message);
any.PackFrom(message);

// Oneof
oneof.has_foo();
```
