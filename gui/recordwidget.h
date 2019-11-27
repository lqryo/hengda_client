//
// Created by ffiirree on 2019/9/21.
// 报警记录界面
//

#ifndef BORDER_RECORD_WIDGET_H
#define BORDER_RECORD_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QDateTimeEdit>
#include <QHeaderView>
#include <mysql.h>
#include <QDebug>
#include <string>
#include "alarmservice.h"

using namespace std;
class RecordWidget : public QWidget {
	Q_OBJECT
public:
	explicit RecordWidget(QWidget * parent = nullptr);

public slots:
	void query();
	void search();

	void lastPage();
	void prevPage();
	void nextPage();

private:
	void updatePage(const std::vector<Alarm>& alarms);
	void init_ui();

	static const int page_size_ = 20;
	int max_page_ = 1;
	int current_page_ = 1;

	QLineEdit *ip_input;
	QDateTimeEdit *st_input;
	QDateTimeEdit *et_input;
	QTableWidget *tableWidget;
	QLabel *max_page_w_;
	QLineEdit *current_page_w_;

	bool is_search_ = false;
};

#endif //BORDER_RECORD_WIDGET_H
