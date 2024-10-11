#pragma once
#ifdef __unix__
#include "linux.hpp"
#include "neuron/pre.hpp"

namespace neuron {
    class NEURON_API WaylandPlatform final : public LinuxPlatform {
      public:
        WaylandPlatform();

        std::weak_ptr<Window> create_window(const WindowDescription &description) override;

        void run_event_loop() override;
    };
} // namespace neuron
#endif
