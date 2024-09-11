/*
 * @Date: 2024-08-28 14:56:49
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-11 10:34:18
 * @FilePath: /CppLLMTranslate/GUI/qt/intricate_page.cpp
 */

#include "intricate_page.h"
#include "MessageManager.h"
#include "agreement.h"

#include <QClipboard>
#include <QApplication>
#include "ConfigManager.h"
#include "StateManager.h"

IntricatePage::IntricatePage(QWidget *parent) : QMainWindow(parent)
{
    // 设置主窗口的布局
    m_mainLayout = new QVBoxLayout();

    // 第一行
    m_modeComboBox = new QComboBox();
    m_modeComboBox->addItem("英译中");
    m_modeComboBox->addItem("中译英");
    m_modeComboBox->addItem("聊天");
    m_modeComboBox->addItem("文件翻译");
    m_toggleSettingsButton = new QPushButton("繁");
    m_toggleSettingsButton->setToolTip("切换到简易页面");

    QHBoxLayout *firstRowLayout = new QHBoxLayout();
    firstRowLayout->addWidget(m_modeComboBox);
    firstRowLayout->addWidget(m_toggleSettingsButton);
    m_mainLayout->addLayout(firstRowLayout);

    /********************************************************/
    /********************************************************/

    /********************************************************/
    // 创建聊天页面的水平布局，并添加两个 QTextEdit
    m_textEdit1 = new QTextEdit();
    m_textEdit2 = new QTextEdit();

    m_translateButton = new QPushButton("提交🚀");
    m_checkBox = new QCheckBox("剪贴板替换");
    m_reconnectButton = new QPushButton("重连🔗");

    m_translateButton->setToolTip("(Ctrl+Enter) 组合键也可以提交 \n (Ctrl+)字体变大 (Ctrl-)字体变小");
    m_checkBox->setToolTip("是否替换剪贴板粘贴 (Ctrl+V) 的内容");

    QHBoxLayout *fourthRowLayout = new QHBoxLayout();
    fourthRowLayout->addWidget(m_translateButton);
    fourthRowLayout->addWidget(m_checkBox);
    fourthRowLayout->addWidget(m_reconnectButton);

    QHBoxLayout *chatLayout = new QHBoxLayout();
    chatLayout->addWidget(m_textEdit1);
    chatLayout->addWidget(m_textEdit2);

    QVBoxLayout *chatVBoLayout = new QVBoxLayout();
    chatVBoLayout->addLayout(chatLayout);
    chatVBoLayout->addLayout(fourthRowLayout);

    /********************************************************/

    /********************************************************/

    // 创建输入框
    m_textEditSystem = new QTextEdit();
    m_textEditChatPrefix = new QTextEdit();
    m_textEditChatSuffix = new QTextEdit();
    m_textEditUserMsg1 = new QTextEdit();
    m_textEditUserMsg2 = new QTextEdit();
    m_textEditUserMsg3 = new QTextEdit();
    m_textEditAssistantMsg1 = new QTextEdit();
    m_textEditAssistantMsg2 = new QTextEdit();
    m_textEditAssistantMsg3 = new QTextEdit();

    // 创建标签
    QLabel *setLabel1 = new QLabel("系统设置  :");
    QLabel *setLabel2 = new QLabel("消息前缀  :");
    QLabel *setLabel3 = new QLabel("消息后缀  :");
    QLabel *setLabel4 = new QLabel("优质提问1:");
    QLabel *setLabel5 = new QLabel("优质回答1:");
    QLabel *setLabel6 = new QLabel("优质提问2:");
    QLabel *setLabel7 = new QLabel("优质回答2:");
    QLabel *setLabel8 = new QLabel("优质提问3:");
    QLabel *setLabel9 = new QLabel("优质回答3:");

    // 创建布局管理器
    QVBoxLayout *setLayout = new QVBoxLayout();

    // 添加控件到布局管理器

    QHBoxLayout *setHBoxLayout1 = new QHBoxLayout();
    setHBoxLayout1->addWidget(setLabel1);
    setHBoxLayout1->addWidget(m_textEditSystem);
    setLayout->addLayout(setHBoxLayout1);

    QHBoxLayout *setHBoxLayout2 = new QHBoxLayout();
    setHBoxLayout2->addWidget(setLabel2);
    setHBoxLayout2->addWidget(m_textEditChatPrefix);
    setLayout->addLayout(setHBoxLayout2);

    QHBoxLayout *setHBoxLayout3 = new QHBoxLayout();
    setHBoxLayout3->addWidget(setLabel3);
    setHBoxLayout3->addWidget(m_textEditChatSuffix);
    setLayout->addLayout(setHBoxLayout3);

    QHBoxLayout *setHBoxLayout4 = new QHBoxLayout();
    setHBoxLayout4->addWidget(setLabel4);
    setHBoxLayout4->addWidget(m_textEditUserMsg1);
    setLayout->addLayout(setHBoxLayout4);

    QHBoxLayout *setHBoxLayout5 = new QHBoxLayout();
    setHBoxLayout5->addWidget(setLabel5);
    setHBoxLayout5->addWidget(m_textEditAssistantMsg1);
    setLayout->addLayout(setHBoxLayout5);

    QHBoxLayout *setHBoxLayout6 = new QHBoxLayout();
    setHBoxLayout6->addWidget(setLabel6);
    setHBoxLayout6->addWidget(m_textEditUserMsg2);
    setLayout->addLayout(setHBoxLayout6);

    QHBoxLayout *setHBoxLayout7 = new QHBoxLayout();
    setHBoxLayout7->addWidget(setLabel7);
    setHBoxLayout7->addWidget(m_textEditAssistantMsg2);
    setLayout->addLayout(setHBoxLayout7);

    QHBoxLayout *setHBoxLayout8 = new QHBoxLayout();
    setHBoxLayout8->addWidget(setLabel8);
    setHBoxLayout8->addWidget(m_textEditUserMsg3);
    setLayout->addLayout(setHBoxLayout8);

    QHBoxLayout *setHBoxLayout9 = new QHBoxLayout();
    setHBoxLayout9->addWidget(setLabel9);
    setHBoxLayout9->addWidget(m_textEditAssistantMsg3);
    setLayout->addLayout(setHBoxLayout9);

    QPushButton *getButton = new QPushButton("获取配置");
    QPushButton *setButton = new QPushButton("更新配置");
    QPushButton *resetButton = new QPushButton("恢复默认");

    QHBoxLayout *setButtonLayout = new QHBoxLayout();
    setButtonLayout->addWidget(getButton);
    setButtonLayout->addWidget(setButton);
    setButtonLayout->addWidget(resetButton);
    setLayout->addLayout(setButtonLayout);

    /********************************************************/

    // 创建翻译页面的 QWidget 并设置布局
    QWidget *chatTabWidgetPage = new QWidget();
    chatTabWidgetPage->setLayout(chatVBoLayout);

    // 创建翻译页面的设置页面 QWidget 并设置布局
    QWidget *chatTabWidgetPageSet = new QWidget();
    chatTabWidgetPageSet->setLayout(setLayout);

    // 页面布局
    QTabWidget *tabWidget = new QTabWidget();
    tabWidget->addTab(chatTabWidgetPage, "运行");
    tabWidget->addTab(chatTabWidgetPageSet, "设置");

    QHBoxLayout *tabWidgetLayout = new QHBoxLayout();
    tabWidgetLayout->addWidget(tabWidget);
    m_mainLayout->addLayout(tabWidgetLayout);

    /********************************************************/
    /********************************************************/

    // 设置布局到中心窗口
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(m_mainLayout);
    setCentralWidget(centralWidget);

    // 创建定时器
    m_copyTimer = new QTimer(this);

    // 创建定时器
    m_translateTimer = new QTimer(this);

    // 连接定时器的timeout信号到槽函数
    connect(m_copyTimer, &QTimer::timeout, this, [=]() {
        // 槽函数的内容
        // 获取剪贴板对象
        QClipboard *clipboard = QApplication::clipboard();
        static std::string copyTextLast;
        std::string copyText = clipboard->text().toStdString(); // 从剪贴板获取文本;
        if (copyText != copyTextLast && !copyText.empty()) {
            m_textEdit1->clear();
            m_textEdit1->append(copyText.c_str());
            copyTextLast = copyText;
        }
    });

    // 连接定时器的timeout信号到槽函数
    connect(m_translateTimer, &QTimer::timeout, this, [=]() {
        if (StateManager::getInstance().ShowPage == 2) {
            std::string showText;
            if (MessageManager::getInstance().popFromInputQueueNoWait(showText)) {

                agreementInfo info = agreement::getInstance().parseJson(showText);

                if (info.cmd == (int)AgreementCmd::success_msg) {
                    // 完全翻译的信息覆盖
                    m_textEdit2->clear();
                    m_textEdit2->append(info.msg.c_str());
                    if (m_checkBox->isChecked()) {
                        QClipboard *clipboard = QApplication::clipboard();
                        clipboard->setText(info.msg.c_str()); // 将文本复制到剪贴板
                    }
                    QTextCursor cursor = m_textEdit2->textCursor();
                    cursor.movePosition(QTextCursor::Start); // 移动光标到文本开头
                    m_textEdit2->setTextCursor(cursor);        // 更新 QTextEdit 的光标位置

                } else if (info.cmd == (int)AgreementCmd::course_msg) {
                    // 过程中的信息追加

                    // // 获取QTextEdit的文本内容
                    // QString currentText = m_textEdit2->toPlainText();
                    // currentText += info.msg.c_str();
                    // // 设置合并后的文本到QTextEdit
                    // m_textEdit2->setPlainText(currentText);
                    QTextCursor cursor = m_textEdit2->textCursor();
                    cursor.movePosition(QTextCursor::End);        // 移动光标到文本末尾
                    m_textEdit2->setTextCursor(cursor);             // 更新 QTextEdit 的光标位置
                    m_textEdit2->insertPlainText(info.msg.c_str()); // 插入文本
                    m_textEdit2->ensureCursorVisible();             // 确保光标可见，即滚动到末尾

                } else {
                    // 其他消息覆盖
                    m_textEdit2->clear();
                    m_textEdit2->append(info.msg.c_str());
                }
            }
        }
    });

    // 连接信号和槽
    connect(m_reconnectButton, &QPushButton::clicked, this, [this]() {
        // 开始按钮点击后的操作
        // 切换到开始页面
        StateManager::getInstance().ShowPage = 0;
        StateManager::getInstance().ModeIndex = 0;
    });

    // 使用lambda表达式连接信号和槽
    connect(m_modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        if (StateManager::getInstance().ShowPage == 2) {
            qDebug("intricate_page index=%d", index);

            StateManager::getInstance().ModeIndex = index;
            if (index == 3) {
                // 切换页面
                StateManager::getInstance().ShowPage = 3;
            }

            UpdateInfo(index);
        }
    });

    // 连接按钮的点击信号到槽函数
    connect(m_toggleSettingsButton, &QPushButton::clicked, this, &IntricatePage::onToggleSettingsButtonClicked);

    // 连接按钮的点击信号到槽函数
    connect(getButton, &QPushButton::clicked, this, [this]() {
        UpdateInfo(0); // 传0单纯是为了不报错
    });

    connect(setButton, &QPushButton::clicked, this, [this]() {
        agreementInfo info;

        info.system = m_textEditSystem->toPlainText().toStdString();
        info.chat_prefix = m_textEditChatPrefix->toPlainText().toStdString();
        info.chat_suffix = m_textEditChatSuffix->toPlainText().toStdString();
        info.user_msg_1 = m_textEditUserMsg1->toPlainText().toStdString();
        info.user_msg_2 = m_textEditUserMsg2->toPlainText().toStdString();
        info.user_msg_3 = m_textEditUserMsg3->toPlainText().toStdString();
        info.assistant_msg_1 = m_textEditAssistantMsg1->toPlainText().toStdString();
        info.assistant_msg_2 = m_textEditAssistantMsg2->toPlainText().toStdString();
        info.assistant_msg_3 = m_textEditAssistantMsg3->toPlainText().toStdString();

        if (StateManager::getInstance().ModeIndex == 0) {
            ConfigManager::getInstance().Set_config_en_to_zh(info);
        } else if (StateManager::getInstance().ModeIndex == 1) {
            ConfigManager::getInstance().Set_config_zh_to_en(info);
        } else if (StateManager::getInstance().ModeIndex == 2) {
            ConfigManager::getInstance().Set_config_chat(info);
        }
    });

    connect(resetButton, &QPushButton::clicked, this, [this]() {
        agreementInfo info;

        if (StateManager::getInstance().ModeIndex == 0) {
            info = ConfigManager::getInstance().default_config_en_to_zh();
            ConfigManager::getInstance().Set_config_en_to_zh(info);
        } else if (StateManager::getInstance().ModeIndex == 1) {
            info = ConfigManager::getInstance().default_config_zh_to_en();
            ConfigManager::getInstance().Set_config_zh_to_en(info);
        } else if (StateManager::getInstance().ModeIndex == 2) {
            info = ConfigManager::getInstance().default_config_chat();
            ConfigManager::getInstance().Set_config_chat(info);
        }

        UpdateInfo(0); // 传0单纯是为了不报错
    });

    // 连接 QTabWidget 的 currentChanged 信号
    QObject::connect(tabWidget, &QTabWidget::currentChanged, this, &IntricatePage::UpdateInfo);

    // 连接信号和槽
    connect(m_translateButton, &QPushButton::clicked, this, &IntricatePage::SendToServer);

    // 启动定时器，间隔时间为毫秒
    m_copyTimer->start(100);

    // 启动定时器，间隔时间为毫秒
    m_translateTimer->start(1);
}

IntricatePage::~IntricatePage() {}

void IntricatePage::UpdateModeComboBox()
{
    if (StateManager::getInstance().ShowPage == 2) {
        m_modeComboBox->setCurrentIndex(StateManager::getInstance().ModeIndex);
    }
}

void IntricatePage::onToggleSettingsButtonClicked()
{
    // 切换 简 页面
    StateManager::getInstance().ShowPage = 1;
}

void IntricatePage::UpdateInfo(int index)
{
    agreementInfo info;
    if (StateManager::getInstance().ModeIndex == 0) {
        // info = agreement::getInstance().default_en_to_zh();
        info = ConfigManager::getInstance().Get_config_en_to_zh();
    } else if (StateManager::getInstance().ModeIndex == 1) {
        // info = agreement::getInstance().default_zh_to_en();
        info = ConfigManager::getInstance().Get_config_zh_to_en();
    } else if (StateManager::getInstance().ModeIndex == 2) {
        // info = agreement::getInstance().default_chat();
        info = ConfigManager::getInstance().Get_config_chat();
    }

    m_textEditSystem->setText(info.system.c_str());
    m_textEditChatPrefix->setText(info.chat_prefix.c_str());
    m_textEditChatSuffix->setText(info.chat_suffix.c_str());
    m_textEditUserMsg1->setText(info.user_msg_1.c_str());
    m_textEditUserMsg2->setText(info.user_msg_2.c_str());
    m_textEditUserMsg3->setText(info.user_msg_3.c_str());
    m_textEditAssistantMsg1->setText(info.assistant_msg_1.c_str());
    m_textEditAssistantMsg2->setText(info.assistant_msg_2.c_str());
    m_textEditAssistantMsg3->setText(info.assistant_msg_3.c_str());
}

void IntricatePage::SendToServer()
{
    std::string srcText = m_textEdit1->toPlainText().toStdString();

    agreementInfo info;

    if (StateManager::getInstance().ModeIndex == 0) {
        // info = agreement::getInstance().default_en_to_zh();
        info = ConfigManager::getInstance().Get_config_en_to_zh();
    } else if (StateManager::getInstance().ModeIndex == 1) {
        // info = agreement::getInstance().default_zh_to_en();
        info = ConfigManager::getInstance().Get_config_zh_to_en();
    } else if (StateManager::getInstance().ModeIndex == 2) {
        // info = agreement::getInstance().default_chat();
        info = ConfigManager::getInstance().Get_config_chat();
    }

    info.msg = srcText;
    info.cmd = (int)AgreementCmd::translate_msg;

    std::string msgTranslate = agreement::getInstance().wrapToJson(info);

    MessageManager::getInstance().pushToOutputQueue(msgTranslate);

    m_textEdit2->clear();
    m_textEdit2->append("Please wait ...");
}

void IntricatePage::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return && event->modifiers().testFlag(Qt::ControlModifier)) {
        SendToServer();
    } else if (event->key() == Qt::Key_Equal && event->modifiers().testFlag(Qt::ControlModifier)) {
        ChangeFontSize(1);
    } else if (event->key() == Qt::Key_Minus && event->modifiers().testFlag(Qt::ControlModifier)) {
        ChangeFontSize(-1); // 减小字体大小
    } else {
        QWidget::keyPressEvent(event);
    }
}

void IntricatePage::ChangeFontSize(int delta)
{
    QFont font = QApplication::font();
    int newSize = font.pointSize() + delta; // 调整字体大小
    if (newSize < 5) {
        newSize = 5;
    }

    if (newSize > 30) {
        newSize = 30;
    }

    font.setPointSize(newSize);
    QApplication::setFont(font);
}
