#include "error_code.h"
#include <QObject>

QString codemsg(int code) {
    switch (code) { // TASK_OK 不属于错误码，应在外部提前处理
    case TASK_CANCEL:
        return QObject::tr("Operation canceled.\n操作已取消。");
    case TASK_FILE_OPEN_ERROR:
        return QObject::tr(
            "Cannot open source files (E.g., one of large files, the encrypted data file, etc.)! "
            "Please confirm whether the storage space is sufficient.\n"
            "无法打开源文件（如某大文件、加密数据文件等）！请确认存储空间是否充足。"
        );
    case TASK_FILE_READ_FAILED:
        return QObject::tr(
            "Some files in the folder cannot be read! "
            "Please uncheck the \"read-only\" option in the corresponding file permissions.\n"
            "文件夹中部分文件无法读取数据！请取消勾选对应文件权限中的“只读”复选框。"
        );
    case TASK_FILE_DELETE_FAILED:
        return QObject::tr(
            "Some files in the folder were not deleted correctly. "
            "Please make sure you have closed the relevant files and then manually delete them.\n"
            "文件夹中，部分文件未能正确删除。请确保您已关闭并手动删除残留文件，以免泄漏信息。"
        );
    case TASK_FILE_WRITE_FAILED:
        return QObject::tr("Cannot write files after creating the folder!\n文件夹已创建，但无法写入文件！");
    case TASK_FOLDER_CREATE_FAILED:
        return QObject::tr("The folder cannot be created at the preset location!\n无法在预设位置创建文件夹！");
    case TASK_DATA_ILLEGAL:
        return QObject::tr(
            "The original data is illegal! The file cannot be extracted correctly, "
            "possibly due to an abnormal encryption process.\n"
            "原始数据非法！文件无法正确提取，可能因加密过程异常所致！"
        );
    case TASK_VERIFY_MAGIC:
        return QObject::tr(
            "This file is not the encrypted database of this software.\n"
            "该文件不是本软件的加密数据库。"
        );
    case TASK_VERIFY_VERSION:
        return QObject::tr("Current version is incompatible!\n当前版本不兼容！");
    case TASK_VERIFY_PATH_LEN:
        return QObject::tr("Original data verification error!\n原始数据校验错误！");
    case TASK_WARN_DB_REMAIN:
        return QObject::tr(
            "The encrypted database failed to be deleted correctly. Remember to delete it manually.\n"
            "加密数据库文件未能正确删除，请手动删除此文件。"
        );
    case TASK_WARN_EXTRACT_REMAIN:
        return QObject::tr(
            "Operation canceled. However, the extraction files are not deleted properly.\n"
            "操作已取消，但提取的文件未能完全删除。"
        );
    default:
        return QObject::tr("Unknown Error! Code: %1").arg(code);
    }
}