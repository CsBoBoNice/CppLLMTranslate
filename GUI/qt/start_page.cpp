#include "start_page.h"
#include "StateManager.h"
#include "agreement.h"
#include "ConfigManager.h"
#include <QThread>
#include "HttpManager.h"
#include "MessageManager.h"
#include <QMessageBox>

// 构造函数
StartPage::StartPage(QWidget *parent) : QMainWindow(parent)
{
    // 获取当前服务器配置
    m_serverConfig = ModelsInfo::getInstance().getCurrentServerConfig();

    // 初始化UI组件
    initializeUI();
    // 设置布局
    setupLayouts();
    // 连接信号和槽
    connectSignalsAndSlots();
    // 更新模型选择器
    updateModelSelector();
}

// 初始化UI组件
void StartPage::initializeUI()
{
    // 创建主页面部件
    m_mainPage = new QWidget(this);
    
    // 创建输入框并设置初始值
    m_urlInput = new QLineEdit(QString::fromStdString(m_serverConfig.url), this);
    m_modelInput = new QLineEdit(QString::fromStdString(m_serverConfig.model), this);
    m_apiKeyInput = new QLineEdit(QString::fromStdString(m_serverConfig.apiKey), this);
    m_titleInput = new QLineEdit("模型名称", this);
    
    // 创建按钮
    m_testConnectionButton = new QPushButton("测试🔍", this);
    m_startApplicationButton = new QPushButton("开始🌟", this);
    
    // 创建文本编辑框
    m_outputTextEdit = new QTextEdit(this);
    
    // 创建模型选择器和相关按钮
    m_modelSelector = new QComboBox(this);
    m_deleteButton = new QPushButton("删除", this);
    m_refreshButton = new QPushButton("刷新", this);
}

// 设置布局
void StartPage::setupLayouts()
{
    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget());

    // 创建模型选择器布局
    QHBoxLayout *selectorLayout = new QHBoxLayout();
    selectorLayout->addWidget(new QLabel("选择模型: ", this));
    selectorLayout->addWidget(m_modelSelector);
    selectorLayout->addWidget(m_deleteButton);
    selectorLayout->addWidget(m_refreshButton);
    mainLayout->addLayout(selectorLayout);

    // 添加输入字段到布局
    addInputFieldToLayout(mainLayout, "标题: ", m_titleInput);
    addInputFieldToLayout(mainLayout, "URL: ", m_urlInput);
    addInputFieldToLayout(mainLayout, "Model: ", m_modelInput);
    addInputFieldToLayout(mainLayout, "API Key: ", m_apiKeyInput);

    // 创建按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(m_testConnectionButton);
    buttonLayout->addWidget(m_startApplicationButton);
    mainLayout->addLayout(buttonLayout);

    // 添加输出文本编辑框到布局
    mainLayout->addWidget(m_outputTextEdit);

    // 设置主页面布局
    m_mainPage->setLayout(mainLayout);
    setCentralWidget(m_mainPage);
}

// 添加输入字段到布局的辅助函数
void StartPage::addInputFieldToLayout(QVBoxLayout *layout, const QString &label, QLineEdit *input)
{
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(new QLabel(label, this));
    inputLayout->addWidget(input);
    layout->addLayout(inputLayout);
}

// 连接信号和槽
void StartPage::connectSignalsAndSlots()
{
    connect(m_testConnectionButton, &QPushButton::clicked, this, &StartPage::testConnection);
    connect(m_startApplicationButton, &QPushButton::clicked, this, &StartPage::startApplication);
    connect(m_deleteButton, &QPushButton::clicked, this, &StartPage::deleteSelectedModel);
    connect(m_refreshButton, &QPushButton::clicked, this, &StartPage::updateModelSelector);
    connect(m_modelSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StartPage::onModelSelected);
}

// 测试连接
void StartPage::testConnection()
{
    // 清空输出文本框并显示等待消息
    m_outputTextEdit->clear();
    m_outputTextEdit->append("请稍等...");

    // 初始化HTTP管理器
    HttpManager::InitHttpManager(m_urlInput->text(), m_apiKeyInput->text(), m_modelInput->text(), 300000, 3);

    // 准备测试消息
    agreementInfo infoSend;
    infoSend.system = "You are a helpful assistant. ";
    infoSend.cmd = (int)AgreementCmd::translate_msg;
    infoSend.msg = "hi";
    std::string msgTranslate = agreement::getInstance().wrapToJson(infoSend);

    // 发送HTTP请求
    HttpManager httpManager;
    httpManager.SendRequestJson(msgTranslate);

    // 获取响应
    std::string showText;
    MessageManager::getInstance().popFromInputQueueNoWait(showText);
    agreementInfo info = agreement::getInstance().parseJson(showText);

    // 显示响应结果
    if (info.cmd == (int)AgreementCmd::success_msg) {
        m_outputTextEdit->clear();
        m_outputTextEdit->append(info.msg.c_str());
    }
}

// 启动应用程序
void StartPage::startApplication()
{
    // 初始化HTTP管理器
    HttpManager::InitHttpManager(m_urlInput->text(), m_apiKeyInput->text(), m_modelInput->text(), 300000, 3);

    // 设置状态管理器的显示页面
    StateManager::getInstance().ShowPage = 1;

    // 创建新的服务器配置
    ModelsInfo_s newServerConfig;
    newServerConfig.title = m_titleInput->text().toStdString();
    newServerConfig.url = m_urlInput->text().toStdString();
    newServerConfig.apiKey = m_apiKeyInput->text().toStdString();
    newServerConfig.model = m_modelInput->text().toStdString();

    // 保存新的服务器配置
    ModelsInfo::getInstance().saveServerConfig(newServerConfig);
}

// 删除选中的模型
void StartPage::deleteSelectedModel()
{
    int currentIndex = m_modelSelector->currentIndex();
    if (currentIndex > 0) {
        ModelsInfo_s selectedInfo = m_modelSelector->itemData(currentIndex).value<ModelsInfo_s>();
        
        // 创建确认对话框
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "确认删除",
                                      "您确定要删除 '" + QString::fromStdString(selectedInfo.title) + "' 吗？",
                                      QMessageBox::Yes|QMessageBox::No);
        
        // 如果用户确认删除，则执行删除操作
        if (reply == QMessageBox::Yes) {
            ModelsInfo::getInstance().deleteServerConfig(selectedInfo.title);
            updateModelSelector();
        }
    }
}

// 更新模型选择器
void StartPage::updateModelSelector()
{
    m_modelSelector->clear();

    // 添加默认配置
    ModelsInfo_s defaultConfig = ModelsInfo::getInstance().getDefaultServerConfig();
    defaultConfig.title = "默认配置";
    m_modelSelector->addItem(QString::fromStdString(defaultConfig.title), QVariant::fromValue(defaultConfig));

    // 添加所有保存的服务器配置
    std::vector<ModelsInfo_s> allServerConfigs = ModelsInfo::getInstance().getAllServerConfigs();
    for (const auto &config : allServerConfigs) {
        m_modelSelector->addItem(QString::fromStdString(config.title), QVariant::fromValue(config));
    }
}

// 当选择模型时的处理函数
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