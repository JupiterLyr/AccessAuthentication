<h1>Help Manual - 帮助</h1>

Language: [English](#how-to-use) | [简体中文](#使用方法)
<br>Click the colored text to jump to the corresponding location.</br>

---

<br></br>
<h2 id="how-to-use">How to Use</h2>

1. Create a TXT file and write the configuration you want to register. Details are in the [next section](#customize-reg). The software needs to be verified according to the configuration you set.
2. Run `config_gen.exe` to complete configuration registration, and **delete the TXT file** to avoid information leakage!
3. Run the main software `AccessAuthentication.exe`. Only by entering the correct identifier and password can you access the specified folder.
4. **Click "Protect" to lock the folder** after accessing it. Otherwise, your folder will be unprotected.
5. Click "Cancel" to exit. To move the window, please hold down the upper left corner and drag.

<br></br>
<h2 id="customize-reg">How to customize the registration</h2>

1. Create a new text file. The content of the TXT file contains an identifier, a password, and a folder path that jumps after verification, which are separated by a symbol `|` with no spaces on both sides, i.e.:
```
        Identifier|Password|Folder_Path
```
- If the software is used for removable storage media (CD-ROM, USB flash drive, removable hard disk, etc.), please change the drive letter of the "Jump Folder Path" to `Z:` to avoid the drive letter change of this storage media. The software will automatically identify the so-called `Z:` and redirect to the drive letter where the software is located.
- In the TXT file, the `#` at the beginning of the line is used as a comment. **Identifiers must NOT be duplicate**, that is, each folder to be protected can only correspond to different identifiers. If you want an identifier to encrypt and decrypt multiple folders at the same time, you can move these folders to the same new folder.
2. Run `config_gen.exe` and select the TXT file with your configurations.
3. After reading, the number of valid configuration information will be displayed. The configuration file will be generated automatically, and then the main software can be used normally. This software can be installed in multiple locations on your computer, and each Access Authentication Software has its own configuration file.
4. Remember to **delete the TXT file after using it**, so as not to reveal the configuration that you should keep secret.

<br></br>
<h2>TXT File</h2>

1. The separator of the 3 parts of information is a vertical bar `|` Only one data label is written per line, and multiple lines may be included. The standard format is `Identifier|Password|Folder_Path`. The path to the jump folder can be copied directly from the folder path in File Explorer. If the folder is placed in a removable storage medium (CD-ROM, USB flash drive, removable hard disk, etc.), please manually change the drive letter to `Z:`, and the software needs to run on this storage medium. Because `Z:` will be redirected to the drive letter where the software runs. E.g.:
```txt
        pictures|Abc666|D:\My Photos\2024-01-01
        Flash Disk Files|doNOTopen|Z:\documents
```
- Among them, the first item indicates that the identifier is "pictures", the password is "Abc666", and the accessed folder is "D:\My Photos\2024-01-01"; In the second article, `Z:` will be automatically replaced by the drive letter where the software is located, which should be used in removable storage media.

2. Comments are indicated by the `#` sign at the beginning of the line. The configuration generator will not read the contents after it, but it will still be read if it appears in the middle of the line.
3. After running `config_gen.exe`, please delete this TXT file in time to avoid leakage.
4. If the folder is protected and you have forgotten the identifier and password, the data in the folder will be difficult to recover; Therefore, it is recommended that you use the most commonly used password, and the identifier can be recorded somewhere, so that you can access it like a login account when you need it.
5. Please contact the developer if you have **really forgotten the identifier and password**: [JupiterLyr](mailto:jupiterlyr@foxmail.com)

---
<br></br>
<h2 id="使用方法">使用方法</h2>

1. 首先创建一个 TXT 文件，写入想注册的配置，具体步骤见[注册配置方法](#配置方法)。软件需要根据你设定的配置进行验证。
2. 运行 `config_gen.exe` 完成配置注册，并**删除 TXT 文件**，以免泄漏信息！
3. 运行主软件 `AccessAuthentication.exe`，只有输入正确的标识符和密码，点击 “确定” 才能访问指定的文件夹。
4. 文件夹访问完毕后，再**点击 “保护” 为文件夹上锁**，否则你的文件夹将处于未保护状态。
5. 退出软件请点击 “取消”，挪动窗口请按住左上角拖动。

<br></br>
<h2 id="配置方法">定制化注册配置的方法</h2>

1. 新建一个文本文档（TXT文件），每行为一个内容包含标识符、密码、验证后跳转的文件夹路径，两两用符号 `|` 分隔，注意符号两侧不加空格，即：
```
        标识符|密码|文件夹路径
```
- 若软件用于可移动存储介质（光盘、U盘、移动硬盘等），请将“跳转文件夹路径”的盘符改为 `Z:`，避免盘符更改导致不可用。软件会自动识别所谓的Z盘，并重定向至软件所在盘符。
- 在 TXT 文件中，行首的 `#` 可用于注释。**标识符不得重复**，即各个需要保护的文件夹都只能对应互不相同的标识符。如想做到一个标识符可同时加密、解密多个文件夹，您可以将这些文件夹挪到同一个新文件夹中。
2. 运行 `config_gen.exe`，选择包含配置的 TXT 文件。
3. 文件读取后，会显示有效配置信息的数量，配置文件会自动生成，此后即可正常使用主软件。你可以在计算机的多个位置安装本软件，每一个访问认证软件都独享自己的配置文件。
4. 完成后**请删除 TXT 文件**，以免泄露本应保密的配置。

<br></br>
<h2>TXT 文件说明</h2>

1. 三部分信息的分隔符为半角竖线 `|`，每行仅写入 1 个数据标签（可包含多行），其标准格式为：`标识符|密码|跳转路径`。跳转路径可以直接从资源管理器的文件夹路径处复制；若文件夹放置于可移动存储媒介（光盘、U盘、移动硬盘等）中，请将盘符手动改为 `Z:`，同时该软件需要在该存储媒介上运行，因为 `Z:` 会重定向至软件运行目录所在盘符。例如：
```txt
        pictures|Abc666|D:\我的照片\2024-01-01
        U盘资料|doNOTopen|Z:\documents
```
- 其中，第一条表示标识符为“pictures”，密码为“Abc666”，验证通过后访问的文件夹为“D:\我的照片\2024-01-01”；第二条中，Z会被自动替换为软件所在盘符，在可移动存储媒介中应采用这种写法。

2. 行首的 # 号表示注释，配置生成器不会读取行首井号后的内容；但在行的中间出现井号依旧会被读取。
3. 在运行 `config_gen.exe` 后，请及时删除这个 TXT 文件，以免造成泄漏。
4. 若文件夹已被保护，且您已遗忘标识符和密码，则文件夹内的数据将难以恢复；因此建议您采用最常用的密码，而标识符可以自行记录在某处，需要取用时就像登录账号一样即可访问。
5. 若您确实**已经遗忘了标识符和密码**，请联系开发者：[JupiterLyr](mailto:jupiterlyr@foxmail.com)