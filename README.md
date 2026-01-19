# Access Authentication
## What is this?
The software is designed to protect the access rights of files or folders that need to be kept secret.
Every time you compile, it will read the user name and password you set as well as the corresponding accessible address.</br>
Among them, the password will be stored in some encrypted method.
When using, you need to enter the user name (or identification name) and its correct password to jump out of the corresponding folder window.

## How to use?
1. Create a CSV file. You can use `tools/reg.csv` as a template to fill in the information according to the format requirements.
2. If the CSV file is garbled after opening, please try to open it with Notepad.
3. Run `config_gen.exe`, select the CSV file with just set information and press enter, then the new configuration file should be written.
4. **Delete the CSV file** to avoid information leakage!
5. Run the main software `AccessAuthentication.exe`. Only by entering the correct identifier and password can you access the specified folder.

## Warning
- The files on the removable disk **have NOT been tested** yet!

# 访问认证软件
## 说明
软件旨在为保密文件或文件夹的访问权限提供保护措施。每次编译都会针对性地读取您设定的用户名和密码，以及对应的可访问地址。</br>
其中，密码以某种加密方式储存。使用时，需要输入用户名（或标识名）及其正确密码，才能跳转出对应的文件夹窗口。

## 使用方法
1. 首先创建一个 CSV 文件，可以使用 `tools/reg.csv` 作为模板，根据格式要求填写信息。
2. 若 CSV 文件打开后是乱码，请尝试使用记事本打开。
3. 运行 `config_gen.exe`，选择刚刚写好配置的 CSV 文件并回车，新的配置文件即可写入。
4. **删除 CSV 文件**以免泄漏信息！
5. 运行主软件 `AccessAuthentication.exe`，只有输入正确的标识符和密码，才能访问指定的文件夹。

## 注意
- 目前，尚未对可移动磁盘上的文件进行测试！

---

# Update Logs - 更新日志
This chapter is written in Chinese for record only.

### Vesion 1.1.3
累积更新 v1.1.1 ~ v1.1.3，当前版本**测试未通过**，仅用于备份
- 开发了文件夹打包伪装功能，目前尚未部署
- 在 UI 中增设了 `Protect` 按钮，用于被解包的文件夹重新打包伪装，目前拟采用仅验证标识符的方式
- 修改了验证通过后的实现逻辑
#### To Do List
1. 将 `Protect` 功能部署到软件中
2. 完善解包、打包等功能

### Version 1.1.0
首个可用版本！已发布至 `develop` 和 `main` 分支。
- 提高了 `encoder.h` 的加密算法复杂度
- 配置了验证机制，可以在本地正常使用
#### Issues
- 文件与文件夹的安全性需要由用户自行配置，使用难度大，后续将解决此问题</br>The security of files and folders needs to be configured by users themselves, which is difficult to use. This problem will be solved later.

### Version 1.0.0-beta
测试版，旨在测试该软件各模块能否正常运作。
- 开发了信息验证模块，其中加密验证代码被保护在了未上传的 `encoder.h` 中，目前该方法正处于调试阶段
- 开放了 `config_gen.cpp` 的版本控制机制，其中核心加密算法移动到了 `encoder.h` 中

### Version 0.7.2
累积更新 v0.6.4、v0.7.0 ~ 0.7.2，其中 v0.6.4 优化了背景阴影效果，v0.7.x 则致力于开发新功能。考虑到数据安全性问题，配置生成器的算法暂不开源。The algorithm of "Configuration Generator" is not open source considering that data security needs to be protected.
- GUI 界面部分完工，仅剩 `Confirm` 的功能链接未配置
- 加入了首个用于生成 BIN 文件的**配置生成器**工具，其源文件放置于 `tools` 文件夹下

### Version 0.6.3
累积更新 v0.6.0 ~ v0.6.3。该版本**交付了较完善的 UI 设计，`Confirm` 的操作暂且用 `QMessageBox` 拦截**，同时发布至 `develop` 和 `main` 分支。
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