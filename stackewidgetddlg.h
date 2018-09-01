#ifndef STACKEWIDGETDDLG_H
#define STACKEWIDGETDDLG_H

#include <QDialog>
#include <QListWidget>
#include <QStackedWidget>
#include <QLabel>

class stackeWidgetdDlg : public QDialog
{
    Q_OBJECT
public:
    explicit stackeWidgetdDlg(QWidget *parent = nullptr);
private:
    QListWidget *m_listWid;
    QStackedWidget *m_stackWid;
    QLabel *m_labelTextOne;
    QLabel *m_labelTextTwo;
    QLabel *m_labelTextThree;

    void init();
signals:

public slots:
};

#endif // STACKEWIDGETDDLG_H
