#include "tcpclient.h"

void TcpClient::request(
	const std::string& path,
	const nlohmann::json& content,
	std::function<void(nlohmann::json data)> callback)
{
	request(path, "json", content.dump(), callback);
}

void TcpClient::request(
	const std::string& path,
	const std::string& content_type,
	const std::string& content,
	std::function<void(nlohmann::json data)> callback)
{
	PackageHeader header;
	header.route(path);
	header.type(REQUEST);
	header.contenttype(content_type);
	header.contentlength(content.length());

	send(header.to_string() + content);
}

void TcpClient::response(const std::string& path, const nlohmann::json& json)
{
	response(path, "json", json.dump());
}

void TcpClient::response(const std::string& path, const std::string& content_type, const std::string& content)
{
	PackageHeader header;
	header.route(path);
	header.type(RESPONSE);
	header.contenttype(content_type);
	header.contentlength(content.length());

	send(header.to_string() + content);
}