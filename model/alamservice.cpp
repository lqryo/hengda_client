#include "alarmservice.h"

using namespace std;

int AlarmService::count()
{
	auto db = &Database::instance();
	string query_str = "select count(*) from alarms";
	auto res = db->query(query_str);
	if (get<0>(res) != nullptr && get<1>(res) == 1) {
		auto item = mysql_fetch_row(get<0>(res));
		return atoi(item[0]);
	}
	return 0;
}

int AlarmService::count(const string& ip, const string& start_time, const string& end_time)
{
	auto db = &Database::instance();
	string query_str = "select * from alarms where ip like CONCAT('%','" + ip + "','%') and time between '"
		+ start_time + "' and '" + end_time + "'";

	auto res = db->query(query_str);
	if (get<0>(res) != nullptr && get<1>(res) == 1) {
		auto item = mysql_fetch_row(get<0>(res));
		return atoi(item[0]);
	}
	return 0;
}

vector<Alarm> AlarmService::query(int start, int size)
{
	auto db = &Database::instance();
	string query_str = "select * from alarms order by time desc limit " + to_string(start) + ", " + to_string(size);
	return toAlarmVector(db->query(query_str));
}

bool AlarmService::insert(const Alarm& alarm)
{
	auto db = &Database::instance();

	string insert_str = "insert into alarms(ip, type, time, image) values('" 
		+ alarm.ip_ + "', '" + std::to_string(alarm.type_) + "', '" + alarm.time_ + "', '" + alarm.path_ + "')";
	return !db->insert(insert_str);
}

vector<Alarm> AlarmService::searchByIP(const string& ip, const string& start_time, const string& end_time, int start, int size)
{
	auto db = &Database::instance();
	string query_str = "select * from alarms where ip like CONCAT('%','" + ip + "','%') and time between '" 
		+ start_time + "' and '" + end_time + "' order by time desc limit " + to_string(start) + ", " + to_string(size);
	return toAlarmVector(db->query(query_str));
}

vector<Alarm> AlarmService::toAlarmVector(const tuple<MYSQL_RES*, int, int>& res)
{
	vector<Alarm> alarms;
	if (get<0>(res) != nullptr && get<2>(res) == 5) {
		MYSQL_ROW item;
		while (item = mysql_fetch_row(get<0>(res))) {
			Alarm alarm;
			alarm.id_ = atoi(item[0]);
			alarm.ip_ = string(item[1]);
			alarm.type_ = atoi(item[2]);
			alarm.time_ = string(item[3]);
			alarm.path_ = string(item[4]);
			alarms.push_back(alarm);
		}
	}
	return alarms;
}