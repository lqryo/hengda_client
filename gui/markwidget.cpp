#include "markwidget.h"
#include "system.h"
#include "videotest.h"
#include "base/co.h"
#include "base/hash.h"
#include "base/fastring.h"

extern std::string g_cameraip;

extern std::string g_server_ip;
extern std::string g_serverip;

void MarkWidget::showEvent(QShowEvent *event)
{
    auto cameras = System::instance().devices();

    for(const auto& camera : cameras) {
		if(cam_cbox_->findText(QString::fromStdString(camera.first)) < 0)
			cam_cbox_->addItem(QString::fromStdString(camera.first));
    }
}

#define markrequest(port) \
TcpClient##port::instance().request("/device/frame", params, [this, device](std::string data) {	\
qDebug() << "data.size() is "<< data.size();	\
qDebug() << "/device/frame";	\
auto xx = QImage::fromData(reinterpret_cast<const uchar *>(data.c_str()), data.size());	\
qDebug() <<xx.size();	\
QPixmap pixmap = QPixmap::fromImage(xx);	\
canvas_->setPixmap(pixmap);	\
canvas_->setKey(cam_cbox_->currentText().toStdString());	\
});


void MarkWidget::showFrameOfCamera(QString idx)
{
    auto devices = System::instance().devices();
	if (devices.empty()) return;

	auto device = devices[idx.toStdString()];

	ip_ = device.name();
	g_cameraip = device.name();
	go(&MarkWidget::client_fun, this);




}

void MarkWidget::client_fun()
{
	Json req, res;

	req.add_member("req_id", now::ms());
	req.add_member("method", "get_key_frame");

	rpc::Client* c = rpc::new_client(g_serverip.c_str(), 9910, "");

	Json params = json::object();
	{
		params.add_member("ip", ip_.c_str());
		params.add_member("port", 9910);
	}
	req.add_member("params", params);
	std::cout << req.pretty() << std::endl;
	
	c->call(req, res);

	std::cout << res.pretty() << std::endl;

	fastring data = base64_decode(res["data"].get_string());

	auto xx = QImage::fromData(reinterpret_cast<const uchar *>(data.c_str()), data.size());	
	qDebug() << xx.size();	
	QPixmap pixmap = QPixmap::fromImage(xx);	
	canvas_->setPixmap(pixmap);	
	canvas_->setKey(cam_cbox_->currentText().toStdString());	




	delete c;
}
