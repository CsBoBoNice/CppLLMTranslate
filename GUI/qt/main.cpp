/*
 * @Date: 2024-08-28 15:21:35
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-30 15:21:02
 * @FilePath: /CppLLMTranslate/GUI/qt/main.cpp
 */

// 包含必要的Qt头文件
#include <QApplication>
#include <QTimer>
#include <QIcon>
#include <QFontDatabase>
#include <QFont>
#include <QStringList>

// 包含自定义头文件
#include "StateManager.h"
#include "start_page.h"
#include "simple_page.h"
#include "intricate_page.h"
#include "FileTranslation_page.h"

// 定义应用程序标题
#define TITLE_V "CppLLMTranslate_V1.0"

// 声明全局指针,用于管理不同页面的窗口
StartPage *startPageWindow;
SimplePage *simplePageWindow;
IntricatePage *intricatePageWindow;
FileTranslationPage *fileTranslationPageWindow;

int main(int argc, char *argv[])
{
    // 创建QApplication对象
    QApplication app(argc, argv);
    
    // 设置应用程序图标
    app.setWindowIcon(QIcon(QCoreApplication::applicationDirPath() + "/logo.ico"));

    // 加载自定义字体
    QString fontPath = QCoreApplication::applicationDirPath() + "/font.ttf";
    int fontId = QFontDatabase::addApplicationFont(fontPath);
    if (fontId != -1) {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        if (!fontFamilies.empty()) {
            // 使用第一个字体家族名称创建自定义字体
            QFont customFont(fontFamilies.at(0));
            customFont.setPointSize(13); // 设置默认字体大小

            // 为整个应用程序设置字体
            app.setFont(customFont);
            QApplication::setFont(customFont);
        }
    }

    // 创建并初始化开始页面窗口
    startPageWindow = new StartPage;
    startPageWindow->resize(500, 200);
    startPageWindow->hide();
    startPageWindow->setWindowTitle(TITLE_V);

    // 创建并初始化简单页面窗口
    simplePageWindow = new SimplePage;
    simplePageWindow->resize(500, 300);
    simplePageWindow->hide();
    simplePageWindow->setWindowTitle(TITLE_V);

    // 创建并初始化复杂页面窗口
    intricatePageWindow = new IntricatePage;
    intricatePageWindow->resize(800, 800);
    intricatePageWindow->hide();
    intricatePageWindow->setWindowTitle(TITLE_V);

    // 创建并初始化文件翻译页面窗口
    fileTranslationPageWindow = new FileTranslationPage;
    fileTranslationPageWindow->resize(800, 800);
    fileTranslationPageWindow->hide();
    fileTranslationPageWindow->setWindowTitle(TITLE_V);

    // 创建定时器用于页面切换
    QTimer *pageChangeTimer = new QTimer();

    // 连接定时器的timeout信号到lambda函数
    QObject::connect(pageChangeTimer, &QTimer::timeout, startPageWindow, [&]() {
        // 根据StateManager中的ShowPage值切换显示的页面
        switch (StateManager::getInstance().ShowPage) {
            case 0:
                startPageWindow->show();
                simplePageWindow->hide();
                intricatePageWindow->hide();
                fileTranslationPageWindow->hide();
                break;
            case 1:
                startPageWindow->hide();
                simplePageWindow->show();
                intricatePageWindow->hide();
                fileTranslationPageWindow->hide();
                break;
            case 2:
                startPageWindow->hide();
                simplePageWindow->hide();
                intricatePageWindow->show();
                fileTranslationPageWindow->hide();
                break;
            case 3:
                startPageWindow->hide();
                simplePageWindow->hide();
                intricatePageWindow->hide();
                fileTranslationPageWindow->show();
                break;
        }

        // 更新各页面的模式下拉框
        simplePageWindow->UpdateModeComboBox();
        intricatePageWindow->UpdateModeComboBox();
        fileTranslationPageWindow->UpdateModeComboBox();
    });

    // 启动定时器,每100毫秒触发一次
    pageChangeTimer->start(100);

    // 进入应用程序的主事件循环
    return app.exec();
}
