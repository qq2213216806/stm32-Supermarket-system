#ifndef SHOPWIDGET_H
#define SHOPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTimer>
class ShopWidget : public QWidget
{
    Q_OBJECT
public:
    ShopWidget(QWidget *parent = nullptr);
    ~ShopWidget();

private:
    QLabel *tipLabel;
    QLabel *sumLabel;
    QPushButton *btn_scan;
    QPushButton *btn_clear;
    QPushButton *btn_allclear;
    QPushButton *btn_payment;
    QWidget *widget1;
    QTableView *tableView;
    QStandardItemModel *theModel;
    QItemSelectionModel *theSelectModel;
    QVBoxLayout *vBoxLayout;
    QHBoxLayout *hBoxLayout;
    QString goodslist;
    float sum;
    QTimer *timer1;
signals:
    void request_card_singal(void);
    void request_goodslist_singal(void);

public:
    void  received_card(QString card);
    void  received_goodslist(QString str);

public slots:
    void btn_scan_clicked(void);
    void btn_clear_clicked(void);
    void btn_allclear_clicked(void);
    void btn_payment_clicked(void);
    /*timer1 超时槽函数*/
    void timer1_timeout(void);
};

#endif // SHOPWIDGET_H
