#ifndef START_PAGE_H
#define START_PAGE_H

#include <QMainWindow>

#include <QMainWindow>
#include <QStackedLayout>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTabWidget>
#include <QLabel>
#include <QClipboard>
#include <QCheckBox>

#include "UDP_Client.h"

class start_page : public QMainWindow {
    Q_OBJECT
  public:
    explicit start_page(QWidget *parent = nullptr);

  private:
    QWidget *firstPage;

    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QPushButton *testButton;
    QPushButton *startButton;
    QTextEdit *textEdit;


    UDP_Client *UI_client_p= nullptr;

  signals:
};

#endif // START_PAGE_H
