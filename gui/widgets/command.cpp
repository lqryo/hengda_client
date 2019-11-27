#include "command.h"
#include <QPainter>
#include <QDebug>


void PaintCommand::execute(QPainter *painter_)
{
    painter_->save();
    painter_->setPen(pen());
    painter_->setRenderHint(QPainter::Antialiasing, true);

    switch (graph()) {
        case RECTANGLE:
            painter_->drawRect(QRect(points()[0], points()[1]));
            break;

        case ELLIPSE:
            painter_->drawEllipse(QRect(points()[0], points()[1]));
            break;

        case LINE:
            painter_->drawLine(points()[0], points()[1]);
            break;

        case BROKEN_LINE:
            painter_->drawPolyline(points());
            break;

        case CURVES:
            painter_->drawPolyline(points());
            break;

        case POLYGON:
        {
            auto ps = new QPoint[points().size()];
            for(auto i = 0; i < points().size(); ++i) {
                ps[i] = points()[i];
            }

            painter_->drawPolygon(ps, points().size());
            delete[] ps;
            break;
        }


        default: break;
    }
    painter_->restore();
}

