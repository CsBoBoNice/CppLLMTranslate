

#include "start_page.h"
#include "StateManager.h"
#include "agreement.h"

#include "ConfigManager.h"

#include <QThread>

#include "HttpManager.h"
#include "MessageManager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

start_page::start_page(QWidget *parent) : QMainWindow{parent}
{

    ServerInfo server_info = ConfigManager::getInstance().GetServerIP();

    // 初始化UI
    mainPage = new QWidget(this);

    urlInput = new QLineEdit(server_info.url.c_str(), this);
    modelInput = new QLineEdit(server_info.model.c_str(), this);
    apiKeyInput = new QLineEdit(server_info.apiKey.c_str(), this);
    resetButton = new QPushButton("默认🔄", this);
    testConnectionButton = new QPushButton("测试🔍", this);
    startApplicationButton = new QPushButton("开始🌟", this);
    outputTextEdit = new QTextEdit(this);

    // 页面1布局
    QVBoxLayout *mainPageLayout = new QVBoxLayout(centralWidget());

    QHBoxLayout *inputLayout_1 = new QHBoxLayout();
    inputLayout_1->addWidget(new QLabel("URL: ", this));
    inputLayout_1->addWidget(urlInput);
    mainPageLayout->addLayout(inputLayout_1);

    QHBoxLayout *inputLayout_2 = new QHBoxLayout();
    inputLayout_2->addWidget(new QLabel("Model: ", this));
    inputLayout_2->addWidget(modelInput);
    mainPageLayout->addLayout(inputLayout_2);

    QHBoxLayout *inputLayout_3 = new QHBoxLayout();
    inputLayout_3->addWidget(new QLabel("API Key: ", this));
    inputLayout_3->addWidget(apiKeyInput);
    mainPageLayout->addLayout(inputLayout_3);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(resetButton);
    buttonLayout->addWidget(testConnectionButton);
    buttonLayout->addWidget(startApplicationButton);
    mainPageLayout->addLayout(buttonLayout);
    mainPageLayout->addWidget(outputTextEdit);

    mainPage->setLayout(mainPageLayout);

    // 设置主窗口的布局
    setCentralWidget(mainPage);

    connect(testConnectionButton, &QPushButton::clicked, this, [this]() {
        outputTextEdit->clear();
        outputTextEdit->append("Please wait ...");

        HttpManager::InitHttpManager(urlInput->text(), apiKeyInput->text(), modelInput->text(), 180000, 3);

        agreementInfo info_send;
        // info_send = agreement::getInstance().default_chat();
        info_send.system = "You are a helpful assistant. ";
        info_send.cmd = (int)AgreementCmd::translate_msg;
        info_send.msg = "hi";
        std::string msg_translate = agreement::getInstance().wrapToJson(info_send);

        HttpManager httpManager;
        httpManager.sendRequestJson(msg_translate);

        std::string show_text;
        MessageManager::getInstance().popFromInputQueueNoWait(show_text);
        agreementInfo info = agreement::getInstance().parseJson(show_text);

        if (info.cmd == (int)AgreementCmd::success_msg) {
            outputTextEdit->clear();
            outputTextEdit->append(info.msg.c_str());
        }
    });

    connect(startApplicationButton, &QPushButton::clicked, this, [this]() {
        HttpManager::InitHttpManager(urlInput->text(), apiKeyInput->text(), modelInput->text(), 180000, 3);

        // 开始按钮点击后的操作
        // 切换到第二个页面
        StateManager::getInstance().ShowPage = 1;

        ServerInfo newServerInfo;
        newServerInfo.url = urlInput->text().toStdString();
        newServerInfo.apiKey = apiKeyInput->text().toStdString();
        newServerInfo.model = modelInput->text().toStdString();

        ConfigManager::getInstance().SetServerIP(newServerInfo);
    });

    connect(resetButton, &QPushButton::clicked, this, [this]() {
        ServerInfo defaultServerInfo = ConfigManager::getInstance().DefaultGetServerIP();

        urlInput->setText(defaultServerInfo.url.c_str());
        apiKeyInput->setText(defaultServerInfo.apiKey.c_str());
        modelInput->setText(defaultServerInfo.model.c_str());
    });
}
