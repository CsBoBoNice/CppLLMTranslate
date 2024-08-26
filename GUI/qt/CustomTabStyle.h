#ifndef CUSTOMTABSTYLE_H
#define CUSTOMTABSTYLE_H




#include <QApplication>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QProxyStyle>
#include <QPainter>
#include <QTextOption>
#include <QTabBar> // 包含 QStyleOptionTab 的定义
#include <QStyleOption> // 包含 qstyleoption_cast 的定义
#include <QPainter>
#include <QProxyStyle>
#include <QLabel>
#include <QCheckBox>


class CustomTabStyle : public QProxyStyle
{
  public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                           const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 200; // 设置每个tabBar中item的大小
            s.rheight() = 100;
        }
        return s;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                QRect allRect = tab->rect;

                if (tab->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(0x89cfff);
                    painter->setBrush(QBrush(0x89cfff));
                    painter->drawRect(allRect.adjusted(6, 6, -6, -6));
                    painter->restore();
                }
                QTextOption option;
                option.setAlignment(Qt::AlignCenter);
                if (tab->state & QStyle::State_Selected) {
                    painter->setPen(0xf8fcff);
                }
                else {
                    painter->setPen(0x5d5d5d);
                }

                painter->drawText(allRect, tab->text, option);
                return;
            }
        }

        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};





#endif // CUSTOMTABSTYLE_H
