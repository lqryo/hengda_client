//
// ffiirree@20190906
//

#ifndef BORDER_BORDERS_LOADER_H
#define BORDER_BORDERS_LOADER_H

#include <iostream>
#include "command.h"
#include "json.hpp"

class BordersLoader {
public:

    static BordersLoader& Instance() {
        static BordersLoader loader;
        return loader;
    }

	nlohmann::json save(std::map<std::string, CommandStack> stacks);
	std::map<std::string, CommandStack> load();

private:
    BordersLoader() = default;

    QString borders_file_ = "borders.json";
	nlohmann::json borders_;
};


#endif //BORDER_BORDERS_LOADER_H
