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
#include "tcpclient.h"
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
//		int port = alarm.port_;
//		alarmrequest(1);
		/*
		if (port == 0) {
			alarmrequest(1)
		}
		else if (port == 1) {
			alarmrequest(2)
		}
		else if (port == 2) {
			alarmrequest(3)
		}
		else if (port == 3) {
			alarmrequest(4)
		}
		else if (port == 4) {
			alarmrequest(5)
		}
		else if (port == 5) {
			alarmrequest(6)
		}
		else if (port == 6) {
			alarmrequest(7)
		}
		else if (port == 7) {
			alarmrequest(8)
		}
		else if (port == 8) {
			alarmrequest(9)
		}
		else if (port == 9) {
			alarmrequest(10)
		}
		else if (port == 10) {
			alarmrequest(11)
		}
		else if (port == 11) {
			alarmrequest(12)
		}
		else if (port == 12) {
			alarmrequest(13)
		}
		else if (port == 13) {
			alarmrequest(14)
		}
		else if (port == 14) {
			alarmrequest(15)
		}
		*/

/*
		TcpClient1::instance().request("/camera/alarm/image", params, [this](std::string data) {
			qDebug() << "request /camera/alarm/image";
			auto xx = QImage::fromData(reinterpret_cast<const uchar *>(data.c_str()), data.size());
			QPixmap pixmap = QPixmap::fromImage(xx).scaled(QSize(960, 540), Qt::KeepAspectRatio, Qt::SmoothTransformation);
			image_->setPixmap(pixmap);
		});
*/
	}

public:
	QLabel * text_;
	QLabel * image_;
};

#endif // !GUI_ALARM_WINDOW_H
