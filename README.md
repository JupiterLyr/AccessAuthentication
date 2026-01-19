# Access Authentication
## What is this?
The software is designed to protect the access rights of folders that need to be kept secret.
Every time you compile, it will read the identifier and password you set as well as the corresponding accessible address.</br>
Among them, the password will be stored in some encrypted method.
When using, you need to enter the identifier and its correct password to jump out of the corresponding folder window.</br>
Remember to lock the folder that needs to be protected again after using it!

## How to use?
1. Create a TXT file. You can use `tools/reg.txt` as a template to fill in the information according to the format requirements.
2. If the TXT file is garbled after opening, please try to open it with other opening methods or encoding formats.
3. Run `config_gen.exe`, select the TXT file with just set information and press enter, then the new configuration file should be written.
4. **Delete the TXT file** to avoid information leakage!
5. Run the main software `AccessAuthentication.exe`. Only by entering the correct identifier and password can you access the specified folder.
6. **Click "Protect" to lock the folder** after accessing it. Otherwise, your folder will be unprotected.
7. Click "Cancel" to exit. To move the window, please hold down the upper left corner and drag.

## Warning
- **Don't use it directly for files**! Because its extension will be left behind. Files before protection cannot be cleaned automatically either.
- The files on the removable disk **have NOT been tested** yet!

# 访问认证软件
## 说明
软件旨在为保密文件夹的访问权限提供保护措施。每次编译都会针对性地读取您设定的标识符和密码，以及对应的可访问地址。</br>
其中，密码以某种加密方式储存。使用时，需要输入标识符及其正确密码，才能跳转出对应的文件夹窗口。</br>
使用完毕后，记得重新给需要保护的文件夹上锁！

## 使用方法
1. 首先创建一个 TXT 文件，可以使用 `tools/reg.txt` 作为模板，根据格式要求填写信息。
2. 若 TXT 文件打开后是乱码，请尝试使用其他打开方式或编码格式打开。
3. 运行 `config_gen.exe`，选择刚刚写好配置的 TXT 文件并回车，新的配置文件即可写入。
4. **删除 TXT 文件**以免泄漏信息！
5. 运行主软件 `AccessAuthentication.exe`，只有输入正确的标识符和密码，点击 “确定” 才能访问指定的文件夹。
6. 文件夹访问完毕后，再**点击 “保护” 为文件夹上锁**，否则你的文件夹将处于未保护状态。
7. 退出软件请点击 “取消”，挪动窗口请按住左上角拖动。

## 注意
- **不推荐直接用于文件**！因为其扩展名会被保留，保护前的文件也无法自动清理。
- 目前，尚未对可移动磁盘上的文件进行测试！

---

# Update Logs - 更新日志
This chapter is written in Chinese for record only.

### Version 1.3.0
累积更新 v1.2.1、v1.2.2、v1.3.0，**对文件夹而言的3S版本**：Safe + Stable + Simple！
- 对原本的配置信息加入了混淆机制，防止配置文件内容泄漏
- 用 `encoder.txt` 记录了编码算法框架
- 原先的工具要求用户使用 CSV 文件作为辅助文件，用以写入配置；为了更加用户友好，现改为了 TXT 文件
- 加入防误触机制，用户点击 `Cancel` 按钮后，需二次确认是否关闭
#### Issues
1. 当路径指向的是文件而非文件夹时，仍然无法完美处理
2. 当前无法使用软件解析可移动磁盘上的文件，因为绝对路径不固定，此后将想方设法解决此问题

### Version 1.2.0
包含文件保护-释放机制的首个版本！
- 开发并部署了 `Protect` 的文件夹打包伪装功能
- 完善了打包、解包行为并通过测试
- 将消息弹窗的内容规范化、合理化，不轻易显示文件夹位置

### Vesion 1.1.3
累积更新 v1.1.1 ~ v1.1.3，当前版本**测试未通过**，仅用于备份。
- 开发了文件夹打包伪装功能，目前尚未部署
- 在 UI 中增设了 `Protect` 按钮，用于被解包的文件夹重新打包伪装，目前拟采用仅验证标识符的方式
- 修改了验证通过后的实现逻辑

### Version 1.1.0
首个可用验证模块的版本！</br>
当前版本中，文件夹的安全性需要由用户自行配置，使用难度大，后续版本提出全新方法解决了此问题。</br>
The security of files and folders needs to be configured by users themselves, which is difficult to use. This problem will be solved later.
- 提高了 `encoder.h` 的加密算法复杂度
- 配置了验证机制，可以在本地正常使用

### Version 1.0.0-beta
测试版，旨在测试该软件各模块能否正常运作。
- 开发了信息验证模块，其中加密验证代码被保护在了未上传的 `encoder.h` 中，目前该方法正处于调试阶段
- 开放了 `config_gen.cpp` 的版本控制机制，其中核心加密算法移动到了 `encoder.h` 中

### Version 0.7.2
累积更新 v0.6.4、v0.7.0 ~ 0.7.2，其中 v0.6.4 优化了背景阴影效果，v0.7.x 则致力于开发新功能。考虑到数据安全性问题，配置生成器的算法暂不开源。The algorithm of "Configuration Generator" is not open source considering that data security needs to be protected.
- GUI 界面部分完工，仅剩 `Confirm` 的功能链接未配置
- 加入了首个用于生成 BIN 文件的**配置生成器**工具，其源文件放置于 `tools` 文件夹下

### Version 0.6.3
累积更新 v0.6.0 ~ v0.6.3。该版本交付了较完善的 UI 设计，`Confirm` 的操作暂且用 `QMessageBox` 拦截。
- 优化了 UI 层，将 `centralWidget` 作为总底板，`centralLayout` 用于界面居中，所有组件挪至其内的 `mainWidget` 中
- 将 `mainWidget` 作为交互窗口，并在窗口后方添加了阴影
- 将 `Confirm` 的快捷键设定为 `Enter`
- 优化了 QSS 组件效果设置
- 配置并修复了软件图标无法正常显示的 bug

### Version 0.5.1
累积更新 v0.5.0、v0.5.1
- 将界面全部组件的 UI 变得更现代，并调整了界面配色
- 添加了 `Cancel` 按钮，用于关闭界面

### Version 0.4.5
累积更新 v0.4.3 ~ v0.4.5
- 重写并优化了 `SlideButton.h` 和 `SlideButton.cpp`，提供了组件接口，能够丝滑地交互滑动按钮组件
- 利用 QSS 将确认按钮的 UI 进行了优化，并在 `main.cpp` 中调用，将主界面改为圆角，隐藏了系统窗口，使得界面看起来更加现代
- 设计了图标文件

### Version 0.4.2
累积更新 v0.4.0 ~ v0.4.2，当前版本尚未加入主要功能，且无法正确播放动画！
- 加入了 `SlideButton` 自定义组件，编写了配套的 H 文件和 CPP 文件
- 微调了 `CMakeLists.txt` 中的项目架构设置
- 完成了 UI 初步设计，但仍需调整
- 用直接修改文字的方式加入了中英文切换功能
- 图标文件已准备好，但尚未配置

### Version 0.3.0 - After Initial Commit
开启了 `develop` 分支和 `main` 分支，后者仅用于重大更新的同步。
- 基本框架已完成
- 暂未设计 UI 界面