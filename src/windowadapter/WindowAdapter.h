#ifndef WINDOW_ADAPTER_DEF
#define WINDOW_ADAPTER_DEF

#include <vsg/viewer/Window.h>

class WindowAdapter: public vsg::Window
{
    public:
        WindowAdapter() = delete;
        WindowAdapter(const WindowAdapter &) = delete;
        WindowAdapter &operator = (const WindowAdapter &) = delete;

        template <typename... Args>
            static vsg::ref_ptr<vsg::Window> create( vsg::ref_ptr<vsg::WindowTraits> traits, Args&&... args )
        {
            return vsg::ref_ptr<vsg::Window>( new WindowAdapter( traits, args... ) );
        }

        bool valid() const override;
        bool pollEvents(vsg::Events& events) override;
        bool resized() const override;
        void resize() override;

    protected:
        template <typename... Args>
        WindowAdapter( vsg::ref_ptr<vsg::WindowTraits> traits, Args... );
        ~WindowAdapter() {}

        void _init();

    public:
        class NativeAdapter: public vsg::Object
        {
            public:
                template <typename T, typename... Args>
                    static vsg::ref_ptr<NativeAdapter>create( Args ... args )
                {
                    return vsg::ref_ptr<NativeAdapter>( new T( args... ) );
                }

                virtual bool valid() { return false; }
                virtual void createVulkanSurface( VkInstance instance, VkSurfaceKHR &surface ) {}
                virtual bool pollEvents( vsg::Events & ) {return false;}
                virtual bool resized() { return false; }
                virtual bool getFramebufferSize( uint32_t &width, uint32_t &height ) { width = height = 0; return false; }

            protected:
                NativeAdapter() {}
                virtual ~NativeAdapter() {}
        };

    private:
        bool _valid = false;
        vsg::ref_ptr<NativeAdapter> _nativeAdapter;
};
#endif
