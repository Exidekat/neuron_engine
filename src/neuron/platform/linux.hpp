#pragma once
#include "generic.hpp"
#ifdef __unix__
namespace neuron {
    class LinuxPlatform : public Platform {
    protected:
        LinuxPlatform();
    public:
        virtual ~LinuxPlatform();
    };
}
#endif
