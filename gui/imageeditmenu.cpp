#include "imageeditmenu.h"
#include <QPushButton>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QStyle>
#include <QHBoxLayout>
#include <QMoveEvent>
#include <QButtonGroup>
#include "gui/widgets/iconbutton.h"

ImageEditMenu::ImageEditMenu(QWidget* parent)
    : EditMenu(parent)
{
    group_ = new ButtonGroup(this);
    connect(group_, &ButtonGroup::uncheckedAll, [this](){ paint(NONE); });

    const auto btn_size = QSize(HEIGHT, HEIGHT);
    const auto icon_size = QSize(ICON_W, ICON_W);

    auto rectangle_btn = new IconButton(QPixmap(":/image/res/rectangle"), btn_size, icon_size, true);
    rectangle_btn->setToolTip(tr("Rectangle"));
    connect(rectangle_btn, &IconButton::clicked, [this](){ emit changed(RECTANGLE); });
    group_->addButton(rectangle_btn);
    addButton(rectangle_btn);

    auto circle_btn = new IconButton(QPixmap(":/image/res/circle"), btn_size, icon_size, true);
    circle_btn->setToolTip(tr("Ellipse"));
    connect(circle_btn, &IconButton::clicked, [this](){ emit changed(ELLIPSE); });
    group_->addButton(circle_btn);
    addButton(circle_btn);

    auto polygon_btn = new IconButton(QPixmap(":/image/res/polygon"), btn_size, icon_size, true);
    polygon_btn->setToolTip(tr("Polygon"));
    connect(polygon_btn, &IconButton::clicked, [this](){ emit changed(POLYGON); });
    group_->addButton(polygon_btn);
    addButton(polygon_btn);

    auto line_btn = new IconButton(QPixmap(":/image/res/line"), btn_size, icon_size, true);
    line_btn->setToolTip(tr("Line"));
    connect(line_btn, &IconButton::clicked, [this](){ emit changed(LINE); });
    group_->addButton(line_btn);
    addButton(line_btn);

    auto lines_btn = new IconButton(QPixmap(":/image/res/lines"), btn_size, icon_size, true);
    lines_btn->setToolTip(tr("Lines"));
    connect(lines_btn, &IconButton::clicked, [this](){ emit changed(BROKEN_LINE); });
    group_->addButton(lines_btn);
    addButton(lines_btn);

    auto pen_btn = new IconButton(QPixmap(":/image/res/feather"), btn_size, icon_size, true);
    pen_btn->setToolTip(tr("Pencil"));
    connect(pen_btn, &IconButton::clicked, [this](){ emit changed(CURVES); });
    group_->addButton(pen_btn);
    addButton(pen_btn);


    /////////////////////////////////////////////////////////////////////////////////
    addSeparator();

    undo_btn_ = new IconButton(QPixmap(":/image/res/undo"), btn_size, icon_size);
    undo_btn_->setDisabled(true);
    connect(undo_btn_, &IconButton::clicked, this, &ImageEditMenu::undo);
    addButton(undo_btn_);

    redo_btn_ = new IconButton(QPixmap(":/image/res/redo"), btn_size, icon_size);
    redo_btn_->setDisabled(true);
    connect(redo_btn_, &CustomButton::clicked, this, &ImageEditMenu::redo);
    addButton(redo_btn_);
}


void ImageEditMenu::reset()
{
    group_->uncheckAll();
}
