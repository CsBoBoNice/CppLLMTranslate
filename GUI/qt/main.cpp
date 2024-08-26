#include "mainwindow.h"
#include "start_page.h"

#include <QApplication>
#include <QTimer>

#include "StateManager.h"

MainWindow* w;
start_page *start_page;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    //  w=new MainWindow;
    // w->resize(800, 500);
    // w->hide();

    start_page = new class start_page;
    start_page->resize(300,50);
    start_page->show();

    // 创建定时器
    QTimer* page_change = new QTimer();
    page_change->setInterval(100);

    // 连接定时器的timeout信号到槽函数
    QObject::connect(page_change, &QTimer::timeout,start_page, [&]() {
        if(0==StateManager::getInstance().ShowPage)
        {
            start_page->show();
            // w->hide();
        }
        else
        {
            // w->show();
            start_page->hide();
        }

    });


    // 启动定时器
    page_change->start();

    return a.exec();
}
