#include <QApplication>
#include "gui/mainwindow.h"
#include "utils.h"
#include "tcpclient.h"
#include "videotest.h"
#include <functional>

QListWidget* VideotestWidget::videoList = nullptr;
QComboBox* MarkWidget::cam_cbox_ = nullptr;

void showvideo(nlohmann::json j)
{
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


	TcpClient::instance().connect();
	std::function<void(nlohmann::json)> callback;
	VideotestWidget videotest;
	callback = showvideo;
	TcpClient::instance().responseMapping("videotest", callback);

    MainWindow window;
    window.show();

    return a.exec();
}