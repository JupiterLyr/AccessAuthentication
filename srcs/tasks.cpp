#include "tasks.h"
#include "locker.h"

ProtectTask::ProtectTask(QString folder, QString db) {
    folderPath = folder;
    dbPath = db;
    setAutoDelete(true);
}

void ProtectTask::run() {
    bool ok = folder2db(folderPath, dbPath);
    if (!ok) {
        emit finished(false, "Protect failed");
        return;
    }
    protectDbFile(dbPath);
    emit finished(true, dbPath);
}

RestoreTask::RestoreTask(QString db, QString folder) {
    dbPath = db;
    folderPath = folder;
    setAutoDelete(true);
}

void RestoreTask::run() {
    bool ok = db2folder(dbPath, folderPath);
    if (!ok) {
        emit finished(false, "Restore failed");
        return;
    }
    emit finished(true, folderPath);
}