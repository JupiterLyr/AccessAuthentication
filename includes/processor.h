#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QMutex>
#include <QObject>
#include <QThread>
#include <QThreadPool>
#include "tasks.h"

class Processor : public QObject {
    Q_OBJECT

public:
    explicit Processor(QObject* parent = nullptr);

    void protectFolder(QString folderPath, QString dbPath);
    void restoreFolder(QString dbPath, QString folderPath);

public slots:
    void cancelTask();

signals:
    void protectProgress(quint64 done, quint64 total);
    void restoreProgress(quint64 done, quint64 total);
    void protectFinished(int condition, QString message);
    void restoreFinished(int condition, QString message);
    void reportReceived(int type, QString message);

private:
    QThreadPool threadPool;
    TaskBase* currentTask = nullptr;
    mutable QMutex mutex;
};

#endif