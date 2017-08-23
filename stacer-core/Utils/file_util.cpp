#include "file_util.h"

FileUtil::FileUtil()
{

}

QString FileUtil::readStringFromFile(QString path, QIODevice::OpenMode mode)
{
    QSharedPointer<QFile> file(new QFile(path));

    QString data;

    if(file->open(mode)) {

      data = file->readAll();

      file->close();
    }

    return data;
}

QStringList FileUtil::readListFromFile(QString path, QIODevice::OpenMode mode)
{
    QStringList list = FileUtil::readStringFromFile(path, mode).trimmed().split("\n");

    return list;
}

bool FileUtil::writeFile(QString path, QString content, QIODevice::OpenMode mode)
{
    QFile file(path);

    if(file.open(mode))
    {
        QTextStream stream(&file);
        stream << content.toUtf8();

        file.close();

        return true;
    }

    return false;
}

QStringList FileUtil::directoryList(QString path)
{
    QDir dir(path);

    QStringList list;

    foreach (QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files))
        list << info.fileName();

    return list;
}

quint64 FileUtil::getFileSize(QString path)
{
    quint64 totalSize = 0;

    QFileInfo info(path);

    if (info.exists())
    {
        if (info.isFile()) {
            totalSize += info.size();
        }
        else if (info.isDir()) {

            QDir dir(path);

            foreach (QFileInfo i, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs)) {
                totalSize += getFileSize(i.absoluteFilePath());
            }
        }
    }

    return totalSize;
}


