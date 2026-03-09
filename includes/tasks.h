#ifndef TASKS_H
#define TASKS_H

#include <QRunnable>
#include <QObject>
#include <QString>

/// @brief 将文件夹保护为 DB 文件
class ProtectTask : public QObject, public QRunnable {
    Q_OBJECT

public:
    ProtectTask(QString folder, QString db);
    void run() override;

signals:
    void finished(bool success, QString message);

private:
    QString folderPath;
    QString dbPath;
};

/// @brief 将 DB 文件恢复成文件夹
class RestoreTask : public QObject, public QRunnable {
    Q_OBJECT

public:
    RestoreTask(QString db, QString folder);
    void run() override;

signals:
    void finished(bool success, QString message);

private:
    QString dbPath;
    QString folderPath;
};

#endif