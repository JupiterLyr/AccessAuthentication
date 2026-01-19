#ifndef AUTHENTICATOR_H
#define AUTHENTICATOR_H

#include <QMap>
#include <QObject>
#include <QPair>
#include <QString>

class Authenticator : public QObject {
    Q_OBJECT
public:
    explicit Authenticator(const QString& binPath, QObject* parent = nullptr);

public slots:
    void authenticate(const QString& identifier, const QString& password);
    void searchPath(const QString& identifier);

signals:
    void autSuccess(const QString& folderPath);
    void autFailed(const QString& reason);
    void autPath(const QString& folderPath);

private:
    void loadBinFile(const QString& binPath);
    QMap<QString, QPair<QString, QString>> m_tagData;
};

#endif