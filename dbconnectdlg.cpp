#include "dbconnectdlg.h"
#include <QGridLayout>
#include <QPushButton>
#include <QRegExpValidator>
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>

DBConnectDlg::DBConnectDlg(QWidget *parent) : QDialog(parent)
{
    isCancel = true;


    this->setAutoFillBackground(true);  //设置背景自动填充
    QPalette palette;
    //palette.setBrush(QPalette::Background, QBrush(QPixmap(":/icon/icon.png")));
    this->setPalette(palette);
    //this->setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    //this->setAttribute(Qt::WA_TranslucentBackground);

    this->init();
}

DBConnectDlg::~DBConnectDlg()
{

}

void DBConnectDlg::init()
{
    /*  QString::fromLocal8Bit
     * This is an overloaded function.
    Returns a QString initialized with the 8-bit string str.
    This function was introduced in Qt 5.0.*/
    this->setWindowTitle(QString::fromLocal8Bit("登录：").toUtf8());
    m_labelUser = new QLabel(QString::fromLocal8Bit("用户名："), this);
    m_labelPwd = new QLabel(QString::fromLocal8Bit("密码："), this);
    m_labelDbName = new QLabel(QString::fromLocal8Bit("数据库名称："), this);
    m_labelServerIp = new QLabel(QString::fromLocal8Bit("主机地址："), this);

    m_lineEditUser = new QLineEdit("jage", this);
    m_lineEditPwd = new QLineEdit("283587151", this);
    m_lineEditPwd->setEchoMode(QLineEdit::Password);     //1
    m_lineEditDbName = new QLineEdit("jage", this);
    m_lineEditServerIp = new QLineEdit("192.168.146.166", this);

    //安装事件过滤器
    m_lineEditUser->installEventFilter(this);
    m_lineEditPwd->installEventFilter(this);
    m_lineEditDbName->installEventFilter(this);
    m_lineEditServerIp->installEventFilter(this);

    m_btnLogin = new QPushButton(QString::fromLocal8Bit("确定"), this);
    m_btnCancel = new QPushButton(QString::fromLocal8Bit("取消"), this);

    QGridLayout *layout = new QGridLayout(this);    //1
    layout->addWidget(m_labelUser, 0, 0);
    layout->addWidget(m_lineEditUser, 0, 1);
    layout->addWidget(m_labelPwd, 1, 0);
    layout->addWidget(m_lineEditPwd, 1, 1);
    layout->addWidget(m_labelDbName, 2, 0);
    layout->addWidget(m_lineEditDbName, 2, 1);
    layout->addWidget(m_labelServerIp, 3, 0);
    layout->addWidget(m_lineEditServerIp, 3, 1);
    layout->addWidget(m_btnLogin, 4, 0);
    layout->addWidget(m_btnCancel, 4, 1);

    layout->setColumnStretch(0, 1);     //1
    layout->setColumnStretch(1, 1);     //1

    layout->setMargin(15);      //1
    layout->setSpacing(10);     //1
    layout->setSizeConstraint(QLayout::SetFixedSize);   //1

    //QValidator
    //QRegExp regExp("^[A-Za-z][1-9][0-9]");
    //m_lineEditUser->setValidator(new QRegExpValidator(regExp, this));


    //connect(loginBtn, SIGNAL(clicked()), this, SLOT(loginBtnOnclick()));
    connect(m_btnLogin, &QPushButton::clicked, this, &DBConnectDlg::on_login);
    connect(m_btnCancel, &QPushButton::clicked, this, &DBConnectDlg::on_cancel);

    connect(m_lineEditUser, &QLineEdit::textEdited, this, &DBConnectDlg::on_lineEdit_textChanged);

}

bool DBConnectDlg::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        qDebug() << keyEvent->key();
    }
    return QWidget::eventFilter(obj, event);
}

void DBConnectDlg::on_login()
{
    isCancel = false;
    this->m_user = m_lineEditUser->text();
    this->m_passwd = m_lineEditPwd->text();
    this->m_dbName = m_lineEditDbName->text();
    this->m_host = m_lineEditServerIp->text();

    this->close();
}

void DBConnectDlg::on_cancel()
{
    this->isCancel = true;
    this->close();
}

void DBConnectDlg::on_lineEdit_textChanged()
{
    qDebug() << "text changed.";
    //m_btnLogin->setEnabled(m_lineEditServerIp->hasAcceptableInput());
}
