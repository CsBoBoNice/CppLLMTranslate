/*
 * @Date: 2024-08-26 08:35:34
 * @LastEditors: csbobo 751541594@qq.com
 * @LastEditTime: 2024-08-30 13:41:37
 * @FilePath: /CppLLMTranslate/GUI/qt/CustomTabStyle.h
 */
#ifndef CUSTOMTABSTYLE_H
#define CUSTOMTABSTYLE_H

#include <QApplication>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QProxyStyle>
#include <QPainter>
#include <QTextOption>
#include <QTabBar>      // 包含 QStyleOptionTab 的定义
#include <QStyleOption> // 包含 qstyleoption_cast 的定义
#include <QPainter>
#include <QProxyStyle>
#include <QLabel>
#include <QCheckBox>

class CustomTabStyle : public QProxyStyle {
  public:
    QSize
    sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
    {
        QSize newSize = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            newSize.transpose();
            newSize.rwidth() = 200; // 设置每个tabBar中item的大小
            newSize.rheight() = 100;
        }
        return newSize;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tabOption = qstyleoption_cast<const QStyleOptionTab *>(option)) {
                QRect allRect = tabOption->rect;

                if (tabOption->state & QStyle::State_Selected) {
                    painter->save();
                    painter->setPen(0x89cfff);
                    painter->setBrush(QBrush(0x89cfff));
                    painter->drawRect(allRect.adjusted(6, 6, -6, -6));
                    painter->restore();
                }
                QTextOption textOption;
                textOption.setAlignment(Qt::AlignCenter);
                if (tabOption->state & QStyle::State_Selected) {
                    painter->setPen(0xf8fcff);
                } else {
                    painter->setPen(0x5d5d5d);
                }

                painter->drawText(allRect, tabOption->text, textOption);
                return;
            }
        }

        if (element == CE_TabBarTab) {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};

#endif // CUSTOMTABSTYLE_H
