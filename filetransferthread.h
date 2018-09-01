#ifndef FILETRANSFERTHREAD_H
#define FILETRANSFERTHREAD_H

#include <QObject>

class FileTransferThread : public QObject
{
    Q_OBJECT
public:
    explicit FileTransferThread(QObject *parent = nullptr);

signals:

public slots:
    void readData();
};

#endif // FILETRANSFERTHREAD_H
