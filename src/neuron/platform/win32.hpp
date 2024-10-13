#pragma once
#include "neuron/pre.hpp"
#include "generic.hpp"
#ifdef WIN32

namespace neuron {
    class NEURON_API Win32Platform final : public Platform {
      public:
        Win32Platform();
        ~Win32Platform() override;

        std::weak_ptr<Window> create_window(const WindowDescription &description) override;
        [[nodiscard]] size_t get_window_count() const override;
        void run_event_loop() override;
        void step_event_loop() override;
    };
} // namespace neuron

#endif
