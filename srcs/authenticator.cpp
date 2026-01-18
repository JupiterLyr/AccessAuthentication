#include "authenticator.h"
#include "encoder.h"
#include <QDataStream>
#include <QFile>

Authenticator::Authenticator(const QString& binPath, QObject* parent): QObject(parent) {
    loadBinFile(binPath);
}

void Authenticator::loadBinFile(const QString& binPath){
    QFile file(binPath);
    if (!file.open(QIODevice::ReadOnly)){
        emit autFailed(
            "Unable to load authentication data file!\n"
            "无法加载认证数据文件！"
        );
        return;
    }
    QDataStream stream(&file);
    stream.setVersion(QDataStream::Qt_6_0);
    quint32 tagCount;
    stream >> tagCount;
    for (quint32 i = 0; i < tagCount; ++i) {
        quint32 strLen;
        QString identifier, passwordHash, folderPath;
        stream >> strLen;
        QByteArray idData = file.read(strLen);
        identifier = QString::fromUtf8(idData);
        stream >> strLen;
        QByteArray hashData = file.read(strLen);
        passwordHash = QString::fromLatin1(hashData);
        stream >> strLen;
        QByteArray pathData = file.read(strLen);
        folderPath = QString::fromUtf8(pathData);

        if (!identifier.isEmpty() && !passwordHash.isEmpty())
            m_tagData[identifier] = qMakePair(passwordHash, folderPath);
    }
}

void Authenticator::authenticate(const QString& identifier, const QString& password) {
    if (identifier.isEmpty() || password.isEmpty()) {
        emit autFailed("Identifier and password cannot be empty!\n标识符和密码不能为空！");
        return;
    }
    auto it = m_tagData.find(identifier);
    if (it == m_tagData.end()) {
        emit autFailed("Identifier does not exist!\n标识符不存在！");
        return;
    }
    QString inputHash = password_encode(password);
    if (it.value().first != inputHash) {
        emit autFailed("Incorrect password!\n密码错误！");
        return;
    }
    emit autSuccess(it.value().second);
}