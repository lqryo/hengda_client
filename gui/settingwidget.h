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
				startIPCBtn_ = new QPushButton("启动ipc");
				addCameraBtn_ = new QPushButton("添加摄像头");

				_layout->addWidget(addCameraBtn_);
				_layout->addWidget(startIPCBtn_);
				//            _layout->addWidget(reset);

			}

			connect(addCameraBtn_, &QPushButton::clicked, this, &SettingWidget::addCamera);
			connect(startIPCBtn_, &QPushButton::clicked, this, &SettingWidget::startIPC);
			connect(this, &SettingWidget::info, this, &SettingWidget::info_process);

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

	QPushButton* addCameraBtn_;
	QPushButton* startIPCBtn_;

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

	void startIPC()  //启动ipc
	{
		QString ip = serverip_->text();
		System::instance().server_ip_ = ip;

	//	client = rpc::new_client(ip.toStdString().c_str(), 9910, "");
		go(&SettingWidget::client_fun,this);


	}

	void client_fun();

	void info_process(int flag)
	{
		if (flag == 1) {
			QMessageBox::information(this, tr("错误"), tr("网络连接失败"), QMessageBox::Ok);
		}
		else if(flag == 2){
			QMessageBox::information(this, tr("错误"), tr("启动ipc失败"), QMessageBox::Ok);
		}
		else if (flag == 0) {
			QMessageBox::information(this, tr("提示"), tr("启动ipc成功"), QMessageBox::Ok);
		}
		else {
			QMessageBox::information(this, tr("提示"), tr("未知错误"), QMessageBox::Ok);
		}
	}

signals:
	void info(int flag);

};// SettingWidget class


#endif //BORDER_SETTINGWIDGET_H
