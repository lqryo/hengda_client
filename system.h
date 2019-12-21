//
// ffiirree@20191024
//

#ifndef BORDER_SYSTEM_H
#define BORDER_SYSTEM_H

#include <vector>
#include "camera.h"

class CameraInfo {
public:
	CameraInfo() = default;
	CameraInfo(QString cameraName_, QString cameraPwd_, QString username_, QString channum_, QString codestream_,int filmtime_) :
		cameraName(cameraName_), cameraPwd(cameraPwd_), username(username_), channum(channum_), codestream(codestream_),filmtime(filmtime_) {}
	QString cameraName;
	QString cameraPwd;
	QString username;
	QString channum;
	QString codestream;
	int filmtime;
};

class System {
public:
    static System& instance()
    {
        static System instance;
        return instance;
    }
    System(const System&) = delete;

	std::map<std::string, Device> devices() const { return devices_; }
	std::map<std::string, CameraInfo> cameras() const { return cameras_;}
	void addDevice(const Device& device) {
		devices_[device.name()] = device;
	}

	void addCamera(const CameraInfo& camerainfo) {
		cameras_[camerainfo.cameraName.toStdString()] = camerainfo;
	}

	QString server_ip_ = "10.13.2.230";
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
	std::map<std::string, CameraInfo> cameras_;
};

#endif //BORDER_SYSTEM_H
