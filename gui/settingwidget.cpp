//
// Created by ffiirree on 2019/9/21.
//

#include "settingwidget.h"
#include "iptest.h"

void SettingWidget::client_fun()
{
	QString camera_ip = cameraName_->text();
	QString server_ip = serverip_->text();
	g_serverip = server_ip.toStdString();

	std::cout << "ip is " << server_ip.toStdString() << std::endl;
	std::string ip = server_ip.toStdString();

	if (!ICMPPing(ip.c_str())) {
		emit info(1);
		return;
	}

	rpc::Client* c = rpc::new_client(ip.c_str(), 9910, "");
//	rpc::Client* c = rpc::new_client("127.0.0.1", 9910, "");
	
	Json req, res;
	req.add_member("req_id", now::ms());
	req.add_member("method", "set_egbd_task");
	Json params = json::object();
	{
		params.add_member("ip", camera_ip.toStdString().c_str());
		params.add_member("port", 9910);
		std::string url = username_->text().toStdString();
		std::cout << url << std::endl;
		params.add_member("rtsp_url", url.c_str());
		params.add_member("type", 1);
	}
	req.add_member("params", params);
	std::cout << req.pretty() << std::endl;
	c->call(req, res);
	std::cout << res.pretty() << std::endl;

	if (res.is_null()) {
		emit info(2);
	}
	else {
		if (res["code"].get_int() == 200) {
			emit info(0);
		}
		else { emit info(2); }
	}


	delete c;
	WLOG << "delete c";
}

