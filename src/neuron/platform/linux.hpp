#pragma once
#include "generic.hpp"
#ifdef __unix__
namespace neuron {
    class NEURON_API LinuxPlatform : public Platform {
    protected:
        LinuxPlatform();
    public:
        ~LinuxPlatform() override;
    };
}
#endif
