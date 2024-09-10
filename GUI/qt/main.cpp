/*
 * @Date: 2024-08-28 15:21:35
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-10 09:38:02
 * @FilePath: /CppLLMTranslate/GUI/qt/main.cpp
 */

#include <QApplication>
#include <QTimer>

#include "StateManager.h"

#include "start_page.h"
#include "simple_page.h"

#include "intricate_page.h"
#include "FileTranslation_page.h"

#include <QFontDatabase>
#include <QFont>

#define TITLE_V "CppLLMTranslate_V1.0"

start_page *start_page_w;
SimplePage *simple_page_w;
intricate_page *intricate_page_w;
FileTranslation_page *FileTranslation_page_w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/logo.ico")); // 使用文件系统路径

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
    start_page_w->resize(500, 200);
    start_page_w->hide();
    start_page_w->setWindowTitle(TITLE_V);

    simple_page_w = new SimplePage;
    simple_page_w->resize(500, 300);
    simple_page_w->hide();
    simple_page_w->setWindowTitle(TITLE_V);

    intricate_page_w = new intricate_page;
    intricate_page_w->resize(800, 800);
    intricate_page_w->hide();
    intricate_page_w->setWindowTitle(TITLE_V);

    FileTranslation_page_w = new FileTranslation_page;
    FileTranslation_page_w->resize(800, 800);
    FileTranslation_page_w->hide();
    FileTranslation_page_w->setWindowTitle(TITLE_V);

    // 创建定时器
    QTimer *page_change = new QTimer();

    // 连接定时器的timeout信号到槽函数
    QObject::connect(page_change, &QTimer::timeout, start_page_w, [&]() {
        if (0 == StateManager::getInstance().ShowPage) {
            start_page_w->show();
            simple_page_w->hide();
            intricate_page_w->hide();
            FileTranslation_page_w->hide();
        } else if (1 == StateManager::getInstance().ShowPage) {
            start_page_w->hide();
            simple_page_w->show();
            intricate_page_w->hide();
            FileTranslation_page_w->hide();
        } else if (2 == StateManager::getInstance().ShowPage) {
            start_page_w->hide();
            simple_page_w->hide();
            intricate_page_w->show();
            FileTranslation_page_w->hide();
        } else if (3 == StateManager::getInstance().ShowPage) {
            start_page_w->hide();
            simple_page_w->hide();
            intricate_page_w->hide();
            FileTranslation_page_w->show();
        }

        simple_page_w->UpdateModeComboBox();
        intricate_page_w->UpdateModeComboBox();
        FileTranslation_page_w->UpdateModeComboBox();
    });

    // 启动定时器
    page_change->start(100);

    return a.exec();
}
