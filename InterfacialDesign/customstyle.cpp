#include "customstyle.h"
#include <QDebug>
#include <QWidget>
#include <QPainter>
#include <QStyleOptionSlider>

CustomStyle::CustomStyle()
{

}

CustomStyle::~CustomStyle()
{

}

void CustomStyle::polish(QWidget *widget)
{
    QProxyStyle::polish(widget);
}

void CustomStyle::unpolish(QWidget *widget)
{
    QProxyStyle::unpolish(widget);
}

void CustomStyle::drawComplexControl(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p, const QWidget *widget) const
{
    switch (cc){
        case CC_Slider:
            /*不采取重绘Slider的操作*/
        default:
            QProxyStyle::drawComplexControl(cc, opt, p, widget);
    }
}

int CustomStyle::pixelMetric(QStyle::PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    return QProxyStyle::pixelMetric(metric, option, widget);
}

int CustomStyle::styleHint(QStyle::StyleHint stylehint, const QStyleOption *opt, const QWidget *widget, QStyleHintReturn *returnData) const
{
    /*设置鼠标左键点击的位置为滑块的值*/
    if(QStyle::SH_Slider_AbsoluteSetButtons == stylehint)
        return Qt::LeftButton;

    return QProxyStyle::styleHint(stylehint, opt, widget, returnData);
}

void CustomStyle::drawPrimitive(QStyle::PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const
{
    /*取消虚线框*/
    if(QStyle::PE_FrameFocusRect == pe)
        return;

    QProxyStyle::drawPrimitive(pe, opt, p, w);
}

QRect CustomStyle::subElementRect(QStyle::SubElement subElement, const QStyleOption *option, const QWidget *widget) const
{
    return QProxyStyle::subElementRect(subElement, option, widget);
}

QRect CustomStyle::subControlRect(QStyle::ComplexControl cc, const QStyleOptionComplex *opt, QStyle::SubControl sc, const QWidget *widget) const
{
    return QProxyStyle::subControlRect(cc, opt, sc, widget);
}

