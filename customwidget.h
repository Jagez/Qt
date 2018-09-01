#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QSpinBox>

class CustomWidget : public QSpinBox
{
    Q_OBJECT

public:
    CustomWidget(QWidget *parent = NULL);

protected:
    //重载三个虚函数
    //QAbstractSpinBox调用这个虚函数来确定输入是否有效。pos参数指示字符串中的位置
    //在各种子类中重新实现。
    QValidator::State validator(QString &text, int &pos) const;
    //当旋转框需要将用户输入的文本解释为值时
    int valueFromText(const QString &text) const;
    //给显示给定值的时候被旋转框使用的字符串，
    QString textFromValue(int val) const;

    void init();
private:
    QRegExpValidator *m_validator;
};

#endif // CUSTOMWIDGET_H
