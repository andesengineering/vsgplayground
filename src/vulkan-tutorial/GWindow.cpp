#include "GWindow.h"

#ifdef USE_GLFW
#include "GWindowGLFW.h"
#endif

#ifdef USE_XCB
#include "GWindowXcb.h"
#endif

#ifdef USE_XLIB
#include "GWindowXlib.h"
#endif

#ifdef USE_XMIX
#include "GWindowXmix.h"
#endif

GWindowPtr GWindow::create( uint32_t width, uint32_t height, const std::function<void(uint32_t, uint32_t)> &callback )
{
#ifdef USE_GLFW
    struct _: public GWindowGLFW
    {
        public: 
            _(uint32_t width, uint32_t height, const std::function<void(uint32_t, uint32_t)> &callback):
                GWindowGLFW(width,height,callback) {} 
            ~ _(){} 
    };
#endif

#ifdef USE_GFX
    struct _: public GWindowGFX
    {
        public: 
            _(uint32_t width, uint32_t height, const std::function<void(uint32_t, uint32_t)> &callback):
                GWindowGFX(width,height,callback) {} 
            ~ _(){} 
    };
#endif

#ifdef USE_XLIB
    struct _: public GWindowXlib
    {
        public: 
            _(uint32_t width, uint32_t height, const std::function<void(uint32_t, uint32_t)> &callback):
                GWindowXlib(width,height,callback) {} 
            ~ _(){} 
    };
#endif

#ifdef USE_XCB

    struct _: public GWindowXcb
    {
        public: 
            _(uint32_t width, uint32_t height, const std::function<void(uint32_t, uint32_t)> &callback):
                GWindowXcb(width,height,callback) {} 
            ~ _(){} 
    };

#endif

#ifdef USE_XMIX
    struct _: public GWindowXmix
    {
        public: 
            _(uint32_t width, uint32_t height, const std::function<void(uint32_t, uint32_t)> &callback):
                GWindowXmix(width,height,callback) {} 
            ~ _(){} 
    };

#endif

    return std::make_shared<_>( width, height, callback ); 
}
