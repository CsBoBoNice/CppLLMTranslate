#include "mainwindow.h"

#include <QApplication>
#include <QTimer>

#include "StateManager.h"

#include "start_page.h"
#include "simple_page.h"

MainWindow* w;
start_page *start_page_w;
simple_page *simple_page_w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    //  w=new MainWindow;
    // w->resize(800, 500);
    // w->hide();

    start_page_w = new start_page;
    start_page_w->resize(350,50);
    start_page_w->hide();

    simple_page_w = new simple_page;
    simple_page_w->resize(500,300);
    simple_page_w->hide();

    // 创建定时器
    QTimer* page_change = new QTimer();
    page_change->setInterval(100);

    // 连接定时器的timeout信号到槽函数
    QObject::connect(page_change, &QTimer::timeout,start_page_w, [&]() {
        if(0==StateManager::getInstance().ShowPage)
        {
            start_page_w->show();
            simple_page_w->hide();
        }
        else
        {
            start_page_w->hide();
            simple_page_w->show();
        }

    });


    // 启动定时器
    page_change->start();

    return a.exec();
}
