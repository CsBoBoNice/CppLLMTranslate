
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

static void Http_thread()
{
    httpManager.SendRequest_thread();
}

SimplePage::SimplePage(QWidget *parent) : QMainWindow(parent)
{
    // 设置主窗口的布局
    mainVBoxLayout = new QVBoxLayout();

    // 第一行
    translationModeComboBox = new QComboBox();
    translationModeComboBox->addItem("英译中");
    translationModeComboBox->addItem("中译英");
    translationModeComboBox->addItem("聊天");
    translationModeComboBox->addItem("文件翻译");
    toggleSettingsButton = new QPushButton("简");
    toggleSettingsButton->setToolTip("切换到可以设置提示词的页面");

    QHBoxLayout *firstRowLayout = new QHBoxLayout();
    firstRowLayout->addWidget(translationModeComboBox);
    firstRowLayout->addWidget(toggleSettingsButton);
    mainVBoxLayout->addLayout(firstRowLayout);

    // 第二行
    sourceTextEdit = new QTextEdit();
    mainVBoxLayout->addWidget(sourceTextEdit);

    // 第三行
    targetTextEdit = new QTextEdit();
    mainVBoxLayout->addWidget(targetTextEdit);

    // 第四行
    submitTranslationButton = new QPushButton("提交🚀");
    checkBox = new QCheckBox("剪贴板替换");
    reconnectButton = new QPushButton("重连🔗");

    // 设置工具提示
    submitTranslationButton->setToolTip("(Ctrl+Enter) 组合键也可以提交 \n (Ctrl+)字体变大 (Ctrl-)字体变小");
    checkBox->setToolTip("是否替换剪贴板粘贴 (Ctrl+V) 的内容");

    QHBoxLayout *fourthRowLayout = new QHBoxLayout();
    fourthRowLayout->addWidget(submitTranslationButton);
    fourthRowLayout->addWidget(checkBox);
    fourthRowLayout->addWidget(reconnectButton);
    mainVBoxLayout->addLayout(fourthRowLayout);

    // 设置布局到中心窗口
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(mainVBoxLayout);
    setCentralWidget(centralWidget);

    // 创建定时器
    copyTimer = new QTimer(this);

    // 创建定时器
    translateTimer = new QTimer(this);

    // 连接定时器的timeout信号到槽函数
    connect(copyTimer, &QTimer::timeout, this, [=]() {
        // 槽函数的内容
        // 获取剪贴板对象
        QClipboard *clipboard = QApplication::clipboard();
        static std::string copyTextLast;
        std::string copyText = clipboard->text().toStdString(); // 从剪贴板获取文本;
        if (copyText != copyTextLast && !copyText.empty()) {
            sourceTextEdit->clear();
            sourceTextEdit->append(copyText.c_str());
            copyTextLast = copyText;
        }
    });

    // 连接定时器的timeout信号到槽函数
    connect(translateTimer, &QTimer::timeout, this, [=]() {
        if (StateManager::getInstance().ShowPage == 1) {

            std::string showText;
            if (MessageManager::getInstance().popFromInputQueueNoWait(showText)) {

                agreementInfo info = agreement::getInstance().parseJson(showText);

                if (info.cmd == (int)AgreementCmd::success_msg) {
                    // 完全翻译的信息覆盖
                    targetTextEdit->clear();
                    targetTextEdit->append(info.msg.c_str());
                    if (checkBox->isChecked()) {
                        QClipboard *clipboard = QApplication::clipboard();
                        clipboard->setText(info.msg.c_str()); // 将文本复制到剪贴板
                    }
                    QTextCursor cursor = targetTextEdit->textCursor();
                    cursor.movePosition(QTextCursor::Start); // 移动光标到文本开头
                    targetTextEdit->setTextCursor(cursor);        // 更新 QTextEdit 的光标位置

                } else if (info.cmd == (int)AgreementCmd::course_msg) {
                    // 过程中的信息追加

                    // // 获取QTextEdit的文本内容
                    // QString currentText = targetTextEdit->toPlainText();
                    // currentText += info.msg.c_str();
                    // // 设置合并后的文本到QTextEdit
                    // targetTextEdit->setPlainText(currentText);
                    QTextCursor cursor = targetTextEdit->textCursor();
                    cursor.movePosition(QTextCursor::End);        // 移动光标到文本末尾
                    targetTextEdit->setTextCursor(cursor);             // 更新 QTextEdit 的光标位置
                    targetTextEdit->insertPlainText(info.msg.c_str()); // 插入文本
                    targetTextEdit->ensureCursorVisible();             // 确保光标可见，即滚动到末尾

                } else {
                    // 其他消息覆盖
                    targetTextEdit->clear();
                    targetTextEdit->append(info.msg.c_str());
                }
            }
        }
    });

    // 使用lambda表达式连接信号和槽
    connect(translationModeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
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
    connect(toggleSettingsButton, &QPushButton::clicked, this, &SimplePage::OnToggleSettingsButtonClicked);

    // 连接信号和槽
    connect(submitTranslationButton, &QPushButton::clicked, this, &SimplePage::SendToServer);

    // 连接信号和槽
    connect(reconnectButton, &QPushButton::clicked, this, [this]() {
        // 开始按钮点击后的操作
        // 切换到开始页面
        StateManager::getInstance().ShowPage = 0;
        StateManager::getInstance().ModeIndex = 0;
    });

    // 启动定时器，间隔时间为毫秒
    copyTimer->start(100);

    // 启动定时器，间隔时间为毫秒
    translateTimer->start(1);

    std::thread t_HTTP_thread(Http_thread);
    t_HTTP_thread.detach();
}

SimplePage::~SimplePage() {}

void SimplePage::UpdateModeComboBox()
{
    if (StateManager::getInstance().ShowPage == 1) {
        translationModeComboBox->setCurrentIndex(StateManager::getInstance().ModeIndex);
    }
}

void SimplePage::OnToggleSettingsButtonClicked()
{
    // 切换 繁 页面
    StateManager::getInstance().ShowPage = 2;
}

void SimplePage::SendToServer()
{
    std::string srcText = sourceTextEdit->toPlainText().toStdString();

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

    targetTextEdit->clear();
    targetTextEdit->append("Please wait ...");
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
