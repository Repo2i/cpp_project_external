//
//  pupsimemory.cpp
//  cpp_project
//
//  Created by aibolat on 23.10.2025.
//

#include "pupsimemory.hpp"

mach_vm_address_t getImageBase(task_t task) {
        mach_vm_address_t address = 0;
        mach_vm_size_t size;
        vm_region_basic_info_data_64_t info;
        mach_msg_type_number_t count = VM_REGION_BASIC_INFO_COUNT_64;
        mach_port_t object_name;
        
        while (true) {
                kern_return_t kr = mach_vm_region(task,
                                                  &address,
                                                  &size,
                                                  VM_REGION_BASIC_INFO_64,
                                                  (vm_region_info_t)&info,
                                                  &count,
                                                  &object_name);
                
                if (kr != KERN_SUCCESS) break;
                
                char path[PROC_PIDPATHINFO_MAXSIZE];
                pid_t pid;
                pid_for_task(task, &pid);
                proc_regionfilename(pid, address, path, sizeof(path));
                
                if ((info.protection & VM_PROT_READ) && (info.protection & VM_PROT_EXECUTE)) {
                        if (strstr(path, "RobloxPlayer") != NULL) return address;
                }
                
                address += size;
        }
        return 0;
}
