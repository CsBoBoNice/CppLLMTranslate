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

#include "CommonUtils.h"

#include <QMainWindow>
#include <QComboBox>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTabWidget>
#include <QTimer>
#include <QDebug>

using namespace std;

FileManager fileManager;

// 进度信息
ThreadSafeString progress_info;
std::string progress_info_last;

// 待翻译内容
ThreadSafeString translation_content;
std::string translation_content_last;

// 翻译结果
ThreadSafeString translation_result;
std::string translation_result_last;

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
            progress_info.Set(content_str);

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

            httpManager_.SendRequestAgreementInfo(prompt_info, zh_string);

            // 界面输出日志
            translation_content.Set(en_string);
            translation_result.Set(zh_string);

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

                ConfigManager::getInstance().set_TranslationProgressConfig(info,
                                                                           fileManager.directory.string()); // 保存进度
            }
        } else {
            if (fileManager.translation_cache.size() > 0) {
                // 将翻译好的段落放入缓冲区
                fileManager.ReadTranslated(fileManager.directory_en, fileManager.directory_ok, fileManager.directory_en,
                                           fileManager.translation_cache);

                // 所有文件翻译完毕 将中文提取放入指定文件夹
                fileManager.SaveTranslatedFiles(fileManager.translation_cache, fileManager.directory_ok);

                // 清除缓冲等待下一次翻译
                fileManager.CleanAll();

                qDebug() << "22222 fileManager.translation_cache.clear()" << fileManager.translation_cache.size();

                // 界面输出日志
                progress_info.Set("翻译完成");
            }
        }
    }
}

FileTranslationPage::FileTranslationPage(QWidget *parent) : QMainWindow(parent)
{

    // 设置主窗口的布局
    m_mainLayout = new QVBoxLayout();
    // 第一行
    m_modeComboBox = new QComboBox();
    m_modeComboBox->addItem("英译中");
    m_modeComboBox->addItem("中译英");
    m_modeComboBox->addItem("聊天");
    m_modeComboBox->addItem("文件翻译");

    m_mainLayout->addWidget(m_modeComboBox);

    /********************************************************/

    m_cutButton = new QPushButton("切割📏");
    m_translateButton = new QPushButton("提交🚀");

    QHBoxLayout *rowLayout = new QHBoxLayout();
    rowLayout->addWidget(m_cutButton);
    rowLayout->addWidget(m_translateButton);

    // 读取配置文件
    TranslationSetInfo translationSetInfo = ConfigManager::getInstance().get_TranslationSetInfo();

    // 创建输入字段
    QStringList labels = {"输入路径", "输出路径", "切割路径", "对照路径", "完成路径"};
    QStringList paths = {translationSetInfo.Input_file_path.c_str(), translationSetInfo.Output_file_path.c_str(),
                         translationSetInfo.Cut_file_path.c_str(), translationSetInfo.Reference_file_path.c_str(),
                         translationSetInfo.Success_file_path.c_str()};

    QVBoxLayout *infoPageLayout = new QVBoxLayout();

    m_inputFilePath = new QLineEdit(this);
    m_outputFilePath = new QLineEdit(this);
    m_cutFilePath = new QLineEdit(this);
    m_referenceFilePath = new QLineEdit(this);
    m_successFilePath = new QLineEdit(this);

    QVector<QLineEdit*> lineEdits = {m_inputFilePath, m_outputFilePath, m_cutFilePath, m_referenceFilePath, m_successFilePath};

    for (int i = 0; i < labels.size(); ++i) {
        QHBoxLayout *inputLayout = new QHBoxLayout();
        inputLayout->addWidget(new QLabel(labels[i] + ": "));
        lineEdits[i]->setText(paths[i]);
        inputLayout->addWidget(lineEdits[i]);
        infoPageLayout->addLayout(inputLayout);
    }

    // 段落设置
    m_paragraphEffective = new QLineEdit(this);
    m_paragraphMin = new QLineEdit(this);
    m_paragraphMax = new QLineEdit(this);

    QStringList paragraphLabels = {"段落有效值", "段落最小值", "段落最大值"};
    QVector<QLineEdit*> paragraphLineEdits = {m_paragraphEffective, m_paragraphMin, m_paragraphMax};
    QStringList paragraphValues = {QString::number(translationSetInfo.paragraph_effective),
                                   QString::number(translationSetInfo.paragraph_min),
                                   QString::number(translationSetInfo.paragraph_max)};

    for (int i = 0; i < paragraphLabels.size(); ++i) {
        QHBoxLayout *inputLayout = new QHBoxLayout();
        inputLayout->addWidget(new QLabel(paragraphLabels[i] + ": "));
        paragraphLineEdits[i]->setText(paragraphValues[i]);
        paragraphLineEdits[i]->setValidator(new QIntValidator(1, 131072, this));
        inputLayout->addWidget(paragraphLineEdits[i]);
        infoPageLayout->addLayout(inputLayout);
    }

    m_resetButton = new QPushButton("恢复默认🔄");
    m_cleanButton = new QPushButton("清理翻译文件🗑️");
    m_reconnectButton = new QPushButton("重连🔗");

    QHBoxLayout *rowLayout2 = new QHBoxLayout();
    rowLayout2->addWidget(m_resetButton);
    rowLayout2->addWidget(m_cleanButton);
    rowLayout2->addWidget(m_reconnectButton);
    infoPageLayout->addLayout(rowLayout2);

    m_textEdit1 = new QTextEdit();
    m_textEdit2 = new QTextEdit();
    m_progressEdit = new QTextEdit();

    QVBoxLayout *progressLayout = new QVBoxLayout();

    QHBoxLayout *chatLayout = new QHBoxLayout();
    chatLayout->addWidget(m_textEdit1);
    chatLayout->addWidget(m_textEdit2);

    progressLayout->addWidget(m_progressEdit);
    progressLayout->addLayout(chatLayout);

    QVBoxLayout *filePatLayout = new QVBoxLayout();
    filePatLayout->addLayout(rowLayout);
    filePatLayout->addLayout(infoPageLayout);
    filePatLayout->addLayout(progressLayout);

    // 创建页面并设置布局
    QWidget *progressWidgetPage = new QWidget();
    progressWidgetPage->setLayout(filePatLayout);

    /********************************************************/

    /********************************************************/

    m_fileTypeComboBox = new QComboBox();
    QStringList fileTypes = {".md", ".txt", ".rst", ".h"};
    m_fileTypeComboBox->addItems(fileTypes);

    m_textEditSystem = new QTextEdit(this);
    m_textEditChatPrefix = new QTextEdit(this);
    m_textEditChatSuffix = new QTextEdit(this);
    m_textEditUserMsg1 = new QTextEdit(this);
    m_textEditUserMsg2 = new QTextEdit(this);
    m_textEditUserMsg3 = new QTextEdit(this);
    m_textEditAssistantMsg1 = new QTextEdit(this);
    m_textEditAssistantMsg2 = new QTextEdit(this);
    m_textEditAssistantMsg3 = new QTextEdit(this);

    QVector<QTextEdit*> textEdits = {m_textEditSystem,        m_textEditChatPrefix,    m_textEditChatSuffix,
                                      m_textEditUserMsg1,      m_textEditAssistantMsg1, m_textEditUserMsg2,
                                      m_textEditAssistantMsg2, m_textEditUserMsg3,      m_textEditAssistantMsg3};

    QStringList labels2 = {"系统设置  :", "消息前缀  :", "消息后缀  :", "优质提问1:", "优质回答1:",
                           "优质提问2:",  "优质回答2:",  "优质提问3:",  "优质回答3:"};

    QVBoxLayout *setLayout = new QVBoxLayout();
    setLayout->addWidget(m_fileTypeComboBox);

    for (int i = 0; i < textEdits.size(); ++i) {
        if (textEdits[i] == nullptr) {
            textEdits[i] = new QTextEdit();
        }
        QHBoxLayout *hBoxLayout = new QHBoxLayout();
        hBoxLayout->addWidget(new QLabel(labels2[i]));
        hBoxLayout->addWidget(textEdits[i]);
        setLayout->addLayout(hBoxLayout);
    }

    QPushButton *rstButton = new QPushButton("重置prompt");
    QPushButton *getButton = new QPushButton("获取prompt");
    QPushButton *setButton = new QPushButton("更新prompt");

    QHBoxLayout *setButtonLayout = new QHBoxLayout();
    setButtonLayout->addWidget(rstButton);
    setButtonLayout->addWidget(getButton);
    setButtonLayout->addWidget(setButton);
    setLayout->addLayout(setButtonLayout);

    QWidget *promptsWidgetPage = new QWidget();
    promptsWidgetPage->setLayout(setLayout);

    /********************************************************/

    // 页面布局
    QTabWidget *tabWidget = new QTabWidget();
    tabWidget->addTab(progressWidgetPage, "进度");
    tabWidget->addTab(promptsWidgetPage, "提示");

    QHBoxLayout *tabWidgetLayout = new QHBoxLayout();
    tabWidgetLayout->addWidget(tabWidget);

    m_mainLayout->addLayout(tabWidgetLayout);

    // 设置布局到中心窗口
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(m_mainLayout);
    setCentralWidget(centralWidget);

    std::thread tFileTranslationThread(FileTranslation_thread);
    tFileTranslationThread.detach();

    // 模式选择
    connect(m_modeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        if (StateManager::getInstance().ShowPage == 3) {
            qDebug("FileTranslationPage index=%d", index);
            StateManager::getInstance().ModeIndex = index;
            if (index != 3) {
                // 切换页面
                StateManager::getInstance().ShowPage = 1;
                m_modeComboBox->setCurrentIndex(index);
            }
        }
    });

    // 切割按钮按下
    connect(m_cutButton, &QPushButton::clicked, this, [this]() {
        if (fileManager.translation_cache.size() == 0) {

            // 更新文件路径
            fileManager.directory = m_inputFilePath->text().toStdString();
            fileManager.directory_output = m_outputFilePath->text().toStdString();
            fileManager.directory_cut = m_cutFilePath->text().toStdString();
            fileManager.directory_en = m_referenceFilePath->text().toStdString();
            fileManager.directory_ok = m_successFilePath->text().toStdString();

            // 更新段落切割参数
            fileManager.paragraph_effective = m_paragraphEffective->text().toInt();
            fileManager.paragraph_min = m_paragraphMin->text().toInt();
            fileManager.paragraph_max = m_paragraphMax->text().toInt();

            TranslationProgressConfig progressInfo;

            progressInfo.file_index = 0;
            progressInfo.paragraph_index = 0;

            TranslationSetInfo translationSetInfo;

            translationSetInfo.paragraph_effective = m_paragraphEffective->text().toInt();
            translationSetInfo.paragraph_min = m_paragraphMin->text().toInt();
            translationSetInfo.paragraph_max = m_paragraphMax->text().toInt();

            translationSetInfo.Input_file_path = m_inputFilePath->text().toStdString();
            translationSetInfo.Output_file_path = m_outputFilePath->text().toStdString();
            translationSetInfo.Cut_file_path = m_cutFilePath->text().toStdString();
            translationSetInfo.Reference_file_path = m_referenceFilePath->text().toStdString();
            translationSetInfo.Success_file_path = m_successFilePath->text().toStdString();

            ConfigManager::getInstance().set_TranslationSetInfo(translationSetInfo); // 保存设置

            QString progressInfoPath = m_inputFilePath->text() + "/TranslationProgressConfig.json";

            QFile file(progressInfoPath);
            if (file.exists() == true) {
                progressInfo = ConfigManager::getInstance().get_TranslationProgressConfig(
                    progressInfoPath.toStdString()); // 读取进度
                progress_info.Set("文件存在，已读取进度，可继续翻译");

                fileManager.m_file_index = progressInfo.file_index; // 更新翻译进度
                fileManager.m_paragraph_index = 0;

            } else {
                // 删除输出路径
                deleteFolder(m_cutFilePath->text());
                deleteFolder(m_referenceFilePath->text());
                deleteFolder(m_successFilePath->text());
                deleteFolder(m_outputFilePath->text());

                // 切割段落
                fileManager.ProcessFilesCut(translationSetInfo.Input_file_path, translationSetInfo.Cut_file_path,
                                            translationSetInfo.Input_file_path);
                fileManager.m_cut_sign = true;

                ConfigManager::getInstance().set_TranslationProgressConfig(
                    progressInfo,
                    translationSetInfo.Input_file_path); // 保存进度

                progress_info.Set("切割完成");
            }
        }
    });

    // 翻译按钮按下
    connect(m_translateButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "111111 fileManager.translation_cache.size()" << fileManager.translation_cache.size();
        if (fileManager.translation_cache.size() == 0) {

            // 更新文件路径
            fileManager.directory = m_inputFilePath->text().toStdString();
            fileManager.directory_output = m_outputFilePath->text().toStdString();
            fileManager.directory_cut = m_cutFilePath->text().toStdString();
            fileManager.directory_en = m_referenceFilePath->text().toStdString();
            fileManager.directory_ok = m_successFilePath->text().toStdString();

            // 更新段落切割参数
            fileManager.paragraph_effective = m_paragraphEffective->text().toInt();
            fileManager.paragraph_min = m_paragraphMin->text().toInt();
            fileManager.paragraph_max = m_paragraphMax->text().toInt();

            TranslationProgressConfig progressInfo;

            progressInfo.file_index = 0;
            progressInfo.paragraph_index = 0;

            TranslationSetInfo translationSetInfo;

            translationSetInfo.paragraph_effective = m_paragraphEffective->text().toInt();
            translationSetInfo.paragraph_min = m_paragraphMin->text().toInt();
            translationSetInfo.paragraph_max = m_paragraphMax->text().toInt();

            translationSetInfo.Input_file_path = m_inputFilePath->text().toStdString();
            translationSetInfo.Output_file_path = m_outputFilePath->text().toStdString();
            translationSetInfo.Cut_file_path = m_cutFilePath->text().toStdString();
            translationSetInfo.Reference_file_path = m_referenceFilePath->text().toStdString();
            translationSetInfo.Success_file_path = m_successFilePath->text().toStdString();

            ConfigManager::getInstance().set_TranslationSetInfo(translationSetInfo); // 保存设置

            QString progressInfoPath = m_inputFilePath->text() + "/TranslationProgressConfig.json";

            QFile file(progressInfoPath);
            if (file.exists() == true) {
                progressInfo = ConfigManager::getInstance().get_TranslationProgressConfig(
                    progressInfoPath.toStdString()); // 读取进度
                progress_info.Set("文件存在，已读取进度，可继续翻译");

                fileManager.m_file_index = progressInfo.file_index; // 更新翻译进度
                fileManager.m_paragraph_index = 0;

            } else {

                if (fileManager.m_cut_sign != true) {

                    // 删除输出路径
                    deleteFolder(m_cutFilePath->text());
                    deleteFolder(m_referenceFilePath->text());
                    deleteFolder(m_successFilePath->text());
                    deleteFolder(m_outputFilePath->text());

                    // 切割段落
                    fileManager.ProcessFilesCut(translationSetInfo.Input_file_path, translationSetInfo.Cut_file_path,
                                                translationSetInfo.Input_file_path);
                    fileManager.m_cut_sign = true;
                }

                progress_info.Set("切割完成");

                fileManager.m_file_index = 0;
                fileManager.m_paragraph_index = 0;
            }

            // 将切割好的段落放入缓冲区
            fileManager.ProcessFilesRecursive(fileManager.directory_cut, fileManager.directory_en,
                                                fileManager.directory_cut, fileManager.translation_cache);

            ConfigManager::getInstance().set_TranslationProgressConfig(progressInfo,
                                                                     translationSetInfo.Input_file_path); // 保存进度

            qDebug() << "22222 fileManager.translation_cache.size()" << fileManager.translation_cache.size();

            progress_info.Set("开始翻译");
        }
    });

    // 模式选择
    connect(m_fileTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        if (StateManager::getInstance().ShowPage == 3) {
            qDebug("fileTypeComboBox index=%d", index);
            m_fileTypeComboBoxIndex = index;
            // 更新提示
            UpdatePrompt(index);
        }
    });

    // 恢复默认
    connect(m_resetButton, &QPushButton::clicked, this, [this]() {
        if (fileManager.translation_cache.size() == 0) {

            TranslationSetInfo info = ConfigManager::getInstance().default_get_TranslationSetInfo();
            ConfigManager::getInstance().set_TranslationSetInfo(info);

            progress_info.Set("已恢复默认");

            // 将进信息显示到UI
            m_paragraphEffective->setText(QString::number(info.paragraph_effective));
            m_paragraphMin->setText(QString::number(info.paragraph_min));
            m_paragraphMax->setText(QString::number(info.paragraph_max));

            m_inputFilePath->setText(info.Input_file_path.c_str());
            m_outputFilePath->setText(info.Output_file_path.c_str());
            m_cutFilePath->setText(info.Cut_file_path.c_str());
            m_referenceFilePath->setText(info.Reference_file_path.c_str());
            m_successFilePath->setText(info.Success_file_path.c_str());
        }
    });

    // 清理翻译文件
    connect(m_cleanButton, &QPushButton::clicked, this, [this]() {
        if (fileManager.translation_cache.size() == 0) {
            // 删除输出路径
            deleteFolder(m_cutFilePath->text());
            deleteFolder(m_referenceFilePath->text());
            deleteFolder(m_successFilePath->text());
            deleteFolder(m_outputFilePath->text());

            QString progressInfoPath = m_inputFilePath->text() + "/TranslationProgressConfig.json";

            QFile file(progressInfoPath);
            if (file.exists() == true) {
                file.remove();
            }

            fileManager.m_cut_sign = false;

            progress_info.Set("已清理");
        }
    });

    // 连接按钮的点击信号到槽函数
    connect(getButton, &QPushButton::clicked, this, [this]() {
        UpdatePrompt(0); // 传0单纯是为了不报错
    });

    connect(setButton, &QPushButton::clicked, this, [this]() {
        agreementInfo info;

        info.system = m_textEditSystem->toPlainText().toStdString();
        info.chat_prefix = m_textEditChatPrefix->toPlainText().toStdString();
        info.chat_suffix = m_textEditChatSuffix->toPlainText().toStdString();
        info.user_msg_1 = m_textEditUserMsg1->toPlainText().toStdString();
        info.user_msg_2 = m_textEditUserMsg2->toPlainText().toStdString();
        info.user_msg_3 = m_textEditUserMsg3->toPlainText().toStdString();
        info.assistant_msg_1 = m_textEditAssistantMsg1->toPlainText().toStdString();
        info.assistant_msg_2 = m_textEditAssistantMsg2->toPlainText().toStdString();
        info.assistant_msg_3 = m_textEditAssistantMsg3->toPlainText().toStdString();

        if (m_fileTypeComboBoxIndex == 0) {
            ConfigManager::getInstance().set_prompt_md_file(info);
        } else if (m_fileTypeComboBoxIndex == 1) {
            ConfigManager::getInstance().set_prompt_txt_file(info);
        } else if (m_fileTypeComboBoxIndex == 2) {
            ConfigManager::getInstance().set_prompt_rst_file(info);
        } else if (m_fileTypeComboBoxIndex == 3) {
            ConfigManager::getInstance().set_prompt_h_file(info);
        }
    });

    connect(rstButton, &QPushButton::clicked, this, [this]() {
        agreementInfo info;

        if (m_fileTypeComboBoxIndex == 0) {
            info = ConfigManager::getInstance().default_get_prompt_md_file();
            ConfigManager::getInstance().set_prompt_md_file(info);
        } else if (m_fileTypeComboBoxIndex == 1) {
            info = ConfigManager::getInstance().default_get_prompt_txt_file();
            ConfigManager::getInstance().set_prompt_txt_file(info);
        } else if (m_fileTypeComboBoxIndex == 2) {
            info = ConfigManager::getInstance().default_get_prompt_rst_file();
            ConfigManager::getInstance().set_prompt_rst_file(info);
        } else if (m_fileTypeComboBoxIndex == 3) {
            info = ConfigManager::getInstance().default_get_prompt_h_file();
            ConfigManager::getInstance().set_prompt_h_file(info);
        }

        UpdatePrompt(0); // 传0单纯是为了不报错
    });

    // 连接信号和槽
    connect(m_reconnectButton, &QPushButton::clicked, this, [this]() {
        // 开始按钮点击后的操作
        // 切换到开始页面
        StateManager::getInstance().ShowPage = 0;
        StateManager::getInstance().ModeIndex = 0;
    });

    // 连接 QTabWidget 的 currentChanged 信号
    QObject::connect(tabWidget, &QTabWidget::currentChanged, this, &FileTranslationPage::UpdatePrompt);

    // 创建定时器
    m_translateTimer = new QTimer(this);

    // 连接定时器的timeout信号到槽函数
    connect(m_translateTimer, &QTimer::timeout, this, [=]() {
        if (translation_content_last != translation_content.Get()) {
            translation_content_last = translation_content.Get();
            // 完翻译的信息覆盖
            m_textEdit1->clear();
            m_textEdit1->append(translation_content.Get().c_str());
            QTextCursor cursor1 = m_textEdit1->textCursor();
            cursor1.movePosition(QTextCursor::Start); // 移动光标到文本开头
            m_textEdit1->setTextCursor(cursor1);      // 更新 QTextEdit 的光标位置
        }

        if (translation_result_last != translation_result.Get()) {
            translation_result_last = translation_result.Get();

            // 完全翻译的信息覆盖
            m_textEdit2->clear();
            m_textEdit2->append(translation_result.Get().c_str());
            QTextCursor cursor2 = m_textEdit2->textCursor();
            cursor2.movePosition(QTextCursor::Start); // 移动光标到文本开头
            m_textEdit2->setTextCursor(cursor2);      // 更新 QTextEdit 的光标位置
        }

        if (progress_info_last != progress_info.Get()) {
            progress_info_last = progress_info.Get();
            // 完全翻译的信息覆盖
            m_progressEdit->clear();
            m_progressEdit->append(progress_info.Get().c_str());
            QTextCursor cursor3 = m_progressEdit->textCursor();
            cursor3.movePosition(QTextCursor::Start); // 移动光标到文本开头
            m_progressEdit->setTextCursor(cursor3);   // 更新 QTextEdit 的光标位置
        }
        // 完全翻译的信息覆盖
        m_progressEdit->clear();
        m_progressEdit->append(progress_info.Get().c_str());
        QTextCursor cursor3 = m_progressEdit->textCursor();
        cursor3.movePosition(QTextCursor::Start); // 移动光标到文本开头
        m_progressEdit->setTextCursor(cursor3);   // 更新 QTextEdit 的光标位置
    });

    // 启动定时器，间隔时间为毫秒
    m_translateTimer->start(100);

    // 将窗口移动到屏幕中心
    CommonUtils::moveToCenter(this);
}

FileTranslationPage::~FileTranslationPage() {}

void FileTranslationPage::UpdateModeComboBox()
{

    if (StateManager::getInstance().ShowPage == 3) {
        m_modeComboBox->setCurrentIndex(StateManager::getInstance().ModeIndex);
    }
}

void FileTranslationPage::UpdatePrompt(int index)
{
    agreementInfo info;
    if (m_fileTypeComboBoxIndex == 0) {
        info = ConfigManager::getInstance().get_prompt_md_file();
    } else if (m_fileTypeComboBoxIndex == 1) {
        info = ConfigManager::getInstance().get_prompt_txt_file();
    } else if (m_fileTypeComboBoxIndex == 2) {
        info = ConfigManager::getInstance().get_prompt_rst_file();
    } else if (m_fileTypeComboBoxIndex == 3) {
        info = ConfigManager::getInstance().get_prompt_h_file();
    }

    m_textEditSystem->setText(info.system.c_str());
    m_textEditChatPrefix->setText(info.chat_prefix.c_str());
    m_textEditChatSuffix->setText(info.chat_suffix.c_str());
    m_textEditUserMsg1->setText(info.user_msg_1.c_str());
    m_textEditUserMsg2->setText(info.user_msg_2.c_str());
    m_textEditUserMsg3->setText(info.user_msg_3.c_str());
    m_textEditAssistantMsg1->setText(info.assistant_msg_1.c_str());
    m_textEditAssistantMsg2->setText(info.assistant_msg_2.c_str());
    m_textEditAssistantMsg3->setText(info.assistant_msg_3.c_str());
}