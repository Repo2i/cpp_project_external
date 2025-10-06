//
//  pupsimemory.hpp
//  cpp_project
//
//  Created by aibolat on 23.10.2025.
//

#pragma once


#include "pupsi.hpp"

mach_vm_address_t getImageBase(task_t task);

template <typename T>
static T readmem(mach_vm_address_t address) {
        T value{};
        
        vm_size_t size = sizeof(T);
        
        kern_return_t kr = vm_read_overwrite(globalTask,
                                             address,
                                             sizeof(T),
                                             reinterpret_cast<vm_address_t>(&value),
                                             &size);
        
        if (kr != KERN_SUCCESS || size != sizeof(T)) {
                std::cout << "mem read fail" << std::endl;
                return 0;
        }
        
        return value;
}

template <typename T>
static bool writemem(mach_vm_address_t address, const T& value) {
        
        mach_msg_type_number_t datasize = sizeof(T);
        
        kern_return_t kr = vm_write(globalTask,
                                    address,
                                    reinterpret_cast<vm_offset_t>(&value),
                                    datasize);
        
        if (kr != KERN_SUCCESS) {
                std::cout << "write mem fail" << std::endl;
                
                return false;
        }
        
        return true;
}
