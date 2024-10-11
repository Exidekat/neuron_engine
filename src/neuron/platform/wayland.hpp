#pragma once
#ifdef __unix__
#include "neuron/pre.hpp"
#include "linux.hpp"

namespace neuron {
    class NEURON_API WaylandPlatform : public LinuxPlatform {
      public:
        WaylandPlatform();

        std::shared_ptr<Window> create_window(const WindowDescription &description) override;
    };
} // namespace neuron
#endif