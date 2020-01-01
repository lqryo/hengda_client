#include <QApplication>
#include "gui/mainwindow.h"
#include "utils.h"
#include "tcpclient.h"
#include "tcpclient2.h"
#include "videotest.h"
#include <functional>
#include <iostream>
#include "settingwidget.h"

#include "base/rpc.h"
#include "rpcclient.h"
#include <fstream>


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

std::string g_server_ip;
std::string g_client_ip;

void showvideo(nlohmann::json j)
{
	qDebug() << "in showvideo function";
	VideotestWidget::videoList->clear();
	for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
		std::string temp = j[it.key()].get<std::string>();
		VideotestWidget::videoList->addItem(QString::fromStdString(temp));
	}
}


int main(int argc, char* argv[])
{
	flag::init(argc, argv);
	log::init();

    QApplication a(argc, argv);

    LOAD_QSS(qApp, ":/qss/res/mainwindow.qss");

	VideotestWidget::videoList = new QListWidget();
	MarkWidget::cam_cbox_ = new QComboBox();
	VideotestWidget::CameraList = new QListWidget();
//	TcpClient2::instance().connect();
//	TcpClient::instance().connect();

	//192.168.2.111	8893
	std::function<void(nlohmann::json)> callback;
	VideotestWidget videotest;
	callback = showvideo;


//	rpc::Server* server = rpc::new_server("192.168.2.12", 9900, "");
//	server->add_service(new DeServImpl);
//	server->start();

    MainWindow window;
    window.show();

	

    return a.exec();


}