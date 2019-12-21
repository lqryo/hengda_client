#include "tcpclient.h"
# define tcpfun(class_name)	\
void class_name::request(	\
	const std::string& path,	\
	const nlohmann::json& content,	\
	std::function<void(nlohmann::json data)> callback)	\
{	\
	request(path, "json", content.dump(), callback);	\
}	\
void class_name::request(	\
	const std::string& path,	\
	const std::string& content_type,	\
	const std::string& content,	\
	std::function<void(nlohmann::json data)> callback)	\
{	\
	mapping_table_[path] = callback;	\
	PackageHeader header;	\
	header.route(path);	\
	header.type(REQUEST);	\
	header.contenttype(content_type);	\
	header.contentlength(content.length());	\
	send(header.to_string() + content);	\
}	\
void class_name::response(const std::string& path, const nlohmann::json& json)	\
{	\
	response(path, "json", json.dump());	\
}	\
void class_name::response(const std::string& path, const std::string& content_type, const std::string& content)	\
{	\
	PackageHeader header;	\
	header.route(path);	\
	header.type(RESPONSE);	\
	header.contenttype(content_type);	\
	header.contentlength(content.length());	\
	send(header.to_string() + content);	\
}

tcpfun(TcpClient1)
tcpfun(TcpClient2)
tcpfun(TcpClient3)
tcpfun(TcpClient4)
tcpfun(TcpClient5)
tcpfun(TcpClient6)
tcpfun(TcpClient7)
tcpfun(TcpClient8)
tcpfun(TcpClient9)
tcpfun(TcpClient10)
tcpfun(TcpClient11)
tcpfun(TcpClient12)
tcpfun(TcpClient13)
tcpfun(TcpClient14)
tcpfun(TcpClient15)

