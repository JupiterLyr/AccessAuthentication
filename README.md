# Access Authentication
## What is this?
The software is designed to protect the access rights of folders that need to be kept secret.
Every time you compile, it will read the identifier and password you set as well as the corresponding accessible address.</br>
Among them, the password will be stored in some encrypted method.
When using, you need to enter the identifier and its correct password to jump out of the corresponding folder window.</br>
Remember to lock the folder that needs to be protected again after using it!

## How to use?
1. Create a TXT file and write the configuration you want to register. Details are in the next section.
2. Run `config_gen.exe` to complete configuration registration, and **delete the TXT file** to avoid information leakage!
3. Run the main software `AccessAuthentication.exe`. Only by entering the correct identifier and password can you access the specified folder.
4. **Click "Protect" to lock the folder** after accessing it. Otherwise, your folder will be unprotected.
5. Click "Cancel" to exit. To move the window, please hold down the upper left corner and drag.

## How to customize the registration?
1. Create a new text file. You can use `tools/reg.txt` as a template to fill in the information according to the format requirements.
2. The content of the TXT file contains an identifier, a password, and a folder path that jumps after verification, which are separated by a symbol `|` with no spaces on both sides, i.e.:</br>Identifier|Password|Folder_Path
    - If the software is used for removable storage media (CD-ROM, USB flash drive, removable hard disk, etc.), please change the drive letter of the "Jump Folder Path" to `Z:` to avoid the drive letter change of this storage media. The software will automatically identify the so-called `Z:` and redirect to the drive letter where the software is located.
    - In the TXT file, the `#` at the beginning of the line is used as a comment.
3. Run `config_gen.exe` and select the TXT file with your configurations.
4. After reading, the number of valid configuration information will be displayed. The configuration file will be generated automatically, and then the main software can be used normally. This software can be installed in multiple locations on your computer, and each Access Authentication Software has its own configuration file.
5. Remember to **delete the TXT file after using it**, so as not to reveal the configuration that you should keep secret.

## Warning
- Every time you read a new TXT file with `config_gen.exe`, the original configuration will be overwritten into a brand new configuration!
- **Don't use it directly for files**! Because its extension will be left behind. Files before protection cannot be cleaned automatically either. It was designed to protect folders (and files in them) rather than files themselves.
- The files on the removable disk **have NOT been tested** yet!

# 访问认证软件
## 说明
软件旨在为保密文件夹的访问权限提供保护措施。每次编译都会针对性地读取您设定的标识符和密码，以及对应的可访问地址。</br>
其中，密码以某种加密方式储存。使用时，需要输入标识符及其正确密码，才能跳转出对应的文件夹窗口。</br>
使用完毕后，记得重新给需要保护的文件夹上锁！

## 使用方法
1. 首先创建一个 TXT 文件，写入想注册的配置，具体步骤在下一节。
2. 运行 `config_gen.exe` 完成配置注册，并**删除 TXT 文件**，以免泄漏信息！
3. 运行主软件 `AccessAuthentication.exe`，只有输入正确的标识符和密码，点击 “确定” 才能访问指定的文件夹。
4. 文件夹访问完毕后，再**点击 “保护” 为文件夹上锁**，否则你的文件夹将处于未保护状态。
5. 退出软件请点击 “取消”，挪动窗口请按住左上角拖动。

## 定制化注册配置的方法
1. 新建一个文本文档（TXT文件），可以使用 `tools/reg.txt` 作为模板，根据格式要求填写信息。
2. TXT 文件的内容包含标识符、密码、验证后跳转的文件夹路径，两两用符号 `|` 分隔，注意符号两侧不加空格，即：</br>标识符|密码|文件夹路径
    - 若软件用于可移动存储介质（光盘、U盘、移动硬盘等），请将“跳转文件夹路径”的盘符改为 `Z:`，避免盘符更改导致不可用。软件会自动识别所谓的Z盘，并重定向至软件所在盘符。
    - 在 TXT 文件中，行首的 `#` 可用于注释。
3. 运行 `config_gen.exe`，选择包含配置的 TXT 文件。
4. 文件读取后，会显示有效配置信息的数量，配置文件会自动生成，此后即可正常使用主软件。你可以在计算机的多个位置安装本软件，每一个访问认证软件都独享自己的配置文件。
5. 完成后**请删除 TXT 文件**，以免泄露本应保密的配置。

## 注意
- 每用 `config_gen.exe` 读取一个新的 TXT 文件，原有的配置都会被覆盖成全新配置！
- **不推荐直接用于文件**！因为其扩展名会被保留，保护前的文件也无法自动清理。软件设计的初衷是保护文件夹（及其中的文件）而非文件本身。
- 目前，尚未对可移动磁盘上的文件进行测试！

---

# Latest Version - 最新版本
This chapter is written in Chinese for record only.
`AccessAuthentication.exe`: main software
`config_gen.exe`: sub-software called configurations generator (配置生成器)

## Version 2.1.0-pre
**重要更新**！完善了多线程的管理机制，规范化了各种错误、校验、警告、中断等返回值，加入了进度条，但尚未配置各种返回值的后续操作，当前版本**仅供预览功能**！
### Added
- 添加了 `TaskBase` 基类，便于获取当前任务以精细化管理
- 添加了 `normalizeDbFile()` 函数，在未成功删除 DB 文件时，用户可以直接手动删除 DB 文件，以免造成额外残留
### Fixed
- 利用 `enum` 增设了规范化管理的返回值 `TaskResult`，为后续管理返回值提供便利
- 调整了部分 UI，将 `Confirm` 和 `Cancel` 改为 `Unlock` 和 `Close`，并在点击 `Protect` 或 `Unlock` 时暂时禁用两按钮，直到相关操作完成
- 优化了信号-槽机制，加入了百分比进度条，实时显示运行进度，但进度条数据显示还有进一步优化空间
### Issue
1. 当前版本在 `mainwindow` 中只是简单接收了来自线程的信号，并没有进行后续处理，在后续的版本中将完善体验
2. 在未来的版本中，可能进一步优化进度条的显示内容和 UI
3. 未来可能会优化多语言的问题，减少干扰信息
4. 为提升用户体验，后续版本可能会针对配置生成器进行升级，可能变成基于 UI 的管理工具。考虑工程量较大，此工期可能较长，敬请期待！