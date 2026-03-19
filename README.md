# Local AuthVault
[中文](#本地数据保险柜)  |  [Latest Version](#latest-version)
## I. What is this?
This software is designed to protect access permissions for specified folders. You only need to enter the **identifier** and **password** to access your preconfigured folder.</br>
The identifier, password, and the folder path associated with its identifier must be registered using a *Configuration Generator* tool. All configuration data is stored in encrypted form to enhance security.</br>
After use, remember to lock the protected folder again!

## II. How to use?
It is recommended to follow the steps below:

### 1. Prepare the Configuration File
- Create a new text file (\*.TXT), you may use `tools/reg.txt` as a template.
- Fill in the content using the following format:
```txt
Identifier|Password|Folder_Path
```
E.g.:
```txt
myfile|abc123|D:\SecretFolder
```
- Format Note:
  - Use `|` as a separator. **Do NOT add spaces around it**.
  - Multiple configurations are supported (one per line), but **identifiers must be unique**.
  - Lines starting with `#` are treated as comments.
  - If used on a USB drive or external storage device, change the drive letter in the path to `Z:`. The software will automatically map `Z:` to the actual drive where the program is running.

### 2. Generate The Configuration
- Run `config_gen.exe`.
- Select the TXT file you just created.
- The program will generate the configuration file and display the number of valid entries. If an error occurs, please revise the TXT file according to the prompt.

**Note**:
- After configuration, **delete the TXT file immediately** to prevent plaintext password leakage!
- Each time you regenerate the configuration, all existing configurations will be overwritten! This behavior may be improved in a future version.

### 3. Protect the Folder
- Run the main program `AccessAuthentication.exe`.
- Enter the correct identifier, then click "Protect" to lock the folder associated with the identifier and set the encrypted file as system-protected.

### 4. Access the Protected Folder
- Run the main program `AccessAuthentication.exe`.
- Enter the correct identifier and password, then click "Unlock". Upon successful verification, the corresponding folder will be opened automatically.
- After use, **please click "Protect" again to relock the folder**, otherwise it will remain unprotected.

## III. Tips
- Click "Close" to exit the software.
- You can drag the window by holding the icon at the top-left corner.
- If needed, click the `?` in the bottom-right corner to open the help manual.
- Before the configuration generator is upgraded, it is recommended to manually record all identifiers. If you forget an identifier, you will need to contact the developer for further assistance.

## IV. Common Misuse Cases
- Forgetting to delete the TXT file → leads to password leakage
- Not clicking "Protect" after use → folder becomes exposed (unless you intentionally want it unprotected)
- Configuration overwritten → previous access becomes invalid
- Incorrect path / drive letter change → unable to open folder
- Using `Z:` in configurations on removable storage, but running the software outside the device → no redirection to the intended storage device

## V. Notice
1. In the future, you can install the software via an installer on a local disk. If you need to use it on removable storage, simply copy the installation directory to a USB drive or external disk—no reinstallation is required.
2. **Not recommended for direct use on individual files**! File extensions will remain unchanged, and original files cannot be automatically cleaned before protection. This software is designed to protect folders (and their contents), not individual files. To protect a file, place it inside a folder instead.
3. When using the software on removable storage and configuring paths with `Z:`, you **must run the software from that storage device**, because `Z:` is essentially redirected to the drive where the main program is executed.

---

# 本地数据保险柜
[English](#local-authvault) | [最新版本](#latest-version)
## 一、软件简介
本软件用于保护指定文件夹的访问权限，您只需输入**标识符**和**密码**即可访问您预设好的文件夹。</br>
配置标识符、密码和您想通过此标识符打开的文件夹，需要用子软件注册配置。配置信息会被加密存储，提高安全性。</br>
使用完毕后，记得重新给需要保护的文件夹上锁！

## 二、使用方法
建议按照如下顺序操作：

### 1. 准备配置文件
- 新建一个文本文档（TXT），可以使用 `tools/reg.txt` 作为模板
- 按照下列格式填写内容：
```txt
标识符|密码|文件夹路径
```
例如：
```txt
myfile|abc123|D:\SecretFolder
```
- 格式说明：
  - 使用 `|` 分隔，**不要在该符号两侧添加空格**
  - 支持多行配置，每行一组，但**标识符不得重复**
  - 以 `#` 开头的行会被视为注释
  - 如果在 U 盘或移动硬盘中使用，请将路径盘符改写为 `Z:`，软件会自动将 `Z:` 映射为实际运行的软件所在盘符

### 2. 生成配置
- 运行 `config_gen.exe`
- 选择刚刚创建的 TXT 文件
- 程序会生成配置文件，并提示有效配置数量；若报错，请根据提示修改 TXT 文件内容

**注意**：
- 配置完成后，请**立即删除 TXT 文件**，防止你的明文密码泄露！
- 每次重新生成配置，都会覆盖原有的全部配置！在将来的某个版本中，我们可能会优化配置生成器的操作逻辑

### 3. 保护文件夹
- 运行主软件 `AccessAuthentication.exe`
- 输入正确的标识符，点击 “保护” 为标识符所对应的文件夹上锁，并将加密文件设为受系统保护

### 4. 访问受保护文件夹
- 运行主软件 `AccessAuthentication.exe`
- 输入正确的标识符和密码，点击 “解锁”，验证成功后会自动打开对应的文件夹
- 使用完文件夹后，请**再次点击 “保护” 为文件夹上锁**，否则你的文件夹将处于未保护状态

## 三、使用技巧
- 点击 “关闭” 以退出软件
- 可以按住左上角的图标拖动窗口
- 必要时，可以点击右下角的 `?` 打开帮助手册
- 在配置生成器全新升级以前，建议手动记录所有标识符，一旦忘记标识符，您只能联系作者寻求进一步帮助

## 四、常见错误使用案例
- 忘记删除 TXT → 导致密码泄露
- 使用后未点击 “保护” → 文件夹暴露（当然你也可以故意不再保护这个文件夹）
- 配置被覆盖 → 原有访问失效
- 路径写错 / 盘符变化 → 无法打开文件
- 在 U 盘、移动硬盘等移动存储媒介中配置盘符为 `Z:`，但运行了移动媒介外的软件 → 不会重定向到对应移动存储媒介所在盘符

## 五、注意事项
1. 未来您可以下载安装包，在本地磁盘安装本软件，若有在移动存储媒介上使用的需求，可以复制安装目录到移动硬盘或 U 盘，无需二次安装。
2. **不推荐直接用于文件**！因为其扩展名会被保留，保护前的文件也无法自动清理。软件设计的初衷是保护文件夹（及其中的文件）而非文件本身。若想保护某个文件，建议移动到文件夹中！
3. 在移动存储媒介中使用软件，并配置路径盘符为 `Z:`，则**必须运行移动存储媒介内的软件**，因为 `Z:` 本质上是重定向到实际运行的主软件所在盘符。

---

# Latest Version
`AccessAuthentication.exe`: main software</br>
`config_gen.exe`: sub-software called configurations generator (配置生成器)

## Version 2.2.1
A **stable running version** using multithreading and its error discrimination mechanism!</br>
采用多线程及其错误判别机制的**稳定运行版**！