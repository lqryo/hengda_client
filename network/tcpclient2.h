#ifndef NETWORK_TCP_CLIENT2_H
#define NETWORK_TCP_CLIENT2_H

#include <QThread>
#include <QTcpSocket>
#include <QMessageBox>
//#include "videotest.h"
#include "json.hpp"
#include "system.h"


class MainTcpClient :public QObject {
	Q_OBJECT

public:
	static MainTcpClient& instance() {
		static MainTcpClient instance;
		return instance;
	}

	MainTcpClient(const MainTcpClient&) = delete;
	MainTcpClient& operator=(const MainTcpClient&) = delete;

	void responseMapping(const std::string& path, std::function<void(nlohmann::json)> callback)
	{
		mapping_table_[path] = callback;
	}

public slots:
	void recevie()
	{
	//	qDebug() << "123";
		buffer_ = socket_->readAll().toStdString();

		nlohmann::json data;
		data = nlohmann::json::parse(buffer_);
		if (data.find("type") != data.end()) {
			QMessageBox::information(nullptr, tr("提示"), tr("视频打开成功"), QMessageBox::Ok);
		}
		else {
			auto cb = mapping_table_["videotest"];
			if (cb == nullptr) {
				qDebug() << "cb is null";
			}
			else {
				cb(data);
			}
		}

	}

	void connect()
	{
		if (is_connected_) return;
		socket_->abort();
		socket_->connectToHost(System::instance().server_ip_, 10000);
		qDebug() << socket_->state();
//		socket_->connectToHost("10.13.3.125", 10000);
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
	MainTcpClient(QObject *parent = nullptr)
		:QObject(parent)
	{
		socket_ = new QTcpSocket(this);

		QObject::connect(socket_, &QTcpSocket::readyRead, this, &MainTcpClient::recevie);
		QObject::connect(socket_, &QTcpSocket::connected, this, &MainTcpClient::connected);
		QObject::connect(socket_, &QTcpSocket::disconnected, this, &MainTcpClient::disconnected);


	}

	std::string buffer_;
	QTcpSocket *socket_ = nullptr;
	std::map<std::string, std::function<void(nlohmann::json)>> mapping_table_;

	bool is_connected_ = false;



};

#endif