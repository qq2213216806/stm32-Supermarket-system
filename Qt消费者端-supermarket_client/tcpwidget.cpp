#include "tcpwidget.h"
#include <QThread>
TcpWidget::TcpWidget(QWidget *parent) : QWidget(parent)
{
    /* tcp套接字 */
    tcpSocket = new QTcpSocket(this);

    /* 开始监听按钮 */
    pushButton[0] = new QPushButton();
    /* 停止监听按钮 */
    pushButton[1] = new QPushButton();
    /* 清空聊天文本按钮 */
    pushButton[2] = new QPushButton();
    /* 发送消息按钮 */
    pushButton[3] = new QPushButton();

    /* 水平布局一 */
    hBoxLayout[0] = new QHBoxLayout();
    /* 水平布局二 */
    hBoxLayout[1] = new QHBoxLayout();
    /* 水平布局三 */
    hBoxLayout[2] = new QHBoxLayout();
    /* 水平布局四 */
    //hBoxLayout[3] = new QHBoxLayout();

    /* 水平容器一 */
    hWidget[0] =  new QWidget();
    /* 水平容器二 */
    hWidget[1] =  new QWidget();
    /* 水平容器三 */
    hWidget[2] =  new QWidget();


    //vWidget = new QWidget();
    vBoxLayout = new QVBoxLayout();

    /* 标签实例化 */
    label[0] = new QLabel();
    label[1] = new QLabel();

    lineEdit = new QLineEdit();
    IPlineEdit = new QLineEdit();
    //comboBox = new QComboBox();
    spinBox = new QSpinBox();
    textBrowser = new QTextBrowser();

    label[0]->setText("服务器地址：");
    label[1]->setText("服务器端口：");

    /* 设置标签根据文本文字大小自适应大小  */
    label[0]->setSizePolicy(QSizePolicy::Fixed,
                            QSizePolicy::Fixed);
    label[1]->setSizePolicy(QSizePolicy::Fixed,
                            QSizePolicy::Fixed);

    /* 设置端口号的范围，注意不要与主机的已使用的端口号冲突 */
    spinBox->setRange(1000, 99999);
    spinBox->setValue(8888);

    pushButton[0]->setText("连接服务器");
    pushButton[1]->setText("断开连接");
    pushButton[2]->setText("清空文本");
    pushButton[3]->setText("发送消息");

    /* 设置停止监听状态不可用 */
    pushButton[1]->setEnabled(false);

    /* 设置输入框默认的文本 */
    lineEdit->setText("无人超市客户消费端");

    /* 水平布局一添加内容 */
    hBoxLayout[0]->addWidget(pushButton[0]);
    hBoxLayout[0]->addWidget(pushButton[1]);
    hBoxLayout[0]->addWidget(pushButton[2]);

    /* 设置水平容器的布局为水平布局一 */
    hWidget[0]->setLayout(hBoxLayout[0]);

    hBoxLayout[1]->addWidget(label[0]);
    hBoxLayout[1]->addWidget(IPlineEdit);
    hBoxLayout[1]->addWidget(label[1]);
    hBoxLayout[1]->addWidget(spinBox);

    /* 设置水平容器的布局为水平布局二 */
    hWidget[1]->setLayout(hBoxLayout[1]);

    /* 水平布局三添加内容 */
    hBoxLayout[2]->addWidget(lineEdit);
    hBoxLayout[2]->addWidget(pushButton[3]);

    /* 设置水平容器三的布局为水平布局一 */
    hWidget[2]->setLayout(hBoxLayout[2]);

    /* 垂直布局添加内容 */
    vBoxLayout->addWidget(textBrowser);
    vBoxLayout->addWidget(hWidget[1]);
    vBoxLayout->addWidget(hWidget[0]);
    vBoxLayout->addWidget(hWidget[2]);

    /* 设置垂直容器的布局为垂直布局 */
    this->setLayout(vBoxLayout);

   // timer1 = new QTimer;
    //timer1->setInterval(10000);

    /* 信号槽连接 */
    connect(pushButton[0], SIGNAL(clicked()),
            this, SLOT(toConnect()));
    connect(pushButton[1], SIGNAL(clicked()),
            this, SLOT(toDisConnect()));
    connect(pushButton[2], SIGNAL(clicked()),
            this, SLOT(clearTextBrowser()));
    connect(pushButton[3], SIGNAL(clicked()),
            this, SLOT(sendMessages()));
    connect(tcpSocket, SIGNAL(connected()),
            this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(disconnected()),
            this, SLOT(disconnected()));
    connect(tcpSocket, SIGNAL(readyRead()),
            this, SLOT(receiveMessages()));
    connect(tcpSocket,
            SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,
            SLOT(socketStateChange(QAbstractSocket::SocketState)));
    //connect(timer1,SIGNAL(timeout()),this,SLOT(timer1_timeout()));
}

TcpWidget::~TcpWidget()
{
}
void TcpWidget::toConnect()
{
    /* 如果连接状态还没有连接 */
    if (tcpSocket->state() != tcpSocket->ConnectedState) {
        /* 指定IP地址和端口连接 */
        tcpSocket->connectToHost(IPlineEdit->text(),
                spinBox->value());

        //QThread::msleep(500);
        //开始定时获取商品列表
        //timer1->start();
    }

}

void TcpWidget::toDisConnect()
{
    /* 断开连接 */
    tcpSocket->disconnectFromHost();

    /* 关闭socket*/
    tcpSocket->close();

    //timer1->stop();
}

void TcpWidget::connected()
{
    /* 显示已经连接 */
    textBrowser->append("已经连上服务端");

    /* 设置按钮与下拉列表框的状态 */
    pushButton[0]->setEnabled(false);
    pushButton[1]->setEnabled(true);
    //comboBox->setEnabled(false);
    IPlineEdit->setEnabled(false);
    spinBox->setEnabled(false);
}

void TcpWidget::disconnected()
{
    /* 显示已经断开连接 */
    textBrowser->append("已经断开服务端");

    /* 设置按钮与下拉列表框的状态  */
    pushButton[1]->setEnabled(false);
    pushButton[0]->setEnabled(true);
    //comboBox->setEnabled(true);
    IPlineEdit->setEnabled(true);
    spinBox->setEnabled(true);
}

/* 清除文本浏览框里的内容 */
void TcpWidget::clearTextBrowser()
{
    /* 清除文本浏览器的内容 */
    textBrowser->clear();
}

/* 客户端接收消息 */
void TcpWidget::receiveMessages()
{
    /* 读取接收到的消息 */
    QByteArray messages = tcpSocket->readAll();
    textBrowser->append("服务端：" + messages);
    QList<QString> str_list = QString(messages).trimmed().split(':');
    /*
    qDebug()<<"mes_list[0]"<<str_list[0];
    qDebug()<<"mes_list[1]"<<str_list[1];
    */
    if(str_list[0] == "卡号")
    {
        emit recv_card_singal(str_list[1]);
    }else if(str_list[0] == "商品列表")
    {
        emit recv_goodslist_singal(str_list[1]);
    }
}

/* 客户端发送消息 */
void TcpWidget::sendMessages()
{
    if(NULL == tcpSocket)
        return;

    if(tcpSocket->state() == tcpSocket->ConnectedState) {
        /* 客户端显示发送的消息 */
        textBrowser->append("客户端：" + lineEdit->text());

        /* 发送消息 */
        tcpSocket->write(lineEdit->text().toUtf8().data());
    }
}

/* 客户端状态改变 */
void TcpWidget::socketStateChange(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        textBrowser->append("scoket状态：UnconnectedState");
        break;
    case QAbstractSocket::ConnectedState:
        textBrowser->append("scoket状态：ConnectedState");
        break;
    case QAbstractSocket::ConnectingState:
        textBrowser->append("scoket状态：ConnectingState");
        break;
    case QAbstractSocket::HostLookupState:
        textBrowser->append("scoket状态：HostLookupState");
        break;
    case QAbstractSocket::ClosingState:
        textBrowser->append("scoket状态：ClosingState");
        break;
    case QAbstractSocket::ListeningState:
        textBrowser->append("scoket状态：ListeningState");
        break;
    case QAbstractSocket::BoundState:
        textBrowser->append("scoket状态：BoundState");
        break;
    default:
        break;
    }
}
void TcpWidget::send_request_goodslist()
{
    QString str = "请求商品列表";
    textBrowser->append("客户端：" + str);
    tcpSocket->write(str.toUtf8().data());
}
void TcpWidget::timer1_timeout()
{
    this->send_request_goodslist();
}
void TcpWidget::send_request_card()
{
    QString str = "请求卡号";
    textBrowser->append("客户端：" + str);
    tcpSocket->write(str.toUtf8().data());
}
