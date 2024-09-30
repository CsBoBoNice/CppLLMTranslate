#include "start_page.h"
#include "StateManager.h"
#include "agreement.h"
#include "ConfigManager.h"
#include <QThread>
#include "HttpManager.h"
#include "MessageManager.h"

StartPage::StartPage(QWidget *parent) : QMainWindow(parent)
{
    m_serverConfig = ModelsInfo::getInstance().getCurrentServerConfig();

    initializeUI();
    setupLayouts();
    connectSignalsAndSlots();
    updateModelSelector();
}

void StartPage::initializeUI()
{
    m_mainPage = new QWidget(this);
    m_urlInput = new QLineEdit(QString::fromStdString(m_serverConfig.url), this);
    m_modelInput = new QLineEdit(QString::fromStdString(m_serverConfig.model), this);
    m_apiKeyInput = new QLineEdit(QString::fromStdString(m_serverConfig.apiKey), this);
    m_titleInput = new QLineEdit("模型名称", this);
    m_testConnectionButton = new QPushButton("测试🔍", this);
    m_startApplicationButton = new QPushButton("开始🌟", this);
    m_outputTextEdit = new QTextEdit(this);
    m_modelSelector = new QComboBox(this);
    m_deleteButton = new QPushButton("删除", this);
    m_refreshButton = new QPushButton("刷新", this);
}

void StartPage::setupLayouts()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget());

    QHBoxLayout *selectorLayout = new QHBoxLayout();
    selectorLayout->addWidget(new QLabel("选择模型: ", this));
    selectorLayout->addWidget(m_modelSelector);
    selectorLayout->addWidget(m_deleteButton);
    selectorLayout->addWidget(m_refreshButton);
    mainLayout->addLayout(selectorLayout);

    addInputFieldToLayout(mainLayout, "标题: ", m_titleInput);
    addInputFieldToLayout(mainLayout, "URL: ", m_urlInput);
    addInputFieldToLayout(mainLayout, "Model: ", m_modelInput);
    addInputFieldToLayout(mainLayout, "API Key: ", m_apiKeyInput);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_testConnectionButton);
    buttonLayout->addWidget(m_startApplicationButton);
    mainLayout->addLayout(buttonLayout);

    mainLayout->addWidget(m_outputTextEdit);

    m_mainPage->setLayout(mainLayout);
    setCentralWidget(m_mainPage);
}

void StartPage::addInputFieldToLayout(QVBoxLayout *layout, const QString &label, QLineEdit *input)
{
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(new QLabel(label, this));
    inputLayout->addWidget(input);
    layout->addLayout(inputLayout);
}

void StartPage::connectSignalsAndSlots()
{
    connect(m_testConnectionButton, &QPushButton::clicked, this, &StartPage::testConnection);
    connect(m_startApplicationButton, &QPushButton::clicked, this, &StartPage::startApplication);
    connect(m_deleteButton, &QPushButton::clicked, this, &StartPage::deleteSelectedModel);
    connect(m_refreshButton, &QPushButton::clicked, this, &StartPage::updateModelSelector);
    connect(m_modelSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StartPage::onModelSelected);
}

void StartPage::testConnection()
{
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
}

void StartPage::startApplication()
{
    HttpManager::InitHttpManager(m_urlInput->text(), m_apiKeyInput->text(), m_modelInput->text(), 300000, 3);

    StateManager::getInstance().ShowPage = 1;

    ModelsInfo_s newServerConfig;
    newServerConfig.title = m_titleInput->text().toStdString();
    newServerConfig.url = m_urlInput->text().toStdString();
    newServerConfig.apiKey = m_apiKeyInput->text().toStdString();
    newServerConfig.model = m_modelInput->text().toStdString();

    ModelsInfo::getInstance().saveServerConfig(newServerConfig);
}

void StartPage::deleteSelectedModel()
{
    int currentIndex = m_modelSelector->currentIndex();
    if (currentIndex > 0) {
        ModelsInfo_s selectedInfo = m_modelSelector->itemData(currentIndex).value<ModelsInfo_s>();
        ModelsInfo::getInstance().deleteServerConfig(selectedInfo.title);
        updateModelSelector();
    }
}

void StartPage::updateModelSelector()
{
    m_modelSelector->clear();

    ModelsInfo_s defaultConfig = ModelsInfo::getInstance().getDefaultServerConfig();
    defaultConfig.title = "默认配置";
    m_modelSelector->addItem(QString::fromStdString(defaultConfig.title), QVariant::fromValue(defaultConfig));

    std::vector<ModelsInfo_s> allServerConfigs = ModelsInfo::getInstance().getAllServerConfigs();
    for (const auto &config : allServerConfigs) {
        m_modelSelector->addItem(QString::fromStdString(config.title), QVariant::fromValue(config));
    }
}

void StartPage::onModelSelected(int index)
{
    if (index >= 0) {
        ModelsInfo_s selectedConfig = m_modelSelector->itemData(index).value<ModelsInfo_s>();
        m_titleInput->setText(QString::fromStdString(selectedConfig.title));
        m_urlInput->setText(QString::fromStdString(selectedConfig.url));
        m_modelInput->setText(QString::fromStdString(selectedConfig.model));
        m_apiKeyInput->setText(QString::fromStdString(selectedConfig.apiKey));
    }
}