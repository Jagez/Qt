#include "stackewidgetddlg.h"
#include <QHBoxLayout>

stackeWidgetdDlg::stackeWidgetdDlg(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle(tr("堆栈窗口"));
    this->setFixedSize(300, 400);
    this->init();
}

void stackeWidgetdDlg::init()
{
    QWidget *cenWidget = new QWidget(this);
    m_listWid = new QListWidget(cenWidget);
    m_listWid->insertItem(0, tr("体积"));
    m_listWid->insertItem(1, tr("重量"));
    m_listWid->insertItem(2, tr("速度"));
    m_listWid->insertItem(3, tr("22"));
    //只要m_stackWid没有对应的元素就不会有所改变

    m_labelTextOne = new QLabel(tr("list1 text"));
    m_labelTextTwo = new QLabel(tr("list2 text"));
    m_labelTextThree = new QLabel(tr("list3 text"));

    m_stackWid = new QStackedWidget(cenWidget);
    m_stackWid->addWidget(m_labelTextOne);
    m_stackWid->addWidget(m_labelTextTwo);
    m_stackWid->addWidget(m_labelTextThree);

    QHBoxLayout *mainLayout = new QHBoxLayout(cenWidget);
    mainLayout->setMargin(5);
    mainLayout->setSpacing(5);
    mainLayout->addWidget(m_listWid);
    mainLayout->addWidget(m_stackWid, 0, Qt::AlignHCenter);
    mainLayout->setStretchFactor(m_listWid, 1);
    mainLayout->setStretchFactor(m_stackWid, 3);
    connect(m_listWid, &QListWidget::currentRowChanged, m_stackWid, &QStackedWidget::setCurrentIndex);
}
