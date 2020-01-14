// 绘制边界的主界面

#ifndef BORDER_MARK_WIDGET_H
#define BORDER_MARK_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <fstream>
#include <QComboBox>
#include <QMessageBox>
#include "imageeditmenu.h"
#include "canvaswidget.h"
#include "base/json.h"
#include "base/time.h"
#include "base/rpc.h"


class MarkWidget : public QWidget {
    Q_OBJECT

public:
    explicit MarkWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
        auto layout = new QVBoxLayout();
        layout->setSpacing(0);
        layout->setMargin(0);
        setLayout(layout);

        // 设置菜单栏
        {
            auto menubar = new QHBoxLayout();
            menubar->setContentsMargins(0, 0, 15, 0);
            menubar->setSpacing(0);

            menu_ = new ImageEditMenu();
            menu_->setFixedHeight(40);
            menubar->addWidget(menu_);
            menubar->addSpacerItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));

            auto chose_label = new QLabel("选择摄像头: ");
            menubar->addWidget(chose_label);

            cam_cbox_ = new QComboBox();
            cam_cbox_->setFixedWidth(300);
			
//			cam_cbox_->addItem("1");
//			cam_cbox_->addItem("2");
//			cam_cbox_->addItem("3");

            menubar->addWidget(cam_cbox_);
            layout->addLayout(menubar);
        }


        // 绘制控件
        {
            canvas_ = new CanvasWidget();
//            canvas_->setPixmap(QPixmap("/home/ffiirree/code/border/cover.png"));
            layout->addWidget(canvas_);
        }

        // 底部操作
        {
            auto op_layout = new QHBoxLayout();
            op_layout->setContentsMargins(0, 0, 15, 0);
            op_layout->setSpacing(15);

            ok_btn_ = new QPushButton("保存");
            cancel_btn_ = new QPushButton("清空");
            op_layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::Expanding, QSizePolicy::Minimum));
            op_layout->addWidget(ok_btn_);
            op_layout->addWidget(cancel_btn_);

            layout->addLayout(op_layout);
        }

        // 信号操作
        connect(menu_, &ImageEditMenu::undo, canvas_, &CanvasWidget::undo);
        connect(menu_, &ImageEditMenu::redo, canvas_, &CanvasWidget::redo);

//		connect(cam_cbox_, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &MarkWidget::showFrameOfCamera);
		connect(cam_cbox_, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::activated), this, &MarkWidget::showFrameOfCamera);

        connect(canvas_, &CanvasWidget::undoEmptied, menu_, &ImageEditMenu::disableUndo);
        connect(canvas_, &CanvasWidget::redoEmptied, menu_, &ImageEditMenu::disableRedo);

        connect(menu_, &ImageEditMenu::changed, canvas_, &CanvasWidget::graph);

        connect(ok_btn_, &QPushButton::clicked, canvas_, &CanvasWidget::save);
		connect(cancel_btn_, &QPushButton::clicked, canvas_, &CanvasWidget::clear);

		connect(this, &MarkWidget::info, this, &MarkWidget::info_process);

		cam_cbox_->setDuplicatesEnabled(true);
    }

    static QComboBox* cam_cbox_;

public slots:
    void showFrameOfCamera(QString idx);
	void info_process(int code) {
		if (code == 1) {
			QMessageBox::information(this, tr("错误"), tr("无法连接ipc"), QMessageBox::Ok);
		}
		else if (code == 2) {
			QMessageBox::information(this, tr("错误"), tr("获取图片失败"), QMessageBox::Ok);
		}
		else if (code == 3) {
			QMessageBox::information(this, tr("错误"), tr("图片打开失败或不存在"), QMessageBox::Ok);
		}
		else {
			QMessageBox::information(this, tr("错误"), tr("未知错误"), QMessageBox::Ok);
		}
	}

protected:
    void showEvent(QShowEvent *event) override ;

private:
    CanvasWidget * canvas_ = nullptr;               // 绘制区域
    ImageEditMenu * menu_ = nullptr;                // 操作菜单

    QPushButton * ok_btn_ = nullptr;
    QPushButton * cancel_btn_ = nullptr;

	void client_fun();

signals:
	void info(int code);

};


#endif //BORDER_MARK_WIDGET_H
