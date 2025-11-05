# Qt Android Flash Assistant (.pro)
用 Qt Creator 打开 `QtAndroidFlashAssistant.pro` 直接编译运行。

## 使用说明
1. 安装 Qt (建议 5.12+ 或 6.x)，用 Qt Creator 打开本工程。
2. 编译运行后，在可执行文件同级创建 `tools\` 目录，放入 `adb.exe` 和 `fastboot.exe`。
3. 选择 ADB 或 Fastboot 模式进行操作。

> 警告：刷机有风险，务必确认镜像/包与机型匹配，解锁会清空数据。