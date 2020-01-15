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
#include <base/json.h>
#include <base/rpc.h>
#include <base/time.h>
#include <base/fastring.h>
#include <base/co.h>

extern bool ICMPPing(const char* szIP);
extern std::string g_serverip;

class VideotestWidget : public QWidget {
	Q_OBJECT
public:
	explicit VideotestWidget(QWidget *parent = nullptr)
		: QWidget(parent) {
		auto layout = new QVBoxLayout();
		setLayout(layout);

		{
			auto _layout = new QHBoxLayout();
			layout->addLayout(_layout);

			auto label = new QLabel("测试视频列表");
			_layout->addWidget(label);
		}

		layout->addWidget(videoList);

		layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

		{
			auto _layout = new QHBoxLayout();
			layout->addLayout(_layout);

			_layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
			select = new QPushButton("选择视频");
			auto apply = new QPushButton("获取视频");

			select->setDisabled(true);

			connect(apply, &QPushButton::clicked, this, &VideotestWidget::applyclick);
			connect(select, &QPushButton::clicked, this, &VideotestWidget::selection);


			_layout->addWidget(apply);
			_layout->addWidget(select);
			
		}

		connect(this, &VideotestWidget::info, this, &VideotestWidget::info_process);
	}

	static QListWidget* videoList;
//	static QListWidget* CameraList;

public slots:
	void info_process(int code)
	{
		if (code == 1) {
			QMessageBox::information(this, tr("错误"), tr("无法连接服务器"), QMessageBox::Ok);
		}
		else if (code == 2) {
			QMessageBox::information(this, tr("错误"), tr("获取视频列表失败"), QMessageBox::Ok);
		}
		else if (code == 3) {
			QMessageBox::information(this, tr("错误"), tr("视频打开失败"), QMessageBox::Ok);
		}
		else {
			QMessageBox::information(this, tr("错误"), tr("未知错误"), QMessageBox::Ok);
		}
	}



private:
	QPushButton* select;


	void selection()
	{
		go(&VideotestWidget::selection_client_fun, this);
	}

	void applyclick()
	{
		go(&VideotestWidget::apply_client_fun, this);
	}

	void apply_client_fun()   //请求测试视频列表
	{
		if (!ICMPPing(g_serverip.c_str())) {
			emit info(1); //code=1,服务器连接失败 
			return;
		}

		Json req, res;

		req.add_member("req_id", now::ms());
		req.add_member("method", "apply_video");

		rpc::Client* c = rpc::new_client(g_serverip.c_str(), 9910, "");

		c->call(req, res);
		if (res.is_null() || res["code"].get_int()!= 200) {
			emit info(2); //code=2,.获取视频列表失败
			return;
		}
		else {
			auto videos = res["videos"];

			for (uint32 i = 0; i < videos.size(); i++) {
				Json& v = videos[i];
				videoList->addItem(v.str().c_str());
			}
			if(videos.size() != 0) select->setDisabled(false);
		}		
	}

	void selection_client_fun()
	{
		if (!ICMPPing(g_serverip.c_str())) {
			emit info(1); //code=1,服务器连接失败 
			return;
		}

		Json req, res;
		req.add_member("req_id", now::ms());
		req.add_member("method", "select_video");
		QString videoname = videoList->currentItem()->text();
		req.add_member("videoname", videoname.toStdString().c_str());

		rpc::Client* c = rpc::new_client(g_serverip.c_str(), 9910, "");

		c->call(req, res);
		if (res.is_null() || res["code"].get_int() != 200) {
			emit info(3);
			return;
		}
	}



signals:
	int info(int code);
	


};



