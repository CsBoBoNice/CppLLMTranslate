#include "start_page.h"
#include "StateManager.h"
#include "agreement.h"
#include "ConfigManager.h"
#include <QThread>
#include "HttpManager.h"
#include "MessageManager.h"
#include "ModelsInfo.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

StartPage::StartPage(QWidget *parent) : QMainWindow{parent}
{
    ModelsInfo_s serverInfo = ModelsInfo::getInstance().GetServerIP();

    // Initialize UI
    m_mainPage = new QWidget(this);

    m_urlInput = new QLineEdit(serverInfo.url.c_str(), this);
    m_modelInput = new QLineEdit(serverInfo.model.c_str(), this);
    m_apiKeyInput = new QLineEdit(serverInfo.apiKey.c_str(), this);
    m_resetButton = new QPushButton("默认🔄", this);
    m_testConnectionButton = new QPushButton("测试🔍", this);
    m_startApplicationButton = new QPushButton("开始🌟", this);
    m_outputTextEdit = new QTextEdit(this);

    // Page 1 layout
    QVBoxLayout *mainPageLayout = new QVBoxLayout(centralWidget());

    QHBoxLayout *inputLayout_1 = new QHBoxLayout();
    inputLayout_1->addWidget(new QLabel("URL: ", this));
    inputLayout_1->addWidget(m_urlInput);
    mainPageLayout->addLayout(inputLayout_1);

    QHBoxLayout *inputLayout_2 = new QHBoxLayout();
    inputLayout_2->addWidget(new QLabel("Model: ", this));
    inputLayout_2->addWidget(m_modelInput);
    mainPageLayout->addLayout(inputLayout_2);

    QHBoxLayout *inputLayout_3 = new QHBoxLayout();
    inputLayout_3->addWidget(new QLabel("API Key: ", this));
    inputLayout_3->addWidget(m_apiKeyInput);
    mainPageLayout->addLayout(inputLayout_3);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_resetButton);
    buttonLayout->addWidget(m_testConnectionButton);
    buttonLayout->addWidget(m_startApplicationButton);
    mainPageLayout->addLayout(buttonLayout);
    mainPageLayout->addWidget(m_outputTextEdit);

    m_mainPage->setLayout(mainPageLayout);

    // Set main window layout
    setCentralWidget(m_mainPage);

    connect(m_testConnectionButton, &QPushButton::clicked, this, [this]() {
        m_outputTextEdit->clear();
        m_outputTextEdit->append("Please wait ...");

        HttpManager::InitHttpManager(m_urlInput->text(), m_apiKeyInput->text(), m_modelInput->text(), 300000, 3);

        agreementInfo infoSend;
        infoSend.system = "You are a helpful assistant. ";
        infoSend.cmd = (int)AgreementCmd::translate_msg;
        infoSend.msg = "hi";
        std::string msgTranslate = agreement::getInstance().wrapToJson(infoSend);

        HttpManager httpManager;
        httpManager.SendRequestJson(msgTranslate);

        std::string showText;
        MessageManager::getInstance().popFromInputQueueNoWait(showText);
        agreementInfo info = agreement::getInstance().parseJson(showText);

        if (info.cmd == (int)AgreementCmd::success_msg) {
            m_outputTextEdit->clear();
            m_outputTextEdit->append(info.msg.c_str());
        }
    });

    connect(m_startApplicationButton, &QPushButton::clicked, this, [this]() {
        HttpManager::InitHttpManager(m_urlInput->text(), m_apiKeyInput->text(), m_modelInput->text(), 300000, 3);

        // Start button clicked operation
        // Switch to second page
        StateManager::getInstance().ShowPage = 1;

        ModelsInfo_s newServerInfo;
        newServerInfo.url = m_urlInput->text().toStdString();
        newServerInfo.apiKey = m_apiKeyInput->text().toStdString();
        newServerInfo.model = m_modelInput->text().toStdString();

        ModelsInfo::getInstance().SetServerIP(newServerInfo);
    });

    connect(m_resetButton, &QPushButton::clicked, this, [this]() {
        ModelsInfo_s defaultServerInfo = ModelsInfo::getInstance().DefaultGetServerIP();

        m_urlInput->setText(defaultServerInfo.url.c_str());
        m_apiKeyInput->setText(defaultServerInfo.apiKey.c_str());
        m_modelInput->setText(defaultServerInfo.model.c_str());
    });
}
