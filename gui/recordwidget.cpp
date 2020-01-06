#include "recordwidget.h"
#include "database.h"
#include "alarmwindow.h"

/*
	TcpClient1::instance().responseMapping("/camera/alarm", [this](nlohmann::json data) {
		qDebug() << "in responseMapping /camera/alarm callbackfunction";
		Alarm alarm(data);
		if (!AlarmService::insert(alarm)) {
			qDebug() << "插入报警失败";
		}

		AlarmWindow *alarm_window = new AlarmWindow();
		alarm_window->setAlarm(alarm);
		alarm_window->show();

		query();
	});
*/


RecordWidget::RecordWidget(QWidget * parent)
	: QWidget(parent) 
{
	rpcthread = new RPCThread();
	alarmthread = new AlramThread();

	rpcthread->start();
	alarmthread->start();
	connect(alarmthread, &AlramThread::hasAlarm, this, &RecordWidget::showAlarm);

}

void RecordWidget::showAlarm()
{
	
	std::vector<AlarmWindow*> alarms_;
	{
		{
			MutexGuard g(mtx);
			alarms_.swap(DeServImpl::alarms);
		}
		for (size_t i = 0; i < alarms_.size(); ++i) {
			alarms_[i]->show();
		}
	}


}

void RecordWidget::updatePage(const std::vector<Alarm>& alarms)
{
	tableWidget->clearContents();

	for (size_t i = 0; i < alarms.size(); ++i) {
		auto alarm = alarms[i];

		tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(alarm.ip_)));
		tableWidget->setItem(i, 1, new QTableWidgetItem(alarm.type_ == 1 ? "越界" : "入侵"));
		tableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(alarm.time_)));
		tableWidget->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(alarm.path_)));
	}
}

void RecordWidget::query() 
{
	if (current_page_ > max_page_) current_page_ = max_page_;
	if (current_page_ < 1) current_page_ = 1;
	
	current_page_w_->setText(std::to_string(current_page_).c_str());

	std::vector<Alarm> alarms;

	if (is_search_) {
		auto ip = ip_input->text().toStdString();
		auto stime = st_input->text().toStdString();
		auto etime = et_input->text().toStdString();
		alarms = AlarmService::searchByIP(ip, stime, etime, (current_page_ - 1) * page_size_, page_size_);
	}
	else {
		alarms = AlarmService::query((current_page_ - 1) * page_size_, page_size_);
	}
	updatePage(alarms);
}

void RecordWidget::search()
{
	is_search_ = true;

	current_page_ = 1;

	auto ip = ip_input->text().toStdString();
	auto stime = st_input->text().toStdString();
	auto etime = et_input->text().toStdString();

	max_page_ = (AlarmService::count(ip, stime, etime) + page_size_ - 1) / page_size_;
	max_page_w_->setText("/ " + QString::number(max_page_));

	query();
}

void RecordWidget::lastPage() 
{
	current_page_ = max_page_;
	query();
}

void RecordWidget::nextPage() 
{
	current_page_ += 1;
	query();
}

void RecordWidget::prevPage()
{
	current_page_ -= 1;
	query();
}

void RecordWidget::init_ui()
{
	auto layout = new QVBoxLayout();
	setLayout(layout);

	auto menu_layout = new QHBoxLayout();
	layout->addLayout(menu_layout);

	ip_input = new QLineEdit();
	ip_input->setPlaceholderText(tr("请输入IP地址"));
	menu_layout->addWidget(ip_input);

	auto label = new QLabel(tr("时间"));

	st_input = new QDateTimeEdit();
	st_input->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
	st_input->setCalendarPopup(true);
	auto sp = new QLabel("-");
	et_input = new QDateTimeEdit();
	et_input->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
	et_input->setDateTime(QDateTime::currentDateTime());
	et_input->setCalendarPopup(true);

	menu_layout->addWidget(label);
	menu_layout->addWidget(st_input);
	menu_layout->addWidget(sp);
	menu_layout->addWidget(et_input);

	auto search_btn = new QPushButton(tr("查询"));
	menu_layout->addWidget(search_btn);

	tableWidget = new QTableWidget();
	layout->addWidget(tableWidget);

	tableWidget->horizontalHeader()->setStretchLastSection(true);
	tableWidget->verticalHeader()->hide();

	tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	tableWidget->setColumnCount(4);
	tableWidget->setRowCount(page_size_);
	tableWidget->setHorizontalHeaderLabels({ "IP地址", "类型", "时间", "报警图片地址" });

	QObject::connect(search_btn, SIGNAL(clicked()), this, SLOT(search()));

	auto _layout = new QHBoxLayout();
	layout->addLayout(_layout);

	auto pre_btn = new QPushButton(tr("上一页"));
	current_page_w_ = new QLineEdit(QString::fromStdString(std::to_string(current_page_)));
	current_page_w_->setFixedWidth(50);
	max_page_w_ = new QLabel("/ " + QString::number(max_page_));
	auto next_btn = new QPushButton(tr("下一页"));

	_layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::Expanding, QSizePolicy::Minimum));
	_layout->addWidget(pre_btn);
	_layout->addWidget(current_page_w_);
	_layout->addWidget(max_page_w_);
	_layout->addWidget(next_btn);
	_layout->addSpacerItem(new QSpacerItem(20, 50, QSizePolicy::Expanding, QSizePolicy::Minimum));

	QObject::connect(pre_btn, SIGNAL(clicked()), this, SLOT(prevPage()));
	QObject::connect(next_btn, SIGNAL(clicked()), this, SLOT(nextPage()));
}