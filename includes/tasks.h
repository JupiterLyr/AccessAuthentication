#ifndef TASKS_H
#define TASKS_H

#include <atomic>
#include <QFile>
#include <QRunnable>
#include <QObject>
#include <QString>

class TaskBase : public QObject, public QRunnable {
    Q_OBJECT

public:
    TaskBase() { setAutoDelete(false); }
    void requestCancel() { m_cancel.store(true); }
    bool isCanceled() const { return m_cancel.load(); }

signals:
    void finished(int condition, QString message);
    void report(int type, QString message);

protected:
    void afterCancel(QFile& out);
    void afterCancel(QFile& in, QFile& out);
    bool afterCancel(QFile& in, QFile& out, const QString& outPath);
    std::atomic<bool> m_cancel{ false };  // 原子变量跨线程访问，保证线程安全、内存同步
};

/// @brief 将文件夹保护为 DB 文件
class ProtectTask : public TaskBase {
    Q_OBJECT

public:
    ProtectTask(QString folder, QString db);
    void run() override;

signals:
    void progress(quint64 done, quint64 total);

private:
    QString folderPath;
    QString dbPath;
    int folder2db(const QString& folderPath, const QString& dbPath);
    bool protectDbFile(const QString& filePath);
};

/// @brief 将 DB 文件恢复成文件夹
class RestoreTask : public TaskBase {
    Q_OBJECT

public:
    RestoreTask(QString db, QString folder);
    void run() override;

signals:
    void progress(quint64 done, quint64 total);

private:
    QString dbPath;
    QString folderPath;
    int db2folder(const QString& dbPath, const QString& outputDir);
    bool normalizeDbFile(const QString& filePath);
};

#endif