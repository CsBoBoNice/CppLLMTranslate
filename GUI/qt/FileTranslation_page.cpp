/*
 * @Date: 2024-09-02 14:46:46
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-02 15:17:07
 * @FilePath: /CppLLMTranslate/GUI/qt/FileTranslation_page.cpp
 */
/*
 * @Date: 2024-08-31 13:42:22
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-02 14:44:39
 * @FilePath: /CppLLMTranslate/GUI/qt/FileTranslation_page.cpp
 */
#include "FileTranslation_page.h"
#include "FileManager.h"
#include <QThread>
#include <mutex>
#include "FileManager.h"

#include "MessageManager.h"
#include "agreement.h"

#include <QClipboard>
#include <QApplication>
#include <QDir>
#include "ConfigManager.h"
#include "StateManager.h"
#include "HttpManager.h"

FileManager fileManager;

bool deleteFolder(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        // 文件夹不存在，无需删除
        return true;
    }

    // 遍历文件夹中的所有条目
    QFileInfoList entries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    for (const QFileInfo &entryInfo : entries) {
        if (entryInfo.isDir()) {
            // 如果是文件夹，递归删除
            if (!deleteFolder(entryInfo.absoluteFilePath())) {
                return false; // 如果删除失败，返回false
            }
        } else {
            // 如果是文件，直接删除
            if (!dir.remove(entryInfo.fileName())) {
                return false; // 如果删除失败，返回false
            }
        }
    }

    // 删除当前文件夹
    return dir.rmdir(path);
}

static void FileTranslation_thread()
{
    while (1) {

        if (fileManager.translation_cache.size() > 0) {
            qDebug() << "fileManager.translation_cache.size() > 0";
        } else {
            // qDebug() << "QThread::msleep(1000)";
            QThread::msleep(1000);
            continue;
        }

        if (fileManager.translation_cache.size() > fileManager.m_file_index) {

            qDebug() << "fileManager.translation_cache.size()" << fileManager.translation_cache.size();
            qDebug() << "fileManager.m_file_index" << fileManager.m_file_index;

            // 原英文未翻译段落
            std::string en_string =
                fileManager.translation_cache[fileManager.m_file_index].content[fileManager.m_paragraph_index];

            // // 确认文件类型
            // FileType file_type = fileManager.checkFileType(fileManager.translation_cache[fileManager.m_file_index].path);
            // if (file_type == FileType::RST)
            // {
            //     buffer = fileManager.string_chat_prefix_rst +
            //         fileManager.translation_cache[fileManager.m_file_index].content[g_manager.m_paragraph_index] +
            //         fileManager.string_chat_suffix_rst;
            // }
            // else if (file_type == FileType::HPP)
            // {
            //     buffer = g_manager.string_chat_prefix_hpp +
            //         g_manager.translation_cache[g_manager.m_file_index].content[g_manager.m_paragraph_index] +
            //         g_manager.string_chat_suffix_hpp;
            // }
            // else
            // {
            //     buffer = g_manager.string_chat_prefix_md +
            //         g_manager.translation_cache[g_manager.m_file_index].content[g_manager.m_paragraph_index] +
            //         g_manager.string_chat_suffix_md;
            // }
            agreementInfo info;
            info.cmd = (int)AgreementCmd::translate_msg;
            info.system = "你是专业翻译员，你需要将英文文档翻译成简体中文,翻译后仅输出翻译内容，无需其他解释说明。";
            info.chat_prefix =
                "将英文文档翻译成简体中文,翻译后仅输出翻译内容，无需其他解释说明。\n\n[待翻译内容开始]\n\n";
            info.chat_suffix = "\n[待翻译内容结束]\n\n开始将英文文档翻译成简体中文。\n\n";
            info.user_msg_1 = "Clipboard_Singleton_thread";
            info.user_msg_2 = "getInstance";
            info.user_msg_3 = "Life is actually like the weather, with its sunny days, cloudy days, and occasional "
                              "rain showers. It's "
                              "the natural order of things. Life isn't simple, but we should strive to simplify it as "
                              "much as  possible.";
            info.assistant_msg_1 = "剪贴板单例线程";
            info.assistant_msg_2 = "获得实例";
            info.assistant_msg_3 = "生活其实和天气一样，有晴，有阴，偶尔还会下点雨，自然规律，生活不简单尽量简单过。";
            info.msg = en_string;
            // 已翻译中文段落
            std::string zh_string;

            HttpManager httpManager_;

            httpManager_.sendRequestAgreementInfo(info, zh_string);

            // 输出至文件
            std::string output_to_file_string =
                fileManager.Separator_cut + "\n" + en_string + fileManager.Separator_cut + "\n" + zh_string + "\n";

            // 将翻译好的中文内容替换英文内容
            fileManager.translation_cache[fileManager.m_file_index].content[fileManager.m_paragraph_index] =
                output_to_file_string;

            fileManager.m_paragraph_index++; // 翻译有效进行下一段翻译
            // 文件翻译成功 转到翻译下一个文件
            if (fileManager.m_paragraph_index >=
                fileManager.translation_cache[fileManager.m_file_index].content.size()) {

                // 将翻译内容输出至文件
                fileManager.SaveToFilesystem(fileManager.translation_cache, fileManager.directory_en,
                                             fileManager.m_file_index);

                fileManager.m_file_index++;
                fileManager.m_paragraph_index = 0;
            }
        } else {
            if (fileManager.translation_cache.size() > 0) {
                // 将翻译好的段落放入缓冲区
                fileManager.ProcessFilesRecursive(fileManager.directory_en, fileManager.directory_ok,
                                                  fileManager.directory_en, fileManager.translation_cache);

                // 所有文件翻译完毕 将中文提取放入指定文件夹
                fileManager.SaveTranslatedFiles(fileManager.translation_cache, fileManager.directory_ok);

                // 清除缓冲等待下一次翻译
                fileManager.CleanAll();

                qDebug() << "22222 fileManager.translation_cache.clear()" << fileManager.translation_cache.size();
            }
        }
    }
}

FileTranslation_page::FileTranslation_page(QWidget *parent) : QMainWindow(parent)
{

    // 设置主窗口的布局
    mainLayout = new QVBoxLayout();
    // 第一行
    modeComboBox = new QComboBox();
    modeComboBox->addItem("英译中");
    modeComboBox->addItem("中译英");
    modeComboBox->addItem("聊天");
    modeComboBox->addItem("文件翻译");

    mainLayout->addWidget(modeComboBox);

    /********************************************************/

    cutButton = new QPushButton("切割📏");
    translateButton = new QPushButton("提交🚀");

    QHBoxLayout *RowLayout = new QHBoxLayout();
    RowLayout->addWidget(cutButton);
    RowLayout->addWidget(translateButton);

    QString InputPath = QCoreApplication::applicationDirPath() + "/input";
    QString OutputPath = QCoreApplication::applicationDirPath() + "/output";
    QString CutPath = QCoreApplication::applicationDirPath() + "/output/cut";
    QString ReferencePath = QCoreApplication::applicationDirPath() + "/output/reference";
    QString SuccessPath = QCoreApplication::applicationDirPath() + "/output/success";

    Input_file_path = new QLineEdit(InputPath);
    Output_file_path = new QLineEdit(OutputPath);
    Cut_file_path = new QLineEdit(CutPath);
    Reference_file_path = new QLineEdit(ReferencePath);
    Success_file_path = new QLineEdit(SuccessPath);

    QVBoxLayout *infoPageLayout = new QVBoxLayout();

    QHBoxLayout *inputLayout_1 = new QHBoxLayout();
    inputLayout_1->addWidget(new QLabel("输入路径: "));
    inputLayout_1->addWidget(Input_file_path);
    infoPageLayout->addLayout(inputLayout_1);

    QHBoxLayout *inputLayout_2 = new QHBoxLayout();
    inputLayout_2->addWidget(new QLabel("输出路径: "));
    inputLayout_2->addWidget(Output_file_path);
    infoPageLayout->addLayout(inputLayout_2);

    QHBoxLayout *inputLayout_3 = new QHBoxLayout();
    inputLayout_3->addWidget(new QLabel("切割路径: "));
    inputLayout_3->addWidget(Cut_file_path);
    infoPageLayout->addLayout(inputLayout_3);

    QHBoxLayout *inputLayout_4 = new QHBoxLayout();
    inputLayout_4->addWidget(new QLabel("对照路径: "));
    inputLayout_4->addWidget(Reference_file_path);
    infoPageLayout->addLayout(inputLayout_4);

    QHBoxLayout *inputLayout_5 = new QHBoxLayout();
    inputLayout_5->addWidget(new QLabel("完成路径: "));
    inputLayout_5->addWidget(Success_file_path);
    infoPageLayout->addLayout(inputLayout_5);

    textEdit1 = new QTextEdit();
    textEdit2 = new QTextEdit();
    progressEdit = new QTextEdit();

    QVBoxLayout *progressLayout = new QVBoxLayout();

    QHBoxLayout *chatLayout = new QHBoxLayout();
    chatLayout->addWidget(textEdit1);
    chatLayout->addWidget(textEdit2);

    progressLayout->addWidget(progressEdit);
    progressLayout->addLayout(chatLayout);

    QVBoxLayout *filePatLayout = new QVBoxLayout();
    filePatLayout->addLayout(RowLayout);
    filePatLayout->addLayout(infoPageLayout);
    filePatLayout->addLayout(progressLayout);

    // 创建页面并设置布局
    QWidget *progressWidgetPage = new QWidget();
    progressWidgetPage->setLayout(filePatLayout);

    /********************************************************/

    /********************************************************/

    fileTypeComboBox = new QComboBox();
    fileTypeComboBox->addItem(".md");
    fileTypeComboBox->addItem(".txt");
    fileTypeComboBox->addItem(".rst");
    fileTypeComboBox->addItem(".h");

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
    QVBoxLayout *set_layout = new QVBoxLayout();

    set_layout->addWidget(fileTypeComboBox);

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

    QPushButton *GetButton = new QPushButton("获取配置");
    QPushButton *SetButton = new QPushButton("更新配置");

    QHBoxLayout *SetButtonLayout = new QHBoxLayout();
    SetButtonLayout->addWidget(GetButton);
    SetButtonLayout->addWidget(SetButton);
    set_layout->addLayout(SetButtonLayout);

    // 创建页面并设置布局
    QWidget *promptsWidgetPage = new QWidget();
    promptsWidgetPage->setLayout(set_layout);

    /********************************************************/

    // 页面布局
    QTabWidget *TabWidget_ = new QTabWidget();
    TabWidget_->addTab(progressWidgetPage, "进度");
    TabWidget_->addTab(promptsWidgetPage, "提示");

    QHBoxLayout *TabWidget_Layout = new QHBoxLayout();
    TabWidget_Layout->addWidget(TabWidget_);

    mainLayout->addLayout(TabWidget_Layout);

    // 设置布局到中心窗口
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    std::thread t_FileTranslation_thread(FileTranslation_thread);
    t_FileTranslation_thread.detach();

    // 模式选择
    connect(modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        if (StateManager::getInstance().ShowPage == 3) {
            qDebug("FileTranslation_page index=%d", index);
            StateManager::getInstance().ModeIndex = index;
            if (index != 3) {
                // 切换页面
                StateManager::getInstance().ShowPage = 1;
                modeComboBox->setCurrentIndex(index);
            }
        }
    });

    // 切割按钮按下
    connect(cutButton, &QPushButton::clicked, this, [this]() {
        if (fileManager.translation_cache.size() == 0) {

            // 更新文件路径
            fileManager.directory = Input_file_path->text().toStdString();
            fileManager.directory_output = Output_file_path->text().toStdString();
            fileManager.directory_cut = Cut_file_path->text().toStdString();
            fileManager.directory_en = Reference_file_path->text().toStdString();
            fileManager.directory_ok = Success_file_path->text().toStdString();

            // 删除输出路径
            deleteFolder(Cut_file_path->text());
            deleteFolder(Reference_file_path->text());
            deleteFolder(Success_file_path->text());
            deleteFolder(Output_file_path->text());

            fileManager.ProcessFilesCut(fileManager.directory, fileManager.directory_cut,
                                        fileManager.directory); // 切割段落
            fileManager.m_cut_sign = true;
        }
    });

    // 翻译按钮按下
    connect(translateButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "111111 fileManager.translation_cache.size()" << fileManager.translation_cache.size();
        if (fileManager.translation_cache.size() == 0) {

            // 更新文件路径
            fileManager.directory = Input_file_path->text().toStdString();
            fileManager.directory_output = Output_file_path->text().toStdString();
            fileManager.directory_cut = Cut_file_path->text().toStdString();
            fileManager.directory_en = Reference_file_path->text().toStdString();
            fileManager.directory_ok = Success_file_path->text().toStdString();

            if (fileManager.m_cut_sign != true) {
                // 删除输出路径
                deleteFolder(Cut_file_path->text());
                deleteFolder(Reference_file_path->text());
                deleteFolder(Success_file_path->text());
                deleteFolder(Output_file_path->text());

                fileManager.ProcessFilesCut(fileManager.directory, fileManager.directory_cut,
                                            fileManager.directory); // 切割段落
            }
            // 将切割好的段落放入缓冲区
            fileManager.ProcessFilesRecursive(fileManager.directory_cut, fileManager.directory_en,
                                              fileManager.directory_cut, fileManager.translation_cache);

            qDebug() << "22222 fileManager.translation_cache.size()" << fileManager.translation_cache.size();
        }
    });
}

FileTranslation_page::~FileTranslation_page() {}

void FileTranslation_page::updataModeComboBox()
{

    if (StateManager::getInstance().ShowPage == 3) {
        modeComboBox->setCurrentIndex(StateManager::getInstance().ModeIndex);
    }
}
