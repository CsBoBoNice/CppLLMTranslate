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

    QLineEdit *Input_file_path;
    QLineEdit *Output_file_path;
    QLineEdit *Cut_file_path;
    QLineEdit *Reference_file_path;
    QLineEdit *Success_file_path;
};

#endif // FILETRANSLATION_PAGE_H
