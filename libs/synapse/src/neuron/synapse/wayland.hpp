#pragma once
#ifdef __linux__
#include "linux.hpp"
#include "neuron/synapse/pre.hpp"

namespace neuron::synapse {
    class SYNAPSE_API WaylandPlatform final : public LinuxPlatform {
      public:
        WaylandPlatform();

        std::weak_ptr<Window> create_window(const WindowDescription &description) override;
        [[nodiscard]] size_t  get_window_count() const override;

        void run_event_loop() override;
        void step_event_loop() override;
    };
} // namespace neuron
#endif
