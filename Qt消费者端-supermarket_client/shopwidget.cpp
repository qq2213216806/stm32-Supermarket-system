#include "shopwidget.h"
#include <QDebug>
#include <QMessageBox>
ShopWidget::ShopWidget(QWidget *parent):QWidget(parent)
{
    //标签
    tipLabel = new QLabel(tr("提示:请扫描商品电子标签"));
    sumLabel = new QLabel(tr("总价:0元"));

    //tableview
    tableView = new QTableView();
    tableView->setShowGrid(false); //不显示网格
    theModel = new QStandardItemModel();
    theSelectModel = new QItemSelectionModel(theModel);
    QStringList labels =QObject::tr("编号,名称,单价(元),品牌,数量").simplified().split(",");
    theModel->setHorizontalHeaderLabels(labels);
    tableView->setModel(theModel);
    tableView->setSelectionModel(theSelectModel);
    tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectItems); // 设置只选择行
    //表格列大小 对于空间自适应 设置之后用户与编程都将不能更改列大小
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //pushbutton
    btn_scan = new QPushButton(tr("开始扫描商品"));
    btn_clear = new QPushButton(tr("删除选中商品"));
    btn_allclear = new QPushButton(tr("删除全部商品"));
    btn_payment = new QPushButton(tr("付款"));

    //按钮水平布局
    hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(btn_scan);
    hBoxLayout->addWidget(btn_clear);
    hBoxLayout->addWidget(btn_allclear);
    hBoxLayout->addWidget(btn_payment);
    widget1 = new QWidget();
    widget1->setLayout(hBoxLayout);

    //垂直布局
    vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(tipLabel,0,Qt::AlignCenter);
    vBoxLayout->addWidget(tableView);
    vBoxLayout->addWidget(sumLabel,0,Qt::AlignRight);
    vBoxLayout->addWidget(widget1,0,Qt::AlignCenter);

    this->setLayout(vBoxLayout);

    timer1 = new QTimer;
    timer1->setInterval(3000);
    sum = 0;
    connect(btn_scan,SIGNAL(clicked()),this,SLOT(btn_scan_clicked()));
    connect(btn_clear,SIGNAL(clicked()),this,SLOT(btn_clear_clicked()));
    connect(btn_allclear,SIGNAL(clicked()),this,SLOT(btn_allclear_clicked()));
    connect(btn_payment,SIGNAL(clicked()),this,SLOT(btn_payment_clicked()));
    connect(timer1,SIGNAL(timeout()),this,SLOT(timer1_timeout()));

}


ShopWidget::~ShopWidget()
{

}

void ShopWidget::received_card(QString card)
{
    /*卡号 与 商品列表配对
     *配对完成显示商品
     *计算总价
     */
    QTextStream in(&goodslist);
    QString line;
    line = in.readLine();
    while (!line.isNull())
    {
        //qDebug()<<line;

        QList<QStandardItem*> itemList;
        QList<QString> str;
        QStandardItem *item;
        str = line.trimmed().split(",");

        if(card == str[0])
        {
            /*计算总价*/
            sum += str[2].toInt();
            QString label_str =  "总价:" + QString::number(sum) +"元";
            sumLabel->setText(label_str);

            /*判断商品是否已经存在数量1*/
            for(int i=0;i<theModel->rowCount();i++)
            {

                item = theModel->item(i,0);
                if(item->text() == card)
                {
                    /*商品已存在*/
                    item = theModel->item(i,4);
                    if(item != nullptr)
                    {
                        int goods_cnt = item->text().toInt() + 1;
                        item = new QStandardItem(QString::number(goods_cnt));
                        theModel->setItem(i,4,item);
                        return;  //立即退出避免执行下面的代码
                    }
                }

            }
            //商品不曾存在
            for (int i=0;i<4;i++)
            {
                item = new QStandardItem(str[i]);
                itemList<<item;
            }

            int goods_cnt =1;
            item = new QStandardItem(QString::number(goods_cnt));
            itemList<<item;
            theModel->insertRow(theModel->rowCount(),itemList);
            QModelIndex index  = theModel->index(theModel->rowCount()-1,0);
            theSelectModel->setCurrentIndex(index,QItemSelectionModel::Select);

        }

        line = in.readLine();
    }
}

void ShopWidget::received_goodslist(QString str)
{
    this->goodslist = str;
    qDebug()<<goodslist;


}
//开始扫描商品
void ShopWidget::btn_scan_clicked()
{
    if(btn_scan->text() == "开始扫描商品")
    {
        timer1->start();
        emit request_goodslist_singal();
        btn_scan->setText("停止扫描");
    }else
    {
        timer1->stop();
        btn_scan->setText("开始扫描商品");
    }
}
/*timer1 超时槽函数*/
void ShopWidget::timer1_timeout()
{
    emit request_card_singal();
}


//删除选中行
void ShopWidget::btn_clear_clicked()
{
    QModelIndex index = theSelectModel->currentIndex();
    QStandardItem *item1 = theModel->item(index.row(),2); //单价
    QStandardItem *item2 = theModel->item(index.row(),4); //数量

    sum = sum - item1->text().toInt()*item2->text().toInt();
    QString label_str =  "总价:" + QString::number(sum) +"元";
    sumLabel->setText(label_str);

    theModel->removeRow(index.row());  //删除一行


}
//全部删除
void ShopWidget::btn_allclear_clicked()
{
    sum = 0;
    QString label_str =  "总价:" + QString::number(sum) +"元";
    sumLabel->setText(label_str);

    theModel->removeRows(0,theModel->rowCount()); //删除所有行

}
//付款
void ShopWidget::btn_payment_clicked()
{
    timer1->stop();
    btn_scan->setText("开始扫描商品");
    /*付款示例*/
    QMessageBox::information(this,"提示","请出示付款码");
}
