//
// Created by ffiirree on 2019/9/21.
//

#ifndef BORDER_ADD_CAMERA_WINDOW_H
#define BORDER_ADD_CAMERA_WINDOW_H

#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QGridLayout>
#include <QDebug>
#include <QFileDialog>
#include <QtCore/QStandardPaths>
#include "system.h"

class CameraAddWindow : public QDialog {
    Q_OBJECT
public:
    explicit CameraAddWindow(QWidget * parent = nullptr)
        : QDialog(parent) {

        setModal(true);
        setFixedSize(300, 300);
        setWindowTitle("添加设备");

        auto layout = new QVBoxLayout();
        setLayout(layout);

        auto cbox = new QComboBox();
        {
            auto _layout = new QHBoxLayout();
            layout->addLayout(_layout);

            auto cb_label = new QLabel("类型:");
            _layout->addWidget(cb_label);

            cbox->addItem("摄像头", 0);
            cbox->addItem("视频", 1);
            _layout->addWidget(cbox);

        }

        auto cw = new QWidget();
        auto vw = new QWidget();

        // 添加摄像头
        {
            auto cw_layout = new QGridLayout();

            auto ip_input = new QLineEdit();
            auto pwd_input = new QLineEdit();

            cw_layout->addWidget(new QLabel("摄像头IP:"), 0, 0);
            cw_layout->addWidget(ip_input, 0, 1);
            cw_layout->addWidget(new QLabel("摄像头密码:"), 1, 0);
            cw_layout->addWidget(pwd_input, 1, 1);

            cw->setLayout(cw_layout);
            layout->addWidget(cw);
        }

        // 添加视频
        {
            auto vw_layout = new QGridLayout();

            auto addr_input = new QLineEdit();
            auto addr_btn = new QPushButton("...");
            addr_btn->setFixedWidth(35);

            vw_layout->addWidget(new QLabel("视频地址:"), 0, 0);
            vw_layout->addWidget(addr_input, 0, 1);
            vw_layout->addWidget(addr_btn, 0, 2);

            vw->setLayout(vw_layout);
            layout->addWidget(vw);

            connect(addr_btn, &QPushButton::clicked, this, [this, addr_input](){
                video_path_ = QFileDialog::getOpenFileName(this, "请选择文件路径...",
                        QStandardPaths::writableLocation(QStandardPaths::MoviesLocation));
                if(!video_path_.isEmpty()) {
                    addr_input->setText(video_path_);
                    add_btn_->setDisabled(false);
                }
            });
        }
        vw->hide();

        connect(cbox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [cw,vw](int idx){
           qDebug() << idx;
           if(idx == 0) {
               cw->show();
               vw->hide();
           }
           else {
               vw->show();
               cw->hide();
           }
        });

        // 添加/取消按钮
        {
            auto _layout = new QHBoxLayout();
            layout->addLayout(_layout);

            add_btn_ = new QPushButton("添加");
            add_btn_->setDisabled(true);
            cancel_btn_ = new QPushButton("取消");

            connect(cancel_btn_, &QPushButton::clicked, this, &QDialog::close);

            _layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::Expanding, QSizePolicy::Minimum));
            _layout->addWidget(cancel_btn_);
            _layout->addWidget(add_btn_);
        }

        connect(add_btn_, &QPushButton::clicked, this, [this](){
            System::instance().addDevice(Device(video_path_.toStdString()));
            emit added();
        });
    }

signals:
    void added();

private:
    QPushButton *add_btn_ = nullptr;
    QPushButton *cancel_btn_ = nullptr;
    QString video_path_{};
};


#endif //BORDER_ADD_CAMERA_WINDOW_H
