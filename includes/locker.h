#ifndef LOCKER_H
#define LOCKER_H

#include <QString>

bool folder2db(const QString& folderPath, const QString& dbPath);
bool db2folder(const QString& dbPath, const QString& outputDir);

#endif