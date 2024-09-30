#include "CommonUtils.h"
#include <QScreen>
#include <QApplication>

namespace CommonUtils {
    void moveToCenter(QWidget* widget) {
        if (widget) {
            QScreen *screen = QGuiApplication::primaryScreen();
            QRect screenGeometry = screen->geometry();

            int x = (screenGeometry.width() - widget->width()) / 2;
            int y = (screenGeometry.height() - widget->height()) / 2;

            widget->move(x, y);
        }
    }
}