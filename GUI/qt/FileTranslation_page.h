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

class FileTranslation_page : public QMainWindow {
    Q_OBJECT
  public:
    FileTranslation_page(QWidget *parent = nullptr);
    ~FileTranslation_page();

    void updataModeComboBox();

  private:
    QVBoxLayout *mainLayout;
    QComboBox *modeComboBox;

    QPushButton *cutButton;
    QPushButton *translateButton;
    QPushButton *reconnectButton;

    QTimer *translate_timer;
};

#endif // FILETRANSLATION_PAGE_H
