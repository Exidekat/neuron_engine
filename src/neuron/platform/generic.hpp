#pragma once

#include "neuron/pre.hpp"
#include "neuron/utils.hpp"

#include <memory>
#include <optional>
#include <string>
#include <variant>

#include <glm/glm.hpp>

namespace neuron {
    class Window;

    struct WindowFixedPosition {
        glm::ivec2 position;
    };

    struct WindowCenterMonitor {
        size_t monitor_index;
    };

    struct WindowCenterHoveredMonitor {};

    using WindowPlacement = std::variant<std::monostate, WindowFixedPosition, WindowCenterMonitor, WindowCenterHoveredMonitor>;

    constexpr auto WINDOW_PLACEMENT_USE_DEFAULT = std::monostate{};
    constexpr auto WINDOW_PLACEMENT_CENTER_HOVERED_MONITOR = WindowCenterHoveredMonitor{};

    enum class WindowPlacementMethod : size_t {
        USE_DEFAULT            = variant_index<WindowPlacement, std::monostate>,
        FIXED_POSITION         = variant_index<WindowPlacement, WindowFixedPosition>,
        CENTER_MONITOR         = variant_index<WindowPlacement, WindowCenterMonitor>,
        CENTER_HOVERED_MONITOR = variant_index<WindowPlacement, WindowCenterHoveredMonitor>,
    };

    struct WindowDescription {
        std::string     title;
        glm::uvec2      size;
        WindowPlacement placement;

        bool resizable;
    };

    class NEURON_API Platform {
      public:
        virtual ~Platform() = default;

        virtual std::weak_ptr<Window> create_window(const WindowDescription &description) = 0;

        virtual void run_event_loop() = 0;

        static const std::unique_ptr<Platform> &get();

        static void init();
        static void cleanup();
    };

    class NEURON_API Window : public std::enable_shared_from_this<Window> {
      protected:
        Window(/* TODO: does anything need to be here */);

      public:
        virtual ~Window();

        /**
         * It is not suggested to store the window as a shared pointer. It is allowed, and internally everything will work perfectly fine.
         * However,
         *
         * @param description The information used to create a window (title, size, position, etc...)
         * @return A std::weak_ptr to the Window object.
         */
        static std::weak_ptr<Window> create(const WindowDescription &description);

      private:
    };
} // namespace neuron
