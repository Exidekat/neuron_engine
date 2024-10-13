#pragma once

#include "neuron/pre.hpp"
#include "neuron/utils.hpp"

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <variant>

#include <glm/glm.hpp>


#define CALLBACKF(N, ...)                                                                                                                                                          \
  public:                                                                                                                                                                          \
    using F_##N = void(__VA_ARGS__);                                                                                                                                               \
    inline void set_on_##N(const std::function<void(__VA_ARGS__)> &f) {                                                                                                                        \
        m_on_##N = f;                                                                                                                                                              \
    };                                                                                                                                                                             \
    inline void unset_on_##N() {                                                                                                                                                   \
        m_on_##N = std::nullopt;                                                                                                                                                   \
    };                                                                                                                                                                             \
                                                                                                                                                                                   \
  protected:                                                                                                                                                                       \
    template <typename... Args>                                                                                                                                                    \
    inline void call_##N##_callback(Args... a) {                                                                                                                                   \
        if (m_on_##N.has_value())                                                                                                                                                  \
            (*m_on_##N)(a...);                                                                                                                                                     \
    };                                                                                                                                                                             \
                                                                                                                                                                                   \
  private:                                                                                                                                                                         \
    std::optional<std::function<F_##N>> m_on_##N = std::nullopt;                                                                                                                   \
                                                                                                                                                                                   \
  public:

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

    constexpr auto WINDOW_PLACEMENT_USE_DEFAULT            = std::monostate{};
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
        [[nodiscard]] virtual size_t  get_window_count() const                            = 0;

        virtual void run_event_loop()  = 0;
        virtual void step_event_loop() = 0;

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

        virtual glm::uvec2 get_inner_size() const = 0;

        CALLBACKF(resize, const glm::uvec2 &new_size);
        CALLBACKF(close_request, bool *close);
        CALLBACKF(close);
    };
} // namespace neuron

#undef CALLBACKF
