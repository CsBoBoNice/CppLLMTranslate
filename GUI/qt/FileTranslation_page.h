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

    void UpdatePrompt(int index);

  private:
    QVBoxLayout *m_mainLayout;
    QComboBox *m_modeComboBox;

    QPushButton *m_cutButton;
    QPushButton *m_translateButton;
    QPushButton *m_reconnectButton;
    QPushButton *m_resetButton;
    QPushButton *m_cleanButton;

    QTimer *m_translateTimer;

    QTextEdit *m_textEdit1;
    QTextEdit *m_textEdit2;
    QTextEdit *m_progressEdit;

    QComboBox *m_fileTypeComboBox;
    int m_fileTypeComboBoxIndex = 0;

    QTextEdit *m_textEditSystem;
    QTextEdit *m_textEditChatPrefix;
    QTextEdit *m_textEditChatSuffix;
    QTextEdit *m_textEditUserMsg1;
    QTextEdit *m_textEditUserMsg2;
    QTextEdit *m_textEditUserMsg3;
    QTextEdit *m_textEditAssistantMsg1;
    QTextEdit *m_textEditAssistantMsg2;
    QTextEdit *m_textEditAssistantMsg3;

    QLineEdit *m_inputFilePath;
    QLineEdit *m_outputFilePath;
    QLineEdit *m_cutFilePath;
    QLineEdit *m_referenceFilePath;
    QLineEdit *m_successFilePath;

    QLineEdit *m_paragraphEffective;
    QLineEdit *m_paragraphMin;
    QLineEdit *m_paragraphMax;
};

#endif // _FILETRANSLATIONPAGE_H
