#pragma once
#include "neuron/cfg.hpp"

#ifdef WIN32
# ifdef NEURON_SHARED
#  ifdef NEURON_SHARED_EXPORTS
#   define NEURON_API __declspec(dllexport)
#  else
#   define NEURON_API __declspec(dllimport)
#  endif
# else
#  define NEURON_API
# endif
#else
# define NEURON_API
#endif
