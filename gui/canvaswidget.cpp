#include "canvaswidget.h"
#include <QMouseEvent>
#include <QDebug>

void CanvasWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && !key_.empty()) {
        QPoint pos = event->pos();
        // 对点进行范围限制 & 并转为实际图片坐标
        if(pos.x() > parea_.width()) pos.rx() = parea_.width();
        if(pos.y() > parea_.height()) pos.ry() = parea_.height();
        if(pos.x() < 0) pos.rx() = 0;
        if(pos.y() < 0) pos.ry() = 0;

        pos.rx() = ((float)pos.x() / parea_.width()) * pixmap_.width();
        pos.ry() = ((float)pos.y() / parea_.height()) * pixmap_.height();

        switch (status_) {
            case START_PAINTING_RECTANGLE:
            case START_PAINTING_ELLIPSE:
            case START_PAINTING_LINE:
            case START_PAINTING_CURVES:
            case START_PAINTING_POLYGON:
            case START_PAINTING_BROKEN_LINE:
                command_ = make_shared<PaintCommand>(graph_, QPen(Qt::green, 2), QVector<QPoint>{ pos, pos });
                undo_stacks_[key_].push(command_); redo_stacks_[key_].clear();

                status_ = PAINTING | graph_;
                break;

            case PAINTING_POLYGON:
            case PAINTING_BROKEN_LINE:
                command_->points().push_back(pos);
                break;

            default: break;
        }
    }
    else if(event->button() == Qt::RightButton) {
        switch (status_) {
            case PAINTING_POLYGON:
            case PAINTING_BROKEN_LINE:
                status_ = START_PAINTING | graph_;
                break;

            default: break;
        }
    }

    update();
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint pos = event->pos();
    if(pos.x() > parea_.width()) pos.rx() = parea_.width();
    if(pos.y() > parea_.height()) pos.ry() = parea_.height();
    if(pos.x() < 0) pos.rx() = 0;
    if(pos.y() < 0) pos.ry() = 0;

    pos.rx() = ((float)pos.x() / parea_.width()) * pixmap_.width();
    pos.ry() = ((float)pos.y() / parea_.height()) * pixmap_.height();

    if(status_ == PAINTING_POLYGON || status_ == PAINTING_BROKEN_LINE) {
        command_->points().back() = pos;
        update();
    }

    if(!(event->buttons() & Qt::LeftButton)) return;

    switch(status_)
    {
        case PAINTING_RECTANGLE:
        case PAINTING_ELLIPSE:
        case PAINTING_LINE:
            command_->points()[1] = pos;
            break;

        case PAINTING_CURVES:
            command_->points().push_back(pos);
            break;

        default: break;
    }

    update();
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() != Qt::LeftButton) return;

    switch(status_)
    {
        case PAINTING_RECTANGLE:
        case PAINTING_ELLIPSE:
        case PAINTING_LINE:
        case PAINTING_CURVES:
            status_ = START_PAINTING | graph_;
            break;

        default: break;
    }
    update();
}

void CanvasWidget::paintEvent(QPaintEvent *)
{
    auto canvas = pixmap_.copy();

    // 将图形绘制在图片上
    if(!canvas.isNull()) {
        painter_.begin(&canvas);

        for(const auto& cmd : undo_stacks_[key_].commands()) {
            cmd->execute(&painter_);
        }
        painter_.end();
    }

    painter_.begin(this);
    // 设置画布背景
    painter_.fillRect(rect(), Qt::gray);

    if(!canvas.isNull()) {
		painter_.drawPixmap(QRect({ 0, 0 }, parea_), canvas);
    }

    painter_.end();
}

void CanvasWidget::resizeEvent(QResizeEvent *)
{
	if (pixmap_.isNull()) return;

	parea_ = pixmap_.scaled(rect().size(), Qt::KeepAspectRatio, Qt::SmoothTransformation).size();
	update();
}

void CanvasWidget::undo()
{
	if (undo_stacks_[key_].empty() || key_.empty()) return;

	redo_stacks_[key_].push(undo_stacks_[key_].back());
	undo_stacks_[key_].pop();

	update();
}

void CanvasWidget::redo()
{
	if (redo_stacks_[key_].empty() || key_.empty()) return;

	undo_stacks_[key_].push(redo_stacks_[key_].back());
	redo_stacks_[key_].pop();

	update();
}

void CanvasWidget::clear()
{
	if (key_.empty()) return;

	if (undo_stacks_.find(key_) != undo_stacks_.end()) {
		undo_stacks_[key_].clear();
	}
	if (redo_stacks_.find(key_) != redo_stacks_.end()) {
		redo_stacks_[key_].clear();
	}

	update();
}