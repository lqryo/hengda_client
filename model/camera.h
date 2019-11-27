//
// ffiirree@20190921
//

#ifndef BORDER_CAMERA_H
#define BORDER_CAMERA_H

#include <string>
#include "utils.h"
#include "json.hpp"

class Device {
public:
    enum CameraStatus:unsigned int {
		STOPED,
		RUNNING,
    };

    enum DeviceType{
        UNKNOWN,
        CAMERA,
        VIDEO
    };

public:
    Device() = default;

    explicit Device(std::string addr)
        : addr_(std::move(addr)),type_(VIDEO) { }

    Device(std::string ip, std::string pwd)
        : ip_(std::move(ip)), pwd_(std::move(pwd)), type_(CAMERA) { }

	Device(const nlohmann::json& device) : type_(CAMERA) {
		json_get(device, "ip", ip_);
		json_get(device, "password", pwd_);
		json_get(device, "height", height_);
		json_get(device, "width", width_);
		json_get(device, "status", status_);
		
	}

    std::string name() const
    {
        switch(type_) {
            case UNKNOWN:
                return {};
            case CAMERA:
                return ip_;
            case VIDEO:
                return addr_;
        }
    }

    DeviceType type() const { return type_; }

	std::string ip() const { return ip_; }
	uint32_t height() const { return height_; }
	uint32_t width() const { return width_; }

private:
    std::string ip_ {};
    std::string pwd_{};

	uint32_t height_ = 0;
	uint32_t width_ = 0;

    DeviceType type_{ UNKNOWN };

	uint32_t status_{ STOPED };

    // 视频测试使用
    std::string addr_{};
};

#endif //BORDER_CAMERA_H
