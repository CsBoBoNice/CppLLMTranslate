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

#include "UDP_Client.h"

class start_page : public QMainWindow {
    Q_OBJECT
  public:
    explicit start_page(QWidget *parent = nullptr);

  private:
    QWidget *firstPage;

    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QPushButton *testButton;
    QPushButton *startButton;
    QTextEdit *textEdit;

    UDP_Client *UI_client_p = nullptr;

  signals:
};

#endif // START_PAGE_H
