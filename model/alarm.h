//
// ffiirree@20191016
//

#ifndef MODEL_ALARM_H
#define MODEL_ALARM_H

#include <string>
#include <QDateTime>
#include <mysql.h>
#include "utils.h"
#include "json.hpp"

class Alarm {
public:
	Alarm() = default;
	Alarm(const nlohmann::json& alarm) {
		json_get(alarm, "ip", ip_);
		json_get(alarm, "type", type_);
		//json_get(alarm, "time", time_);
		time_ = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toStdString();
		json_get(alarm, "path", path_);
	}

	int id_ = -1;
	std::string ip_;
	int type_;
	std::string time_;
	std::string path_;
};

#endif // !MODEL_ALARM_H
