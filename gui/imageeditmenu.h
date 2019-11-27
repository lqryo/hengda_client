// 绘制时使用的菜单

#ifndef BORDER_MENU_H
#define BORDER_MENU_H

#include <map>
#include <QPushButton>
#include "utils.h"
#include "editmenu.h"
#include "gui/widgets/iconbutton.h"
#include "gui/widgets/buttongroup.h"

class ImageEditMenu : public EditMenu
{
    Q_OBJECT

public:
    explicit ImageEditMenu(QWidget* parent = nullptr);

    void reset();

signals:
    void paint(Graph);

    void undo();
    void redo();

    void changed(Graph);

public slots:
    void disableUndo(bool val) { undo_btn_->setDisabled(val); }
    void disableRedo(bool val) { redo_btn_->setDisabled(val); }

private:
    IconButton* undo_btn_ = nullptr;
    IconButton* redo_btn_ = nullptr;

    ButtonGroup * group_ = nullptr;
};


#endif //BORDER_MENU_H
