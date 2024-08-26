// mainwindow.cpp
#include "mainwindow.h"
#include "CustomTabStyle.h"

#include "UDP_Client.h"
#include <QThread>
#include "agreement.h"
#include <QTimer>
#include "MessageManager.h"

UDP_Client *client_p = nullptr;

void UDP_Client_Recv_thread()
{
    client_p->Recv_thread();
}

void UDP_Client_Send_thread()
{
    client_p->Send_thread();
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 创建定时器
    copy_timer = new QTimer(this);

    // 连接定时器的timeout信号到槽函数
    connect(copy_timer, &QTimer::timeout, this, [=]() {
        // 槽函数的内容

        static std::string copy_text_last;
        std::string copy_text = clipboard->text().toStdString(); // 从剪贴板获取文本;
        if (copy_text != copy_text_last) {
            leftTextEdit->clear();
            leftTextEdit->append(copy_text.c_str());
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
                rightTextEdit->clear();
                rightTextEdit->append(info.msg.c_str());

            } else if (info.cmd == (int)AgreementCmd::course_msg) {
                // 过程中的信息追加
                // rightTextEdit->append(info.msg.c_str());

                // 获取QTextEdit的文本内容
                QString currentText = rightTextEdit->toPlainText();
                currentText += info.msg.c_str();
                // 设置合并后的文本到QTextEdit
                rightTextEdit->setPlainText(currentText);

            } else {
                // 其他消息覆盖
                rightTextEdit->clear();
                rightTextEdit->append(info.msg.c_str());
            }
        }

    });




    // 初始化UI
    firstPage = new QWidget(this);
    secondPage = new QWidget(this);

    ipLineEdit = new QLineEdit("127.0.0.1", this);
    portLineEdit = new QLineEdit("59218", this);
    testButton = new QPushButton("测试", this);
    startButton = new QPushButton("开始", this);
    textEdit = new QTextEdit(this);

    tabWidget = new QTabWidget(this);
    // 设置选项卡的位置为左侧
    tabWidget->setTabPosition(QTabWidget::West);
    // 自定义选项卡的样式
    tabWidget->tabBar()->setStyle(new CustomTabStyle);

    // 页面1布局
    QVBoxLayout *firstPageLayout = new QVBoxLayout(firstPage);
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->addWidget(new QLabel("IP: ", this));
    inputLayout->addWidget(ipLineEdit);
    inputLayout->addWidget(new QLabel("Port: ", this));
    inputLayout->addWidget(portLineEdit);
    firstPageLayout->addLayout(inputLayout);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(testButton);
    buttonLayout->addWidget(startButton);
    firstPageLayout->addLayout(buttonLayout);
    firstPageLayout->addWidget(textEdit);

    // 页面2布局
    QTabWidget *copyTranslateTabWidget = new QTabWidget(secondPage);
    // copyTranslateTabWidget->addTab(new QWidget(), "运行");
    // copyTranslateTabWidget->addTab(new QWidget(), "设置");
    QTabWidget *fileTranslateTabWidget = new QTabWidget(secondPage);
    fileTranslateTabWidget->addTab(new QWidget(), "运行");
    fileTranslateTabWidget->addTab(new QWidget(), "设置");

    // 创建聊天页面的左右两个 QTextEdit
    leftTextEdit = new QTextEdit(secondPage);
    rightTextEdit = new QTextEdit(secondPage);

    // 创建翻译按钮和剪贴板替换复选框
    QPushButton *translateButton = new QPushButton("翻译", secondPage);
    QCheckBox *clipboardReplaceCheckBox = new QCheckBox("剪贴板替换", secondPage);

    // 创建聊天页面的水平布局，并添加两个 QTextEdit
    QHBoxLayout *chatLayout = new QHBoxLayout();
    chatLayout->addWidget(leftTextEdit);
    chatLayout->addWidget(rightTextEdit);

    // 创建按钮和复选框的布局，并添加到聊天页面下方
    QHBoxLayout *buttonLayout_chat = new QHBoxLayout();
    buttonLayout_chat->addWidget(translateButton);
    buttonLayout_chat->addWidget(clipboardReplaceCheckBox);

    // 创建垂直布局，并添加聊天布局和按钮布局
    QVBoxLayout *chatTabVBoxLayout = new QVBoxLayout();
    chatTabVBoxLayout->addLayout(chatLayout);
    chatTabVBoxLayout->addLayout(buttonLayout_chat);

    // 创建翻译页面的 QWidget 并设置布局
    QWidget *chatTabWidgetPage = new QWidget();
    chatTabWidgetPage->setLayout(chatTabVBoxLayout);

    // 将翻译页面添加到 copyTranslateTabWidget
    copyTranslateTabWidget->addTab(chatTabWidgetPage, "翻译");
    copyTranslateTabWidget->addTab(new QWidget(), "设置");

    // 获取剪贴板对象
    clipboard = QApplication::clipboard();
    // 连接信号和槽
    connect(translateButton, &QPushButton::clicked, this, [this]() {

        // QString text = clipboard->text(); // 从剪贴板获取文本
        // leftTextEdit->setPlainText(text);
        // text+="nuce";
        // clipboard->setText(text); // 将文本复制到剪贴板

        std::string src_text = leftTextEdit->toPlainText().toStdString();

        agreementInfo info;
        info.cmd = (int)AgreementCmd::translate_msg;
        info.msg = src_text;
        info.system = "你是专业翻译员，你需要将英文文档翻译成简体中文,翻译后仅输出翻译内容，无需其他解释说明。";
        info.chat_prefix = "将英文文档翻译成简体中文,翻译后仅输出翻译内容，无需其他解释说明。\n\n[待翻译内容开始]\n\n";
        info.chat_suffix = "\n[待翻译内容结束]\n\n开始将英文文档翻译成简体中文。\n\n";
        info.user_msg_1 = "Clipboard_Singleton_thread";
        info.user_msg_2 = "getInstance";
        info.user_msg_3 =
            "Life is actually like the weather, with its sunny days, cloudy days, and occasional rain showers. It's "
            "the natural order of things. Life isn't simple, but we should strive to simplify it as much as  possible.";
        info.assistant_msg_1 = "剪贴板单例线程";
        info.assistant_msg_2 = "获得实例";
        info.assistant_msg_3 = "生活其实和天气一样，有晴，有阴，偶尔还会下点雨，自然规律，生活不简单尽量简单过。";

        std::string msg_translate = agreement::getInstance().wrapToJson(info);

        MessageManager::getInstance().pushToOutputQueue(msg_translate);

    });

    tabWidget->addTab(copyTranslateTabWidget, "拷贝翻译");
    tabWidget->addTab(fileTranslateTabWidget, "聊天");

    QVBoxLayout *secondPageLayout = new QVBoxLayout(secondPage);
    secondPageLayout->addWidget(tabWidget);

    // 堆叠布局
    QStackedLayout *stackedLayout = new QStackedLayout();
    stackedLayout->addWidget(firstPage);
    stackedLayout->addWidget(secondPage);

    // 设置主窗口的布局
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(stackedLayout);
    setCentralWidget(centralWidget);

    // 连接信号和槽
    connect(testButton, &QPushButton::clicked, this, &MainWindow::onTestButtonClicked);
    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartButtonClicked);


    // 启动定时器，间隔时间为毫秒
    copy_timer->start(500);

    // 启动定时器，间隔时间为毫秒
    translate_timer->start(100);
}

MainWindow::~MainWindow() {}



void MainWindow::onTestButtonClicked() {

    QString IP=ipLineEdit->text();
    bool ok;
    quint16 Port=portLineEdit->text().toUShort(&ok);

    if(!client_p)
    {
        client_p = new UDP_Client(IP, Port);
        UI_client_p=client_p;
    }

    // 初始化UDP客户端
    if (!client_p->Initialize(IP, Port)) {
        qDebug()   << "Failed to initialize UDP client.";
    }

    agreementInfo info;
    info.cmd = (int)AgreementCmd::test;
    std::string msg_translate = agreement::getInstance().wrapToJson(info);

    // 发送并接受一条消息
    client_p->Send(QString::fromStdString(msg_translate));
    QString recv_mag=client_p->Recv(100);
    std::string show_text=recv_mag.toStdString();
    agreementInfo recv_info = agreement::getInstance().parseJson(show_text);

    QString mag_show=QString::fromStdString(recv_info.msg);
    textEdit->clear();
    textEdit->append(mag_show);

}

void MainWindow::onStartButtonClicked() {

    // 创建UDP客户端实例

    if(!client_p)
    {
        QString IP=ipLineEdit->text();
        bool ok;
        quint16 Port=portLineEdit->text().toUShort(&ok);

        client_p = new UDP_Client(IP, Port);
        UI_client_p=client_p;
        // 初始化UDP客户端
        if (!client_p->Initialize(IP, Port)) {
            qDebug()   << "Failed to initialize UDP client.";
        }
    }
    std::thread t_UDP_Client_Recv_thread(UDP_Client_Recv_thread);
    std::thread t_UDP_Client_Send_thread(UDP_Client_Send_thread);
    t_UDP_Client_Recv_thread.detach();
    t_UDP_Client_Send_thread.detach();


    // 开始按钮点击后的操作
    // 切换到第二个页面
    QStackedLayout *stackedLayout = static_cast<QStackedLayout*>(centralWidget()->layout());
    stackedLayout->setCurrentIndex(1);
}
