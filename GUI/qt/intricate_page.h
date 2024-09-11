/*
 * @Date: 2024-08-28 14:56:49
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-11 10:34:21
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

class IntricatePage : public QMainWindow {
    Q_OBJECT

  public:
    IntricatePage(QWidget *parent = nullptr);
    ~IntricatePage();

    void UpdateModeComboBox();

  private slots:
    void onToggleSettingsButtonClicked();

    void UpdateInfo(int index);

    void SendToServer();

    void ChangeFontSize(int delta);

  private:
    QVBoxLayout *m_mainLayout;
    QComboBox *m_modeComboBox;
    QPushButton *m_toggleSettingsButton;
    QTextEdit *m_textEdit1;
    QTextEdit *m_textEdit2;
    QPushButton *m_translateButton;
    QCheckBox *m_checkBox;
    QPushButton *m_reconnectButton;

    QTimer *m_copyTimer;
    QTimer *m_translateTimer;

    QTextEdit *m_textEditSystem;
    QTextEdit *m_textEditChatPrefix;
    QTextEdit *m_textEditChatSuffix;
    QTextEdit *m_textEditUserMsg1;
    QTextEdit *m_textEditUserMsg2;
    QTextEdit *m_textEditUserMsg3;
    QTextEdit *m_textEditAssistantMsg1;
    QTextEdit *m_textEditAssistantMsg2;
    QTextEdit *m_textEditAssistantMsg3;

  protected:
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // INTRICATE_PAGE_H
