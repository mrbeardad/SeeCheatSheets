# Chromium

## Process Model

- [Multi-process Architecture](https://www.chromium.org/developers/design-documents/multi-process-architecture/)
- [Process Model and Site Isolation](https://chromium.googlesource.com/chromium/src/+/main/docs/process_model_and_site_isolation.md)
- [Sanbox](https://chromium.googlesource.com/chromium/src/+/HEAD/docs/design/sandbox.md)
- [Chromium UI Platform](https://chromium.googlesource.com/chromium/src.git/+/HEAD/docs/ui/index.md)

## Threads

```cpp
enum class MessagePumpType {
  // This type of pump only supports tasks and timers.
  DEFAULT,

  // This type of pump also supports native UI events (e.g., Windows
  // messages).
  UI,

  // User provided implementation of MessagePump interface
  CUSTOM,

  // This type of pump also supports asynchronous IO.
  IO,

};

enum class ThreadType : int {
  // Suitable for threads that have the least urgency and lowest priority, and
  // can be interrupted or delayed by other types.
  kBackground,
  // Suitable for threads that are less important than normal type, and can be
  // interrupted or delayed by threads with kDefault type.
  kUtility,
  // Suitable for threads that produce user-visible artifacts but aren't
  // latency sensitive. The underlying platform will try to be economic
  // in its usage of resources for this thread, if possible.
  kResourceEfficient,
  // Default type. The thread priority or quality of service will be set to
  // platform default. In Chrome, this is suitable for handling user
  // interactions (input), only display and audio can get a higher priority.
  kDefault,
  // Suitable for display critical threads, ie. threads critical to compositing
  // and presenting the foreground content.
  kDisplayCritical,
  // Suitable for low-latency, glitch-resistant audio.
  kRealtimeAudio,
  kMaxValue = kRealtimeAudio,
};
```

```cpp
wWinMain()
    content::ContentMain()
        content::ContentMainRunner::Initialize();
            content::ContentMainDelegate::BasicStartupComplete();
                tracing::TracingSamplerProfiler::CreateOnMainThread();  // LoaderLockSampler 线程，负责周期性检测 DllMain 锁
            sandbox::policy::Sandbox::Initialize();                     // BrokerEvent 线程，负责监听 Job 对象
        content::ContentMainRunner::Run();
            content::ContentMainRunnerImpl::RunBrowser();
                base::ThreadPoolInstance::Start();                      // ThreadPoolServiceThread 线程，负责转发异步事件到目标线程
                                                                        // ThreadPoolForegroundWorker 线程（若干）
                                                                        // ThreadPoolBackgroundWorker 线程（若干）
                content::BrowserTaskExecutor::CreateIOThread();         // Chrome_IOThread 线程，负责异步 IO
```

```cpp
// electron
BaseWindow        -> Widget
WebContentsView   -> View
WebContents

```

## Sandbox
