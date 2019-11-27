//
// ffiirree@20191016
//

#ifndef MODEL_ALARM_SERVICE_H
#define MODEL_ALARM_SERVICE_H

#include <vector>
#include <string>
#include <tuple>
#include "alarm.h"
#include "database.h"
#include <mysql.h>

class AlarmService {
public:
	static int count();
	static int count(const std::string& ip, const std::string& start_time, const std::string& end_time);
	static std::vector<Alarm> query(int start, int size);
	static bool insert(const Alarm& alarm);

	static std::vector<Alarm> searchByIP(const std::string& ip, const std::string& start_time, const std::string& end_time, int start, int size);
	static std::vector<Alarm> toAlarmVector(const std::tuple<MYSQL_RES*, int, int>& res);
};

#endif