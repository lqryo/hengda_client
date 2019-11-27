#ifndef ICON_BUTTON_H
#define ICON_BUTTON_H

#include "utils.h"
#include "gui/widgets/custombutton.h"

class IconButton : public CustomButton
{
public:
    explicit IconButton(const QPixmap &, QWidget *parent = nullptr);
    IconButton(const QPixmap &, const QSize&, const QSize&, bool checkable = false, QWidget *parent = nullptr);

    virtual void paint(QPainter *) override;

    void setIcon(const QPixmap& icon) { icon_ = icon; update(); }
private:
    QPixmap icon_;
    QSize isize_;
};

#endif // ICON_BUTTON_H
