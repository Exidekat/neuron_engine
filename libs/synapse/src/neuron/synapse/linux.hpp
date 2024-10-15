#pragma once
#include "generic.hpp"
#ifdef __linux__
namespace neuron::synapse {
    class SYNAPSE_API LinuxPlatform : public Platform {
    protected:
        LinuxPlatform();
    public:
        ~LinuxPlatform() override;
    };
}
#endif
