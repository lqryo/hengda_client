//
// ffiirree@20191016
// TODO: 适用于稀疏无并发的网络通信，如果是消息稠密情况，回调会在应答前会被顶替掉，可添加消息队列进行缓冲
//

#ifndef NETWORK_TCP_CLIENT_H
#define NETWORK_TCP_CLIENT_H

#include <QThread>
#include <QTcpSocket>
#include "json.hpp"
#include "system.h"
#include <iostream>
//#include "alarm.h"
//#include "alarmservice.h"



enum {
	REQUEST, RESPONSE
};

class PackageHeader {
public:
	PackageHeader() = default;
	PackageHeader(nlohmann::json& j) {
		route_ = j["route"].get<std::string>();
		type_ = j["type"].get<int>();
		content_type_ = j["content-type"].get<std::string>();
		content_length_ = j["content-length"].get<size_t>();
	}

	std::string dump() {
		nlohmann::json data;
		data["route"] = route_;
		data["type"] = type_;
		data["content-type"] = content_type_;
		data["content-length"] = content_length_;
		return data.dump();
	}

	std::string to_string()
	{
		auto header_str = dump();
		auto header_len = int2str4l(header_str.size());
		return header_len + dump();
	}

	void route(std::string r) { route_ = std::move(r); }
	std::string route() const { return route_; }
	void type(int t) { type_ = t; }
	int type() const { return type_; }
	void contenttype(std::string t) { content_type_ = std::move(t); }
	std::string contenttype() const { return content_type_; }
	void contentlength(size_t len) { content_length_ = len; }
	size_t contentlength() const { return content_length_; }

private:
	std::string int2str4l(size_t len) {
		char len_str[5]{0};
		sprintf(len_str, "%04ld", len);
		return len_str;
	}

	std::string route_;
	int type_ = REQUEST;			// request, response
	std::string content_type_;		// text, json, image/jpg, image/png
	std::size_t content_length_ = 0;
};

# define tcp(class_name) \
class class_name : public QObject{ \
	Q_OBJECT  \
public:	\
	static class_name& instance() {	\
		static class_name instance;	\
		return instance;	\
	}	\
	class_name(const class_name&) = delete; \
	class_name& operator=(const class_name&) = delete;	\
	void responseMapping(const std::string& path, std::function<void(nlohmann::json)> callback) \
	{	\
		mapping_table_[path] = callback;	\
	}	\
public slots:	\
	void recevie()	\
	{	\
		buffer_ += socket_->readAll().toStdString();	\
		size_t header_len = std::atoi(buffer_.substr(0, 4).c_str());	\
		std::string header_str = buffer_.substr(4, header_len);	\
		nlohmann::json header_json;	\
		try {	\
			header_json = nlohmann::json::parse(header_str);	\
		}	\
		catch (nlohmann::json::parse_error& e) {	\
			qDebug() << "parse header error: " << e.id << QString::fromStdString(e.what());	\
			buffer_ = "";	\
			return;	\
		}	\
		PackageHeader header(header_json);	\
		qDebug() << "Tcpclient recevied message";	\
		std::cout << "header route is " << header.route() << std::endl;	\
		if (header.contentlength() > buffer_.size() - (4 + header_len)) {	\
			return;	\
		}	\
		std::string content = buffer_.substr(4 + header_len, header.contentlength());	\
		buffer_ = "";	\
		if (header.type() == RESPONSE) {	\
			if (header.contenttype() == "json") {	\
				nlohmann::json data;	\
				try {	\
					data = nlohmann::json::parse(content);	\
				}	\
				catch (nlohmann::json::parse_error& e) {	\
					qDebug() << "RESPONSE content parse error: " << e.id << QString::fromStdString(e.what());	\
					buffer_ = "";	\
					return;	\
				}	\
				auto cb = mapping_table_[header.route()];	\
				if (cb != nullptr) {	\
					cb(data);	\
				}	\
			}	\
			else {	\
				auto cb = mapping_table_[header.route()];	\
				if (cb != nullptr) {	\
					cb(content);	\
				}	\
			}	\
		}	\
	}	\
	void connect(int port)	\
	{	\
		if (is_connected_) return;	\
		socket_->abort();	\
		socket_->connectToHost(System::instance().server_ip_, port);	\
		qDebug() << "connect222";	\
		qDebug() << socket_->state();	\
	}	\
	void request(	\
		const std::string& path,	\
		const nlohmann::json& content,	\
		std::function<void(nlohmann::json data)> callback = nullptr);	\
	void request(	\
		const std::string& path,	\
		const std::string& content_type = "json",	\
		const std::string& content = "",	\
		std::function<void(nlohmann::json data)> callback = nullptr);	\
	void response(const std::string& path, const nlohmann::json& json);	\
	void response(const std::string& path, const std::string& content_type, const std::string& content);	\
	void disconnected()	\
	{	\
		is_connected_ = false;	\
		qDebug() << "disconnected";	\
	}	\
	void connected() \
	{	\
		is_connected_ = true;	\
		qDebug() << "connected";	\
	}	\
	void send(const std::string& message)	\
	{	\
		qDebug() << "[S]: " << QString::fromStdString(message);	\
		socket_->write(message.c_str(), message.length());	\
	}	\
private:	\
	class_name(QObject *parent = nullptr)	\
		: QObject(parent)	\
	{	\
			socket_ = new QTcpSocket(this);	\
		QObject::connect(socket_, &QTcpSocket::readyRead, this, &class_name::recevie);	\
		QObject::connect(socket_, &QTcpSocket::connected, this, &class_name::connected);	\
		QObject::connect(socket_, &QTcpSocket::disconnected, this, &class_name::disconnected);	\
	}	\
	std::string buffer_;	\
	std::map<std::string, std::function<void(nlohmann::json)>> mapping_table_;	\
public:	\
	bool is_connected_ = false;	\
	QTcpSocket *socket_ = nullptr;	\
};	

tcp(TcpClient1)
tcp(TcpClient2)
tcp(TcpClient3)
tcp(TcpClient4)
tcp(TcpClient5)
tcp(TcpClient6)
tcp(TcpClient7)
tcp(TcpClient8)
tcp(TcpClient9)
tcp(TcpClient10)
tcp(TcpClient11)
tcp(TcpClient12)
tcp(TcpClient13)
tcp(TcpClient14)
tcp(TcpClient15)

/*
class TcpClient2 : public QObject {
	Q_OBJECT

public:
	static TcpClient2& instance() {
		static TcpClient2 instance;
		return instance;
	}

	TcpClient2(const TcpClient2&) = delete;
	TcpClient2& operator=(const TcpClient2&) = delete;


	void responseMapping(const std::string& path, std::function<void(nlohmann::json)> callback)
	{
		std::cout << "in responseMapping " << path << std::endl;
		mapping_table_[path] = callback;
	}

public slots:
	void recevie()
	{
		buffer_ += socket_->readAll().toStdString();
		//qDebug() << "[B]: " << buffer.size();

		// 1. parse header
		size_t header_len = std::atoi(buffer_.substr(0, 4).c_str());
		std::string header_str = buffer_.substr(4, header_len);

		nlohmann::json header_json;
		try {
			header_json = nlohmann::json::parse(header_str);
		}
		catch (nlohmann::json::parse_error& e) {
			qDebug() << "parse header error: " << e.id << QString::fromStdString(e.what());
			buffer_ = "";
			return;
		}

		PackageHeader header(header_json);
		qDebug() << "Tcpclient recevied message";
		std::cout << "header route is " << header.route() << std::endl;

		// 2. parse content
		if (header.contentlength() > buffer_.size() - (4 + header_len)) {
			return;
		}

		std::string content = buffer_.substr(4 + header_len, header.contentlength());
		buffer_ = "";

		if (header.type() == RESPONSE) {
			if (header.contenttype() == "json") {
				nlohmann::json data;
				try {
					data = nlohmann::json::parse(content);
				}
				catch (nlohmann::json::parse_error& e) {
					qDebug() << "RESPONSE content parse error: " << e.id << QString::fromStdString(e.what());
					buffer_ = "";
					return;
				}

				auto cb = mapping_table_[header.route()];
				if (cb != nullptr) {
					cb(data);
				}
			}
			else {
				auto cb = mapping_table_[header.route()];
				if (cb != nullptr) {
					cb(content);
				}
			}
		}
	}

	void connect(int port)
	{
		if (is_connected_) return;

		//	QObject::connect(socket_, &QTcpSocket::readyRead, this, &TcpClient::recevie);
		//	QObject::connect(socket_, &QTcpSocket::connected, this, &TcpClient::connected);
		//	QObject::connect(socket_, &QTcpSocket::disconnected, this, &TcpClient::disconnected);

		socket_->abort();
		//if a connection is established,, QAbstractSocket enters ConnectedState and emits connected().
		socket_->connectToHost(System::instance().server_ip_, port);
		qDebug() << "connect222";
		//		socket_->connectToHost("10.13.3.56", 10000);
		qDebug() << socket_->state();
	}

	void request(
		const std::string& path,
		const nlohmann::json& content,
		std::function<void(nlohmann::json data)> callback = nullptr);
	void request(
		const std::string& path,
		const std::string& content_type = "json",
		const std::string& content = "",
		std::function<void(nlohmann::json data)> callback = nullptr);

	void response(const std::string& path, const nlohmann::json& json);
	void response(const std::string& path, const std::string& content_type, const std::string& content);

	void disconnected()
	{
		is_connected_ = false;
		qDebug() << "disconnected";
	}

	void connected()
	{
		is_connected_ = true;
		qDebug() << "connected";
	}

	void send(const std::string& message)
	{
		qDebug() << "[S]: " << QString::fromStdString(message);
		socket_->write(message.c_str(), message.length());
	}


private:

	TcpClient2(QObject *parent = nullptr)
		: QObject(parent)
	{
		socket_ = new QTcpSocket(this);

		QObject::connect(socket_, &QTcpSocket::readyRead, this, &TcpClient2::recevie);
		QObject::connect(socket_, &QTcpSocket::connected, this, &TcpClient2::connected);
		QObject::connect(socket_, &QTcpSocket::disconnected, this, &TcpClient2::disconnected);
	}


	std::string buffer_;
	std::map<std::string, std::function<void(nlohmann::json)>> mapping_table_;
public:
	bool is_connected_ = false;
	QTcpSocket *socket_ = nullptr;
};
*/


#endif // !NETWORK_TCP_CLIENT_H
