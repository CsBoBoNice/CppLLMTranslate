/*
 * @Date: 2024-08-31 13:42:22
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-02 10:47:21
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
#include "ConfigManager.h"
#include "StateManager.h"
#include "HttpManager.h"

FileManager fileManager;

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

            qDebug() << "fileManager.translation_cache.size()"<<fileManager.translation_cache.size();
            qDebug() << "fileManager.m_file_index"<<fileManager.m_file_index;


            // 原英文未翻译段落
            std::string en_string = fileManager.translation_cache[fileManager.m_file_index].content[fileManager.m_paragraph_index];

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
            info.msg=en_string;
            // 已翻译中文段落
            std::string zh_string;

            HttpManager httpManager_;

            httpManager_.sendRequestAgreementInfo(info,zh_string);

            // 输出至文件
            std::string output_to_file_string = fileManager.Separator_cut + "\n" + en_string + fileManager.Separator_cut + "\n" + zh_string + "\n";

            // 将翻译好的中文内容替换英文内容
            fileManager.translation_cache[fileManager.m_file_index].content[fileManager.m_paragraph_index] = output_to_file_string;

            fileManager.m_paragraph_index++; // 翻译有效进行下一段翻译
            // 文件翻译成功 转到翻译下一个文件
            if (fileManager.m_paragraph_index >= fileManager.translation_cache[fileManager.m_file_index].content.size())
            {

                // 将翻译内容输出至文件
                fileManager.SaveToFilesystem(fileManager.translation_cache, fileManager.directory_en, fileManager.m_file_index);

                fileManager.m_file_index++;
                fileManager.m_paragraph_index = 0;

            }
        }else{
            if(fileManager.translation_cache.size()>0)
            {
                // 将翻译好的段落放入缓冲区
                fileManager.ProcessFilesRecursive(fileManager.directory_en, fileManager.directory_ok, fileManager.directory_en,
                                                  fileManager.translation_cache);

                // 所有文件翻译完毕 将中文提取放入指定文件夹
                fileManager.SaveTranslatedFiles(fileManager.translation_cache,fileManager.directory_ok);

                // 清除缓冲等待下一次翻译
                fileManager.CleanAll();

                qDebug() << "22222 fileManager.translation_cache.clear()"<<fileManager.translation_cache.size();
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

    // modeComboBox->setCurrentIndex(3);

    // 使用lambda表达式连接信号和槽
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

    // 第四行
    cutButton = new QPushButton("切割📏");
    translateButton = new QPushButton("提交🚀");

    // 连接信号和槽
    connect(cutButton, &QPushButton::clicked, this, [this]() {
        if(fileManager.translation_cache.size()==0)
        {
        fileManager.ProcessFilesCut(fileManager.directory, fileManager.directory_cut,
                                  fileManager.directory); // 切割段落
            fileManager.m_cut_sign=true;
        }
    });

    // 连接信号和槽
    connect(translateButton, &QPushButton::clicked, this, [this]() {
        qDebug() << "111111 fileManager.translation_cache.size()"<<fileManager.translation_cache.size();
        if(fileManager.translation_cache.size()==0)
        {
            if(fileManager.m_cut_sign!=true)
            {
                fileManager.ProcessFilesCut(fileManager.directory, fileManager.directory_cut,
                                            fileManager.directory); // 切割段落
            }
            // 将切割好的段落放入缓冲区
            fileManager.ProcessFilesRecursive(fileManager.directory_cut, fileManager.directory_en, fileManager.directory_cut,
                                              fileManager.translation_cache);

            qDebug() << "22222 fileManager.translation_cache.size()"<<fileManager.translation_cache.size();
        }

    });

    QHBoxLayout *RowLayout = new QHBoxLayout();
    RowLayout->addWidget(cutButton);
    RowLayout->addWidget(translateButton);
    mainLayout->addWidget(modeComboBox);
    mainLayout->addLayout(RowLayout);

    std::thread t_FileTranslation_thread(FileTranslation_thread);
    t_FileTranslation_thread.detach();

    // 设置布局到中心窗口
    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

}

FileTranslation_page::~FileTranslation_page() {}

void FileTranslation_page::updataModeComboBox()
{

    if (StateManager::getInstance().ShowPage == 3) {
        modeComboBox->setCurrentIndex(StateManager::getInstance().ModeIndex);
    }
}
