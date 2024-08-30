/*
 * @Date: 2024-08-29 09:52:10
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-29 11:40:50
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

class simple_page : public QMainWindow {
    Q_OBJECT

  public:
    simple_page(QWidget *parent = nullptr);
    ~simple_page();

  private slots:
    void onToggleSettingsButtonClicked();

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

    int mode_index = 0;

    QTimer *copy_timer;
    QTimer *translate_timer;

  protected:
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // SIMPLE_PAGE_H
