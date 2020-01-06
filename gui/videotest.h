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
#include <iostream>
#include <string>
#include <windows.h>
#include <QTcpSocket>



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
			auto apply = new QPushButton("获取视频");

			openCamera = new QPushButton("选择相机");
			closeCamera = new QPushButton("关闭相机");
			connectCamera = new QPushButton("连接相机");

//			auto connection = new QPushButton("连接");

			select->setDisabled(true);

//			connect(connection, &QPushButton::clicked, this, &VideotestWidget::connection);
			connect(apply, &QPushButton::clicked, this, &VideotestWidget::applyclick);
			connect(select, &QPushButton::clicked, this, &VideotestWidget::selection);
			connect(openCamera, &QPushButton::clicked, this, &VideotestWidget::Open);
			connect(closeCamera, &QPushButton::clicked, this, &VideotestWidget::Close);
			connect(connectCamera, &QPushButton::clicked, this, &VideotestWidget::link);

			_layout->addWidget(openCamera);
//			_layout->addWidget(closeCamera);
//			_layout->addWidget(connectCamera);
			_layout->addWidget(apply);
			_layout->addWidget(select);
			
		}

	}

	static QListWidget* videoList;
	static QListWidget* CameraList;
	static std::map<std::string, int> ip2port;
	static std::map<int, std::string> port2ip;

private:


	void selection()
	{

	}

	void applyclick()
	{

	}

	void link()
	{
		
	}

	void Open()
	{

		

	}

	void Close()
	{

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
	QPushButton* connectCamera;

private:
	int port_off = 0;
	int count = 0;


};



