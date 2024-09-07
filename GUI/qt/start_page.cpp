

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
    firstPage = new QWidget(this);

    urlLineEdit = new QLineEdit(server_info.url.c_str(), this);
    modelLineEdit = new QLineEdit(server_info.model.c_str(), this);
    apiKeyLineEdit = new QLineEdit(server_info.apiKey.c_str(), this);
    defaultButton = new QPushButton("默认🔄", this);
    testButton = new QPushButton("测试🔍", this);
    startButton = new QPushButton("开始🌟", this);
    textEdit = new QTextEdit(this);

    // 页面1布局
    QVBoxLayout *firstPageLayout = new QVBoxLayout(centralWidget());

    QHBoxLayout *inputLayout_1 = new QHBoxLayout();
    inputLayout_1->addWidget(new QLabel("URL: ", this));
    inputLayout_1->addWidget(urlLineEdit);
    firstPageLayout->addLayout(inputLayout_1);

    QHBoxLayout *inputLayout_2 = new QHBoxLayout();
    inputLayout_2->addWidget(new QLabel("Model: ", this));
    inputLayout_2->addWidget(modelLineEdit);
    firstPageLayout->addLayout(inputLayout_2);

    QHBoxLayout *inputLayout_3 = new QHBoxLayout();
    inputLayout_3->addWidget(new QLabel("API Key: ", this));
    inputLayout_3->addWidget(apiKeyLineEdit);
    firstPageLayout->addLayout(inputLayout_3);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(defaultButton);
    buttonLayout->addWidget(testButton);
    buttonLayout->addWidget(startButton);
    firstPageLayout->addLayout(buttonLayout);
    firstPageLayout->addWidget(textEdit);

    firstPage->setLayout(firstPageLayout);

    // 设置主窗口的布局
    setCentralWidget(firstPage);

    connect(testButton, &QPushButton::clicked, this, [this]() {
        textEdit->clear();
        textEdit->append("Please wait ...");

        HttpManager::InitHttpManager(urlLineEdit->text(), apiKeyLineEdit->text(), modelLineEdit->text(), 180000, 3);

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
            textEdit->clear();
            textEdit->append(info.msg.c_str());
        }
    });

    connect(startButton, &QPushButton::clicked, this, [this]() {
        HttpManager::InitHttpManager(urlLineEdit->text(), apiKeyLineEdit->text(), modelLineEdit->text(), 180000, 3);

        // 开始按钮点击后的操作
        // 切换到第二个页面
        StateManager::getInstance().ShowPage = 1;

        ServerInfo newServerInfo;
        newServerInfo.url = urlLineEdit->text().toStdString();
        newServerInfo.apiKey = apiKeyLineEdit->text().toStdString();
        newServerInfo.model = modelLineEdit->text().toStdString();

        ConfigManager::getInstance().SetServerIP(newServerInfo);
    });

    connect(defaultButton, &QPushButton::clicked, this, [this]() {
        ServerInfo defaultServerInfo = ConfigManager::getInstance().DefaultGetServerIP();

        urlLineEdit->setText(defaultServerInfo.url.c_str());
        apiKeyLineEdit->setText(defaultServerInfo.apiKey.c_str());
        modelLineEdit->setText(defaultServerInfo.model.c_str());
    });
}
