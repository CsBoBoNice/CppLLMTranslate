/*
 * @Date: 2024-08-26 15:22:31
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-30 15:36:03
 * @FilePath: /CppLLMTranslate/GUI/qt/start_page.h
 */
#ifndef START_PAGE_H
#define START_PAGE_H

// 包含必要的Qt头文件
#include <QMainWindow>
#include <QStackedLayout>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTabWidget>
#include <QLabel>
#include <QClipboard>
#include <QCheckBox>
#include <QComboBox> // 用于模型选择器

#include "ModelsInfo.h" // 包含模型信息相关的头文件

// 定义StartPage类,继承自QMainWindow
class StartPage : public QMainWindow {

    Q_OBJECT // Qt的元对象宏,用于支持信号和槽机制

        public :
        // 构造函数,parent参数默认为nullptr
        explicit StartPage(QWidget *parent = nullptr);

  private:
    QWidget *m_mainPage; // 主页面widget

    // 用户输入控件
    QLineEdit *m_urlInput;    // URL输入框
    QLineEdit *m_modelInput;  // 模型输入框
    QLineEdit *m_apiKeyInput; // API密钥输入框
    QLineEdit *m_titleInput;  // 模型标题输入框

    // 按钮控件
    QPushButton *m_resetButton;            // 重置按钮
    QPushButton *m_testConnectionButton;   // 测试连接按钮
    QPushButton *m_startApplicationButton; // 启动应用按钮

    QTextEdit *m_outputTextEdit; // 输出文本框

    // 新增的控件
    QComboBox *m_modelSelector;   // 模型选择下拉框
    QPushButton *m_deleteButton;  // 删除模型按钮
    QPushButton *m_refreshButton; // 刷新按钮

    ModelsInfo_s m_serverConfig; // 服务器配置信息

    // 私有成员函数
    void updateModelSelector(); // 更新模型选择器

    void initializeUI();           // 初始化UI
    void setupLayouts();           // 设置布局
    void connectSignalsAndSlots(); // 连接信号和槽
    // 添加输入字段到布局
    void addInputFieldToLayout(QVBoxLayout *layout, const QString &label, QLineEdit *input);
    void testConnection();           // 测试连接
    void startApplication();         // 启动应用
    void deleteSelectedModel();      // 删除选中的模型
    void onModelSelected(int index); // 处理模型选择事件

  signals:
    // 这里可以添加自定义信号
};

#endif // START_PAGE_H
