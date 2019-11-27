#include "markwidget.h"
#include "system.h"

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
    auto devices = System::instance().devices();
	if (devices.empty()) return;

    auto device = devices[idx.toStdString()];

	nlohmann::json params;
	params["ip"] = device.name();

	TcpClient::instance().request("/device/frame", params, [this, device](std::string data) {
		auto xx = QImage::fromData(reinterpret_cast<const uchar *>(data.c_str()), data.size());
		QPixmap pixmap = QPixmap::fromImage(xx);
		canvas_->setPixmap(pixmap);
		canvas_->setKey(cam_cbox_->currentText().toStdString());
	});
}
