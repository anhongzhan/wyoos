# wyoos

# 本仓库目标

使用cpp实现一个操作系统，包括中断、UI、网络等内容

# 参考链接

[视频链接1](https://www.bilibili.com/video/BV1RX4y157CM?p=1&vd_source=71ff1308c23d0ef0791f945a3cc515e6)

[视频链接2](https://www.bilibili.com/video/BV1Ut411o7VG?p=2&share_medium=android&share_plat=android&share_source=COPY&share_tag=s_i&timestamp=1614198935&unique_k=Vu8Mmp&vd_source=71ff1308c23d0ef0791f945a3cc515e6)

# 我的博客

[anhongzhan.github.io](anhongzhan.github.io)

博客更新的进度会比代码仓库慢一些

[001-HelloWorld](https://anhongzhan.github.io/2022/09/19/wyoos001/)

[002-HelloWorld](https://anhongzhan.github.io/2022/09/20/wyoos002/)

[003-makeiso]

[004-port]

[005-gdt]

[006-interrupt]

[007-keyboard]

# 注意

因为每次写新的内容时都需要添加新的文件夹，这就导致虚拟机无法找到正确的镜像文件，所以每次运行`make run`之前要确定虚拟机运行的镜像文件是否为我们刚刚生成的镜像文件

建议每次`make run`之后使用`make clean`，这样更换新的文件夹之后虚拟机就会找不到原有的镜像，选择新的镜像并重新`make run`即可