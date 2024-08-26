// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedLayout>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTabWidget>
#include <QLabel>
#include <QClipboard>

#include "UDP_Client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void onTestButtonClicked();
    void onStartButtonClicked();

  private:
    Ui::MainWindow *ui;
    QWidget *firstPage;
    QWidget *secondPage;
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QPushButton *testButton;
    QPushButton *startButton;
    QTextEdit *textEdit;
    QTabWidget *tabWidget;









    QTextEdit *leftTextEdit;
    QTextEdit *rightTextEdit;

    QClipboard *clipboard;



    UDP_Client *UI_client_p = nullptr;

    QTimer *copy_timer;
    QTimer *translate_timer;

};
#endif // MAINWINDOW_H
