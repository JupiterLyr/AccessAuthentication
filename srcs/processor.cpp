#include "processor.h"
#include "tasks.h"

Processor::Processor(QObject* parent) : QObject(parent) {
    threadPool.setMaxThreadCount(QThread::idealThreadCount());
}

void Processor::protectFolder(QString folderPath, QString dbPath) {
    ProtectTask* task = new ProtectTask(folderPath, dbPath);
    connect(task, &ProtectTask::finished, this, &Processor::protectFinished);
    threadPool.start(task);
}

void Processor::restoreFolder(QString dbPath, QString folderPath) {
    RestoreTask* task = new RestoreTask(dbPath, folderPath);
    connect(task, &RestoreTask::finished, this, &Processor::restoreFinished);
    threadPool.start(task);
}