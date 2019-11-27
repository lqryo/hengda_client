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

            auto _input = new QLineEdit();
            _layout->addWidget(_input);

        }

		{
			auto _layout = new QHBoxLayout();
			layout->addLayout(_layout);

			auto label = new QLabel("摄像头ip");
			_layout->addWidget(label);

			cameraName_ = new QLineEdit();
			_layout->addWidget(cameraName_);

			auto label2 = new QLabel("摄像头密码");
			_layout->addWidget(label2);

			cameraPwd_ = new QLineEdit();
			_layout->addWidget(cameraPwd_);

		}

        layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

        {
            auto _layout = new QHBoxLayout();
            layout->addLayout(_layout);

            _layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
            auto reset = new QPushButton("重置");
            auto apply = new QPushButton("应用");
			addBtn_ = new QPushButton("添加");

			_layout->addWidget(addBtn_);
            _layout->addWidget(reset);
            _layout->addWidget(apply);
			
        }

		connect(addBtn_, &QPushButton::clicked, this, &SettingWidget::addCamera);

    }
private:
	QLineEdit* cameraName_;
	QLineEdit* cameraPwd_;
	QPushButton* addBtn_;

	void addCamera()
	{
		auto name = cameraName_->text();
		auto pwd = cameraPwd_->text();
		if (name.isEmpty() || pwd.isEmpty())
		{
			QMessageBox::information(this, tr("提示"), tr("输入为空"), QMessageBox::Ok);
		}
		else {
			if (MarkWidget::cam_cbox_->findText(name) != -1) {
				QMessageBox::information(this, tr("提示"), tr("该相机已存在"), QMessageBox::Ok);
			}
			else {
				Device device(name.toStdString(),pwd.toStdString());
				System::instance().addDevice(device);
				MarkWidget::cam_cbox_->addItem(name);
				QMessageBox::information(this, tr("提示"), tr("摄像头添加成功"), QMessageBox::Ok);
				cameraName_->clear();
				cameraPwd_->clear();
			}

		}
	}
};


#endif //BORDER_SETTINGWIDGET_H
