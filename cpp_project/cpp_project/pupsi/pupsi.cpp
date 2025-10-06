//
//  pupsi.cpp
//  cpp_project
//
//  Created by aibolat on 22.10.2025.
//

#include "pupsi.hpp"
#include "pupsipidutils.hpp"
#include "pupsimemory.hpp"

using pupsipidutils::getProcessId;
using pupsipidutils::attachPid;

const std::function<void()> pupsi::tryinject = []() -> void {
        pid_t pid = getProcessId(target);
        attachPid(pid, globalTask);
};

const std::function<void()> pupsi::tryreadvisualengine = []() -> void {
        mach_vm_address_t baseaddress = getImageBase(globalTask);
        uintptr_t playerconfigurer = readmem<uintptr_t>(baseaddress + RBXOffsets::playerconfigurer);
        writemem(playerconfigurer + RBXOffsets::PartyPlayerInactivityTimeoutInSeconds,
                 1);
};
