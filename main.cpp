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

# define LOGDEL


Mutex mtx;

QListWidget* VideotestWidget::videoList = nullptr;
QComboBox* MarkWidget::cam_cbox_ = nullptr;
int g_port = 0;
QListWidget* VideotestWidget::CameraList = nullptr;
std::map<std::string, int> VideotestWidget::ip2port;
std::map<int, std::string> VideotestWidget::port2ip;

std::vector<AlarmWindow*> DeServImpl:: alarms;

std::string g_serverip;
std::string g_cameraip; // ���ڴ��߽�����
std::string g_client_ip;




int main(int argc, char* argv[])
{
#ifdef LOGDEL
	// ɾ��֮ǰ����־�ļ�
	QFile::remove(QString("./logs/Border.log"));
#endif

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