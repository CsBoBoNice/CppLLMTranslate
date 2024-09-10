/*
 * @Date: 2024-09-10 09:06:02
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-10 09:39:04
 * @FilePath: /CppLLMTranslate/GUI/qt/simple_page.h
 */

#ifndef SIMPLE_PAGE_H
#define SIMPLE_PAGE_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QCheckBox>
#include <QTimer>
#include <QKeyEvent>

class SimplePage : public QMainWindow {
    Q_OBJECT

  public:
    SimplePage(QWidget *parent = nullptr);
    ~SimplePage();

    void UpdateModeComboBox();

  private slots:
    void OnToggleSettingsButtonClicked();

    void SendToServer();

    void ChangeFontSize(int delta);

  private:
    QVBoxLayout *m_mainVBoxLayout;
    QComboBox *m_translationModeComboBox;
    QPushButton *m_toggleSettingsButton;
    QTextEdit *m_sourceTextEdit;
    QTextEdit *m_targetTextEdit;
    QPushButton *m_submitTranslationButton;
    QCheckBox *m_checkBox;
    QPushButton *m_reconnectButton;

    int m_modeIndex = 0;

    QTimer *m_copyTimer;
    QTimer *m_translateTimer;

  protected:
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // SIMPLE_PAGE_H
