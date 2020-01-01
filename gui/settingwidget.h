//
// Created by ffiirree on 2019/9/21.
//

#ifndef BORDER_SETTINGWIDGET_H
#define BORDER_SETTINGWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include "system.h"
#include "markwidget.h"
#include "videotest.h"
#include "base/rpc.h"
#include "base/co.h"
#include "base/json.h"
#include "base/time.h"
#include "base/log.h"
#include "base/flag.h"

extern std::string g_serverip;


class SettingWidget : public QWidget {
	Q_OBJECT
public:
	explicit SettingWidget(QWidget *parent = nullptr)
		: QWidget(parent) {
		auto layout = new QVBoxLayout();
		setLayout(layout);

		auto autorun = new QCheckBox("开机启动");
		layout->addWidget(autorun);

		{
			auto _layout = new QHBoxLayout();
			layout->addLayout(_layout);

			auto label = new QLabel("服务器地址:");
			_layout->addWidget(label);

			serverip_ = new QLineEdit();
			serverip_->setText(QString("192.168.2.111"));
			_layout->addWidget(serverip_);

		}

		{
			auto _layout = new QHBoxLayout();
			layout->addLayout(_layout);

			auto label = new QLabel("摄像头ip");
			_layout->addWidget(label);

			cameraName_ = new QLineEdit("192.168.1.65");
			_layout->addWidget(cameraName_);

			auto label1 = new QLabel("rtsp");
			_layout->addWidget(label1);
			username_ = new QLineEdit("rtsp://admin:IDM8779235101@192.168.1.65:554/h265/ch1/main/av_stream");
			_layout->addWidget(username_);

			
			{
				auto _layout = new QHBoxLayout();
			//	layout->addLayout(_layout);

				auto label1 = new QLabel("用户名");
				_layout->addWidget(label1);

				//username_ = new QLineEdit("admin");
				//_layout->addWidget(username_);

				auto label2 = new QLabel("密码");
				_layout->addWidget(label2);

				cameraPwd_ = new QLineEdit("IDM8779235101");
				_layout->addWidget(cameraPwd_);

				auto label3 = new QLabel("通道号");
				_layout->addWidget(label3);

				channum_ = new QLineEdit("ch1");
				_layout->addWidget(channum_);

				auto label4 = new QLabel("主次码流");
				_layout->addWidget(label4);

				codestream_ = new QLineEdit("main");
				_layout->addWidget(codestream_);

				auto label5 = new QLabel("录像时长（s）");
				_layout->addWidget(label5);

				film_ = new QLineEdit("0");
				_layout->addWidget(film_);

			}
			

			layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

			{
				auto _layout = new QHBoxLayout();
				layout->addLayout(_layout);

				_layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
				//            auto reset = new QPushButton("重置");
				applyBtn_ = new QPushButton("连接服务器");
				addBtn_ = new QPushButton("添加摄像头");

				_layout->addWidget(addBtn_);
				_layout->addWidget(applyBtn_);
				//            _layout->addWidget(reset);

			}

			connect(addBtn_, &QPushButton::clicked, this, &SettingWidget::addCamera);
			connect(applyBtn_, &QPushButton::clicked, this, &SettingWidget::addIP);

		}
	}

private:
	QLineEdit* serverip_;
	QLineEdit* cameraName_;
	QLineEdit* cameraPwd_;
	QLineEdit* username_;
	QLineEdit* channum_;
	QLineEdit* codestream_;
	QLineEdit* film_;

	QPushButton* addBtn_;
	QPushButton* applyBtn_;

	void addCamera()
	{
		auto name = cameraName_->text();
		auto pwd = cameraPwd_->text();
		auto user = username_->text();
		auto channum = channum_->text();
		auto codestream = codestream_->text();
		auto film = film_->text();

		if (name.isEmpty() || pwd.isEmpty() || user.isEmpty() || channum.isEmpty() || codestream.isEmpty() || film.isEmpty())
		{
			QMessageBox::information(this, tr("提示"), tr("输入为空"), QMessageBox::Ok);
		}
		else {
			if (MarkWidget::cam_cbox_->findText(name) != -1) {
				QMessageBox::information(this, tr("提示"), tr("该相机已存在"), QMessageBox::Ok);
			}
			else {
				Device device(name.toStdString(), pwd.toStdString());

				CameraInfo camerainfo(name, pwd, user, channum, codestream, film.toInt());

				System::instance().addDevice(device);
				System::instance().addCamera(camerainfo);

				MarkWidget::cam_cbox_->addItem(name);
				VideotestWidget::CameraList->addItem(name);
				QMessageBox::information(this, tr("提示"), tr("摄像头添加成功"), QMessageBox::Ok);
				//cameraName_->clear();
				//cameraPwd_->clear();
				//username_->clear();
				//channum_->clear();
				//codestream_->clear();
				//film_->clear();
			}

		}
	}

	void addIP()
	{
		QString ip = serverip_->text();
		System::instance().server_ip_ = ip;
		Json x = json::object();
		x.add_member("ip", "1.1.1.1");

	//	client = rpc::new_client(ip.toStdString().c_str(), 9910, "");
		go(&SettingWidget::client_fun,this);


		QMessageBox::information(this, tr("提示"), tr("服务器连接成功"), QMessageBox::Ok);
	}

	void client_fun() {
		LOG << "in client fun";
		QString camera_ip = cameraName_->text();
		QString server_ip = serverip_->text();
		g_serverip = server_ip.toStdString();

		std::cout << "ip is " << server_ip.toStdString() << std::endl;
		std::string ip = server_ip.toStdString();


		rpc::Client* c = rpc::new_client(ip.c_str(), 9910, "");
		Json req, res;
		req.add_member("req_id", now::ms());
		req.add_member("method", "set_egbd_task");
		Json params = json::object();
		{
			params.add_member("ip", camera_ip.toStdString().c_str());
			params.add_member("port", 9910);
			std::string url = username_->text().toStdString();
			std::cout << url << std::endl;
			params.add_member("rtsp_url", url.c_str());
			params.add_member("type", 1);
		}
		req.add_member("params", params);
		std::cout << req.pretty() << std::endl;
		c->call(req, res);
		std::cout << res.pretty() << std::endl;

		delete c;
	}



//	rpc::Client* client;
};// SettingWidget class


#endif //BORDER_SETTINGWIDGET_H
