
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
    QVBoxLayout *mainVBoxLayout;
    QComboBox *translationModeComboBox;
    QPushButton *toggleSettingsButton;
    QTextEdit *sourceTextEdit;
    QTextEdit *targetTextEdit;
    QPushButton *submitTranslationButton;
    QCheckBox *checkBox;
    QPushButton *reconnectButton;

    int modeIndex = 0;

    QTimer *copyTimer;
    QTimer *translateTimer;

  protected:
    void keyPressEvent(QKeyEvent *event) override;
};
#endif // SIMPLE_PAGE_H
