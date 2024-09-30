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
#include <QComboBox>  // 新增

class StartPage : public QMainWindow {
    Q_OBJECT
  public:
    explicit StartPage(QWidget *parent = nullptr);

  private:
    QWidget *m_mainPage;

    QLineEdit *m_urlInput;
    QLineEdit *m_modelInput;
    QLineEdit *m_apiKeyInput;
    QLineEdit *m_titleInput;  // 新增: 用于输入模型标题
    QPushButton *m_resetButton;
    QPushButton *m_testConnectionButton;
    QPushButton *m_startApplicationButton;
    QTextEdit *m_outputTextEdit;

    QComboBox *m_modelSelector;  // 新增
    QPushButton *m_deleteButton;  // 新增
    QPushButton *m_refreshButton;  // 新增

    void updateModelSelector();  // 新增

  signals:
};

#endif // START_PAGE_H
