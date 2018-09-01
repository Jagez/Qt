#include "conversiondlg.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include <QComboBox>


ConversionDlg::ConversionDlg(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle(tr("单位换算"));
    this->setFixedSize(600, 400);
    //初始化
    init();
    //创建Layout
    createLayout();
    //连接信号
    createSignal();


}

void ConversionDlg::addListItem(QString string)
{

}

void ConversionDlg::addListItems(QStringList stringList)
{

}

void ConversionDlg::addComboBoxItem(QString string)
{

}

void ConversionDlg::addComboBoxItems(QStringList list)
{

}

void ConversionDlg::init()
{
    //widget = new QWidget(MainWindow);        //这是窗口的主体内容widget
        m_listWidUnit = new QListWidget(this);
        //m_listWidUnit->setViewMode(QListWidget::IconMode);     //设置为图标模式
        m_listWidUnit->setViewMode(QListWidget::ListMode);       //设置为列表模式（默认）
        m_unitList << "体积" << "质量" << "速度" << "面积";
        //unitTitle = new QLabel("计量单位：", widget);
        m_listWidUnit->addItems(m_unitList);                    //添加List

        m_labelChange = new QLabel(tr("单位换算"), this);
        m_labelMeasureUnitOpt = new QLabel(tr("计量单位"), this);
        m_labelUnitOpt = new QLabel(tr("单位"), this);
        m_lineEditFrom = new QLineEdit(this);
        m_lineEditFrom->setAlignment(Qt::AlignLeft);
        m_lineEditTo = new QLineEdit(this);
        m_lineEditTo->setText("1");
        m_lineEditTo->setEnabled(false);
        m_cBoxFrom = new QComboBox(this);
        m_cBoxTo = new QComboBox(this);
        m_labelSymbol = new QLabel(tr("="), this);

        //通过xml文档
//        m_strListVolume << "加仑" << "夸脱" << "升" << "盎(àng)司";
//        m_strListWeight << "Kg" << "g";
//        m_strListSpeed << "马赫" << "m/s" << "Km/H" ;
//        m_strListArea << "平方米" << "平方分米" << "平方厘米";
        //addComboBoxItem(comboBoxList);
        m_strListVolume << "1" << "2" << "3" << "4";
        m_strListWeight << "1" << "2";
        m_strListSpeed << "1" << "m2s" << "Km/H" ;
        m_strListArea << "1" << "2" << "3";

        m_btnCompute = new QPushButton(tr("计算"), this);

        m_lineEditMeasureUnitOpt = new QLineEdit(this);
        m_lineEditMeasureUnitOpt->setAlignment(Qt::AlignCenter);
        m_btnAddMeasureUnit = new QPushButton(tr("添加"), this);
        m_btnDelMeasureUnit = new QPushButton(tr("删除"), this);

        m_lineEditUnitOpt = new QLineEdit(this);
        m_lineEditUnitOpt->setAlignment(Qt::AlignCenter);
        m_btnAddUnit = new QPushButton(tr("添加"), this);
        m_btnDelUnit = new QPushButton(tr("删除"), this) ;



}

void ConversionDlg::createLayout()
{
    QVBoxLayout *left = new QVBoxLayout;
    left->setMargin(10);
    left->addWidget(m_listWidUnit);

    QHBoxLayout *rightTopLayout = new QHBoxLayout;
    rightTopLayout->setMargin(10);
    rightTopLayout->addWidget(m_labelChange);
    rightTopLayout->addStretch();

    QHBoxLayout *rightLayout = new QHBoxLayout;
    rightLayout->setMargin(10);
    rightLayout->addWidget(m_lineEditFrom);
    rightLayout->addWidget(m_cBoxFrom);
    rightLayout->addWidget(m_labelSymbol);
    rightLayout->addWidget(m_lineEditTo);
    rightLayout->addWidget(m_cBoxTo);
    rightLayout->addStretch();

    QHBoxLayout *rightDownLabelLayout = new QHBoxLayout;
    rightDownLabelLayout->setMargin(10);
    rightDownLabelLayout->addWidget(m_labelMeasureUnitOpt);
    rightDownLabelLayout->addStretch();

    QHBoxLayout *rightDownLayout = new QHBoxLayout;
    rightDownLayout->setMargin(10);
    rightDownLayout->addWidget(m_lineEditMeasureUnitOpt);
    rightDownLayout->addWidget(m_btnAddMeasureUnit);
    rightDownLayout->addWidget(m_btnDelMeasureUnit);
    rightDownLayout->addStretch();

    QHBoxLayout *rightBottomLabelLayout = new QHBoxLayout;
    rightBottomLabelLayout->setMargin(10);
    rightBottomLabelLayout->addWidget(m_labelUnitOpt);
    rightBottomLabelLayout->addStretch();

    QHBoxLayout *rightBottomLayout = new QHBoxLayout;
    rightBottomLayout->setMargin(10);
    rightBottomLayout->addWidget(m_lineEditUnitOpt);
    rightBottomLayout->addWidget(m_btnAddUnit);
    rightBottomLayout->addWidget(m_btnDelUnit);
    rightBottomLayout->addStretch();

    QVBoxLayout *right = new QVBoxLayout;
    right->addLayout(rightTopLayout);
    right->addLayout(rightLayout);
    right->addLayout(rightDownLabelLayout);
    right->addLayout(rightDownLayout);
    right->addLayout(rightBottomLabelLayout);
    right->addLayout(rightBottomLayout);

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setMargin(10);
    mainLayout->setSpacing(10);
    mainLayout->addWidget(m_listWidUnit, 0, 0);
    mainLayout->addLayout(right, 0, 1);

    //此属性保存布局的调整大小模式
    mainLayout->setSizeConstraint(QLayout::SetMinimumSize);
}

void ConversionDlg::createSignal()
{
    /*
     *  这里的函数指针是信号和槽的一个重点，槽函数遇到重载使用:
     *  QOverload<int>::of(&QComboBox::currentIndexChanged)     重载int,QString类型
     *  与SIGNAL(),SLOT()不同，&QListWidget::itemClicked不能带形参！即
     *  &QListWidget::itemClicked(QListWidgetItem*)         X！错误！
     */
    //函数指针
    void (ConversionDlg::*p)(QListWidgetItem*) = &ConversionDlg::on_itemClicked;
    connect(m_listWidUnit, &QListWidget::itemClicked, this, p);
    void (ConversionDlg::*p1)(QListWidgetItem *) = &ConversionDlg::on_itemDoubleClicked;
    connect(m_listWidUnit, &QListWidget::itemDoubleClicked, this, p1);
    connect(m_btnCompute, &QPushButton::clicked, this, &ConversionDlg::on_compute);
    connect(m_btnAddMeasureUnit, &QPushButton::clicked, this, &ConversionDlg::on_addMUnitItem);
    connect(m_btnDelMeasureUnit, &QPushButton::clicked, this, &ConversionDlg::on_delMUnitItem);
    connect(m_btnAddUnit, &QPushButton::clicked, this, &ConversionDlg::on_addUnitItem);
    connect(m_btnDelUnit, &QPushButton::clicked, this, &ConversionDlg::on_delUnitItem);

    //当下拉列表的当前值发生改变时触发，重新计算
    //这里也可以使用函数指针
    //connect(m_cBoxFrom, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){});
    //connect(m_cBoxTo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index){});
    //函数指针方式
    typedef void (QComboBox::*p_combox)(int);
    void (ConversionDlg::*ptextChange)(int) = &ConversionDlg::sel_cBoxFromIndexChange;
    connect(m_cBoxFrom, static_cast<p_combox>(&QComboBox::currentIndexChanged), this, ptextChange);
    connect(m_cBoxTo, static_cast<p_combox>(&QComboBox::currentIndexChanged),
                this, &ConversionDlg::sel_cBoxToIndexChange);
    //QOverload<T>::of 方式
    //connect(m_cBoxTo, QOverload<int>::of(&QComboBox::currentIndexChanged),
    //        this, &ConversionDlg::sel_cBoxToIndexChange);

    //当行文本发生改变时触发,重新计算
    connect(m_lineEditFrom, &QLineEdit::textChanged, this, &ConversionDlg::textChange);
}

void ConversionDlg::on_itemClicked(QListWidgetItem *item)
{
    if (NULL == item)
    {
        return;
    }
    //清除之前的文本
    m_cBoxFrom->clear();
    m_cBoxTo->clear();
    int i = 0;
    for (i = 0; i < m_listWidUnit->count(); i++)
    {

        //判断选中的item是否与字符列表值相同
        if (item->text() == m_unitList.value(i))
        {
            switch (i)
            {
            case 0:
                m_cBoxFrom->addItems(m_strListVolume);
                m_cBoxTo->addItems(m_strListVolume);
                m_lineEditFrom->setFocus();             //光标位置 定焦
                break;
            case 1:
                m_cBoxFrom->addItems(m_strListWeight);
                m_cBoxTo->addItems(m_strListWeight);
                m_lineEditFrom->setFocus();
                break;
            case 2:
                m_cBoxFrom->addItems(m_strListSpeed);
                m_cBoxTo->addItems(m_strListSpeed);
                m_lineEditFrom->setFocus();
                break;
            case 3:
                m_cBoxFrom->addItems(m_strListArea);
                m_cBoxTo->addItems(m_strListArea);
                m_lineEditFrom->setFocus();
                break;
            default:
                m_cBoxFrom->addItems(m_strListVolume);
                m_cBoxTo->addItems(m_strListVolume);
                break;
            }
        }
    }
}

void ConversionDlg::on_itemDoubleClicked(QListWidgetItem *item)
{

}

void ConversionDlg::on_compute()
{

}

void ConversionDlg::on_addMUnitItem()
{

}

void ConversionDlg::on_delMUnitItem()
{

}

void ConversionDlg::on_addUnitItem()
{

}

void ConversionDlg::on_delUnitItem()
{

}

void ConversionDlg::sel_cBoxFromIndexChange(int index)
{
    qDebug() << index;
}

void ConversionDlg::sel_cBoxToIndexChange(int index)
{
    qDebug() << index;
}

void ConversionDlg::textChange()
{

}
