/*
 * @Date: 2024-08-28 15:21:35
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-28 15:21:55
 * @FilePath: /CppLLMTranslate/GUI/qt/main.cpp
 */
#include "mainwindow.h"

#include <QApplication>
#include <QTimer>

#include "StateManager.h"

#include "start_page.h"
#include "simple_page.h"


#include "intricate_page.h"


start_page *start_page_w;
simple_page *simple_page_w;
intricate_page *intricate_page_w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    start_page_w = new start_page;
    start_page_w->resize(350,50);
    start_page_w->hide();
    start_page_w->setWindowTitle("CppLLMTranslate");

    simple_page_w = new simple_page;
    simple_page_w->resize(500,300);
    simple_page_w->hide();
    simple_page_w->setWindowTitle("CppLLMTranslate");

    intricate_page_w = new intricate_page;
    intricate_page_w->resize(800,1000);
    intricate_page_w->hide();
    intricate_page_w->setWindowTitle("CppLLMTranslate");

    // 创建定时器
    QTimer* page_change = new QTimer();
    page_change->setInterval(100);

    // 连接定时器的timeout信号到槽函数
    QObject::connect(page_change, &QTimer::timeout,start_page_w, [&]() {
        if(0==StateManager::getInstance().ShowPage)
        {
            start_page_w->show();
            simple_page_w->hide();
            intricate_page_w->hide();
        }
        else if(1==StateManager::getInstance().ShowPage)
        {
            start_page_w->hide();
            simple_page_w->show();
            intricate_page_w->hide();
        }
        else if(2==StateManager::getInstance().ShowPage)
        {
            start_page_w->hide();
            simple_page_w->hide();
            intricate_page_w->show();
        }


    });


    // 启动定时器
    page_change->start();

    return a.exec();
}
