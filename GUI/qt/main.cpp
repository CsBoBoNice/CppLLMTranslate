/*
 * @Date: 2024-08-28 15:21:35
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-11 11:26:50
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

StartPage *startPageWindow;
SimplePage *simplePageWindow;
IntricatePage *intricatePageWindow;
FileTranslationPage *fileTranslationPageWindow;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/logo.ico")); // 使用文件系统路径

    // 加载自定义字体
    QString fontPath = QCoreApplication::applicationDirPath() + "/font.ttf";
    int fontId = QFontDatabase::addApplicationFont(fontPath);
    if (fontId != -1) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.empty()) {
            // 使用第一个字体家族名称
            QFont customFont(fontFamilies.at(0));
            customFont.setPointSize(13); // 设置默认字体大小

            app.setFont(customFont);

            QApplication::setFont(customFont);
            // QApplication::setStyle("fusion"); // 设置样式为fusion或其他支持抗锯齿的样式
        }
    }

    startPageWindow = new StartPage;
    startPageWindow->resize(500, 200);
    startPageWindow->hide();
    startPageWindow->setWindowTitle(TITLE_V);

    simplePageWindow = new SimplePage;
    simplePageWindow->resize(500, 300);
    simplePageWindow->hide();
    simplePageWindow->setWindowTitle(TITLE_V);

    intricatePageWindow = new IntricatePage;
    intricatePageWindow->resize(800, 800);
    intricatePageWindow->hide();
    intricatePageWindow->setWindowTitle(TITLE_V);

    fileTranslationPageWindow = new FileTranslationPage;
    fileTranslationPageWindow->resize(800, 800);
    fileTranslationPageWindow->hide();
    fileTranslationPageWindow->setWindowTitle(TITLE_V);

    // 创建定时器
    QTimer *pageChangeTimer = new QTimer();

    // 连接定时器的timeout信号到槽函数
    QObject::connect(pageChangeTimer, &QTimer::timeout, startPageWindow, [&]() {
        if (0 == StateManager::getInstance().ShowPage) {
            startPageWindow->show();
            simplePageWindow->hide();
            intricatePageWindow->hide();
            fileTranslationPageWindow->hide();
        } else if (1 == StateManager::getInstance().ShowPage) {
            startPageWindow->hide();
            simplePageWindow->show();
            intricatePageWindow->hide();
            fileTranslationPageWindow->hide();
        } else if (2 == StateManager::getInstance().ShowPage) {
            startPageWindow->hide();
            simplePageWindow->hide();
            intricatePageWindow->show();
            fileTranslationPageWindow->hide();
        } else if (3 == StateManager::getInstance().ShowPage) {
            startPageWindow->hide();
            simplePageWindow->hide();
            intricatePageWindow->hide();
            fileTranslationPageWindow->show();
        }

        simplePageWindow->UpdateModeComboBox();
        intricatePageWindow->UpdateModeComboBox();
        fileTranslationPageWindow->UpdateModeComboBox();
    });

    // 启动定时器
    pageChangeTimer->start(100);

    return app.exec();
}
