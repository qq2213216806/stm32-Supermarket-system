#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(800,480);
    this->setWindowTitle("无人超市客户端");
    //服务器管理页面
    tcpwidget = new TcpWidget();
    //购物页面
    shopwidget = new ShopWidget();

    tabWiget = new QTabWidget();
    tabWiget->addTab(tcpwidget,tr("服务器管理"));
    tabWiget->addTab(shopwidget,tr("购物清单"));

    this->setCentralWidget(tabWiget);

    connect(tcpwidget,SIGNAL(recv_card_singal(QString)),this,SLOT(tcpClient_received_card(QString)));
    connect(tcpwidget,SIGNAL(recv_goodslist_singal(QString)),this,SLOT(tcpClient_received_goodslist(QString)));
    connect(shopwidget,SIGNAL(request_card_singal()),tcpwidget,SLOT(send_request_card()));
    connect(shopwidget,SIGNAL(request_goodslist_singal()),tcpwidget,SLOT(send_request_goodslist()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::tcpClient_received_card(QString card)
{
    shopwidget->received_card(card);
}
void MainWindow::tcpClient_received_goodslist(QString goodslist)
{
   shopwidget->received_goodslist(goodslist);
}
