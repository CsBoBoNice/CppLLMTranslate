#ifndef _FILETRANSLATIONPAGE_H
#define _FILETRANSLATIONPAGE_H

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

class FileTranslationPage : public QMainWindow {
    Q_OBJECT
  public:
    FileTranslationPage(QWidget *parent = nullptr);
    ~FileTranslationPage();

    void UpdateModeComboBox();

    void UpdataPrompt(int index);

  private:
    QVBoxLayout *mainLayout;
    QComboBox *modeComboBox;

    QPushButton *cutButton;
    QPushButton *translateButton;
    QPushButton *reconnectButton;
    QPushButton *resetButton;
    QPushButton *cleanButton;

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

    QLineEdit *Input_file_path;
    QLineEdit *Output_file_path;
    QLineEdit *Cut_file_path;
    QLineEdit *Reference_file_path;
    QLineEdit *Success_file_path;

    QLineEdit *paragraph_effective;
    QLineEdit *paragraph_min;
    QLineEdit *paragraph_max;
};

#endif // _FILETRANSLATIONPAGE_H
