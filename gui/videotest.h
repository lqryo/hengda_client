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
#include "tcpclient.h"
#include <iostream>


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

		layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

		{
			auto _layout = new QHBoxLayout();
			layout->addLayout(_layout);

			_layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
			select = new QPushButton("选择");
			auto apply = new QPushButton("获取视频");

//			auto connection = new QPushButton("连接");

			select->setDisabled(true);

//			connect(connection, &QPushButton::clicked, this, &VideotestWidget::connection);
			connect(apply, &QPushButton::clicked, this, &VideotestWidget::applyclick);
			connect(select, &QPushButton::clicked, this, &VideotestWidget::selection);

			_layout->addWidget(select);
			_layout->addWidget(apply);
//			_layout->addWidget(connection);
			
		}

		std::function<void(nlohmann::json)> callback;
	}

	static QListWidget* videoList;

private:


	void selection()
	{
		qDebug() << videoList->currentRow();
		nlohmann::json data;
		data["data"] = videoList->currentRow();
		TcpClient::instance().request("", data);
	}

	void applyclick()
	{
		nlohmann::json data;

		// 发送视频请求
		// TcpClient::instance().request()

		data["data"] = "videotest";
		TcpClient::instance().request("", data);
	//	QMessageBox::information(this, tr("info"), tr("iteration end"), QMessageBox::Ok);
		select->setDisabled(false);
	}

	void connection()
	{

	//	TcpClient::instance().connect();
	//	QMessageBox::information(this, tr("info"), tr("连接服务端成功"), QMessageBox::Ok);
	//	showvideo();
	}
	QPushButton* select;
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



};



