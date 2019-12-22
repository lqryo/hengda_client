#include "devicewidget.h"
#include "json.hpp"

using JSON = nlohmann::json;

#define deviceRequest(class_name,socket_name)	\
auto socket_name = &class_name##::instance();	\
socket_name->request("/device/list", "json", "", [this](nlohmann::json data) {	\
	if (!data.is_array()) return;	\
	for (const auto& item : data) {	\
		System::instance().addDevice(Device(item));	\
	}	\
	updatePage();	\
});

void DeviceWidget::refresh()
{
	deviceRequest(TcpClient1, socket1)
	deviceRequest(TcpClient2, socket2)
	deviceRequest(TcpClient3, socket3)
	deviceRequest(TcpClient4, socket4)
	deviceRequest(TcpClient5, socket5)
	deviceRequest(TcpClient6, socket6)
	deviceRequest(TcpClient7, socket7)
	deviceRequest(TcpClient8, socket8)
	deviceRequest(TcpClient9, socket9)
	deviceRequest(TcpClient10, socket10)
	deviceRequest(TcpClient11, socket11)
	deviceRequest(TcpClient12, socket12)
	deviceRequest(TcpClient13, socket13)
	deviceRequest(TcpClient14, socket14)
	deviceRequest(TcpClient15, socket15)


	/*
	auto socket = &TcpClient1::instance();
	socket->request("/device/list", "json", "", [this](nlohmann::json data) {
		if (!data.is_array()) return;

		for (const auto& item : data) {
			System::instance().addDevice(Device(item));
		}
		updatePage();
	});
	*/
}