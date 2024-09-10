/*
 * @Date: 2024-08-28 14:56:49
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-29 10:03:21
 * @FilePath: /CppLLMTranslate/GUI/qt/intricate_page.h
 */

#ifndef INTRICATE_PAGE_H
#define INTRICATE_PAGE_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QCheckBox>
#include <QTimer>
#include <QKeyEvent>

class intricate_page : public QMainWindow {
    Q_OBJECT

  public:
    intricate_page(QWidget *parent = nullptr);
    ~intricate_page();

    void UpdateModeComboBox();

  private slots:
    void onToggleSettingsButtonClicked();

    void UpDataInfo(int index);

    void SendtoServer();

    void ChangeFontSize(int delta);

  private:
    QVBoxLayout *mainLayout;
    QComboBox *modeComboBox;
    QPushButton *toggleSettingsButton;
    QTextEdit *textEdit1;
    QTextEdit *textEdit2;
    QPushButton *translateButton;
    QCheckBox *checkBox;
    QPushButton *reconnectButton;

    QTimer *copy_timer;
    QTimer *translate_timer;

    QTextEdit *textEdit_system;
    QTextEdit *textEdit_chat_prefix;
    QTextEdit *textEdit_chat_suffix;
    QTextEdit *textEdit_user_msg_1;
    QTextEdit *textEdit_user_msg_2;
    QTextEdit *textEdit_user_msg_3;
    QTextEdit *textEdit_assistant_msg_1;
    QTextEdit *textEdit_assistant_msg_2;
    QTextEdit *textEdit_assistant_msg_3;

  protected:
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // INTRICATE_PAGE_H
