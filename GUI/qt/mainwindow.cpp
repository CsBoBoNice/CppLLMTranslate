// mainwindow.cpp
#include "mainwindow.h"



#include <QApplication>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QProxyStyle>
#include <QPainter>
#include <QTextOption>
#include <QTabBar> // 包含 QStyleOptionTab 的定义
#include <QStyleOption> // 包含 qstyleoption_cast 的定义

#include <QPainter>
#include <QProxyStyle>
#include <QLabel>
#include <QCheckBox>

class CustomTabStyle : public QProxyStyle
{
  public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 200; // 设置每个tabBar中item的大小
            s.rheight() = 100;
        }
        return s;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                QRect allRect = tab->rect;

                if (tab->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(0x89cfff);
                    painter->setBrush(QBrush(0x89cfff));
                    painter->drawRect(allRect.adjusted(6, 6, -6, -6));
                    painter->restore();
                }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);
                if (tab->state & QStyle::State_Selected) {
                    painter->setPen(0xf8fcff);
                }
                else {
                    painter->setPen(0x5d5d5d);
                }

                painter->drawText(allRect, tab->text, option);
                return;
            }
        }

        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
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
    QTabWidget *chatTabWidget = new QTabWidget(secondPage);
    // chatTabWidget->addTab(new QWidget(), "运行");
    // chatTabWidget->addTab(new QWidget(), "设置");
    QTabWidget *copyTranslateTabWidget = new QTabWidget(secondPage);
    copyTranslateTabWidget->addTab(new QWidget(), "运行");
    copyTranslateTabWidget->addTab(new QWidget(), "设置");
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

    // 创建聊天页面的 QWidget 并设置布局
    QWidget *chatTabWidgetPage = new QWidget();
    chatTabWidgetPage->setLayout(chatTabVBoxLayout);

    // 将聊天页面添加到 chatTabWidget
    chatTabWidget->addTab(chatTabWidgetPage, "聊天");
    chatTabWidget->addTab(new QWidget(), "设置");

    // 获取剪贴板对象
    clipboard = QApplication::clipboard();
    // 连接信号和槽
    connect(translateButton, &QPushButton::clicked, this, [this]() {

        QString text = clipboard->text(); // 从剪贴板获取文本
        leftTextEdit->setPlainText(text);
        text+="nuce";
        clipboard->setText(text); // 将文本复制到剪贴板
    });

    tabWidget->addTab(chatTabWidget, "聊天");
    tabWidget->addTab(copyTranslateTabWidget, "拷贝翻译");
    tabWidget->addTab(fileTranslateTabWidget, "文件翻译");

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
}

MainWindow::~MainWindow() {}

void MainWindow::onTestButtonClicked() {
    // 测试按钮点击后的操作
    textEdit->append("测试按钮被点击");
}

void MainWindow::onStartButtonClicked() {
    // 开始按钮点击后的操作
    // 切换到第二个页面
    QStackedLayout *stackedLayout = static_cast<QStackedLayout*>(centralWidget()->layout());
    stackedLayout->setCurrentIndex(1);
}
