#ifndef FILETEXTEDITWIDGET_H
#define FILETEXTEDITWIDGET_H

#include <QTextEdit>

class FileTextEditWidget : public QTextEdit
{
    Q_OBJECT
public:
    explicit FileTextEditWidget(QString filePath, QWidget *parent = nullptr);
    QString m_filePath;

private:

signals:

public slots:
};

#endif // FILETEXTEDITWIDGET_H
