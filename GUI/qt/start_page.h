/*
 * @Date: 2024-08-26 15:22:31
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-28 14:42:28
 * @FilePath: /CppLLMTranslate/GUI/qt/start_page.h
 */
#ifndef START_PAGE_H
#define START_PAGE_H

#include <QMainWindow>

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

class start_page : public QMainWindow {
    Q_OBJECT
  public:
    explicit start_page(QWidget *parent = nullptr);

  private:
    QWidget *mainPage;

    QLineEdit *urlInput;
    QLineEdit *modelInput;
    QLineEdit *apiKeyInput;
    QPushButton *resetButton;
    QPushButton *testConnectionButton;
    QPushButton *startApplicationButton;
    QTextEdit *outputTextEdit;

  signals:
};

#endif // START_PAGE_H
