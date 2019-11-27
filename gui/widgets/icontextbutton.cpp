#include "icontextbutton.h"

void IconTextButton::paintEvent(QPaintEvent *) 
{
	painter_.begin(this);

	// background
	painter_.fillRect(rect(), background_);

	// icon
	auto mask = icon_.createMaskFromColor(Qt::transparent);

	icon_.fill(icon_color_);
	icon_.setMask(mask);

	QIcon(icon_).paint(&painter_, { margin_.left(), margin_.top(), iconSize().width(), iconSize().height() });

	//  text
	QFont font;
	font.setPixelSize(16);
	font.setFamily("Microsoft Yahei");
	QPen pen;
	pen.setColor(icon_color_);
	painter_.setPen(pen);
	painter_.setFont(font);
	painter_.drawText(QRect{ margin_.left() + iconSize().width() + margin_.right(), 0, 225, 45 }, Qt::AlignVCenter, text_);

	painter_.end();
}

void IconTextButton::enterEvent(QEvent *event)
{
	if (content_hover_flag_) icon_color_ = hover_color_;
	if (isChecked()) {
		background_ = checked_bg_;
	}
	else if (bg_hover_flag_) {
		background_ = hover_bg_;
	}
	update();
}

void IconTextButton::leaveEvent(QEvent *event)
{
	icon_color_ = isChecked() ? hover_color_ : normal_color_;
	background_ = isChecked() ? checked_bg_ : normal_bg_;
	update();
}