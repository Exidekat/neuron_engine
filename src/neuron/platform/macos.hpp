#pragma once
#include "generic.hpp"
#ifdef __APPLE__
namespace neuron {
    class NEURON_API MacOSPlatform : public Platform {
      protected:
        MacOSPlatform();
      public:
        ~MacOSPlatform() override;
    };
}
#endif