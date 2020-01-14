#include "markwidget.h"
#include "system.h"
#include "videotest.h"
#include "base/co.h"
#include "base/hash.h"
#include "base/fastring.h"
#include "base/log.h"
 
extern bool ICMPPing(const char* szIP);

extern std::string g_serverip;
extern std::string g_cameraip;

void MarkWidget::showEvent(QShowEvent *event)
{
    auto cameras = System::instance().devices();

    for(const auto& camera : cameras) {
		if(cam_cbox_->findText(QString::fromStdString(camera.first)) < 0)
			cam_cbox_->addItem(QString::fromStdString(camera.first));
    }
}


void MarkWidget::showFrameOfCamera(QString idx)
{
 //   auto devices = System::instance().devices();
	//if (devices.empty()) return;

	//auto device = devices[idx.toStdString()];

	//ip_ = device.name();
	//g_cameraip = device.name();

	g_cameraip = idx.toStdString();

	go(&MarkWidget::client_fun, this);
}

void MarkWidget::client_fun()
{
	Json req, res;

	req.add_member("req_id", now::ms());
	req.add_member("method", "get_key_frame");

	if (!ICMPPing(g_serverip.c_str())) {
		emit info(1); // code=1 £¬Á¬½Óipc´íÎó
		return;
	}

	rpc::Client* c = rpc::new_client(g_serverip.c_str(), 9910, "");
	WLOG << "g_serverip is " << g_serverip.c_str();

	Json params = json::object();
	{
		params.add_member("ip", g_cameraip.c_str());
		params.add_member("port", 9910);
	}
	req.add_member("params", params);
	std::cout << req.pretty() << std::endl;
	

	c->call(req, res);


	if (res.is_null()) {
		emit info(2);
	}
	else {
		std::cout << res.pretty() << std::endl;
		WLOG <<"res is " <<res.str();

		int code = res["code"].get_int();
		if (code != 200) {
			emit info(3);
		}
		else {
			fastring data = base64_decode(res["data"].get_string());

			auto xx = QImage::fromData(reinterpret_cast<const uchar *>(data.c_str()), data.size());	
			qDebug() << xx.size();	
			QPixmap pixmap = QPixmap::fromImage(xx);	
			canvas_->setPixmap(pixmap);	
			canvas_->setKey(cam_cbox_->currentText().toStdString());	
		}

	}

	

	delete c;
}
