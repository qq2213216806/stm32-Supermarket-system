#include "goodswidget.h"
#include <QMessageBox>
#include <QTextStream>
GoodsWidget::GoodsWidget(QWidget *parent) : QWidget(parent)
{

    //tableview 初始化
    tableView = new QTableView();
    themodel = new QStandardItemModel();
    theSelectModel = new QItemSelectionModel(themodel);
    QStringList labels =QObject::tr("编号,名称,价格(元),品牌,数量").simplified().split(",");
    themodel->setHorizontalHeaderLabels(labels);
    tableView->setModel(themodel);
    tableView->setSelectionModel(theSelectModel);
    tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectItems); // 设置只选择行
    //表格列大小 对于空间自适应 设置之后用户与编程都将不能更改列大小
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



    //按钮初始化
    btn_refresh = new QPushButton(tr("刷新"));
    btn_uplaod = new QPushButton(tr("上传"));
    btn_add = new QPushButton(tr("添加新商品"));
    btn_del = new QPushButton(tr("删除选中的商品"));
    hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(btn_refresh,1);
    hBoxLayout->addWidget(btn_uplaod,1);
    hBoxLayout->addWidget(btn_add,2);
    hBoxLayout->addWidget(btn_del,2);

    //主布局
    vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(tableView);
    vBoxLayout->addLayout(hBoxLayout);

    this->setLayout(vBoxLayout);

    //子窗口初始化并隐藏
    addWidget = new AddWidget();
    addWidget->hide();

    connect(btn_add,SIGNAL(clicked()),this,SLOT(btn_add_clicked()));
    connect(btn_del,SIGNAL(clicked()),this,SLOT(btn_del_clicked()));
    connect(btn_refresh,SIGNAL(clicked()),this,SLOT(btn_refresh_clicked()));
     connect(btn_uplaod,SIGNAL(clicked()),this,SLOT(btn_upload_clicked()));
    connect(addWidget,SIGNAL(goods_add_siganls(QList<QString>)),this,SLOT(goods_add(QList<QString>)));
    connect(addWidget,SIGNAL(request_card_singal()),this,SLOT(goods_request_card()));
}
GoodsWidget::~GoodsWidget()
{

}

//添加新商品
void GoodsWidget::btn_add_clicked()
{

    addWidget->setWindowModality(Qt::ApplicationModal); //阻止父窗口被用户操作
    addWidget->clearlineEdit();
    addWidget->show();
}
//删除商品
void GoodsWidget::btn_del_clicked()
{
    QModelIndex index = theSelectModel->currentIndex();
    themodel->removeRow(index.row());  //删除一行
}

void GoodsWidget::received_card(QString card)
{
    this->addWidget->received_card(card);
}
void GoodsWidget::goods_add(QList<QString> list)
{
    QList<QStandardItem *>itemList;
    QStandardItem *item1 = 0;
    for (int i =0;i<list.count();i++)
    {
        item1 = new QStandardItem(list.at(i));
        itemList<<item1;
    }

    for (int i=0;i<themodel->rowCount();i++)
    {
        item1 = themodel->item(i,0);
        if(item1->text() == itemList[0]->text())
        {
            QMessageBox::warning(this,"警告","商品已经存在");
            return;
        }
    }
    //添加新行
    themodel->insertRow(themodel->rowCount(),itemList);
    //选中最后一列
    QModelIndex index  = themodel->index(themodel->rowCount()-1,0);
    theSelectModel->setCurrentIndex(index,QItemSelectionModel::Select);
}

void GoodsWidget::goods_request_card()
{
    emit request_card_singal();
}
void GoodsWidget::btn_refresh_clicked()
{
    /*删除原表格上的所有数据*/
    themodel->removeRows(0,themodel->rowCount()); //删除所有行

    emit request_goodslist_singal();
}

void GoodsWidget::received_goodslist(QString goodslist)
{
    QTextStream in(&goodslist);
    QString line;
    line = in.readLine();
    while (!line.isNull())
    {
        //qDebug()<<line;
        /*填充会员列表表格*/
        QList<QStandardItem*> itemList;
        QList<QString> str;
        QStandardItem *item;
        str = line.trimmed().split(",");
        for (int i=0;i<5;i++)
        {
            item = new QStandardItem(str[i]);
            itemList<<item;
        }
        themodel->insertRow(themodel->rowCount(),itemList);
        QModelIndex index  = themodel->index(themodel->rowCount()-1,0);
        theSelectModel->setCurrentIndex(index,QItemSelectionModel::Select);

        line = in.readLine();
    }
}

//上传按钮
void GoodsWidget::btn_upload_clicked()
{
    QString goodslist;
    QStandardItem *item = NULL;
    for (int i=0;i<themodel->rowCount();i++)
    {
        for (int j=0;j<5;j++)
        {
            item = themodel->item(i,j);
            goodslist = goodslist + item->text() +',';
        }
        goodslist += '\n';
    }
    emit request_save_goodslist_singal(goodslist);
}


AddWidget::AddWidget(QWidget *parent) : QWidget(parent)
{
    for (int i=0;i<5;i++)
    {
        linEdit[i] = new QLineEdit();
    }
    formLayout = new QFormLayout();
    formLayout->addRow("商品编号:",linEdit[0]);
    formLayout->addRow("商品名称:",linEdit[1]);
    formLayout->addRow("商品价格:",linEdit[2]);
    formLayout->addRow("商品品牌:",linEdit[3]);
    formLayout->addRow("库存数量：",linEdit[4]);
    btn_scan = new QPushButton("扫描商品");
    btn_add = new QPushButton("添加商品");
    btn_clear = new QPushButton("清空");
    hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(btn_add);
    hBoxLayout->addWidget(btn_clear);

    vBoxLayout = new QVBoxLayout();
    vBoxLayout->addLayout(formLayout);
    vBoxLayout->addWidget(btn_scan);
    vBoxLayout->addLayout(hBoxLayout);

    this->setLayout(vBoxLayout);
    this->resize(300,300);
    this->setWindowTitle(tr("添加商品"));

    connect(btn_scan,SIGNAL(clicked()),this,SLOT(btn_scan_clicked()));
    connect(btn_add,SIGNAL(clicked()),this,SLOT(btn_add_clicked()));
     connect(btn_clear,SIGNAL(clicked()),this,SLOT(btn_clear_clicked()));
}
AddWidget::~AddWidget()
{
}
void AddWidget::received_card(QString card)
{
    this->linEdit[0]->clear();
    this->linEdit[0]->setText(card);
    btn_scan->setEnabled(true);
}
void AddWidget::clearlineEdit(void)
{
    for (int i=0;i<5;i++)
    {
        linEdit[i]->clear();
    }
}
void AddWidget::btn_add_clicked()
{
    if(linEdit[0]->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","请输入商品编号");
        linEdit[0]->setFocus();
        return;
    }
    if(linEdit[1]->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","请输入商品名称");
        linEdit[1]->setFocus();
        return;
    }
    if(linEdit[2]->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","请输入商品价格");
        linEdit[2]->setFocus();
        return;
    }
    if(linEdit[3]->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","请输入商品品牌");
        linEdit[3]->setFocus();
        return;
    }
    if(linEdit[4]->text().isEmpty())
    {
        QMessageBox::warning(this,"警告","请输入库存数量");
        linEdit[4]->setFocus();
        return;
    }
    QList<QString> str_list;

    for(int i =0;i<5;i++)
    {
        str_list<<linEdit[i]->text();
    }
    //this->hide();
    emit goods_add_siganls(str_list);
}
void AddWidget::btn_clear_clicked()
{
    clearlineEdit();
}
void AddWidget::btn_scan_clicked()
{
    btn_scan->setEnabled(false);
    emit request_card_singal();
}
