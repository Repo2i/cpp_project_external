//
//  pupsipidutils.hpp
//  cpp_project
//
//  Created by aibolat on 23.10.2025.
//
#pragma once

#include "pupsi.hpp"

namespace pupsipidutils {
extern const std::function<int(const std::string&)> getProcessId;
extern const std::function <bool(pid_t, task_t&)> attachPid;
}
