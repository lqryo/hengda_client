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
#include <QThread>
#include <iostream>
#include "alarmservice.h"
#include "rpcclient.h"
#include "base/time.h"
#include "base/thread.h"
#include "base/fastream.h"
#include "base/fs.h"
#include <fstream>
#include <sstream>
#include <mutex>

using namespace std;

extern Mutex mtx;

extern std::string g_server_ip;
extern std::string g_client_ip;

class RPCThread : public QThread 
{
	Q_OBJECT
public:
	RPCThread()
	{
		fs::file f("setting.json", 'r');
		char buf[512];
		if (f) {
			f.read(buf, 256);
		}
		fastring s(buf);
		Json y = json::parse(s.data(), s.size());
		std::cout << y.pretty() << std::endl;
		Json client_ip = y.find("client_ip");
		if (client_ip.is_string()) {
			g_client_ip = client_ip.get_string();
		}
		Json server_ip = y.find("server_ip");
		if (server_ip.is_string()) {
			g_server_ip = server_ip.get_string();
		}
		f.close();
		std::cout << "client ip is " << g_client_ip << std::endl;
		std::cout << "server ip is " << g_server_ip << std::endl;

	}

	~RPCThread() = default;
protected:

	void run()
	{
		rpc::Server* server = rpc::new_server(g_client_ip.c_str(), 9900, "");
		server->add_service(new DeServImpl);
		server->start();
		while (!stop_) {
			sleep::sec(5);
			std::cout << "running" << std::endl;
		}		
	}
private:
	bool stop_ = false;
	std::string ip;
};

class AlramThread : public QThread
{
	Q_OBJECT
public:
	AlramThread() = default;
	~AlramThread() = default;
protected:

	void run()
	{
		while (!stop_)
		{
			if (DeServImpl::alarms.size() > 0) {
				emit hasAlarm();
			}
			sleep::sec(1);

		}
	}

signals:
	void hasAlarm();

private:
	bool stop_ = false;
};






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

	void showAlarm();

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

	RPCThread* rpcthread;
	AlramThread* alarmthread;

	
};

#endif //BORDER_RECORD_WIDGET_H
