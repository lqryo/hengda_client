#include "tcpclient2.h"

void MainTcpClient::request(
	const std::string& path,
	const nlohmann::json& content,
	std::function<void(nlohmann::json data)> callback)
{
	request(path, "json", content.dump(), callback);
}

void MainTcpClient::request(
	const std::string& path,
	const std::string& content_type,
	const std::string& content,
	std::function<void(nlohmann::json data)> callback)
{
//	PackageHeader header;
//	header.route(path);
//	header.type(REQUEST);
//	header.contenttype(content_type);
//	header.contentlength(content.length());

//	send(header.to_string() + content);
	send(content);
}

void MainTcpClient::response(const std::string& path, const nlohmann::json& json)
{
	response(path, "json", json.dump());
}

void MainTcpClient::response(const std::string& path, const std::string& content_type, const std::string& content)
{
//	PackageHeader header;
//	header.route(path);
//	header.type(RESPONSE);
//	header.contenttype(content_type);
//	header.contentlength(content.length());

//	send(header.to_string() + content);
	send(content);
}