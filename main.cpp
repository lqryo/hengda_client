#include <QApplication>
#include "gui/mainwindow.h"
#include "utils.h"
#include "tcpclient.h"
#include "tcpclient2.h"
#include "videotest.h"
#include <functional>

QListWidget* VideotestWidget::videoList = nullptr;
QComboBox* MarkWidget::cam_cbox_ = nullptr;
QListWidget* VideotestWidget::CameraList = nullptr;
std::map<std::string, int> VideotestWidget::ip2port;
std::map<int, std::string> VideotestWidget::port2ip;

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
	MainTcpClient::instance().responseMapping("videotest", callback);



    MainWindow window;
    window.show();

    return a.exec();
}