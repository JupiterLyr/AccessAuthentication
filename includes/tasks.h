#ifndef TASKS_H
#define TASKS_H

#include <atomic>
#include <QRunnable>
#include <QObject>
#include <QString>

/// @brief 将文件夹保护为 DB 文件
class ProtectTask : public QObject, public QRunnable {
    Q_OBJECT

public:
    ProtectTask(QString folder, QString db);
    void run() override;
    void requestCancel();
    bool isCanceled() const;

signals:
    void finished(bool success, QString message);
    void progress(int current, int total);

private:
    std::atomic<bool> m_cancel{false};  // 原子变量跨线程访问，保证线程安全、内存同步
    QString folderPath;
    QString dbPath;
    int folder2db(const QString& folderPath, const QString& dbPath);
    bool protectDbFile(const QString& filePath);
};

/// @brief 将 DB 文件恢复成文件夹
class RestoreTask : public QObject, public QRunnable {
    Q_OBJECT

public:
    RestoreTask(QString db, QString folder);
    void run() override;
    void requestCancel();
    bool isCanceled() const;

signals:
    void finished(bool success, QString message);
    void progress(int current, int total);

private:
    std::atomic<bool> m_cancel{false};
    QString dbPath;
    QString folderPath;
    int db2folder(const QString& dbPath, const QString& outputDir);
};

#endif