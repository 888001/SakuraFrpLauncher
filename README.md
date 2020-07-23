# SakuraFrpLauncher

#### 介绍
sakura frp客户端启动器，负责管理本地frpc进程，支持不同节点自动分组显示，各分组显示节点名称，隧道名称，启动时间，运行时间，当前运行状态等，可以独立显示每个frpc进程的启动日志

#### 开发环境
- VS2013
- Qt 5.5.1 opensrouce 32bit

#### 相比于官方启动器
1. 可以分节点显示各节点的所有隧道，统一管理
2. 增加各frpc的启动时间，运行时间和运行状态
3. 各frpc的启动日志独立显示，互不影响
4. 没有增加和删除隧道功能

#### 待实现功能列表
1. 隧道可以修改成分组，不按节点名称进行分组
2. 分组名称可以任意修改，不按节点名称进行分组
3. 增加分组的批量启动，停止
4. 增加隧道，删除隧道功能（暂定官网一个入口增加修改）
5. 删除分组，并删除分组的所有隧道
6. 支持隧道frpc运行过程中错误报警，可以在托盘弹出通知信息
7. 跨平台支持，支持linux，mac系统，支持树莓派运行
8. 其他功能

#### 1.0.0版本实现
1. 基础登录功能
2. 按节点名进行自动分组，不支持修改
3. 显示隧道的基础信息，增加启动时间，运行时间和运行状态显示
4. 各隧道启动日志独立显示，支持右键菜单和快捷键拷贝，全选等