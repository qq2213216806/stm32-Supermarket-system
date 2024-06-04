#include "LoginWidget.h"
#include <QGraphicsDropShadowEffect>
#include <QStyle>
#include <QMessageBox>
LoginWidget::LoginWidget(QWidget *parent): QDialog(parent)
{
    this->resize(800,480);
    this->setWindowTitle("无人超市管理员端");
    //标签设置
    label1 = new QLabel();
    label1->setText(tr("欢迎登录"));
    //label1->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    QFont font;
    font.setPixelSize(16);
    label1->setFont(font);

    label2 = new QLabel();
    QPixmap pix(":/images/login.jpg");
    pix = pix.scaled(100,200);
    label2->setPixmap(pix);


    //lineEdit 设置
    UserLine = new QLineEdit();
    PwdLine = new QLineEdit();
    PwdLine->setEchoMode(QLineEdit::Password); //密码开启隐藏模式
    formLayou = new QFormLayout();
    formLayou->addRow("账号:",UserLine);
    formLayou->addRow("密码:",PwdLine);
    widget1 = new QWidget();
    widget1->setLayout(formLayou);

    //pushbutton 设置
    loginPushButton = new QPushButton("登录");
    closeToolButton = new QToolButton();
    QPixmap closePix = style()->standardPixmap(QStyle::SP_TitleBarCloseButton);
    closeToolButton->setIcon(closePix);
    closeToolButton->setToolTip(tr("关闭"));

    //布局设置
    vBoxLayout = new QVBoxLayout();
    vBoxLayout->addWidget(closeToolButton,1,Qt::AlignRight);
    vBoxLayout->addWidget(label1,1,Qt::AlignCenter|Qt::AlignBottom);
    vBoxLayout->addWidget(widget1,3);
    vBoxLayout->addWidget(loginPushButton,1,Qt::AlignTop|Qt::AlignHCenter);
    widget2 = new QWidget();
    widget2->setLayout(vBoxLayout);

    hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(label2);
    hBoxLayout->addWidget(widget2);

    //主窗口设置
    mainWidget = new QWidget(this);
    mainWidget->setLayout(hBoxLayout);
    mainWidget->resize(400,200);
    mainWidget->setGeometry((this->width()-mainWidget->width())/2,(this->height()-mainWidget->height())/2,mainWidget->width(),mainWidget->height());

    setAttribute(Qt::WA_StyledBackground, true);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    QGraphicsDropShadowEffect *shadow =new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setColor(Qt::black);
    shadow->setOffset(0);
    mainWidget->setGraphicsEffect(shadow);
    mainWidget->setObjectName("mainWidget");
    mainWidget->setStyleSheet("QWidget#mainWidget{background-color:rgb(29, 123, 255);\
                         border-radius:15px;}");
    UserLine->setFocus();

    connect(closeToolButton,SIGNAL(clicked()),this,SLOT(closeToolButton_clicked()));
    connect(loginPushButton,SIGNAL(clicked()),this,SLOT(loginPushButton_clicked()));
}
LoginWidget::~LoginWidget()
{

}

void LoginWidget::closeToolButton_clicked()
{
    this->close();
}

void LoginWidget::loginPushButton_clicked()
{
    if(!PwdLine->text().isEmpty())
    {
        if(UserLine->text() == "admin" && PwdLine->text() == "123456")
        {
            QMessageBox::information(this,"成功","登录成功");
            QDialog::accept();

        }else
        {   //信息错误弹出警告
            QMessageBox::warning(this,tr("密码或账号错误！"),tr("请输入正确的信息"),QMessageBox::Ok);
            //清空输入框
            PwdLine->clear();
        }
    }else
    {
        PwdLine->setFocus();
    }
}
//鼠标移动
void LoginWidget::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    //鼠标相对于桌面左上角的位置，鼠标的全局位置
    QPoint y = event->globalPos();
    QPoint x =y-this->z;
    this->move(x);
}
//鼠标点击
void LoginWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    //鼠标相对于桌面左上角的位置，鼠标的全局位置
    QPoint y = event->globalPos();
    //窗口左上角相对于桌面左上角的位置
    QPoint x =this->geometry().topLeft();
    this->z = y-x; //定值
}
//鼠标释放
void LoginWidget::mouseReleaseEvent(QMouseEvent *event)
{

    QWidget::mouseReleaseEvent(event);
    //清空
    this->z=QPoint();
}
