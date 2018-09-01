#include "filetexteditwidget.h"

FileTextEditWidget::FileTextEditWidget(QString filePath, QWidget *parent) : QTextEdit(parent)
  , m_filePath(filePath)
{

}
