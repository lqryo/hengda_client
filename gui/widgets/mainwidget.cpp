#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
	: QWidget(parent)
{
	// 1. 设置为无边框窗口，并设置为北京透明，以此自定义窗口
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);
	setMouseTracking(true);

	setMinimumSize(1350, 825);

	// 2. 添加布局
	auto layout = new QVBoxLayout();
	layout->setSpacing(0);
	layout->setMargin(0);
	setLayout(layout);

	// 3. 新建控件作为可展示的主窗体，接下来所有设置在该窗体进行
	auto widget = new QWidget();
	widget->setObjectName("mainwidget");
	layout->addWidget(widget);

	// 4. 为主窗体设置阴影
	setContentsMargins(25, 25, 25, 25);
	auto effect = new QGraphicsDropShadowEffect(this);
	effect->setBlurRadius(20);
	effect->setOffset(0);
	effect->setColor(QColor(0, 0, 0, 175));
	widget->setGraphicsEffect(effect);

	// 5. 为主窗体设置布局
	auto sub_layout = new QVBoxLayout();
	sub_layout->setSpacing(0);
	sub_layout->setMargin(0);
	widget->setLayout(sub_layout);

	// 设置标题栏
	{
		titlebar_ = new TitleBar();
		titlebar_->setFixedHeight(50);
		titlebar_->setTitle(tr(""));
		connect(titlebar_, &TitleBar::close, this, &MainWidget::close);
		connect(titlebar_, &TitleBar::moved, this, [this](const QPoint& m) {
			this->move(this->pos() + m);
		});
		connect(titlebar_, &TitleBar::minimize, this, &MainWidget::showMinimized);
		connect(titlebar_, &TitleBar::maximize, this, [this]() {
			setContentsMargins(0, 0, 0, 0);
			showMaximized();
		});
		connect(titlebar_, &TitleBar::normal, this, [this]() {
			setContentsMargins(25, 25, 25, 25);
			showNormal();
		});
		sub_layout->addWidget(titlebar_);
	}

	// 主体窗口
	{
		auto center_widget = new QWidget();
		center_widget->setObjectName("center_widget");

		center_layout = new QHBoxLayout();
		center_layout->setSpacing(0);
		center_layout->setMargin(0);
		center_widget->setLayout(center_layout);
		sub_layout->addWidget(center_widget);

		// Menu & content
		auto menu_widget = new QWidget();
		menu_widget->setObjectName("menu_widget");

		menu_layout = new QVBoxLayout();
		menu_layout->setSpacing(0);
		menu_layout->setMargin(0);
		menu_widget->setLayout(menu_layout);
		menu_widget->setFixedWidth(226);
		center_layout->addWidget(menu_widget);

		group_ = new QButtonGroup(this);
	}

	LOAD_QSS(this, ":/qss/res/table.qss");
}

void MainWidget::setWindowName(const QString& name)
{
	titlebar_->setTitle(name);
}

#define FIRST_THEME(item)		item->setNormalColor(QColor("#333333"));\
		item->setIconMargins({ 20, 10, 20, 10 });\
		item->setFixedSize(225, 45);\
		item->setIconSize({25, 25});\
		item->setBackground(QColor("#e9e9ea"))

#define SECOND_THEME(item)		item->setNormalColor(QColor("#666666"));\
		item->setHoverColor(QColor("#c62f2f"));\
		item->setIconMargins({ 20, 10, 20, 10 });\
		item->setFixedSize(225, 45);\
		item->setIconSize({25, 25});\
		item->setBackground(QColor("#ededef"));\
		group_->addButton(item);

void MainWidget::addWidget(IconTextButton * btn, QWidget * widget)
{
	menu_layout->addWidget(btn);

	if (widget != nullptr) {
		widget->hide();
		SECOND_THEME(btn);
		center_layout->addWidget(widget);
		if (current_widget_ == nullptr) {
			current_widget_ = widget;
			btn->setChecked(true);
			widget->show();
		}

		connect(btn, &IconTextButton::clicked, this, [this, widget]() {
			current_widget_->hide();
			current_widget_ = widget;
			widget->show();
		});
	}
	else {
		FIRST_THEME(btn);
	}
}
#undef FIRST_THEME
#undef SECOND_THEME