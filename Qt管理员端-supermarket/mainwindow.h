#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include "tcpwidget.h"
#include "menwidget.h"
#include "goodswidget.h"
#include <QLabel>

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
    QTabWidget *tabWidget;
    TcpWidget *tcpWidget;
    MenWidget *menWidget;
    GoodsWidget *goodsWidget;

public slots:
    void tcpClient_received_card(QString str);
    void tcpClient_received_memlist(QString str);
    void tcpClient_received_goodslist(QString str);
};
#endif // MAINWINDOW_H
