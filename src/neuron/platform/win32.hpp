#pragma once
#include "neuron/pre.hpp"
#include "generic.hpp"
#ifdef WIN32

namespace neuron {
    class NEURON_API Win32Platform : public Platform {
      public:
        Win32Platform();
        ~Win32Platform() override;

        std::shared_ptr<Window> create_window(const WindowDescription &description) override;
    };
} // namespace neuron

#endif
