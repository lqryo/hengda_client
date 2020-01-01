//
// Created by ffiirree on 20190906
//

#include "bordersloader.h"

nlohmann::json BordersLoader::save(std::map<std::string, CommandStack> stacks)
{
	borders_.clear();

	std::cout << stacks.size() << std::endl;
	for (const auto& stack : stacks) {
		auto key = stack.first;
		nlohmann::json stack_json = nlohmann::json::array();
		for (const auto& cmd : stack.second.commands()) {

			nlohmann::json cmd_json = {
					{ "type" , cmd->graph()},
					{ "points", nlohmann::json::array() }
			};

			for (const auto& point : cmd->points()) {
				cmd_json["points"].push_back({ point.x(), point.y() });
			}
			stack_json.push_back(cmd_json);
		}
		borders_.push_back({
			{ "ip", key },
			{ "info", stack_json }
			});
	}

	auto borders_str = borders_.dump();

	QFile file(borders_file_);

	if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
		return borders_;

	QTextStream out(&file);
	out << borders_str.c_str();
	file.close();

	return borders_;
}

std::map<std::string, CommandStack> BordersLoader::load()
{
	std::map<std::string, CommandStack> stacks;

	QString text;
	QFile config_file(borders_file_);
	if (config_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream in(&config_file);
		text = in.readAll();
	}

	try {
		borders_ = nlohmann::json::parse(text.toStdString());
	}
	catch (nlohmann::json::parse_error& e) {
		borders_ = nlohmann::json::array();

		Q_UNUSED(e);
		qDebug() << "Parse config.json failed!";
	}

	//
	for (const auto& stack_json : borders_) {

		auto key = stack_json["ip"].get<std::string>();
		stacks[key] = CommandStack();

		auto stack_value_json = stack_json["info"];

		for (const auto& border : stack_value_json) {
			auto cmd = make_shared<PaintCommand>((Graph)(border["type"].get<unsigned int>()), QPen(Qt::green, 2));
			auto ps = border["points"].get<std::vector<std::vector<int>>>();
			for (const auto& p : ps) {
				cmd->points().push_back({ p[0], p[1] });
			}
			stacks[key].push(cmd);
		}
	}

	return stacks;
}
