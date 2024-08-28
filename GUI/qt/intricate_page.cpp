/*
 * @Date: 2024-08-28 14:56:49
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-28 15:29:02
 * @FilePath: /CppLLMTranslate/GUI/qt/intricate_page.cpp
 */

#include "intricate_page.h"
#include "MessageManager.h"
#include "agreement.h"

#include <QClipboard>
#include <QApplication>
#include "ConfigManager.h"
#include "StateManager.h"

intricate_page::intricate_page(QWidget *parent) : QMainWindow(parent)
{
    // 设置主窗口的布局
    mainLayout = new QVBoxLayout();

    // 第一行
    modeComboBox = new QComboBox();
    modeComboBox->addItem("英译中");
    modeComboBox->addItem("中译英");
    modeComboBox->addItem("聊天");
    toggleSettingsButton = new QPushButton("简");

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
    connect(toggleSettingsButton, &QPushButton::clicked, this, &intricate_page::onToggleSettingsButtonClicked);

    /********************************************************/
    /********************************************************/

    /********************************************************/
    // 创建聊天页面的水平布局，并添加两个 QTextEdit
    textEdit1 = new QTextEdit();
    textEdit2 = new QTextEdit();
    QHBoxLayout *chatLayout = new QHBoxLayout();
    chatLayout->addWidget(textEdit1);
    chatLayout->addWidget(textEdit2);
    /********************************************************/

    /********************************************************/

    // 创建输入框
    textEdit_system = new QTextEdit();
    textEdit_chat_prefix = new QTextEdit();
    textEdit_chat_suffix = new QTextEdit();
    textEdit_user_msg_1 = new QTextEdit();
    textEdit_user_msg_2 = new QTextEdit();
    textEdit_user_msg_3 = new QTextEdit();
    textEdit_assistant_msg_1 = new QTextEdit();
    textEdit_assistant_msg_2 = new QTextEdit();
    textEdit_assistant_msg_3 = new QTextEdit();

    // 创建标签
    QLabel *set_label1 = new QLabel("系统设置  :");
    QLabel *set_label2 = new QLabel("消息前缀  :");
    QLabel *set_label3 = new QLabel("消息后缀  :");
    QLabel *set_label4 = new QLabel("优质提问1:");
    QLabel *set_label5 = new QLabel("优质回答1:");
    QLabel *set_label6 = new QLabel("优质提问2:");
    QLabel *set_label7 = new QLabel("优质回答2:");
    QLabel *set_label8 = new QLabel("优质提问3:");
    QLabel *set_label9 = new QLabel("优质回答3:");

    // 创建布局管理器
    QVBoxLayout *set_layout = new QVBoxLayout(this);

    // 添加控件到布局管理器

    QHBoxLayout *set_HBoxLayout_1 = new QHBoxLayout();
    set_HBoxLayout_1->addWidget(set_label1);
    set_HBoxLayout_1->addWidget(textEdit_system);
    set_layout->addLayout(set_HBoxLayout_1);

    QHBoxLayout *set_HBoxLayout_2 = new QHBoxLayout();
    set_HBoxLayout_2->addWidget(set_label2);
    set_HBoxLayout_2->addWidget(textEdit_chat_prefix);
    set_layout->addLayout(set_HBoxLayout_2);

    QHBoxLayout *set_HBoxLayout_3 = new QHBoxLayout();
    set_HBoxLayout_3->addWidget(set_label3);
    set_HBoxLayout_3->addWidget(textEdit_chat_suffix);
    set_layout->addLayout(set_HBoxLayout_3);

    QHBoxLayout *set_HBoxLayout_4 = new QHBoxLayout();
    set_HBoxLayout_4->addWidget(set_label4);
    set_HBoxLayout_4->addWidget(textEdit_user_msg_1);
    set_layout->addLayout(set_HBoxLayout_4);

    QHBoxLayout *set_HBoxLayout_5 = new QHBoxLayout();
    set_HBoxLayout_5->addWidget(set_label5);
    set_HBoxLayout_5->addWidget(textEdit_assistant_msg_1);
    set_layout->addLayout(set_HBoxLayout_5);

    QHBoxLayout *set_HBoxLayout_6 = new QHBoxLayout();
    set_HBoxLayout_6->addWidget(set_label6);
    set_HBoxLayout_6->addWidget(textEdit_user_msg_2);
    set_layout->addLayout(set_HBoxLayout_6);

    QHBoxLayout *set_HBoxLayout_7 = new QHBoxLayout();
    set_HBoxLayout_7->addWidget(set_label7);
    set_HBoxLayout_7->addWidget(textEdit_assistant_msg_2);
    set_layout->addLayout(set_HBoxLayout_7);

    QHBoxLayout *set_HBoxLayout_8 = new QHBoxLayout();
    set_HBoxLayout_8->addWidget(set_label8);
    set_HBoxLayout_8->addWidget(textEdit_user_msg_3);
    set_layout->addLayout(set_HBoxLayout_8);

    QHBoxLayout *set_HBoxLayout_9 = new QHBoxLayout();
    set_HBoxLayout_9->addWidget(set_label9);
    set_HBoxLayout_9->addWidget(textEdit_assistant_msg_3);
    set_layout->addLayout(set_HBoxLayout_9);

    /********************************************************/

    // 创建翻译页面的 QWidget 并设置布局
    QWidget *chatTabWidgetPage = new QWidget();
    chatTabWidgetPage->setLayout(chatLayout);

    // 创建翻译页面的设置页面 QWidget 并设置布局
    QWidget *chatTabWidgetPageSet = new QWidget();
    chatTabWidgetPageSet->setLayout(set_layout);

    // 页面布局
    QTabWidget *TabWidget_ = new QTabWidget();
    TabWidget_->addTab(chatTabWidgetPage, "运行");
    TabWidget_->addTab(chatTabWidgetPageSet, "设置");

    QHBoxLayout *TabWidget_Layout = new QHBoxLayout();
    TabWidget_Layout->addWidget(TabWidget_);
    mainLayout->addLayout(TabWidget_Layout);

    /********************************************************/
    /********************************************************/

    // 第四行
    translateButton = new QPushButton("提交🚀 (￣▽￣)σ");
    checkBox = new QCheckBox("剪贴板替换");

    QHBoxLayout *fourthRowLayout = new QHBoxLayout();
    fourthRowLayout->addWidget(translateButton);
    fourthRowLayout->addWidget(checkBox);
    mainLayout->addLayout(fourthRowLayout);

    // 设置布局到中心窗口
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 连接信号和槽
    connect(translateButton, &QPushButton::clicked, this, [this]() {
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

        std::string msg_translate = agreement::getInstance().wrapToJson(info);

        MessageManager::getInstance().pushToOutputQueue(msg_translate);

        textEdit2->clear();
    });

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
        // 槽函数的内容

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
    });

    // 启动定时器，间隔时间为毫秒
    copy_timer->start(500);

    // 启动定时器，间隔时间为毫秒
    translate_timer->start(100);
}

intricate_page::~intricate_page() {}

void intricate_page::onToggleSettingsButtonClicked()
{
    // 切换 简 页面
    StateManager::getInstance().ShowPage = 1;
}
