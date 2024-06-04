#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QToolButton>
#include <QEvent>
#include <QMouseEvent>
#include <QPoint>
class LoginWidget : public QDialog
{
    Q_OBJECT

public:
    LoginWidget(QWidget *parent = 0);
    ~LoginWidget();

private:
    QLabel *label1;
    QLabel *label2;
    QLineEdit *UserLine;
    QLineEdit *PwdLine;
    QHBoxLayout *hBoxLayout;
    QFormLayout *formLayou;
    QVBoxLayout *vBoxLayout;
    QWidget *widget1;
    QWidget *widget2;
    QWidget *mainWidget;
    QPushButton *loginPushButton;
    QToolButton *closeToolButton;
     QPoint z;
public:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

public slots:
    void loginPushButton_clicked(void);
    void closeToolButton_clicked(void);

};

#endif // LOGINWIDGET_H
