# Kubernetes

![K8S架构图](images/components-of-kubernetes.svg)

- Namespace ：将 K8S 集群划分为若干个资源不可共享的虚拟集群
- Ingress ：管理集群外部访问，提供负载均衡、SSL 终结和基于名称的虚拟托管（七层转发，主外）
- Service ：即使 Pod 的 IP 动态变化也能通过 Service(Name or IP)访问其后的 Pod（四层转发，主内）
- Deployment：管控 Pod 运行在用户期望状态中（调度、恢复等等）
- Pod ：Pod 中运行若干容器共享 IP 等资源（除非服务存在依赖关系，一般一个 Pod 一个容器）
