#include "customwidget.h"

CustomWidget::CustomWidget(QWidget *parent)
    : QSpinBox(parent)
{

}

void CustomWidget::init()
{
    //设置最值
    this->setRange(0, 255);
    this->m_validator = new QRegExpValidator(QRegExp("0-9A-Fa-f{1, 8}"), this);
}

QValidator::State CustomWidget::validator(QString &text, int &pos) const
{
    return m_validator->validate(text, pos);
}

int CustomWidget::valueFromText(const QString &text) const
{
    bool ok;
    return text.toInt(&ok, 16);
}

QString CustomWidget::textFromValue(int val) const
{
    return QString::number(val, 16).toUpper();
}

