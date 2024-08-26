/*
 * @Date: 2024-08-26 14:05:02
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-26 14:07:30
 * @FilePath: /GUI/qt/mainwindow.h
 */
// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void onTestButtonClicked();
    void onStartButtonClicked();

  private:
    Ui::MainWindow *ui;
    QWidget *firstPage;
    QWidget *secondPage;
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QPushButton *testButton;
    QPushButton *startButton;
    QTextEdit *textEdit;
    QTabWidget *tabWidget;

    QTextEdit *leftTextEdit;
    QTextEdit *rightTextEdit;

    QClipboard *clipboard;

    QCheckBox *clipboardReplaceCheckBox;

    QTextEdit *textEdit_system;
    QTextEdit *textEdit_chat_prefix;
    QTextEdit *textEdit_chat_suffix;
    QTextEdit *textEdit_user_msg_1;
    QTextEdit *textEdit_user_msg_2;
    QTextEdit *textEdit_user_msg_3;
    QTextEdit *textEdit_assistant_msg_1;
    QTextEdit *textEdit_assistant_msg_2;
    QTextEdit *textEdit_assistant_msg_3;

    UDP_Client *UI_client_p = nullptr;

    QTimer *copy_timer;
    QTimer *translate_timer;
};
#endif // MAINWINDOW_H
