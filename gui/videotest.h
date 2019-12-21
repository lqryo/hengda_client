//
// Created by liangqi on 2019/11/25
//

#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QListWidget>
#include "tcpclient2.h"
#include <iostream>
#include <string>
#include "tcpclient.h"
#include <windows.h>
#include <QTcpSocket>

class TcpThread : public QThread
{

public:
	TcpThread() = default;

private:
	virtual void run()//任务处理线程
	{
		auto client = &TcpClient1::instance();
		client->socket_ = new QTcpSocket();
		client->connect(8893+port_off);
		qDebug() << "port is " << port_off;
		while (true)
		{
			sleep(40);
		}
		qDebug() << "end of thread ";

	}
public:
	int port_off;


};


class VideotestWidget : public QWidget {
	Q_OBJECT
public:
	explicit VideotestWidget(QWidget *parent = nullptr)
		: QWidget(parent) {
		auto layout = new QVBoxLayout();
		setLayout(layout);

		//auto autorun = new QCheckBox("开机启动");
		//layout->addWidget(autorun);

		{
			auto _layout = new QHBoxLayout();
			layout->addLayout(_layout);

			auto label = new QLabel("测试视频列表");
			_layout->addWidget(label);

		//	auto _input = new QLineEdit();
		//	_layout->addWidget(_input);
		}

		//列表显示测试视频
		{
		//	videoList = new QListWidget(this);
			layout->addWidget(videoList);
		//	videoList->addItem("1.mp4");
		//	videoList->addItem("2.mp4");
		//	videoList->addItem("3.mp4");
		}

		{
			auto _layout = new QHBoxLayout();
			layout->addLayout(_layout);

			auto label = new QLabel("测试摄像头");
			_layout->addWidget(label);
			
			layout->addWidget(CameraList);
		}

		layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

		{
			auto _layout = new QHBoxLayout();
			layout->addLayout(_layout);

			_layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
			select = new QPushButton("选择视频");
			auto apply = new QPushButton("连接相机");

			openCamera = new QPushButton("选择相机");
			closeCamera = new QPushButton("关闭相机");

//			auto connection = new QPushButton("连接");

			select->setDisabled(true);

//			connect(connection, &QPushButton::clicked, this, &VideotestWidget::connection);
			connect(apply, &QPushButton::clicked, this, &VideotestWidget::applyclick);
			connect(select, &QPushButton::clicked, this, &VideotestWidget::selection);
			connect(openCamera, &QPushButton::clicked, this, &VideotestWidget::Open);
			connect(closeCamera, &QPushButton::clicked, this, &VideotestWidget::Close);

			_layout->addWidget(openCamera);
			_layout->addWidget(closeCamera);
			_layout->addWidget(select);
			_layout->addWidget(apply);
//			_layout->addWidget(connection);
			
		}

		std::function<void(nlohmann::json)> callback;
	}

	static QListWidget* videoList;
	static QListWidget* CameraList;
	static std::map<std::string, int> ip2port;
	static std::map<int, std::string> port2ip;

private:


	void selection()
	{
		qDebug() << videoList->currentRow();
		nlohmann::json data;
		data["type"] = "videotest";
		data["data"] = std::to_string(videoList->currentRow());
		MainTcpClient::instance().request("", data);
	}

	void applyclick()
	{
		
/*
		nlohmann::json data;

		// 发送视频请求
		// TcpClient::instance().request()
		data["type"] = "videotest";
		data["data"] = "-1";
		std::cout << data.dump(4) << std::endl;
		MainTcpClient::instance().request("", data);
	//	QMessageBox::information(this, tr("info"), tr("iteration end"), QMessageBox::Ok);
		select->setDisabled(false);
		
*/

		
/*
		qDebug() << "线程测试开始";
		TcpThread* task = new TcpThread;
		task->port_off = port_off;
		
		task->start();
		port_off++;
*/


		count++;
		if (count == 1) {
			TcpClient1::instance().connect(8893 + 0);
		}
		else if (count == 2) {
			TcpClient2::instance().connect(8893 + 1);
		}
		else if (count == 3) {
			TcpClient3::instance().connect(8893 + 2);
		}
		else if (count == 4) {
			TcpClient4::instance().connect(8893 + 3);
		}
		else if (count == 5) {
			TcpClient5::instance().connect(8893 + 4);
		}
		else if (count == 6) {
			TcpClient6::instance().connect(8893 + 5);
		}
		else if (count == 7) {
			TcpClient7::instance().connect(8893 + 6);
		}
		else if (count == 8) {
			TcpClient8::instance().connect(8893 + 7);
		}
		else if (count == 9) {
			TcpClient9::instance().connect(8893 + 8);
		}
		else if (count == 10) {
			TcpClient10::instance().connect(8893 + 9);
		}
		else if (count == 11) {
			TcpClient11::instance().connect(8893 + 10);
		}
		else if (count == 12) {
			TcpClient12::instance().connect(8893 + 11);
		}
		else if (count == 13) {
			TcpClient13::instance().connect(8893 + 12);
		}
		else if (count == 14) {
			TcpClient14::instance().connect(8893 + 13);
		}
		else if (count == 15) {
			TcpClient15::instance().connect(8893 + 14);
		}
		

	}

	void Open()
	{
		int i = CameraList->currentRow();
		if (i == -1) { return; }
		qDebug() << CameraList->item(i)->text();
		nlohmann::json data;
		QString data_str = CameraList->item(i)->text();
		CameraInfo camerainfo = System::instance().cameras()[data_str.toStdString()];
		QString url;

		url = "rtsp://" + camerainfo.username + ":" + camerainfo.cameraPwd + "@" + camerainfo.cameraName
			+ ":554/h264/" + camerainfo.channum + "/" + camerainfo.codestream + "/av_stream";
		qDebug() << url;

		ip2port[camerainfo.cameraName.toStdString()] = count;
		port2ip[count] = camerainfo.cameraName.toStdString();

		data["type"] = "opencamera";
//		data["data"] = data_str.toStdString();
		data["data"] = url.toStdString();
		MainTcpClient::instance().request("", data);

/*
		count++;
		if (count == 1) {
			TcpClient1::instance().connect(8893 + 0);
		}
		else if (count == 2) {
			TcpClient1::instance().connect(8893 + 1);
		}
		else if (count == 3) {
			TcpClient1::instance().connect(8893 + 2);
		}
		else if (count == 4) {
			TcpClient1::instance().connect(8893 + 3);
		}
		else if (count == 5) {
			TcpClient1::instance().connect(8893 + 4);
		}
		else if (count == 6) {
			TcpClient1::instance().connect(8893 + 5);
		}
		else if (count == 7) {
			TcpClient1::instance().connect(8893 + 6);
		}
		else if (count == 8) {
			TcpClient1::instance().connect(8893 + 7);
		}
		else if (count == 9) {
			TcpClient1::instance().connect(8893 + 8);
		}
		else if (count == 10) {
			TcpClient1::instance().connect(8893 + 9);
		}
		else if (count == 11) {
			TcpClient1::instance().connect(8893 + 10);
		}
		else if (count == 12) {
			TcpClient1::instance().connect(8893 + 11);
		}
		else if (count == 13) {
			TcpClient1::instance().connect(8893 + 12);
		}
		else if (count == 14) {
			TcpClient1::instance().connect(8893 + 13);
		}
		else if (count == 15) {
			TcpClient1::instance().connect(8893 + 14);
		}

		*/

	}

	void Close()
	{
		int i = CameraList->currentRow();
		qDebug() << CameraList->item(i)->text();
		nlohmann::json data;
		QString data_str = CameraList->item(i)->text();
		CameraInfo camerainfo = System::instance().cameras()[data_str.toStdString()];
		QString url;

		url = "rtsp://" + camerainfo.username + ":" + camerainfo.cameraPwd + "@" + camerainfo.cameraName
			+ ":554/h264" + camerainfo.channum + "/" + camerainfo.codestream + "/av_stream";
		qDebug() << url;


		data["type"] = "closecamera";
		data["data"] = url.toStdString();
		MainTcpClient::instance().request("", data);
	}

	void connection()
	{

	//	TcpClient::instance().connect();
	//	QMessageBox::information(this, tr("info"), tr("连接服务端成功"), QMessageBox::Ok);
	//	showvideo();
	}
	QPushButton* select;
	QPushButton* openCamera;
	QPushButton* closeCamera;
public:
	void showvideo(nlohmann::json j)
	{
		videoList->clear();
		for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
			std::string temp = j[it.key()].get<std::string>();
			videoList->addItem(QString::fromStdString(temp));
		}
	}

	/*
	void showvideo()
	{
		videoList->clear();

		nlohmann::json j = {
			{"1","1.mp4"},
			{"2","2.mp4"},
			{"3","3.mp4"},
			{"4","4.mp4"},
		};
		std::cout << j.dump(1) << std::endl;

		for (nlohmann::json::iterator it = j.begin(); it != j.end(); ++it) {
			std::cout << it.key() << " : " << it.value() << "\n";
			std::string temp = j[it.key()].get<std::string>();
			videoList->addItem(QString::fromStdString(temp));
		}

	}
	*/
private:
	int port_off = 0;
	int count = 0;


};



