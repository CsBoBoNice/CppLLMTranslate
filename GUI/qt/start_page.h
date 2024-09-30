/*
 * @Date: 2024-08-26 15:22:31
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-28 14:42:28
 * @FilePath: /CppLLMTranslate/GUI/qt/start_page.h
 */
#ifndef START_PAGE_H
#define START_PAGE_H

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
#include <QComboBox>

class StartPage : public QMainWindow {
    Q_OBJECT
  public:
    explicit StartPage(QWidget *parent = nullptr);

  private:
    QWidget *m_mainPage;

    QLineEdit *m_urlInput;
    QLineEdit *m_modelInput;
    QLineEdit *m_apiKeyInput;
    QLineEdit *m_configNameInput;  // 新增：配置名称输入框
    QPushButton *m_resetButton;
    QPushButton *m_testConnectionButton;
    QPushButton *m_startApplicationButton;
    QPushButton *deleteConfigButton;  // 添加这一行
    QTextEdit *m_outputTextEdit;
    QComboBox *m_configSelector;

    void updateConfigSelector();
    void loadSelectedConfig();
    void saveCurrentConfig();  // 新增：保存当前配置的方法
    void deleteCurrentConfig();  // 新增：删除当前配置的方法

  signals:
};

#endif // START_PAGE_H
