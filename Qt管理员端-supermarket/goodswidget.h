#ifndef GOODSWIDGET_H
#define GOODSWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTableView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QFormLayout>
#include <QItemSelectionModel>

class AddWidget :public QWidget
{
    Q_OBJECT
public:
    explicit AddWidget(QWidget *parent = nullptr);
    ~AddWidget();

private:
    QLineEdit *linEdit[5];
    QPushButton *btn_scan;
    QPushButton *btn_add;
    QPushButton *btn_clear;
    QFormLayout *formLayout;
    QHBoxLayout *hBoxLayout;
    QVBoxLayout *vBoxLayout;

signals:
    void  goods_add_siganls(QList<QString> list);
    void request_card_singal(void);
public:
    void received_card(QString card);
    void clearlineEdit(void);
public slots:
    void btn_add_clicked(void);
    void btn_clear_clicked(void);
    void btn_scan_clicked(void);
};




class GoodsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GoodsWidget(QWidget *parent = nullptr);
    ~GoodsWidget();

private:
    QTableView *tableView;
    QStandardItemModel *themodel;
     QItemSelectionModel *theSelectModel;

    QPushButton *btn_refresh;
    QPushButton *btn_uplaod;
    QPushButton *btn_add;
    QPushButton *btn_del;
    QHBoxLayout *hBoxLayout;
    QVBoxLayout *vBoxLayout;

    AddWidget *addWidget;
signals:
    void request_card_singal(void);
    void request_goodslist_singal(void);
    void request_save_goodslist_singal(QString);
public slots:
    void btn_add_clicked(void);
    void btn_del_clicked(void);
    void btn_refresh_clicked(void);
    void goods_add(QList<QString> list);
    void goods_request_card(void);
    void btn_upload_clicked(void);
public:
    void received_card(QString card);
    void  received_goodslist(QString goodslist);
};
#endif // GOODSWIDGET_H
