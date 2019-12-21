#include "devicewidget.h"
#include "json.hpp"

using JSON = nlohmann::json;

void DeviceWidget::refresh()
{
	auto socket = &TcpClient1::instance();
	socket->request("/device/list", "json", "", [this](nlohmann::json data) {
		if (!data.is_array()) return;

		for (const auto& item : data) {
			System::instance().addDevice(Device(item));
		}
		updatePage();
	});
}