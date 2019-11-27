//
// Created by ffiirree on 2019/9/21.
// 设备管理界面
//

#ifndef BORDER_DEVICE_WIDGET_H
#define BORDER_DEVICE_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include "utils.h"
#include "addcamerawindow.h"
#include "tcpclient.h"


class DeviceWidget : public QWidget {
    Q_OBJECT
public:
    explicit DeviceWidget(QWidget * parent = nullptr)
        : QWidget(parent) {

        auto layout = new QVBoxLayout();
        setLayout(layout);

        // 操作栏
        {
            auto menu_layout = new QHBoxLayout();
            layout->addLayout(menu_layout);

            auto input = new QLineEdit();
			input->setPlaceholderText(tr("请输入设备IP地址"));
            menu_layout->addWidget(input);

            auto search_btn = new QPushButton("查询");
            menu_layout->addWidget(search_btn);

            menu_layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::Expanding, QSizePolicy::Minimum));

            auto refresh_btn = new QPushButton("刷新");
			connect(refresh_btn, &QPushButton::clicked, this, &DeviceWidget::refresh);
            auto add_btn = new QPushButton("添加");

            connect(add_btn, &QPushButton::clicked, this, [this](){
               auto window = new CameraAddWindow(this);
               window->show();

               connect(window, &CameraAddWindow::added, this, [this, window](){
                   //addItem(System::instance().devices().back());
                   window->close();
                   window->deleteLater();
               });
            });

            menu_layout->addWidget(refresh_btn);
            menu_layout->addWidget(add_btn);
        }

        // 设备列表
        {
            table_ = new QTableWidget();
			table_->setSelectionBehavior(QAbstractItemView::SelectRows);
            layout->addWidget(table_);

            table_->horizontalHeader()->setStretchLastSection(true);
            table_->verticalHeader()->hide();

            table_->setColumnCount(6);
            table_->setRowCount(16);
            table_->setHorizontalHeaderLabels({"IP", "宽", "高", "帧率", "状态", "操作" });
            table_->setColumnWidth(0, 300);
        }

		refresh();
    }


    void addItem(const Device &camera)
    {
        auto row = table_->rowCount();

        table_->insertRow(row);
        table_->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(camera.name())));

        auto run_btn = new QPushButton("启动");
        table_->setCellWidget(row, 5, run_btn);
        connect(run_btn, &QPushButton::clicked, this, [this, row, camera](){
            ////auto thread = new DetectThread(this);
            //thread->setDevice(camera);
            //thread->start();
        });
    }

	void updatePage()
	{
		table_->clearContents();
		auto devices = System::instance().devices();
		size_t idx = 0;
		for (const auto& item : devices) {
			table_->setItem(idx, 0, new QTableWidgetItem(QString::fromStdString(item.second.name())));
			idx++;
		}
	}

	void refresh();
private:
    QTableWidget *table_ = nullptr;
};


#endif //BORDER_DEVICE_WIDGET_H
