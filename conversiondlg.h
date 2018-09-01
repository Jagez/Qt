#ifndef CONVERSIONDLG_H
#define CONVERSIONDLG_H

#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>

class ConversionDlg : public QDialog
{
    Q_OBJECT
public:
    explicit ConversionDlg(QWidget *parent = nullptr);

signals:

public:
    void addListItem(QString string);             //添加单位
    void addListItems(QStringList stringList);
    void addComboBoxItem(QString string);         //添加计量单位
    void addComboBoxItems(QStringList list);

private:
    QWidget *m_mainWidget;
    QListWidget *m_listWidUnit;
    QLabel *m_labelUnitTitle;
    QStringList m_unitList;
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;
    QLabel *m_labelSymbol;
    QLabel *m_labelChange;
    QLineEdit *m_lineEditFrom;              //
    QLineEdit *m_lineEditTo;
    QComboBox *m_cBoxFrom;
    QComboBox *m_cBoxTo;
    QPushButton *updateButton;
    QPushButton *m_btnCompute;              //计算
    QLabel *m_labelMeasureUnitOpt;          //计量单位操作
    QLabel *m_labelUnitOpt;                 //单位操作
    QLineEdit *m_lineEditMeasureUnitOpt;
    QLineEdit *m_lineEditUnitOpt;
    QPushButton *m_btnAddMeasureUnit;
    QPushButton *m_btnDelMeasureUnit;
    QPushButton *m_btnAddUnit;
    QPushButton *m_btnDelUnit;
    // QStackedWidget *stack;


    QStringList m_strListVolume;           //体积
    QStringList m_strListWeight;           //重量
    QStringList m_strListSpeed;            //速度
    QStringList m_strListArea;             //面积

private:
    void init();
    void createLayout();
    void createSignal();
private slots:
    void on_itemClicked(QListWidgetItem *item);
    void on_itemDoubleClicked(QListWidgetItem *item);
    void on_compute();
    void on_addMUnitItem();
    void on_delMUnitItem();
    void on_addUnitItem();
    void on_delUnitItem();
    void sel_cBoxFromIndexChange(int index);
    void sel_cBoxToIndexChange(int index);

    void textChange();

};

#endif // CONVERSIONDLG_H
