//
// ffiirree@20191019
//

#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

#include "mainwidget.h"
#include "markwidget.h"
#include "devicewidget.h"
#include "recordwidget.h"
#include "settingwidget.h"
#include "videotest.h"
// #include "glog/logging.h"

class MainWindow : public MainWidget {
	Q_OBJECT
public:
	MainWindow(QWidget *parent = nullptr)
		: MainWidget(parent) {

		setWindowName(tr("周界预警系统"));

		addWidget(new IconTextButton(QPixmap(":/image/res/record"), tr("报警记录"), true), new RecordWidget());
		addWidget(new IconTextButton(QPixmap(":/image/res/camera"), tr("设备列表"), true), new DeviceWidget());
		addWidget(new IconTextButton(QPixmap(":/image/res/pen"),	tr("边界标记"), true), new MarkWidget());
		addWidget(new IconTextButton(QPixmap(":/image/res/setting"), tr("设置"), true), new SettingWidget());
		addWidget(new IconTextButton(QPixmap(":/image/res/camera"), tr("测试"), true), new VideotestWidget());

		menu_layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::Expanding, QSizePolicy::Expanding));
		WLOG << "MainWindow";
		std::cout << "MainWindow" << std::endl;
	}

};

#endif // GUI_MAINWINDOW_H
