#ifdef __APPLE__
#include "cocoas.hpp"

namespace neuron {
    CocoaPlatform::CocoaPlatform() {
        m_app = [NSApplication sharedApplication];
        [m_app setActivationPolicy:NSApplicationActivationPolicyRegular];
    }

    CocoaPlatform::~CocoaPlatform() {
        // Perform any required cleanup
    }

    std::weak_ptr<Window> CocoaPlatform::create_window(const WindowDescription &description) {
        const auto window = std::shared_ptr<CocoaWindow>(new CocoaWindow(description), [](CocoaWindow* p) { delete p; });
        m_open_windows.insert(window->cocoa_handle());
        m_window_map.insert({window->cocoa_handle(), window});
        return window;
    }

    size_t CocoaPlatform::get_window_count() const {
        return m_open_windows.size();
    }

    void CocoaPlatform::run_event_loop() {
        [m_app run];
    }

    void CocoaPlatform::step_event_loop() {
        NSEvent *event;
        while ((event = [m_app nextEventMatchingMask:NSEventMaskAny untilDate:nil inMode:NSDefaultRunLoopMode dequeue:YES])) {
            [m_app sendEvent:event];
        }
    }

    CocoaWindow::CocoaWindow(const WindowDescription &description) {
        NSRect frame = NSMakeRect(0, 0, description.size.x, description.size.y);
        m_window = [[NSWindow alloc] initWithContentRect:frame
                                               styleMask:(NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable)
                                                 backing:NSBackingStoreBuffered
                                                   defer:NO];
        [m_window setTitle:[NSString stringWithUTF8String:description.title.c_str()]];
        [m_window makeKeyAndOrderFront:nil];

        if (!description.resizable) {
            [m_window setStyleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable];
        }

        m_cached_size = description.size;
    }

    CocoaWindow::~CocoaWindow() {
        [m_window close];
    }

    glm::uvec2 CocoaWindow::get_inner_size() const {
        NSRect frame = [m_window contentRectForFrameRect:[m_window frame]];
        return {static_cast<uint32_t>(frame.size.width), static_cast<uint32_t>(frame.size.height)};
    }

    void CocoaWindow::trigger_close() {
        [m_window performClose:nil];
    }
} // namespace neuron
#endif