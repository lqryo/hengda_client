#include "titlebar.h"
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QStyle>
#include <QBitmap>

TitleBar::TitleBar(QWidget *parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StyledBackground);
	setAutoFillBackground(true);

	setContentsMargins(15, 0, 10, 0);

	auto layout = new QHBoxLayout();
	layout->setSpacing(0);
	layout->setMargin(5);
	setLayout(layout);

	// Logo / title
	icon_label_ = new QLabel();
    auto logo = QPixmap(":/image/res/camera");
    auto mask = logo.createMaskFromColor(Qt::transparent);
    logo.fill(Qt::white);
    logo.setMask(mask);
    icon_label_->setPixmap(logo);
    icon_label_->setFixedSize(30, 30);
    icon_label_->setScaledContents(true);

	title_label_ = new QLabel("");
	title_label_->setObjectName("title");
	title_label_->setAlignment(Qt::AlignCenter);

	layout->addWidget(icon_label_);			// logo
	layout->addWidget(title_label_);		// title


	// space
//	layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::Expanding, QSizePolicy::Minimum));

	// min/max/close buttons
	min_btn_ = new IconButton(QPixmap(":/image/res/min"), {30, 30}, {15, 15});
	min_btn_->setObjectName("min");
	min_btn_->setIconColor(QColor("#cccccc"));
	min_btn_->setIconHoverColor(QColor("#ffffff"));
	connect(min_btn_, &IconButton::clicked, this, &TitleBar::minimize);

	max_btn_ = new IconButton(QPixmap(":/image/res/max"),  {30, 30}, {15, 15});
	max_btn_->setObjectName("max");
	max_btn_->setIconColor(QColor("#cccccc"));
	max_btn_->setIconHoverColor(QColor("#ffffff"));
	connect(max_btn_, &IconButton::clicked, this, [this]() {
		if (is_maximized_) {
			is_maximized_ = false;
			normal();
			max_btn_->setIcon(QPixmap(":/image/res/max"));
		}
		else {
			is_maximized_ = true;
			maximize();
			max_btn_->setIcon(QPixmap(":/image/res/normal"));
		}
	});

	close_btn_ = new IconButton(QPixmap(":/image/res/close"), {30, 30}, {15, 15});
	close_btn_->setObjectName("close");
	close_btn_->setIconColor(QColor("#cccccc"));
	close_btn_->setIconHoverColor(QColor("#ffffff"));
	connect(close_btn_, &IconButton::clicked, this, &TitleBar::close);

	
	layout->addWidget(min_btn_);			// 
	layout->addWidget(max_btn_);
	layout->addWidget(close_btn_);
}

void TitleBar::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		begin_ = event->globalPos();
		moving = true;
	}
}

void TitleBar::mouseMoveEvent(QMouseEvent * event)
{
    auto mouse_pos = event->globalPos();
	if (moving) {
        moved(mouse_pos - begin_);
        begin_ = mouse_pos;
	}
}

void TitleBar::mouseReleaseEvent(QMouseEvent * event)
{
	moving = false;
}