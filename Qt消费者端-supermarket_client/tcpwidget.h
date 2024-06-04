#ifndef TCPWIDGET_H
#define TCPWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextBrowser>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QHostInfo>
#include <QLineEdit>
#include <QNetworkInterface>
#include <QDebug>
#include <QTimer>
class TcpWidget : public QWidget
{
    Q_OBJECT
public:
    TcpWidget(QWidget *parent = nullptr);
    ~TcpWidget();
private:
    /* 通信套接字 */
    QTcpSocket *tcpSocket;

    /* 按钮 */
    QPushButton *pushButton[4];

    /* 标签文本 */
    QLabel *label[2];

    /* 水平容器 */
    QWidget *hWidget[3];

    /* 水平布局 */
    QHBoxLayout *hBoxLayout[3];

    /* 垂直容器 */
    QWidget *vWidget;

    /* 垂直布局 */
    QVBoxLayout *vBoxLayout;

    /* 文本浏览框 */
    QTextBrowser *textBrowser;
    /*服务器IP*/
    QLineEdit *IPlineEdit;

    /* 用于选择端口 */
    QSpinBox  *spinBox;

    /* 文本输入框 */
    QLineEdit *lineEdit;

    /* 存储本地的ip列表地址 */
    QList<QHostAddress> IPlist;

    /*定期请求商品列表定时器*/
    QTimer *timer1;

signals:
    void recv_card_singal(QString str);
    void recv_goodslist_singal(QString str);

private slots:
    /* 连接 */
    void toConnect();

    /* 断开连接 */
    void toDisConnect();

    /* 已连接 */
    void connected();

    /* 已断开连接 */
    void disconnected();

    /* 清除文本框时的内容 */
    void clearTextBrowser();

    /* 接收到消息 */
    void receiveMessages();

    /* 发送消息 */
    void sendMessages();

    /* 连接状态改变槽函数 */
    void socketStateChange(QAbstractSocket::SocketState);

    /*timer1 超时槽函数*/
    void timer1_timeout(void);
    /*发送请求商品列表*/
    void send_request_goodslist(void);
    /*发送请求卡号*/
     void send_request_card(void);

};

#endif // TCPWIDGET_H
