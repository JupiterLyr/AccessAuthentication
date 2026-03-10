#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <QObject>
#include <QThread>

#include <QObject>
#include <QThreadPool>

class Processor : public QObject {
    Q_OBJECT

public:
    explicit Processor(QObject* parent = nullptr);

    void protectFolder(QString folderPath, QString dbPath);
    void restoreFolder(QString dbPath, QString folderPath);

signals:
    void protectProgress(int current, int total);
    void restoreProgress(int current, int total);
    void protectFinished(bool success, QString message);
    void restoreFinished(bool success, QString message);

private:
    QThreadPool threadPool;
};

#endif