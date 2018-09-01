#ifndef DBCONNECTDLG_H
#define DBCONNECTDLG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>

class DBConnectDlg : public QDialog
{
    Q_OBJECT
public:
    explicit DBConnectDlg(QWidget *parent = nullptr);
    ~DBConnectDlg();
    bool isCancel;          //flag 是否取消连接
public:

public:
    QString m_user;
    QString m_passwd;
    QString m_dbName;
    QString m_host;

private:
    QLabel *m_labelUser;
    QLabel *m_labelPwd;
    QLabel *m_labelDbName;
    QLabel *m_labelServerIp;

    QLineEdit *m_lineEditUser;
    QLineEdit *m_lineEditPwd;
    QLineEdit *m_lineEditDbName;
    QLineEdit *m_lineEditServerIp;

    QPushButton *m_btnLogin;
    QPushButton *m_btnCancel;
private:
    void init();

protected:
    //[virtual] bool QObject::eventFilter(QObject *watched, QEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
signals:

public slots:
private slots:
    void on_login();                //确定连接的
    void on_cancel();               //取消连接
    void on_lineEdit_textChanged(); //当文本发生改变
};

#endif // DBCONNECTDLG_H
