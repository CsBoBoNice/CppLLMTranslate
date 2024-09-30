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
    m_titleInput = new QLineEdit(this);  // 新增: 用于输入模型标题
    m_testConnectionButton = new QPushButton("测试🔍", this);
    m_startApplicationButton = new QPushButton("开始🌟", this);
    m_outputTextEdit = new QTextEdit(this);

    m_modelSelector = new QComboBox(this);
    m_deleteButton = new QPushButton("删除", this);
    m_refreshButton = new QPushButton("刷新", this);

    // Page 1 layout
    QVBoxLayout *mainPageLayout = new QVBoxLayout(centralWidget());

    QHBoxLayout *selectorLayout = new QHBoxLayout();
    selectorLayout->addWidget(new QLabel("选择模型: ", this));
    selectorLayout->addWidget(m_modelSelector);
    selectorLayout->addWidget(m_deleteButton);
    selectorLayout->addWidget(m_refreshButton);
    mainPageLayout->insertLayout(0, selectorLayout);

    QHBoxLayout *titleLayout = new QHBoxLayout();  // 新增: 标题输入框布局
    titleLayout->addWidget(new QLabel("标题: ", this));
    titleLayout->addWidget(m_titleInput);
    mainPageLayout->addLayout(titleLayout);

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
        newServerInfo.title = m_titleInput->text().toStdString();  // 使用标题输入框的内容
        newServerInfo.url = m_urlInput->text().toStdString();
        newServerInfo.apiKey = m_apiKeyInput->text().toStdString();
        newServerInfo.model = m_modelInput->text().toStdString();

        ModelsInfo::getInstance().SaveServerIP(newServerInfo);
    });

    connect(m_deleteButton, &QPushButton::clicked, this, [this]() {
        int currentIndex = m_modelSelector->currentIndex();
        if (currentIndex > 0) {  // 不允许删除默认配置
            ModelsInfo_s selectedInfo = m_modelSelector->itemData(currentIndex).value<ModelsInfo_s>();
            ModelsInfo::getInstance().DeleteServerIP(selectedInfo.title);
            updateModelSelector();
        }
    });

    connect(m_refreshButton, &QPushButton::clicked, this, [this]() {
        updateModelSelector();
    });

    updateModelSelector();

    connect(m_modelSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        if (index >= 0) {
            ModelsInfo_s selectedInfo = m_modelSelector->itemData(index).value<ModelsInfo_s>();
            m_titleInput->setText(QString::fromStdString(selectedInfo.title));  // 更新标题输入框
            m_urlInput->setText(QString::fromStdString(selectedInfo.url));
            m_modelInput->setText(QString::fromStdString(selectedInfo.model));
            m_apiKeyInput->setText(QString::fromStdString(selectedInfo.apiKey));
        }
    });
}

void StartPage::updateModelSelector()
{
    m_modelSelector->clear();

    // 添加默认配置
    ModelsInfo_s defaultInfo = ModelsInfo::getInstance().DefaultGetServerIP();
    defaultInfo.title = "默认配置";
    m_modelSelector->addItem(QString::fromStdString(defaultInfo.title), QVariant::fromValue(defaultInfo));

    // 添加保存的配置
    std::vector<ModelsInfo_s> allServerInfos = ModelsInfo::getInstance().GetAllServerIPs();
    for (const auto &info : allServerInfos) {
        m_modelSelector->addItem(QString::fromStdString(info.title), QVariant::fromValue(info));
    }
}
