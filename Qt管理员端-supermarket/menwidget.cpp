#include "menwidget.h"
#include "QMessageBox"
#include <QTextStream>
#include <QDebug>
MenWidget::MenWidget(QWidget *parent) : QWidget(parent)
{
    //输入行初始化
    for(int i=0;i<4;i++)
    {
        lineEdit[i] = new QLineEdit();
    }

    formLayout = new QFormLayout();
    formLayout->addRow("卡号:",lineEdit[0]);
    formLayout->addRow("姓名:",lineEdit[1]);
    formLayout->addRow("电话:",lineEdit[2]);
   // widget1 = new QWidget();
   // widget1->setLayout(formLayout);

    //按钮
    btn_upload = new QPushButton("上传");
    btn_acquire = new QPushButton("获取卡号");
    btn_enroll = new QPushButton("注册");
    btn_query =  new QPushButton("查询");
    btn_topUP = new QPushButton("充值");
    btn_refresh = new QPushButton("刷新");
    btn_logout = new QPushButton("注销卡");

    //会员表格
    tableView = new QTableView();
    theModel = new QStandardItemModel();
    theSelectModel = new QItemSelectionModel(theModel);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //tableView->setShowGrid(true); //显示网格
    QStringList labels =QObject::tr("卡号,姓名,手机,余额").simplified().split(",");
    theModel->setHorizontalHeaderLabels(labels);
    tableView->setModel(theModel);
    tableView->setSelectionModel(theSelectModel);
    //tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // 设置只选择行

    //表格列大小 对于空间自适应 设置之后用户与编程都将不能更改列大小
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    for (int i=0;i<3;i++)
    {
        hBoxLayout[i] = new QHBoxLayout();
    }
    hBoxLayout[0]->addWidget(btn_query);
    hBoxLayout[0]->addWidget(btn_topUP);
    hBoxLayout[0]->addWidget(lineEdit[3]);
   lineEdit[3]->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));

    hBoxLayout[1]->addWidget(btn_refresh);
    hBoxLayout[1]->addWidget(btn_upload);
    hBoxLayout[1]->addWidget(btn_logout);

    hBoxLayout[2]->addWidget(btn_acquire);
    hBoxLayout[2]->addWidget(btn_enroll);
    widget2 = new QWidget();
    widget2->setLayout(hBoxLayout[2]);

    vBoxLayout = new QVBoxLayout();
    vBoxLayout->addLayout(formLayout);

    vBoxLayout->addWidget(widget2,0,Qt::AlignCenter);
    vBoxLayout->addLayout(hBoxLayout[0]);
    vBoxLayout->addWidget(tableView);
    vBoxLayout->addLayout(hBoxLayout[1]);

    this->setLayout(vBoxLayout);

    connect(btn_acquire,SIGNAL(clicked()),this,SLOT(btn_acquire_clicked()));
    connect(btn_enroll,SIGNAL(clicked()),this,SLOT(btn_enroll_clicked()));
    connect(btn_topUP,SIGNAL(clicked()),this,SLOT(btn_topUP_clicked()));
    connect(btn_logout,SIGNAL(clicked()),this,SLOT(btn_logout_clicked()));
    connect(btn_query,SIGNAL(clicked()),this,SLOT(btn_query_clicked()));
    connect(btn_refresh,SIGNAL(clicked()),this,SLOT(btn_refresh_clicked()));
     connect(btn_upload,SIGNAL(clicked()),this,SLOT(btn_upload_clicked()));
}
MenWidget::~MenWidget()
{

}
void MenWidget::received_card(QString card)
{
     this->lineEdit[0]->clear();
     this->lineEdit[0]->setText(card);
     btn_acquire->setEnabled(true);
}
void MenWidget::received_memlist(QString menlist)
{
    //qDebug()<<menlist;
    QTextStream in(&menlist);
    QString line;
    line = in.readLine();
    while (!line.isNull())
    {
        qDebug()<<line;
        /*填充会员列表表格*/
        QList<QStandardItem*> itemList;
        QList<QString> str;
        QStandardItem *item;
        str = line.trimmed().split(",");
        for (int i=0;i<4;i++)
        {
            item = new QStandardItem(str[i]);
            itemList<<item;
        }
        theModel->insertRow(theModel->rowCount(),itemList);
        QModelIndex index  = theModel->index(theModel->rowCount()-1,0);
        theSelectModel->setCurrentIndex(index,QItemSelectionModel::Select);

        line = in.readLine();
    }
}
//注册按钮
void MenWidget::btn_enroll_clicked()
{
    if(lineEdit[0]->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","请输入卡号");
        lineEdit[0]->setFocus();
        return;
    }
    if(lineEdit[1]->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","请输入姓名");
        lineEdit[1]->setFocus();
        return;
    }
    if(lineEdit[2]->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","请入手机号");
        lineEdit[2]->setFocus();
        return;
    }
    QStandardItem* item = 0;

    for(int i=0;i<theModel->rowCount();i++)
    {
        item = theModel->item(i,0);
        if(lineEdit[0]->text() == item->text())
        {
            QMessageBox::warning(this,"警告","注册失败,卡号已经存在");
            lineEdit[0]->setFocus();
            return;
        }
    }


    QList<QStandardItem*> itemList;

    for (int i =0;i<3;i++)
    {
        item = new QStandardItem(lineEdit[i]->text());
        itemList<<item;
    }
    item = new QStandardItem("0");
    itemList<<item;
    theModel->insertRow(theModel->rowCount(),itemList);
    QModelIndex index  = theModel->index(theModel->rowCount()-1,0);
    //注册并选择它
    theSelectModel->setCurrentIndex(index,QItemSelectionModel::Select);
}

//充值按钮
void MenWidget::btn_topUP_clicked()
{
    if(lineEdit[3]->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","请输入需要充值的余额");
        lineEdit[3]->setFocus();
        return;
    }
    QModelIndex index = theSelectModel->currentIndex();
   //  qDebug()<<index.row();
    QStandardItem* item = theModel->item(index.row(),3);

    if(!item->text().isEmpty())
    {
        int vlaue = item->text().toInt() + lineEdit[3]->text().toInt();
        QString str = QString::number(vlaue);

        item = new QStandardItem(str);
        theModel->setItem(index.row(),3,item);
    }
}
//注销卡按钮
void MenWidget::btn_logout_clicked()
{
    QModelIndex index = theSelectModel->currentIndex();
    theModel->removeRow(index.row());  //删除一行

}
//查询按钮
void MenWidget::btn_query_clicked()
{
    QStandardItem* item = 0;

    for(int i=0;i<theModel->rowCount();i++)
    {
        item = theModel->item(i,0);
        if(lineEdit[0]->text() == item->text())
        {
            QModelIndex index  = theModel->index(i,0);
            theSelectModel->setCurrentIndex(index,QItemSelectionModel::Select);
            return;
        }
    }
     QMessageBox::warning(this,"警告","该会员不存在");
}
//获取卡号按钮
void MenWidget::btn_acquire_clicked()
{
    btn_acquire->setEnabled(false);
    emit request_card_singal();
}
//刷新按钮 发送 请求商品列表
void MenWidget::btn_refresh_clicked()
{
    /*删除原表格上的所有数据*/
    theModel->removeRows(0,theModel->rowCount()); //删除所有行

    emit request_memlist_singal();
}
//上传按钮
void MenWidget::btn_upload_clicked()
{
    QString memlist;
    QStandardItem *item = NULL;
    for (int i=0;i<theModel->rowCount();i++)
    {
        for (int j=0;j<4;j++)
        {
            item = theModel->item(i,j);
            memlist = memlist + item->text() +',';
        }
        memlist += '\n';
    }
    qDebug()<<memlist;
    emit request_save_memlist_singal(memlist);
}
