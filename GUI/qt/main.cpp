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

#include <QFontDatabase>
#include <QFont>

start_page *start_page_w;
simple_page *simple_page_w;
intricate_page *intricate_page_w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 加载自定义字体
    QString font_path = QCoreApplication::applicationDirPath() + "/font.ttf";
    int fontId = QFontDatabase::addApplicationFont(font_path);
    if (fontId != -1) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.empty()) {
            // 使用第一个字体家族名称
            QFont customFont(fontFamilies.at(0));
            customFont.setPointSize(13); // 设置默认字体大小

            a.setFont(customFont);

            QApplication::setFont(customFont);
            // QApplication::setStyle("fusion"); // 设置样式为fusion或其他支持抗锯齿的样式
        }
    }

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
    page_change->start(100);

    return a.exec();
}
