//
// ffiirree@20191024
//

#ifndef BORDER_SYSTEM_H
#define BORDER_SYSTEM_H

#include <vector>
#include "camera.h"

class System {
public:
    static System& instance()
    {
        static System instance;
        return instance;
    }
    System(const System&) = delete;

	std::map<std::string, Device> devices() const { return devices_; }
	void addDevice(const Device& device) {
		devices_[device.name()] = device;
	}

	QString server_ip_ = "192.168.2.111";
	int server_port_ = 8893;

	struct 
	{
		std::string username_ = "root";
		std::string password_ = "";
		std::string database_ = "";
		unsigned int port_ = 3306;
	} mysql_;

private:
	System() 
	{
		QString text;
		QFile config_file("setting.json");
		if (config_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			QTextStream in(&config_file);
			text = in.readAll();
		}

		nlohmann::json settings;
		try {
			settings = nlohmann::json::parse(text.toStdString());
		}
		catch (nlohmann::json::parse_error& e) {
			settings = nlohmann::json::array();

			Q_UNUSED(e);
			qDebug() << "Parse setting.json failed!";
		}

		if (settings.contains("mysql")) {
			auto mysql_json = settings["mysql"];
			if (mysql_json.contains("username")) mysql_.username_ = mysql_json["username"].get<std::string>();
			if (mysql_json.contains("password")) mysql_.password_ = mysql_json["password"].get<std::string>();
			if (mysql_json.contains("database")) mysql_.database_ = mysql_json["database"].get<std::string>();
			if (mysql_json.contains("port")) mysql_.port_ = mysql_json["port"].get<unsigned int>();
		}

		if (settings.contains("server-ip")) server_ip_ = QString::fromStdString(settings["server-ip"].get<std::string>());
		if (settings.contains("server-port")) server_port_ = settings["server-port"].get<int>();
	}

	std::map<std::string, Device> devices_;
};

#endif //BORDER_SYSTEM_H
