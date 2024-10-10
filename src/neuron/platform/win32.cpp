#include "win32.hpp"
#ifdef WIN32

#error "Windows is currently unsupported"

#else
// define neuron::detail::create_win32_platform as a runtime error
std::shared_ptr<neuron::Platform> create_win32_platform() {
    throw std::runtime_error("Win32 platform can only be created in Windows.");
}


#endif
