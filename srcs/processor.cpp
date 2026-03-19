#include "processor.h"

Processor::Processor(QObject* parent) : QObject(parent) {
    threadPool.setMaxThreadCount(1);  // 仅允许 1 个子线程工作
}

void Processor::cancelTask() {
    QMutexLocker locker(&mutex);
    if (currentTask)
        currentTask->requestCancel();
}

void Processor::protectFolder(QString folderPath, QString dbPath) {
    ProtectTask* task = new ProtectTask(folderPath, dbPath);
    {
        QMutexLocker locker(&mutex);
        currentTask = task;
    }
    connect(task, &ProtectTask::progress, this, &Processor::protectProgress);
    connect(task, &ProtectTask::report, this, &Processor::reportReceived);
    connect(task, &ProtectTask::finished, this, [this, task](int cond, QString msg) {
        {
            QMutexLocker locker(&mutex);
            currentTask = nullptr;
        }
        emit protectFinished(cond, msg);
        task->deleteLater();
        });
    threadPool.start(task);
}

void Processor::restoreFolder(QString dbPath, QString folderPath) {
    RestoreTask* task = new RestoreTask(dbPath, folderPath);
    {
        QMutexLocker locker(&mutex);
        currentTask = task;
    }    
    connect(task, &RestoreTask::progress, this, &Processor::restoreProgress);
    connect(task, &RestoreTask::report, this, &Processor::reportReceived);
    connect(task, &RestoreTask::finished, this, [this, task](int cond, QString msg) {
        {
            QMutexLocker locker(&mutex);
            currentTask = nullptr;
        }        
        emit restoreFinished(cond, msg);
        task->deleteLater();
        });
    threadPool.start(task);
}