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
};
#endif // MAINWINDOW_H
