#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(800,480);
    this->setWindowTitle("无人超市后台管理系统");
    //TCP 服务 页面
    tcpWidget = new TcpWidget();
    //会员管理页面
    menWidget = new MenWidget();
    //商品管理页面
    goodsWidget = new GoodsWidget();

    //多页面小部件
    tabWidget = new QTabWidget();
    tabWidget->addTab(tcpWidget,"服务器");
    tabWidget->addTab(menWidget,"会员管理");
    tabWidget->addTab(goodsWidget,"商品管理");
    this->setCentralWidget(tabWidget);

    connect(tcpWidget,SIGNAL(recv_card_singal(QString)),this,SLOT(tcpClient_received_card(QString)));
    connect(tcpWidget,SIGNAL(recv_menlist_singal(QString)),this,SLOT(tcpClient_received_memlist(QString)));
    connect(tcpWidget,SIGNAL(recv_goodslist_singal(QString)),this,SLOT(tcpClient_received_goodslist(QString)));
    connect(menWidget,SIGNAL(request_card_singal()),tcpWidget,SLOT(send_request_card()));
    connect(menWidget,SIGNAL(request_memlist_singal()),tcpWidget,SLOT(send_request_memlist()));
    connect(menWidget,SIGNAL(request_save_memlist_singal(QString)),tcpWidget,SLOT(send_request_save_memlist(QString)));

    connect(goodsWidget,SIGNAL(request_card_singal()),tcpWidget,SLOT(send_request_card()));
    connect(goodsWidget,SIGNAL(request_goodslist_singal()),tcpWidget,SLOT(send_request_goodslist()));
    connect(goodsWidget,SIGNAL(request_save_goodslist_singal(QString)),tcpWidget,SLOT(send_request_save_goodslist(QString)));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::tcpClient_received_card(QString card)
{
    if(tabWidget->currentIndex() == 1)
    {
        //当前处于会员管理页面
        this->menWidget->received_card(card);
    }else if(tabWidget->currentIndex() == 2)
    {
        //当前处于商品管理页面
        this->goodsWidget->received_card(card);
    }
}
void MainWindow::tcpClient_received_memlist(QString memlist)
{
    this->menWidget->received_memlist(memlist);
}
void MainWindow::tcpClient_received_goodslist(QString goodslist)
{
    this->goodsWidget->received_goodslist(goodslist);
}
