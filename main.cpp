#include <QApplication>
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
QListWidget* VideotestWidget::CameraList = nullptr;
std::map<std::string, int> VideotestWidget::ip2port;
std::map<int, std::string> VideotestWidget::port2ip;

std::vector<AlarmWindow*> DeServImpl:: alarms;

std::string g_serverip;
std::string g_cameraip;

std::string g_client_ip;



int main(int argc, char* argv[])
{
	//google::InitGoogleLogging(argv[0]);
	//google::SetLogDestination(google::GLOG_INFO, "borderlog"); // set log file path

	//LOG(INFO) << "start";

	flag::init(argc, argv);
	log::init();

	LOG << "start" ;

//	log::close();


    QApplication a(argc, argv);

    LOAD_QSS(qApp, ":/qss/res/mainwindow.qss");

	VideotestWidget::videoList = new QListWidget();
	MarkWidget::cam_cbox_ = new QComboBox();
	VideotestWidget::CameraList = new QListWidget();

    MainWindow window;
    window.show();

    return a.exec();
}