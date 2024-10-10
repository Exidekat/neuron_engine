#pragma once

#include <memory>
#include <optional>

#include <glm/glm.hpp>


namespace neuron {
    class Window;

    struct WindowDescription {
        std::string title;
        glm::uvec2 size;
        std::optional<glm::ivec2> position;

        bool resizable;
    };

    class Platform {
    public:
        virtual std::shared_ptr<Window> create_window(const WindowDescription& description) = 0;

        static std::shared_ptr<Platform> get();
        static void init();
        static void cleanup();
    };

    class Window {
    protected:
        Window(/* TODO: does anything need to be here */);

    public:
        virtual ~Window();

        static std::shared_ptr<Window> create_window(const WindowDescription& description);


    private:

    };
}