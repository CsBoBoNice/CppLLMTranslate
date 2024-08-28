#include "start_page.h"
#include "StateManager.h"
#include "agreement.h"

#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

static void UDP_Client_Recv_thread()
{
    StateManager::getInstance().client_p->Recv_thread();
}

static void UDP_Client_Send_thread()
{
    StateManager::getInstance().client_p->Send_thread();
}

start_page::start_page(QWidget *parent) : QMainWindow{parent} {


    // 初始化UI
    firstPage = new QWidget(this);

    ipLineEdit = new QLineEdit("127.0.0.1", this);
    portLineEdit = new QLineEdit("59218", this);
    testButton = new QPushButton("测试", this);
    startButton = new QPushButton("开始", this);
    textEdit = new QTextEdit(this);

    // 页面1布局
    QVBoxLayout *firstPageLayout = new QVBoxLayout(centralWidget());
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

    firstPage->setLayout(firstPageLayout);

    // 设置主窗口的布局
    setCentralWidget(firstPage);



    connect(testButton, &QPushButton::clicked, this, [this]() {
            QString IP = ipLineEdit->text();
            bool ok;
            quint16 Port = portLineEdit->text().toUShort(&ok);

            UI_client_p=StateManager::getInstance().client_p;
            if (!UI_client_p) {
                UI_client_p = new UDP_Client(IP, Port);
                StateManager::getInstance().client_p=UI_client_p;
            }

            // 初始化UDP客户端
            if (!UI_client_p->Initialize(IP, Port)) {
                qDebug() << "Failed to initialize UDP client.";
            }

            agreementInfo info;
            info.cmd = (int)AgreementCmd::test;
            std::string msg_translate = agreement::getInstance().wrapToJson(info);

            // 发送并接受一条消息
            UI_client_p->Send(QString::fromStdString(msg_translate));
            QString recv_mag = UI_client_p->Recv(100);
            std::string show_text = recv_mag.toStdString();
            agreementInfo recv_info = agreement::getInstance().parseJson(show_text);

            QString mag_show = QString::fromStdString(recv_info.msg);
            textEdit->clear();
            textEdit->append(mag_show);

    });

    connect(startButton, &QPushButton::clicked, this, [this]() {
            // 创建UDP客户端实例

            UI_client_p=StateManager::getInstance().client_p;
            if (!UI_client_p) {
                QString IP = ipLineEdit->text();
                bool ok;
                quint16 Port = portLineEdit->text().toUShort(&ok);

                UI_client_p = new UDP_Client(IP, Port);
                StateManager::getInstance().client_p=UI_client_p;
                // 初始化UDP客户端
                if (!UI_client_p->Initialize(IP, Port)) {
                    qDebug() << "Failed to initialize UDP client.";
                }
            }

            std::thread t_UDP_Client_Recv_thread(UDP_Client_Recv_thread);
            std::thread t_UDP_Client_Send_thread(UDP_Client_Send_thread);
            t_UDP_Client_Recv_thread.detach();
            t_UDP_Client_Send_thread.detach();

            // 开始按钮点击后的操作
            // 切换到第二个页面
            StateManager::getInstance().ShowPage=1;

    });

}

