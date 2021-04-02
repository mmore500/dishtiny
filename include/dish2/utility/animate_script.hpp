#pragma once


#include <filesystem>
#include <fstream>
#include <string>

#include "../../../third-party/conduit/include/uitsl/fetch/make_temp_filepath.hpp"

namespace dish2::py {

const std::string script_animate_frames{

#include "../../../script/animate_frames.py"

};

    std::filesystem::path setup_script_animate() {
        const auto path = uitsl::make_temp_filepath();
        std::ofstream ofs(path);

        ofs << script_animate_frames.substr(1, script_animate_frames.size() - 2);

        return path;
    }
}


