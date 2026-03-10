#include "processor.h"

Processor::Processor(QObject* parent) : QObject(parent) {
    threadPool.setMaxThreadCount(QThread::idealThreadCount());
}

void Processor::cancelTask() {
    if (currentTask)
        currentTask->requestCancel();
}

void Processor::protectFolder(QString folderPath, QString dbPath) {
    ProtectTask* task = new ProtectTask(folderPath, dbPath);
    currentTask = task;
    connect(task, &ProtectTask::progress, this, &Processor::protectProgress);
    connect(task, &ProtectTask::finished, this, [this, task](int cond, QString msg) {
        currentTask = nullptr;
        emit protectFinished(cond, msg);
        task->deleteLater();
        });
    threadPool.start(task);
}

void Processor::restoreFolder(QString dbPath, QString folderPath) {
    RestoreTask* task = new RestoreTask(dbPath, folderPath);
    currentTask = task;
    connect(task, &RestoreTask::progress, this, &Processor::restoreProgress);
    connect(task, &RestoreTask::finished, this, [this, task](int cond, QString msg) {
        currentTask = nullptr;
        emit restoreFinished(cond, msg);
        task->deleteLater();
        });
    threadPool.start(task);
}