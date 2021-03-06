#ifndef CUSTOMSTYLE_H
#define CUSTOMSTYLE_H

#include <QProxyStyle>
#include <QPixmap>

class CustomStyle : public QProxyStyle
{
public:
    CustomStyle();
    ~CustomStyle();

public:
    virtual void polish(QWidget * widget);
    virtual void unpolish(QWidget * widget);
    virtual void drawComplexControl(ComplexControl cc, const QStyleOptionComplex *opt, QPainter *p, const QWidget *widget) const;
    virtual int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;
    virtual int styleHint(StyleHint stylehint, const QStyleOption *opt, const QWidget *widget, QStyleHintReturn *returnData) const;
    virtual void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const;
    virtual QRect subElementRect(SubElement subElement, const QStyleOption *option, const QWidget *widget) const;
    virtual QRect subControlRect(ComplexControl cc, const QStyleOptionComplex *opt, SubControl sc, const QWidget *widget) const;


private:

private:

};

#endif // CUSTOMSTYLE_H
