#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include "tcpwidget.h"
#include "shopwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TcpWidget *tcpwidget;
    ShopWidget *shopwidget;
    QTabWidget *tabWiget;

public slots:
    void tcpClient_received_card(QString card);
    void tcpClient_received_goodslist(QString goodslist);
};
#endif // MAINWINDOW_H
