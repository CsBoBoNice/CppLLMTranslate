#include "simple_page.h"
#include "HttpManager.h"
#include "MessageManager.h"
#include "agreement.h"

#include <QClipboard>
#include <QApplication>
#include "ConfigManager.h"
#include "StateManager.h"
#include <QThread>

HttpManager httpManager;

static void HttpThread()
{
    httpManager.SendRequestThread();
}

SimplePage::SimplePage(QWidget *parent) : QMainWindow(parent)
{
    // 设置主窗口的布局
    m_mainVBoxLayout = new QVBoxLayout();

    // 第一行
    m_translationModeComboBox = new QComboBox();
    m_translationModeComboBox->addItem("英译中");
    m_translationModeComboBox->addItem("中译英");
    m_translationModeComboBox->addItem("聊天");
    m_translationModeComboBox->addItem("文件翻译");
    m_toggleSettingsButton = new QPushButton("简");
    m_toggleSettingsButton->setToolTip("切换到可以设置提示词的页面");

    QHBoxLayout *firstRowLayout = new QHBoxLayout();
    firstRowLayout->addWidget(m_translationModeComboBox);
    firstRowLayout->addWidget(m_toggleSettingsButton);
    m_mainVBoxLayout->addLayout(firstRowLayout);

    // 第二行
    m_sourceTextEdit = new QTextEdit();
    m_mainVBoxLayout->addWidget(m_sourceTextEdit);

    // 第三行
    m_targetTextEdit = new QTextEdit();
    m_mainVBoxLayout->addWidget(m_targetTextEdit);

    // 第四行
    m_submitTranslationButton = new QPushButton("提交🚀");
    m_checkBox = new QCheckBox("剪贴板替换");
    m_reconnectButton = new QPushButton("重连🔗");

    // 设置工具提示
    m_submitTranslationButton->setToolTip("(Ctrl+Enter) 组合键也可以提交 \n (Ctrl+)字体变大 (Ctrl-)字体变小");
    m_checkBox->setToolTip("是否替换剪贴板粘贴 (Ctrl+V) 的内容");

    QHBoxLayout *fourthRowLayout = new QHBoxLayout();
    fourthRowLayout->addWidget(m_submitTranslationButton);
    fourthRowLayout->addWidget(m_checkBox);
    fourthRowLayout->addWidget(m_reconnectButton);
    m_mainVBoxLayout->addLayout(fourthRowLayout);

    // 设置布局到中心窗口
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(m_mainVBoxLayout);
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
            m_sourceTextEdit->clear();
            m_sourceTextEdit->append(copyText.c_str());
            copyTextLast = copyText;
        }
    });

    // 连接定时器的timeout信号到槽函数
    connect(m_translateTimer, &QTimer::timeout, this, [=]() {
        if (StateManager::getInstance().ShowPage == 1) {

            std::string showText;
            if (MessageManager::getInstance().popFromInputQueueNoWait(showText)) {

                agreementInfo info = agreement::getInstance().parseJson(showText);

                if (info.cmd == (int)AgreementCmd::success_msg) {
                    // 完全翻译的信息覆盖
                    m_targetTextEdit->clear();
                    m_targetTextEdit->append(info.msg.c_str());
                    if (m_checkBox->isChecked()) {
                        QClipboard *clipboard = QApplication::clipboard();
                        clipboard->setText(info.msg.c_str()); // 将文本复制到剪贴板
                    }
                    QTextCursor cursor = m_targetTextEdit->textCursor();
                    cursor.movePosition(QTextCursor::Start); // 移动光标到文本开头
                    m_targetTextEdit->setTextCursor(cursor);        // 更新 QTextEdit 的光标位置

                } else if (info.cmd == (int)AgreementCmd::course_msg) {
                    // 过程中的信息追加

                    // // 获取QTextEdit的文本内容
                    // QString currentText = m_targetTextEdit->toPlainText();
                    // currentText += info.msg.c_str();
                    // // 设置合并后的文本到QTextEdit
                    // m_targetTextEdit->setPlainText(currentText);
                    QTextCursor cursor = m_targetTextEdit->textCursor();
                    cursor.movePosition(QTextCursor::End);        // 移动光标到文本末尾
                    m_targetTextEdit->setTextCursor(cursor);             // 更新 QTextEdit 的光标位置
                    m_targetTextEdit->insertPlainText(info.msg.c_str()); // 插入文本
                    m_targetTextEdit->ensureCursorVisible();             // 确保光标可见，即滚动到末尾

                } else {
                    // 其他消息覆盖
                    m_targetTextEdit->clear();
                    m_targetTextEdit->append(info.msg.c_str());
                }
            }
        }
    });

    // 使用lambda表达式连接信号和槽
    connect(m_translationModeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        if (StateManager::getInstance().ShowPage == 1) {
            qDebug("SimplePage index=%d", index);
            StateManager::getInstance().ModeIndex = index;
            if (index == 3) {
                // 切换页面
                StateManager::getInstance().ShowPage = 3;
            }
        }
    });

    // 连接按钮的点击信号到槽函数
    connect(m_toggleSettingsButton, &QPushButton::clicked, this, &SimplePage::OnToggleSettingsButtonClicked);

    // 连接信号和槽
    connect(m_submitTranslationButton, &QPushButton::clicked, this, &SimplePage::SendToServer);

    // 连接信号和槽
    connect(m_reconnectButton, &QPushButton::clicked, this, [this]() {
        // 开始按钮点击后的操作
        // 切换到开始页面
        StateManager::getInstance().ShowPage = 0;
        StateManager::getInstance().ModeIndex = 0;
    });

    // 启动定时器，间隔时间为毫秒
    m_copyTimer->start(100);

    // 启动定时器，间隔时间为毫秒
    m_translateTimer->start(1);

    std::thread t_HTTP_thread(HttpThread);
    t_HTTP_thread.detach();
}

SimplePage::~SimplePage() {}

void SimplePage::UpdateModeComboBox()
{
    if (StateManager::getInstance().ShowPage == 1) {
        m_translationModeComboBox->setCurrentIndex(StateManager::getInstance().ModeIndex);
    }
}

void SimplePage::OnToggleSettingsButtonClicked()
{
    // 切换 繁 页面
    StateManager::getInstance().ShowPage = 2;
}

void SimplePage::SendToServer()
{
    std::string srcText = m_sourceTextEdit->toPlainText().toStdString();

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

    m_targetTextEdit->clear();
    m_targetTextEdit->append("Please wait ...");
}

void SimplePage::keyPressEvent(QKeyEvent *event)
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

void SimplePage::ChangeFontSize(int delta)
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
