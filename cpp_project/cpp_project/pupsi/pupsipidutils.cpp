//
//  pupsipidutils.cpp
//  cpp_project
//
//  Created by aibolat on 23.10.2025.
//

#include "pupsipidutils.hpp"

const std::function<pid_t(const std::string&)> pupsipidutils::getProcessId = [](const std::string& target) noexcept -> pid_t {
        int mib[4] =
        {
                CTL_KERN,
                KERN_PROC,
                KERN_PROC_ALL,
                0
        };
        
        size_t bufferSize;
        
        if (sysctl(mib, 4, nullptr, &bufferSize, nullptr, 0) == -1)
        {
                return -1;
        }
        
        std::vector<char> buffer(bufferSize);
        
        if (sysctl(mib, 4, buffer.data(), &bufferSize, nullptr, 0) == -1)
        {
                return -1;
        }
        
        size_t procCount = bufferSize / sizeof(kinfo_proc);
        kinfo_proc* procList = reinterpret_cast<kinfo_proc*>(buffer.data());
        
        for (size_t i = 0; i < procCount; i++)
        {
                pid_t pid = procList[i].kp_proc.p_pid;
                char pathBuf[PROC_PIDPATHINFO_MAXSIZE];
                memset(pathBuf, 0, sizeof(pathBuf));
                
                if (proc_pidpath(pid, pathBuf, sizeof(pathBuf)) > 0)
                {
                        std::string fullPath(pathBuf);
                        std::string baseName = fullPath.substr(fullPath.find_last_of("/") + 1);
                        
                        if (baseName == target)
                        {
                                return pid;
                        }
                }
        }
        
        return 0;
};
const std::function<bool(pid_t, task_t&)> pupsipidutils::attachPid = [](pid_t pid,
                                                                        task_t &outTask) noexcept -> bool {
        outTask = MACH_PORT_NULL;
        
        if (pid <= 0) {
                std::cout << "invalid pid: " << pid << std::endl;
                return false;
        }
        
        kern_return_t KR = task_for_pid(mach_task_self(),
                                        pid,
                                        &outTask);
        
        if (KR != KERN_SUCCESS || outTask == MACH_PORT_NULL) {
                std::cerr << mach_error_string(KR) << std::endl;
                return false;
        }
        
        return true;
};
