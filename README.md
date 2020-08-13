# SakuraFrpLauncher（非官方启动器）

#### 介绍
sakura frp客户端启动器，负责管理本地frpc进程

#### 开发环境
- VS2013（可选）
- Qt 5.5.1（可选高版本，支持x86或x64）

#### 相比于官方启动器
1. 没有开机启动功能，不同系统开机启动方式不具有统一性，可以使用第三方工具进行设置开机启动或通过启动脚本启动
2. 没有在线升级功能，目前后端只有官网api后端，没有独立升级服务器

#### 软件特点
1. 支持自动设置**定时获取**服务器隧道列表，不用手动刷新，另支持手动刷新
2. 支持各隧道进程**自动启动**，服务器增加隧道后，启动器会自动下载隧道信息并自动启动，另支持手动启动
3. 支持各隧道进程运行过程中警告和错误报警，可以在**托盘实时弹出通知信息**
4. 支持各隧道进程的**独立日志查看**，每个进程观察各自的进程输入信息，互不影响
5. 支持各隧道进程查看**启动时间，已运行时间，进程状态和进程运行状态**
6. 支持各隧道按节点自动进行**分组显示**
7. 支持根据操作系统位数选用**不同版本**frpc客户端
8. 支持**跨平台**，测试通过支持windows(x86,x64)，linux(x64)，mac，树莓派(raspbian)等运行，其他版本可通过QtCreator编译源代码获取
9. 支持分组的**批量启动，停止**
10. 支持分组的**批量删除**，支持隧道**单独删除**，支持**增加隧道**功能

#### 待实现功能列表
1. 软件的开机启动功能
2. 其他功能

#### 系统展示
[点我](https://www.yuhencloud.com/?p=411)

#### 联系我
邮箱yuhencloud@qq.com