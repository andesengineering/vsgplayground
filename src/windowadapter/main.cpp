#include <vsg/all.h>

#include "WindowAdapter.h"

#if defined(USE_VSG)

#elif defined(USE_XCB)

#  include "WindowXcb.h"

#elif defined(USE_XLIB)

#  include "WindowXlib.h"

#endif


int main( int argc, char **argv )
{
    auto windowTraits = vsg::WindowTraits::create();

    if( std::getenv("VSG_FILE_PATH") == nullptr )
    {
        std::cerr << "Please set VSG_FILE_PATH to the vsgExamples data directory" << std::endl;
        return 1;
    }

    std::string teapotFile = std::string(std::getenv("VSG_FILE_PATH")) + "/models/teapot.vsgt";
    auto options = vsg::Options::create();
    vsg::ref_ptr<vsg::Node> vsg_scene = vsg::read_cast<vsg::Node>( teapotFile, options );
    if( !vsg_scene )
    {
        std::cerr << "Unable to load teapot" << std::endl;
        return 1;
    }

    auto viewer = vsg::Viewer::create();

    windowTraits->width = 800;
    windowTraits->height = 600;

#if defined(USE_VSG)

     vsg::ref_ptr<vsg::Window> window(vsg::Window::create(windowTraits));
     std::cout << "Using VSG's internal window" << std::endl;

#elif defined(USE_XCB)

    vsg::ref_ptr<WindowXcb> windowXcb( new WindowXcb( windowTraits->width, windowTraits->height ) );
    vsg::ref_ptr<vsg::Window> window( WindowAdapter::create( windowTraits, nullptr, windowXcb->connection, windowXcb->window ) );
    std::cout << "Using xcb window created external to vsg" << std::endl;

#elif defined(USE_XLIB)

    vsg::ref_ptr<WindowXlib> windowXlib( new WindowXlib( windowTraits->width, windowTraits->height ) );
    vsg::ref_ptr<vsg::Window> window( WindowAdapter::create( windowTraits, nullptr, windowXlib->dpy, windowXlib->window ) );
    std::cout << "Using xlib window created external to vsg" << std::endl;

#endif

    if (!window)
    {
        std::cout<<"Could not create windows."<<std::endl;
        return 1;
    }
    viewer->addWindow(window);

    vsg::ComputeBounds computeBounds;
    vsg_scene->accept(computeBounds);
    vsg::dvec3 center = (computeBounds.bounds.min+computeBounds.bounds.max)*0.5;
    double radius = vsg::length(computeBounds.bounds.max-computeBounds.bounds.min)*0.6;
    double nearFarRatio = 0.0001;

    vsg::ref_ptr<vsg::ProjectionMatrix> perspective = vsg::Perspective::create(30.0, static_cast<double>(window->extent2D().width) / static_cast<double>(window->extent2D().height), nearFarRatio*radius, radius * 4.5);

    double d = radius * 2.5;
    auto lookAt = vsg::LookAt::create( center+vsg::dvec3(0.0, 0.0, d), center, vsg::dvec3(0.0, 1.0, 0.0));
    auto camera = vsg::Camera::create(perspective, lookAt, vsg::ViewportState::create(window->extent2D()));

    viewer->addEventHandler(vsg::CloseHandler::create(viewer));

    auto commandGraph = vsg::createCommandGraphForView(window, camera, vsg_scene);
    viewer->assignRecordAndSubmitTaskAndPresentation({commandGraph}
    );

    viewer->compile();

    double a = 0.0;

    while( viewer->advanceToNextFrame() )
    {
        viewer->handleEvents();

        lookAt->eye.set( cos(a) * d, 0.0, sin(a) * d );
        a -= 3.141592/180.0;

        viewer->update();
        viewer->recordAndSubmit();
        viewer->present();
    }

    return 0;
}
