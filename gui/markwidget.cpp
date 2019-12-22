#include "markwidget.h"
#include "system.h"
#include "videotest.h"

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
qDebug() << "/device/frame";	\
auto xx = QImage::fromData(reinterpret_cast<const uchar *>(data.c_str()), data.size());	\
QPixmap pixmap = QPixmap::fromImage(xx);	\
canvas_->setPixmap(pixmap);	\
canvas_->setKey(cam_cbox_->currentText().toStdString());	\
});


void MarkWidget::showFrameOfCamera(QString idx)
{
    auto devices = System::instance().devices();
	if (devices.empty()) return;

    auto device = devices[idx.toStdString()];

	nlohmann::json params;
//	params["ip"] = device.name();
	params["port"] = VideotestWidget::ip2port[device.name()];
	int port = VideotestWidget::ip2port[device.name()];
	g_port = port;
	qDebug() << "In showFrameOfCamera";
	qDebug() << "port is " << port;
	if (port == 0) {
		markrequest(1)
	}
	else if (port == 1) {
		markrequest(2)
	}
	else if (port == 2) {
		markrequest(3)
	}
	else if (port == 3) {
		markrequest(4)
	}
	else if (port == 4) {
		markrequest(5)
	}
	else if (port == 5) {
		markrequest(6)
	}
	else if (port == 6) {
		markrequest(7)
	}
	else if (port == 7) {
		markrequest(8)
	}
	else if (port == 8) {
		markrequest(9)
	}
	else if (port == 9) {
		markrequest(10)
	}
	else if (port == 10) {
		markrequest(11)
	}
	else if (port == 11) {
		markrequest(12)
	}
	else if (port == 12) {
		markrequest(13)
	}
	else if (port == 13) {
		markrequest(14)
	}
	else if (port == 14) {
		markrequest(15)
	}

/*
	TcpClient1::instance().request("/device/frame", params, [this, device](std::string data) {
		qDebug() << "/device/frame";
		auto xx = QImage::fromData(reinterpret_cast<const uchar *>(data.c_str()), data.size());
		QPixmap pixmap = QPixmap::fromImage(xx);
		canvas_->setPixmap(pixmap);
		canvas_->setKey(cam_cbox_->currentText().toStdString());
	});
*/
}
