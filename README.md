# Neuron Engine

This project is a cross-platform Vulkan based rendering/game development engine/framework.

## Modules
The project is split into several modules, each of which is responsible for a certain part of the engine as a whole.
See each modules readme for more information. Most of these modules can be used independent of each other (except for Neuron Stem, which is a base library required by all modules).

### Neuron
This module pulls all the other modules together to make systems work together nicely.

### Synapse
This module is the cross-platform windowing abstraction. Synapse currently supports: Windows, Linux (under X11) and MacOS.
Wayland support is planned.

### Occipital
This module is the main renderer abstraction and implementation.

### Neuron Stem
This module is responsible for allowing the different libraries to talk to each other, and as such it provides interfaces which can be implemented and some basic utilities.


## Contributions
Follow the style guide, found in [STYLE.md](STYLE.md)

