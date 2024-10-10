#pragma once
#include "linux.hpp"

namespace neuron {
    class WaylandPlatform : public LinuxPlatform {
      public:
        WaylandPlatform();

        std::shared_ptr<Window> create_window(const WindowDescription &description) override;
    };
} // namespace neuron
