#ifndef BORDER_ICONTEXT_BUTTON_H
#define BORDER_ICONTEXT_BUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QBitmap>
#include <QPixmap>


class IconTextButton : public QPushButton {
	Q_OBJECT
public:
	explicit IconTextButton(QWidget *parent = nullptr)
		: QPushButton(parent)
	{
		setCursor(Qt::PointingHandCursor);

		connect(this, &IconTextButton::toggled, [this](bool checked) {
			background_ = checked ? checked_bg_ : normal_color_;
			icon_color_ = checked ? hover_color_ : normal_color_;
			update();
		});
	}

	IconTextButton(const QPixmap &icon, const QString& text, bool checkable = false, QWidget *parent = nullptr)
		: QPushButton(parent), icon_(icon), text_(text)
	{
		setCursor(Qt::PointingHandCursor);
		setCheckable(checkable);

		connect(this, &IconTextButton::toggled, [this](bool checked) {
			background_ = checked ? checked_bg_ : normal_bg_;
			icon_color_ = checked ? hover_color_ : normal_color_;
			update();
		});
	}

	void setHoverColor(const QColor& color) { hover_color_ = color; content_hover_flag_ = true; update(); }
	void setNormalColor(const QColor& color) { icon_color_ = normal_color_ = color; update(); }

	void setIcon(const QPixmap& icon) { icon_ = icon; update(); }

	void setIconMargins(const QMargins& margin) { margin_ = margin; }
	void setBackground(const QColor& color) { background_ = normal_bg_ = color; update(); }
	void setHoverBackground(const QColor& color) { bg_hover_flag_ = true; hover_bg_ = color; update(); }

protected:
	virtual void paintEvent(QPaintEvent *) override;
	virtual void enterEvent(QEvent *event) override;
	virtual void leaveEvent(QEvent *event) override;

private:
	QPainter painter_;
	QPixmap icon_;
	QString text_;
	QMargins margin_;
	QSize icon_size_;

	bool content_hover_flag_ = false;
	bool bg_hover_flag_ = false;


	QColor icon_color_ = Qt::white;
	QColor normal_color_ = Qt::white;
	QColor hover_color_ = Qt::gray;

	QColor background_ = Qt::transparent;
	QColor normal_bg_ = Qt::white;
	QColor hover_bg_ = Qt::white;
	QColor checked_bg_ = Qt::white;
};


#endif // BORDER_ICONTEXT_BUTTON_H