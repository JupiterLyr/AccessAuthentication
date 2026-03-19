#ifndef ERROR_CODE_H
#define ERROR_CODE_H
#include <QString>
#pragma once

enum TaskResult {
    TASK_OK = 0,
    TASK_CANCEL = 1,

    // ===== ERROR =====
    TASK_FILE_OPEN_ERROR = 11,      // cannot open the file
    TASK_FILE_READ_FAILED = 12,     // some files cannot be read
    TASK_FILE_DELETE_FAILED = 13,   // some files cannot be deleted
    TASK_FILE_WRITE_FAILED = 14,    // cannot write files after creating the folder
    TASK_FOLDER_CREATE_FAILED = 15, // cannot create the folder

    TASK_DATA_ILLEGAL = 21,         // data in the file is illegal

    TASK_VERIFY_MAGIC = 51,         // magic text verification error
    TASK_VERIFY_VERSION = 52,       // inconsistent versions
    TASK_VERIFY_PATH_LEN = 53,      // path length mismatching in database

    // ===== WARNING =====
    TASK_WARN_DB_REMAIN = 71,       // database file is not deleted properly
    TASK_WARN_EXTRACT_REMAIN = 72   // extraction files are not deleted properly
};

QString codemsg(int code);

#endif