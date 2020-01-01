#include "rpcclient.h"
#include "base/hash.h"
#include "base/fastring.h"
#include <iostream>
#include <mutex>
#include "base/thread.h"

extern Mutex mtx;

DeServImpl::DeServImpl() {
}
DeServImpl::~DeServImpl() {
}


void DeServImpl::report_alarm(const Json& req, Json& res)
{
	std::cout << "report alarm" << std::endl;
	res.add_member("req_id", req["req_id"]);
	res.add_member("method", req["method"]);

	std::string ipc_ip = req["ipc_ip"].get_string();

	fastring img_decode = base64_decode(req["img"].get_string());

	std::string time = req["time"].get_string();
	std::string type = req["type"].get_string();

	Alarm alarm;
	alarm.ip_ = ipc_ip;
	alarm.time_ = time;
	if (type == "border") {
		alarm.type_ = 1; //Ô½½ç
	}
	else {
		alarm.type_ = 0;  //ÈëÇÖ
	}

	AlarmWindow* alarm_window = new AlarmWindow();
	alarm_window->setAlarm(alarm);
	auto xx = QImage::fromData(reinterpret_cast<const uchar *>(img_decode.c_str()), img_decode.size());
	QPixmap pixmap = QPixmap::fromImage(xx).scaled(QSize(960, 540), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	alarm_window->image_->setPixmap(pixmap);

	{
		MutexGuard g(mtx);
		alarms.push_back(alarm_window);
	}

//	alarm_window->show();


	res.add_member("code", 200);
	res.add_member("code_msg", "200 ok");


}



