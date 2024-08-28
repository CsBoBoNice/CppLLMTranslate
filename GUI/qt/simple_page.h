

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

class simple_page : public QMainWindow {
    Q_OBJECT

  public:
    simple_page(QWidget *parent = nullptr);
    ~simple_page();

  private slots:
    void onToggleSettingsButtonClicked();

  private:
    QVBoxLayout *mainLayout;
    QComboBox *modeComboBox;
    QPushButton *toggleSettingsButton;
    QTextEdit *textEdit1;
    QTextEdit *textEdit2;
    QPushButton *translateButton;
    QCheckBox *checkBox;

    int mode_index = 0;

    QTimer *copy_timer;
    QTimer *translate_timer;
};
#endif // SIMPLE_PAGE_H
