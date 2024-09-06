/*
 * @Date: 2024-09-02 14:46:46
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-06 08:42:27
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
#include <QIntValidator>

#include "ConfigManager.h"
#include "StateManager.h"
#include "HttpManager.h"

#include "ThreadSafeString.h"

FileManager fileManager;

// 进度信息
ThreadSafeString progress_info;

// 待翻译内容
ThreadSafeString translation_content;

// 翻译结果
ThreadSafeString translation_result;

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

        std::ostringstream content_ss;

        if (fileManager.translation_cache.size() > 0) {
            qDebug() << "fileManager.translation_cache.size() > 0";
        } else {
            // qDebug() << "QThread::msleep(1000)";
            QThread::msleep(1000);
            continue;
        }

        if (fileManager.translation_cache.size() > fileManager.m_file_index) {

            qDebug() << "fileManager.translation_cache.size()" << fileManager.translation_cache.size();
            qDebug() << "fileManager.m_file_index" << fileManager.m_file_index + 1;

            content_ss << "file count " << fileManager.translation_cache.size() << "\n";
            content_ss << "file index " << fileManager.m_file_index + 1 << "\n";
            content_ss << "paragraph count " << fileManager.translation_cache[fileManager.m_file_index].content.size()
                       << "\n";
            content_ss << "paragraph index " << fileManager.m_paragraph_index + 1 << "\n";
            content_ss << "file name " << fileManager.translation_cache[fileManager.m_file_index].path.c_str() << "\n";
            content_ss << "translating ..." << "\n";

            // 将ostringstream的内容转换为std::string
            std::string content_str = content_ss.str();
            // 界面输出日志
            progress_info.set(content_str);

            // 原英文未翻译段落
            std::string en_string =
                fileManager.translation_cache[fileManager.m_file_index].content[fileManager.m_paragraph_index];

            agreementInfo prompt_info;
            // 确认文件类型
            FileType file_type =
                fileManager.checkFileType(fileManager.translation_cache[fileManager.m_file_index].path);
            if (file_type == FileType::MD) {
                prompt_info = ConfigManager::getInstance().get_prompt_md_file();
            } else if (file_type == FileType::TXT) {
                prompt_info = ConfigManager::getInstance().get_prompt_txt_file();
            } else if (file_type == FileType::RST) {
                prompt_info = ConfigManager::getInstance().get_prompt_rst_file();
            } else if (file_type == FileType::HPP) {
                prompt_info = ConfigManager::getInstance().get_prompt_h_file();
            }

            prompt_info.cmd = (int)AgreementCmd::translate_msg;
            prompt_info.msg = en_string;

            // 已翻译中文段落
            std::string zh_string;

            HttpManager httpManager_;

            httpManager_.sendRequestAgreementInfo(prompt_info, zh_string);

            // 界面输出日志
            translation_content.set(en_string);
            translation_result.set(zh_string);

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

                TranslationProgressConfig info;

                info.file_index = fileManager.m_file_index;
                info.paragraph_index = fileManager.m_paragraph_index;

                info.paragraph_effective = fileManager.paragraph_effective;
                info.paragraph_min = fileManager.paragraph_min;
                info.paragraph_max = fileManager.paragraph_max;

                info.Input_file_path = fileManager.directory;
                info.Output_file_path = fileManager.directory_output;
                info.Cut_file_path = fileManager.directory_cut;
                info.Reference_file_path = fileManager.directory_en;
                info.Success_file_path = fileManager.directory_ok;

                ConfigManager::getInstance().set_TranslationProgressConfig(info); // 保存进度
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

                // 界面输出日志
                progress_info.set("翻译完成");
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
    paragraph_effective = new QLineEdit(std::to_string(fileManager.paragraph_effective).c_str());
    paragraph_min = new QLineEdit(std::to_string(fileManager.paragraph_min).c_str());
    paragraph_max = new QLineEdit(std::to_string(fileManager.paragraph_max).c_str());

    QIntValidator *validator = new QIntValidator(1, 131072, this); // 限制输入在1到131072之间
    paragraph_effective->setValidator(validator);

    QIntValidator *validator1 = new QIntValidator(1, 131072, this); // 限制输入在1到131072之间
    paragraph_min->setValidator(validator1);

    QIntValidator *validator2 = new QIntValidator(1, 131072, this); // 限制输入在1到131072之间
    paragraph_max->setValidator(validator2);

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

    QHBoxLayout *inputLayout_6 = new QHBoxLayout();
    inputLayout_6->addWidget(new QLabel("段落有效值: "));
    inputLayout_6->addWidget(paragraph_effective);
    infoPageLayout->addLayout(inputLayout_6);

    QHBoxLayout *inputLayout_7 = new QHBoxLayout();
    inputLayout_7->addWidget(new QLabel("段落最小值: "));
    inputLayout_7->addWidget(paragraph_min);
    infoPageLayout->addLayout(inputLayout_7);

    QHBoxLayout *inputLayout_8 = new QHBoxLayout();
    inputLayout_8->addWidget(new QLabel("段落最大值: "));
    inputLayout_8->addWidget(paragraph_max);
    infoPageLayout->addLayout(inputLayout_8);

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

    QPushButton *RstButton = new QPushButton("重置prompt");
    QPushButton *GetButton = new QPushButton("获取prompt");
    QPushButton *SetButton = new QPushButton("更新prompt");

    QHBoxLayout *SetButtonLayout = new QHBoxLayout();
    SetButtonLayout->addWidget(RstButton);
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

            // 更新段落切割参数
            fileManager.paragraph_effective = paragraph_effective->text().toInt();
            fileManager.paragraph_min = paragraph_min->text().toInt();
            fileManager.paragraph_max = paragraph_max->text().toInt();

            TranslationProgressConfig info;

            info.file_index = 0;
            info.paragraph_index = 0;

            info.paragraph_effective = paragraph_effective->text().toInt();
            info.paragraph_min = paragraph_min->text().toInt();
            info.paragraph_max = paragraph_max->text().toInt();

            info.Input_file_path = Input_file_path->text().toStdString();
            info.Output_file_path = Output_file_path->text().toStdString();
            info.Cut_file_path = Cut_file_path->text().toStdString();
            info.Reference_file_path = Reference_file_path->text().toStdString();
            info.Success_file_path = Success_file_path->text().toStdString();

            QString info_path = info.Input_file_path.c_str();
            info_path += "/TranslationProgressConfig.json";

            QFile file(info_path);
            if (file.exists() == true) {
                info = ConfigManager::getInstance().get_TranslationProgressConfig(info_path.toStdString()); // 读取进度
                progress_info.set("文件存在，已读取进度，可继续翻译");

                // 将进度信息显示到UI
                paragraph_effective->setText(QString::number(info.paragraph_effective));
                paragraph_min->setText(QString::number(info.paragraph_min));
                paragraph_max->setText(QString::number(info.paragraph_max));

                Input_file_path->setText(info.Input_file_path.c_str());
                Output_file_path->setText(info.Output_file_path.c_str());
                Cut_file_path->setText(info.Cut_file_path.c_str());
                Reference_file_path->setText(info.Reference_file_path.c_str());
                Success_file_path->setText(info.Success_file_path.c_str());

                fileManager.m_file_index = info.file_index; // 更新翻译进度
                fileManager.m_paragraph_index = 0;

            } else {
                // 删除输出路径
                deleteFolder(Cut_file_path->text());
                deleteFolder(Reference_file_path->text());
                deleteFolder(Success_file_path->text());
                deleteFolder(Output_file_path->text());

                // 切割段落
                fileManager.ProcessFilesCut(info.Input_file_path, info.Cut_file_path, info.Input_file_path);
                fileManager.m_cut_sign = true;

                ConfigManager::getInstance().set_TranslationProgressConfig(info); // 保存进度

                progress_info.set("切割完成");
            }

            fileManager.paragraph_effective = info.paragraph_effective;
            fileManager.paragraph_min = info.paragraph_min;
            fileManager.paragraph_max = info.paragraph_max;

            fileManager.directory = info.Input_file_path;
            fileManager.directory_output = info.Output_file_path;
            fileManager.directory_cut = info.Cut_file_path;
            fileManager.directory_en = info.Reference_file_path;
            fileManager.directory_ok = info.Success_file_path;
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

            // 更新段落切割参数
            fileManager.paragraph_effective = paragraph_effective->text().toInt();
            fileManager.paragraph_min = paragraph_min->text().toInt();
            fileManager.paragraph_max = paragraph_max->text().toInt();

            TranslationProgressConfig info;

            info.file_index = 0;
            info.paragraph_index = 0;

            info.paragraph_effective = paragraph_effective->text().toInt();
            info.paragraph_min = paragraph_min->text().toInt();
            info.paragraph_max = paragraph_max->text().toInt();

            info.Input_file_path = Input_file_path->text().toStdString();
            info.Output_file_path = Output_file_path->text().toStdString();
            info.Cut_file_path = Cut_file_path->text().toStdString();
            info.Reference_file_path = Reference_file_path->text().toStdString();
            info.Success_file_path = Success_file_path->text().toStdString();

            QString info_path = info.Input_file_path.c_str();
            info_path += "/TranslationProgressConfig.json";

            QFile file(info_path);
            if (file.exists() == true) {
                info = ConfigManager::getInstance().get_TranslationProgressConfig(info_path.toStdString()); // 读取进度
                progress_info.set("文件存在，已读取进度，可继续翻译");

                // 将进度信息显示到UI
                paragraph_effective->setText(QString::number(info.paragraph_effective));
                paragraph_min->setText(QString::number(info.paragraph_min));
                paragraph_max->setText(QString::number(info.paragraph_max));

                Input_file_path->setText(info.Input_file_path.c_str());
                Output_file_path->setText(info.Output_file_path.c_str());
                Cut_file_path->setText(info.Cut_file_path.c_str());
                Reference_file_path->setText(info.Reference_file_path.c_str());
                Success_file_path->setText(info.Success_file_path.c_str());

                fileManager.m_file_index = info.file_index; // 更新翻译进度
                fileManager.m_paragraph_index = 0;

            } else {

                if (fileManager.m_cut_sign != true) {

                    // 删除输出路径
                    deleteFolder(Cut_file_path->text());
                    deleteFolder(Reference_file_path->text());
                    deleteFolder(Success_file_path->text());
                    deleteFolder(Output_file_path->text());

                    // 切割段落
                    fileManager.ProcessFilesCut(info.Input_file_path, info.Cut_file_path, info.Input_file_path);
                    fileManager.m_cut_sign = true;
                }

                progress_info.set("切割完成");
            }

            fileManager.paragraph_effective = info.paragraph_effective;
            fileManager.paragraph_min = info.paragraph_min;
            fileManager.paragraph_max = info.paragraph_max;

            fileManager.directory = info.Input_file_path;
            fileManager.directory_output = info.Output_file_path;
            fileManager.directory_cut = info.Cut_file_path;
            fileManager.directory_en = info.Reference_file_path;
            fileManager.directory_ok = info.Success_file_path;

            // 将切割好的段落放入缓冲区
            fileManager.ProcessFilesRecursive(fileManager.directory_cut, fileManager.directory_en,
                                              fileManager.directory_cut, fileManager.translation_cache);

            ConfigManager::getInstance().set_TranslationProgressConfig(info); // 保存进度

            qDebug() << "22222 fileManager.translation_cache.size()" << fileManager.translation_cache.size();

            progress_info.set("开始翻译");
        }
    });

    // 模式选择
    connect(fileTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        if (StateManager::getInstance().ShowPage == 3) {
            qDebug("fileTypeComboBox index=%d", index);
            fileTypeComboBoxIndex = index;
            // 更新提示
            UpdataPrompt(index);
        }
    });

    // 连接按钮的点击信号到槽函数
    connect(GetButton, &QPushButton::clicked, this, [this]() {
        UpdataPrompt(0); // 传0单纯是为了不报错
    });

    connect(SetButton, &QPushButton::clicked, this, [this]() {
        agreementInfo info;

        info.system = textEdit_system->toPlainText().toStdString();
        info.chat_prefix = textEdit_chat_prefix->toPlainText().toStdString();
        info.chat_suffix = textEdit_chat_suffix->toPlainText().toStdString();
        info.user_msg_1 = textEdit_user_msg_1->toPlainText().toStdString();
        info.user_msg_2 = textEdit_user_msg_2->toPlainText().toStdString();
        info.user_msg_3 = textEdit_user_msg_3->toPlainText().toStdString();
        info.assistant_msg_1 = textEdit_assistant_msg_1->toPlainText().toStdString();
        info.assistant_msg_2 = textEdit_assistant_msg_2->toPlainText().toStdString();
        info.assistant_msg_3 = textEdit_assistant_msg_3->toPlainText().toStdString();

        if (fileTypeComboBoxIndex == 0) {
            ConfigManager::getInstance().set_prompt_md_file(info);
        } else if (fileTypeComboBoxIndex == 1) {
            ConfigManager::getInstance().set_prompt_txt_file(info);
        } else if (fileTypeComboBoxIndex == 2) {
            ConfigManager::getInstance().set_prompt_rst_file(info);
        } else if (fileTypeComboBoxIndex == 3) {
            ConfigManager::getInstance().set_prompt_h_file(info);
        }
    });

    connect(RstButton, &QPushButton::clicked, this, [this]() {
        agreementInfo info;

        if (fileTypeComboBoxIndex == 0) {
            info = ConfigManager::getInstance().default_get_prompt_md_file();
            ConfigManager::getInstance().set_prompt_md_file(info);
        } else if (fileTypeComboBoxIndex == 1) {
            info = ConfigManager::getInstance().default_get_prompt_txt_file();
            ConfigManager::getInstance().set_prompt_txt_file(info);
        } else if (fileTypeComboBoxIndex == 2) {
            info = ConfigManager::getInstance().default_get_prompt_rst_file();
            ConfigManager::getInstance().set_prompt_rst_file(info);
        } else if (fileTypeComboBoxIndex == 3) {
            info = ConfigManager::getInstance().default_get_prompt_h_file();
            ConfigManager::getInstance().set_prompt_h_file(info);
        }

        UpdataPrompt(0); // 传0单纯是为了不报错
    });

    // 连接 QTabWidget 的 currentChanged 信号
    QObject::connect(TabWidget_, &QTabWidget::currentChanged, this, &FileTranslation_page::UpdataPrompt);

    // 创建定时器
    translate_timer = new QTimer(this);

    // 连接定时器的timeout信号到槽函数
    connect(translate_timer, &QTimer::timeout, this, [=]() {
        // 完全翻译的信息覆盖
        textEdit1->clear();
        textEdit1->append(translation_content.get().c_str());
        QTextCursor cursor1 = textEdit1->textCursor();
        cursor1.movePosition(QTextCursor::Start); // 移动光标到文本开头
        textEdit1->setTextCursor(cursor1);        // 更新 QTextEdit 的光标位置

        // 完全翻译的信息覆盖
        textEdit2->clear();
        textEdit2->append(translation_result.get().c_str());
        QTextCursor cursor2 = textEdit2->textCursor();
        cursor2.movePosition(QTextCursor::Start); // 移动光标到文本开头
        textEdit2->setTextCursor(cursor2);        // 更新 QTextEdit 的光标位置

        // 完全翻译的信息覆盖
        progressEdit->clear();
        progressEdit->append(progress_info.get().c_str());
        QTextCursor cursor3 = progressEdit->textCursor();
        cursor3.movePosition(QTextCursor::Start); // 移动光标到文本开头
        progressEdit->setTextCursor(cursor3);     // 更新 QTextEdit 的光标位置
    });

    // 启动定时器，间隔时间为毫秒
    translate_timer->start(100);
}

FileTranslation_page::~FileTranslation_page() {}

void FileTranslation_page::updataModeComboBox()
{

    if (StateManager::getInstance().ShowPage == 3) {
        modeComboBox->setCurrentIndex(StateManager::getInstance().ModeIndex);
    }
}

void FileTranslation_page::UpdataPrompt(int index)
{
    agreementInfo info;
    if (fileTypeComboBoxIndex == 0) {
        info = ConfigManager::getInstance().get_prompt_md_file();
    } else if (fileTypeComboBoxIndex == 1) {
        info = ConfigManager::getInstance().get_prompt_txt_file();
    } else if (fileTypeComboBoxIndex == 2) {
        info = ConfigManager::getInstance().get_prompt_rst_file();
    } else if (fileTypeComboBoxIndex == 3) {
        info = ConfigManager::getInstance().get_prompt_h_file();
    }

    textEdit_system->setText(info.system.c_str());
    textEdit_chat_prefix->setText(info.chat_prefix.c_str());
    textEdit_chat_suffix->setText(info.chat_suffix.c_str());
    textEdit_user_msg_1->setText(info.user_msg_1.c_str());
    textEdit_user_msg_2->setText(info.user_msg_2.c_str());
    textEdit_user_msg_3->setText(info.user_msg_3.c_str());
    textEdit_assistant_msg_1->setText(info.assistant_msg_1.c_str());
    textEdit_assistant_msg_2->setText(info.assistant_msg_2.c_str());
    textEdit_assistant_msg_3->setText(info.assistant_msg_3.c_str());
}
