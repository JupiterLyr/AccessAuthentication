<h1>Help Manual - 帮助</h1>

Language: [English](#how-to-use) | [简体中文](#使用方法)
<br>Click the colored text to jump to the corresponding location.</br>
<br>Last updated on March 20, 2026</br>

---

<br></br>
<h2 id="how-to-use">How to Use</h2>

### Situation 1: I Haven't Customized The Configurations

#### 1. Prepare the Configuration File
- Create a new text file (\*.TXT), you may use `tools/reg.txt` in Github as a template.
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

#### 2. Generate The Configuration
- Run `config_gen.exe`.
- Select the TXT file you just created.
- The program will generate the configuration file and display the number of valid entries. If an error occurs, please revise the TXT file according to the prompt.

**Note:**
- After configuration, **delete the TXT file immediately** to prevent plaintext password leakage!
- Each time you regenerate the configuration, all existing configurations will be overwritten! This behavior may be improved in a future version.
<br></br>

### Situation 2: How to Use It After Configuration

#### Protect the Folder
- Run the main program `AccessAuthentication.exe`.
- Enter the correct identifier, then click "Protect" to lock the folder associated with the identifier and set the encrypted file as system-protected.

#### Access the Protected Folder
- Run the main program `AccessAuthentication.exe`.
- Enter the correct identifier and password, then click "Unlock". Upon successful verification, the corresponding folder will be opened automatically.
- After use, **please click "Protect" again to relock the folder**, otherwise it will remain unprotected.

---
<br></br>

## Interface Description
### Language
- Click or drag the slider below to switch between Simplified Chinese and English.

### Interactive Components
- Click the green "Protect" button to encrypt and protect the folder associated with the identifier.
- Click the yellow "Unlock" button to extract the encrypted files, which requires the existing identifier and a matching password.
- Click the red "Close" button to exit the software.
- Drag the window by holding the icon at the top-left corner.
- If needed, click the `?` in the bottom-right corner to open the help manual. (*No kidding! How'd you find me then?* :-P)
<br></br>

## Notice
1. Before the configuration generator is upgraded, it is recommended to manually record all identifiers. If you forget an identifier, you will need to contact the developer for further assistance.
2. **Not recommended for direct use on individual files**! File extensions will remain unchanged, and original files cannot be automatically cleaned before protection. This software is designed to protect folders (and their contents), not individual files. To protect a file, place it inside a folder instead.
3. When using software on a removable storage device and needing to access encrypted folders on it, not only must the drive letter be set to `Z:`, but you must also **ensure that the software is running from the removable storage device itself**. Because `Z:` essentially redirects to the drive letter where the main software is actually running.
4. After completing the configuration, you may forget to delete the TXT file, which could cause your password to be exposed in plain text!
5. After decrypting and accessing the folder, you might forget to re-protect it. If you need to protect it, be sure to click the yellow button again! Of course, you can also choose not to protect this folder anymore.
6. In the current version, when reconfiguring using a TXT file, **the existing configuration will be overwritten**.
7. In the future, you will be able to download the installation package and install the software on your local disk. If you need to use it on a removable storage device, you can copy the installation directory to an external hard drive or USB flash drive without having to reinstall it.
<br></br>

## Example of Configuration Rules
```txt
        pictures|Abc666|D:\My Photos\2024-01-01
        Flash Disk Files|doNOTopen|Z:\documents
```
Among them, the first item indicates that the identifier is "pictures", the password is "Abc666", and the accessed folder is "D:\My Photos\2024-01-01"; In the second article, `Z:` will be automatically replaced by the drive letter where the software is located, which should be used in removable storage media.

---

Please contact the developer for assistance if you have **indeed forgotten the identifier and password**. Of course, as for whether we can actually recover it, we’ll just have to give it a try.
<br></br>

---

<br></br>
<h2 id="使用方法">使用方法</h2>

<br></br>

### 情况 1：还没有自定义配置

#### 1. 准备配置文件
- 新建一个文本文档（\*.TXT），可以使用该项目在 Github 中的 `tools/reg.txt` 作为模板
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

#### 2. 生成配置
- 运行 `config_gen.exe`
- 选择刚刚创建的 TXT 文件
- 程序会生成配置文件，并提示有效配置数量；若报错，请根据提示修改 TXT 文件内容

**注意**：
- 配置完成后，请**立即删除 TXT 文件**，防止你的明文密码泄露！
- 每次重新生成配置，都会覆盖原有的全部配置！在将来的某个版本中，我们可能会优化配置生成器的操作逻辑
<br></br>

### 情况 2：配置完成后使用

#### 保护文件夹
- 运行主软件 `AccessAuthentication.exe`
- 输入正确的标识符，点击 “保护” 为标识符所对应的文件夹上锁，并将加密文件设为受系统保护

#### 访问受保护的文件夹
- 运行主软件 `AccessAuthentication.exe`
- 输入正确的标识符和密码，点击 “解锁”，验证成功后会自动打开对应的文件夹
- 使用完文件夹后，请**再次点击 “保护” 为文件夹上锁**，否则你的文件夹将处于未保护状态

---
<br></br>

## 界面说明
### 语言
- 点击或拖动下方的滑块，可以在简体中文和英文之间切换

### 交互组件
- 点击绿色的 “保护”，即可为标识符对应文件夹加密保护
- 点击黄色的 “解锁”，即可在标识符存在、密码正确的情况下将加密文件提取出来
- 点击红色的 “关闭” 以退出软件
- 按住左上角的图标拖动窗口
- 必要时，可以点击右下角的 `?` 打开帮助手册（*废话！不然你怎么找到我的* :-P）
<br></br>

## 注意事项
1. 在配置生成器全新升级以前，建议手动记录所有标识符，一旦忘记标识符，您只能联系作者寻求进一步帮助
2. **不推荐直接用于文件**！因为其扩展名会被保留，保护前的文件也无法自动清理。软件设计的初衷是保护文件夹（及其中的文件）而非文件本身。若想保护某个文件，建议移动到文件夹中！
3. 在移动存储媒介中使用软件，且需要访问移动存储媒介内的加密文件夹，不仅配置路径盘符要为 `Z:`，还**必须保证运行的是移动存储媒介内的软件**，因为 `Z:` 本质上是重定向到实际运行的主软件所在盘符
4. 您在进行配置后，可能会忘记删除 TXT 文件，这可能导致密码以明文泄露！
5. 解密文件夹并使用后，您可能会忘记重新保护文件夹，如果需要保护，切记再点击一次黄色按钮！当然，你也可以选择不再保护这个文件夹
6. 当前版本中，重新用 TXT 配置时，会**直接覆盖原有配置**
7. 未来您可以下载安装包，在本地磁盘安装本软件。若有在移动存储媒介上使用的需求，可以复制安装目录到移动硬盘或 U 盘，无需二次安装。
<br></br>

## 配置规则范例
```txt
        pictures|Abc666|D:\我的照片\2024-01-01
        U盘资料|doNOTopen|Z:\documents
```
其中，第一条表示标识符为“pictures”，密码为“Abc666”，验证通过后访问的文件夹为“D:\我的照片\2024-01-01”；第二条中，Z会被自动替换为软件所在盘符，在可移动存储媒介中应采用这种写法。

---

若您确实**已经遗忘了标识符和密码**，请联系开发者帮助你找回。当然，至于到底能否找回，也只有试试看了。<br></br>

## Email
[JupiterLyr](mailto:jupiterlyr@foxmail.com)