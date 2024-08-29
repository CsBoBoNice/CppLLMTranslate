

#include "simple_page.h"
#include "MessageManager.h"
#include "agreement.h"

#include <QClipboard>
#include <QApplication>
#include "ConfigManager.h"
#include "StateManager.h"

simple_page::simple_page(QWidget *parent) : QMainWindow(parent)
{
    // 设置主窗口的布局
    mainLayout = new QVBoxLayout();

    // 第一行
    modeComboBox = new QComboBox();
    modeComboBox->addItem("英译中");
    modeComboBox->addItem("中译英");
    modeComboBox->addItem("聊天");
    toggleSettingsButton = new QPushButton("简");
    toggleSettingsButton->setToolTip("切换到可以设置提示词的页面");

    // 使用lambda表达式连接信号和槽
    connect(modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        qDebug("index=%d", index);
        mode_index = index;
    });

    QHBoxLayout *firstRowLayout = new QHBoxLayout();
    firstRowLayout->addWidget(modeComboBox);
    firstRowLayout->addWidget(toggleSettingsButton);
    mainLayout->addLayout(firstRowLayout);

    // 连接按钮的点击信号到槽函数
    connect(toggleSettingsButton, &QPushButton::clicked, this, &simple_page::onToggleSettingsButtonClicked);

    // 第二行
    textEdit1 = new QTextEdit();
    mainLayout->addWidget(textEdit1);

    // 第三行
    textEdit2 = new QTextEdit();
    mainLayout->addWidget(textEdit2);

    // 第四行
    translateButton = new QPushButton("提交🚀");
    checkBox = new QCheckBox("剪贴板替换");

    // 设置工具提示
    translateButton->setToolTip("(Ctrl+Enter) 组合键也可以提交 \n (Ctrl+)字体变大 (Ctrl-)字体变小");
    checkBox->setToolTip("是否替换剪贴板粘贴 (Ctrl+V) 的内容");

    QHBoxLayout *fourthRowLayout = new QHBoxLayout();
    fourthRowLayout->addWidget(translateButton);
    fourthRowLayout->addWidget(checkBox);
    mainLayout->addLayout(fourthRowLayout);

    // 设置布局到中心窗口
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 连接信号和槽
    connect(translateButton, &QPushButton::clicked, this, &simple_page::SendtoServer);

    // 创建定时器
    copy_timer = new QTimer(this);

    // 连接定时器的timeout信号到槽函数
    connect(copy_timer, &QTimer::timeout, this, [=]() {
        // 槽函数的内容
        // 获取剪贴板对象
        QClipboard *clipboard = QApplication::clipboard();
        static std::string copy_text_last;
        std::string copy_text = clipboard->text().toStdString(); // 从剪贴板获取文本;
        if (copy_text != copy_text_last && !copy_text.empty()) {
            textEdit1->clear();
            textEdit1->append(copy_text.c_str());
            copy_text_last = copy_text;
        }
    });

    // 创建定时器
    translate_timer = new QTimer(this);

    // 连接定时器的timeout信号到槽函数
    connect(translate_timer, &QTimer::timeout, this, [=]() {
        if (StateManager::getInstance().ShowPage == 1) {
            std::string show_text;
            if (MessageManager::getInstance().popFromInputQueueNoWait(show_text)) {

                agreementInfo info = agreement::getInstance().parseJson(show_text);

                if (info.cmd == (int)AgreementCmd::success_msg) {
                    // 完全翻译的信息覆盖
                    textEdit2->clear();
                    textEdit2->append(info.msg.c_str());
                    if (checkBox->isChecked()) {
                        QClipboard *clipboard = QApplication::clipboard();
                        clipboard->setText(info.msg.c_str()); // 将文本复制到剪贴板
                    }
                    QTextCursor cursor = textEdit2->textCursor();
                    cursor.movePosition(QTextCursor::Start); // 移动光标到文本开头
                    textEdit2->setTextCursor(cursor);        // 更新 QTextEdit 的光标位置

                } else if (info.cmd == (int)AgreementCmd::course_msg) {
                    // 过程中的信息追加

                    // // 获取QTextEdit的文本内容
                    // QString currentText = textEdit2->toPlainText();
                    // currentText += info.msg.c_str();
                    // // 设置合并后的文本到QTextEdit
                    // textEdit2->setPlainText(currentText);
                    QTextCursor cursor = textEdit2->textCursor();
                    cursor.movePosition(QTextCursor::End);        // 移动光标到文本末尾
                    textEdit2->setTextCursor(cursor);             // 更新 QTextEdit 的光标位置
                    textEdit2->insertPlainText(info.msg.c_str()); // 插入文本
                    textEdit2->ensureCursorVisible();             // 确保光标可见，即滚动到末尾

                } else {
                    // 其他消息覆盖
                    textEdit2->clear();
                    textEdit2->append(info.msg.c_str());
                }
            }
        }
    });

    // 启动定时器，间隔时间为毫秒
    copy_timer->start(100);

    // 启动定时器，间隔时间为毫秒
    translate_timer->start(1);
}

simple_page::~simple_page() {}

void simple_page::onToggleSettingsButtonClicked()
{
    // 切换 繁 页面
    StateManager::getInstance().ShowPage = 2;
}

void simple_page::SendtoServer()
{
    std::string src_text = textEdit1->toPlainText().toStdString();

    agreementInfo info;

    if (mode_index == 0) {
        // info = agreement::getInstance().default_en_to_zh();
        info = ConfigManager::getInstance().Get_config_en_to_zh();
    } else if (mode_index == 1) {
        // info = agreement::getInstance().default_zh_to_en();
        info = ConfigManager::getInstance().Get_config_zh_to_en();
    } else {
        // info = agreement::getInstance().default_chat();
        info = ConfigManager::getInstance().Get_config_chat();
    }

    info.msg = src_text;
    info.cmd = (int)AgreementCmd::translate_msg;

    std::string msg_translate = agreement::getInstance().wrapToJson(info);

    MessageManager::getInstance().pushToOutputQueue(msg_translate);

    textEdit2->clear();
}

void simple_page::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return && event->modifiers().testFlag(Qt::ControlModifier)) {
        SendtoServer();
    } else if (event->key() == Qt::Key_Equal && event->modifiers().testFlag(Qt::ControlModifier)) {
        ChangeFontSize(1);
    } else if (event->key() == Qt::Key_Minus && event->modifiers().testFlag(Qt::ControlModifier)) {
        ChangeFontSize(-1); // 减小字体大小
    } else {
        QWidget::keyPressEvent(event);
    }
}

void simple_page::ChangeFontSize(int delta)
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
