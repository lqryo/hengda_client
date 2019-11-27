#ifndef BORDER_MAINWIDGET_H
#define BORDER_MAINWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include "titlebar.h"
#include "icontextbutton.h"

class MainWidget : public QWidget {
Q_OBJECT

public:
	explicit MainWidget(QWidget *parent = nullptr);

	void setWindowName(const QString& name);
	void addWidget(IconTextButton * btn, QWidget * widget);

protected:
	QVBoxLayout *menu_layout = nullptr;

private:
	TitleBar *titlebar_ = nullptr;
    
    QHBoxLayout *center_layout = nullptr;
    QWidget * current_widget_ = nullptr;

    QButtonGroup * group_ = nullptr;
};


#endif //BORDER_MAINWINDOW_H
