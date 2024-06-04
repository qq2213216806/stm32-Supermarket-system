#ifndef MENWIDGET_H
#define MENWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QHeaderView>
class MenWidget : public QWidget
{
   Q_OBJECT
public:
    MenWidget(QWidget *parent= nullptr);
    ~MenWidget();

private:
    QLineEdit *lineEdit[4];
    QFormLayout *formLayout;
    QWidget *widget1;

    //获取卡号
    QPushButton *btn_acquire;
    //上传按钮
    QPushButton *btn_upload;
    //注册按钮
    QPushButton *btn_enroll;
    //查询按钮
    QPushButton *btn_query;
    //充值按钮
    QPushButton *btn_topUP;
    //刷新按钮
    QPushButton *btn_refresh;
    //注销卡
    QPushButton *btn_logout;

    //会员列表显示表格
    QTableView *tableView;
    QStandardItemModel *theModel;
    QItemSelectionModel *theSelectModel;
    //水平布局
    QHBoxLayout *hBoxLayout[3];
    //垂直布局
    QVBoxLayout *vBoxLayout;
    QWidget *widget2;

signals:
    void request_card_singal(void);
    void request_memlist_singal(void);
    void request_save_memlist_singal(QString str);
public slots:
    void btn_acquire_clicked(void);
    void btn_enroll_clicked(void);
    void btn_topUP_clicked(void);
    void btn_logout_clicked(void);
    void btn_query_clicked(void);
    void btn_refresh_clicked(void);
    void btn_upload_clicked(void);
public :
    void  received_card(QString card);
    void  received_memlist(QString menlist);
};

#endif // MENWIDGET_H
