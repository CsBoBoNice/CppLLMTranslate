/*
 * @Date: 2024-09-02 16:50:22
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-09-03 09:08:40
 * @FilePath: /CppLLMTranslate/GUI/qt/FileTranslation_page.h
 */
#ifndef FILETRANSLATION_PAGE_H
#define FILETRANSLATION_PAGE_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include <QCheckBox>
#include <QTimer>
#include <QKeyEvent>
#include <QLineEdit>

class FileTranslation_page : public QMainWindow {
    Q_OBJECT
  public:
    FileTranslation_page(QWidget *parent = nullptr);
    ~FileTranslation_page();

    void updataModeComboBox();

    void UpdataPrompt(int index);

  private:
    QVBoxLayout *mainLayout;
    QComboBox *modeComboBox;

    QPushButton *cutButton;
    QPushButton *translateButton;
    QPushButton *reconnectButton;

    QTimer *translate_timer;

    QTextEdit *textEdit1;
    QTextEdit *textEdit2;
    QTextEdit *progressEdit;

    QComboBox *fileTypeComboBox;
    int fileTypeComboBoxIndex = 0;

    QTextEdit *textEdit_system;
    QTextEdit *textEdit_chat_prefix;
    QTextEdit *textEdit_chat_suffix;
    QTextEdit *textEdit_user_msg_1;
    QTextEdit *textEdit_user_msg_2;
    QTextEdit *textEdit_user_msg_3;
    QTextEdit *textEdit_assistant_msg_1;
    QTextEdit *textEdit_assistant_msg_2;
    QTextEdit *textEdit_assistant_msg_3;

/*
    int paragraph_effective = 1 * 128;  // 段落有效值
    int paragraph_min = 1 * 1024 + 128; // 段落最小值
    int paragraph_max = 2 * 1024 + 128; // 段落最大值
*/

    QLineEdit *Input_file_path;
    QLineEdit *Output_file_path;
    QLineEdit *Cut_file_path;
    QLineEdit *Reference_file_path;
    QLineEdit *Success_file_path;

    QLineEdit *paragraph_effective;
    QLineEdit *paragraph_min;
    QLineEdit *paragraph_max;

};

#endif // FILETRANSLATION_PAGE_H
