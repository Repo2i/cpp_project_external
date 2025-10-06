//
//  pupsi.hpp
//  cpp_project
//
//  Created by aibolat on 22.10.2025.
//

#pragma once

#include <vector>
#include <iostream>
#include <functional>
#include <expected>
#include <optional>

#include "mach/mach.h"
#include "mach/mach_vm.h"
#include "libproc.h"
#include "sys/sysctl.h"
#include "stdexcept"
#include "structs.h"

inline task_t globalTask;
inline mach_vm_address_t RBXPlayer;

inline const std::string target = "RobloxPlayer";

namespace pupsi {
extern const std::function<void()> tryinject;
extern const std::function<void()> tryreadvisualengine;
}
