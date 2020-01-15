#include <QApplication>
#include <QFile>
#include "gui/mainwindow.h"
#include "utils.h"
#include "videotest.h"
#include <functional>
#include <iostream>
#include "settingwidget.h"
#include <fstream>

#include "base/rpc.h"
#include "rpcclient.h"


Mutex mtx;

QListWidget* VideotestWidget::videoList = nullptr;
QComboBox* MarkWidget::cam_cbox_ = nullptr;
int g_port = 0;


std::vector<AlarmWindow*> DeServImpl:: alarms;

std::string g_serverip;
std::string g_cameraip; // 用于传边界数据
std::string g_client_ip;



int main(int argc, char* argv[])
{
#ifdef LOGDEL
	// 删除之前的日志文件
	QFile::remove(QString("./logs/Border.log"));
#endif

	flag::init(argc, argv);
	log::init();

	LOG << "start" ;
    QApplication a(argc, argv);

    LOAD_QSS(qApp, ":/qss/res/mainwindow.qss");

	VideotestWidget::videoList = new QListWidget();
	MarkWidget::cam_cbox_ = new QComboBox();

    MainWindow window;
    window.show();

    return a.exec();
}