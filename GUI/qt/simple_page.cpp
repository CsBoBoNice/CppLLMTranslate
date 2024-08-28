




#include "simple_page.h"
#include "MessageManager.h"
#include "agreement.h"

#include <QClipboard>
#include <QApplication>

simple_page::simple_page(QWidget *parent) : QMainWindow(parent)
{
    // 设置主窗口的布局
    mainLayout = new QVBoxLayout();

    // 第一行
    modeComboBox = new QComboBox();
    modeComboBox->addItem("英译中");
    modeComboBox->addItem("中译英");
    modeComboBox->addItem("聊天");
    toggleSettingsButton = new QPushButton("繁");

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
    translateButton = new QPushButton("翻译");
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

        if(mode_index==0)
        {
            info=agreement::getInstance().default_en_to_zh();
        }
        else if(mode_index==1)
        {
            info=agreement::getInstance().default_zh_to_en();
        }
        else
        {
            info=agreement::getInstance().default_chat();
        }

        info.msg = src_text;

        std::string msg_translate = agreement::getInstance().wrapToJson(info);

        MessageManager::getInstance().pushToOutputQueue(msg_translate);
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

            } else if (info.cmd == (int)AgreementCmd::course_msg) {
                // 过程中的信息追加

                // 获取QTextEdit的文本内容
                QString currentText = textEdit2->toPlainText();
                currentText += info.msg.c_str();
                // 设置合并后的文本到QTextEdit
                textEdit2->setPlainText(currentText);

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

simple_page::~simple_page() {}

void simple_page::onToggleSettingsButtonClicked()
{
    // 切换设置按钮点击后的操作
    // 这里只是简单打印一条信息，可以根据实际需求进行扩展
    qDebug("切换设置按钮被点击");
}
