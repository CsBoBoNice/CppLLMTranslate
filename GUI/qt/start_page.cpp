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

StartPage::StartPage(QWidget *parent) : QMainWindow{parent}
{
    ServerInfo serverInfo = ConfigManager::getInstance().GetServerIP();

    // Initialize UI
    m_mainPage = new QWidget(this);

    m_urlInput = new QLineEdit(serverInfo.url.c_str(), this);
    m_modelInput = new QLineEdit(serverInfo.model.c_str(), this);
    m_apiKeyInput = new QLineEdit(serverInfo.apiKey.c_str(), this);
    m_resetButton = new QPushButton("默认🔄", this);
    m_testConnectionButton = new QPushButton("测试🔍", this);
    m_startApplicationButton = new QPushButton("开始🌟", this);
    m_outputTextEdit = new QTextEdit(this);

    m_configNameInput = new QLineEdit(this);

    // Page 1 layout
    QVBoxLayout *mainPageLayout = new QVBoxLayout(centralWidget());

    QHBoxLayout *selectorLayout = new QHBoxLayout();
    selectorLayout->addWidget(new QLabel("配置: ", this));
    selectorLayout->addWidget(m_configSelector = new QComboBox(this));
    mainPageLayout->insertLayout(0, selectorLayout);

    updateConfigSelector();

    QHBoxLayout *configNameLayout = new QHBoxLayout();
    configNameLayout->addWidget(new QLabel("配置名称: ", this));
    configNameLayout->addWidget(m_configNameInput);
    mainPageLayout->insertLayout(1, configNameLayout);

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
    buttonLayout->addWidget(deleteConfigButton = new QPushButton("删除配置", this));
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

        saveCurrentConfig();
    });

    connect(m_resetButton, &QPushButton::clicked, this, [this]() {
        ServerInfo defaultServerInfo = ConfigManager::getInstance().DefaultGetServerIP();

        m_urlInput->setText(defaultServerInfo.url.c_str());
        m_apiKeyInput->setText(defaultServerInfo.apiKey.c_str());
        m_modelInput->setText(defaultServerInfo.model.c_str());
    });

    connect(m_configSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StartPage::loadSelectedConfig);

    connect(deleteConfigButton, &QPushButton::clicked, this, &StartPage::deleteCurrentConfig);
}

void StartPage::updateConfigSelector()
{
    m_configSelector->clear();
    auto configs = ConfigManager::getInstance().GetServerConfigs();
    for (const auto& config : configs) {
        m_configSelector->addItem(QString::fromStdString(config.alias));
    }
}

void StartPage::loadSelectedConfig()
{
    QString selectedAlias = m_configSelector->currentText();
    auto configs = ConfigManager::getInstance().GetServerConfigs();
    auto it = std::find_if(configs.begin(), configs.end(),
                           [&selectedAlias](const ServerInfo& config) {
                               return config.alias == selectedAlias.toStdString();
                           });
    if (it != configs.end()) {
        m_urlInput->setText(QString::fromStdString(it->url));
        m_apiKeyInput->setText(QString::fromStdString(it->apiKey));
        m_modelInput->setText(QString::fromStdString(it->model));
        m_configNameInput->setText(QString::fromStdString(it->alias));
    }
}

void StartPage::saveCurrentConfig()
{
    ServerInfo newServerInfo;
    newServerInfo.alias = m_configNameInput->text().toStdString();
    newServerInfo.url = m_urlInput->text().toStdString();
    newServerInfo.apiKey = m_apiKeyInput->text().toStdString();
    newServerInfo.model = m_modelInput->text().toStdString();

    if (!newServerInfo.alias.empty()) {
        ConfigManager::getInstance().AddServerConfig(newServerInfo);
        updateConfigSelector();
        m_configSelector->setCurrentText(QString::fromStdString(newServerInfo.alias));
    }
}

void StartPage::deleteCurrentConfig()
{
    QString currentAlias = m_configSelector->currentText();
    if (currentAlias == "默认") {
        QMessageBox::warning(this, "警告", "不能删除默认配置！");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除", 
                                  "您确定要删除配置 '" + currentAlias + "' 吗？",
                                  QMessageBox::Yes|QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        ConfigManager::getInstance().RemoveServerConfig(currentAlias.toStdString());
        updateConfigSelector();
        
        // 如果还有其他配置，选择第一个
        if (m_configSelector->count() > 0) {
            m_configSelector->setCurrentIndex(0);
            loadSelectedConfig();
        } else {
            // 如果没有其他配置，清空输入框
            m_urlInput->clear();
            m_apiKeyInput->clear();
            m_modelInput->clear();
            m_configNameInput->clear();
        }
    }
}
