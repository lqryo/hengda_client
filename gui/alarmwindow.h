//
// ffiirree@20191020
//

#ifndef GUI_ALARM_WINDOW_H
#define GUI_ALARM_WINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include "alarm.h"
#include "videotest.h"

class AlarmWindow : public QWidget {
	Q_OBJECT
public:
	AlarmWindow(QWidget * parent = nullptr)
		: QWidget(parent) {
		setWindowTitle("警报");
		setAttribute(Qt::WA_DeleteOnClose, true); // 关闭自动销毁

		auto layout = new QVBoxLayout();
		layout->setMargin(0);
		setLayout(layout);

		text_ = new QLabel("");
		layout->addWidget(text_);
		text_->setFixedSize(960, 50);
		text_->setAlignment(Qt::AlignCenter);
		image_ = new QLabel("");
		layout->addWidget(image_);
		image_->setFixedSize(960, 540);
	}
//	AlarmWindow(const AlarmWindow&) = default;
//	AlarmWindow& operator=(const AlarmWindow&) = default;

#define alarmrequest(port)	\
TcpClient##port::instance().request("/camera/alarm/image", params, [this](std::string data) {	\
	qDebug() << "request /camera/alarm/image";	\
	auto xx = QImage::fromData(reinterpret_cast<const uchar *>(data.c_str()), data.size());	\
	QPixmap pixmap = QPixmap::fromImage(xx).scaled(QSize(960, 540), Qt::KeepAspectRatio, Qt::SmoothTransformation);	\
	image_->setPixmap(pixmap);	\
	});	

	void setAlarm(const Alarm& alarm)
	{
		int port = alarm.port_;
		std::string ip = VideotestWidget::port2ip[port];

		qDebug() << "in SetAlarm";
		std::string text = ip + " 于 " + alarm.time_ + " 发生 " + (alarm.type_ == 1 ? "越界" : "入侵");
		text_->setText(text.c_str());

		nlohmann::json params;
		params["path"] = alarm.path_;

	}

public:
	QLabel * text_;
	QLabel * image_;
};

#endif // !GUI_ALARM_WINDOW_H
