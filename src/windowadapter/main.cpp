#include <vsg/all.h>

#include "WindowAdapter.h"
#include "WindowXcb.h"
#include "WindowXlib.h"


int main( int argc, char **argv )
{
    vsg::CommandLine arguments(&argc, argv);

    auto windowTraits = vsg::WindowTraits::create();
    windowTraits->width = 800;
    windowTraits->height = 600;

    vsg::Paths searchPaths = vsg::getEnvPaths("VSG_FILE_PATH");
    vsg::Path teapotFile = vsg::findFile("models/teapot.vsgt", searchPaths);
    if (teapotFile.empty())
    {
        std::cerr << "Cannot find file," << std::endl;
        return 1;
    }

    auto options = vsg::Options::create();
    vsg::ref_ptr<vsg::Node> vsg_scene = vsg::read_cast<vsg::Node>( teapotFile, options );
    if( !vsg_scene )
    {
        std::cerr << "Unable to load teapot" << std::endl;
        return 1;
    }

    auto viewer = vsg::Viewer::create();

    vsg::ref_ptr<vsg::Window> window;
    vsg::ref_ptr<WindowXcb> windowXcb;
    vsg::ref_ptr<WindowXlib> windowXlib;

    if (arguments.read("--vsg"))
    {
        window = vsg::Window::create(windowTraits);
        std::cout<<"Create Widnow insg VSG "<<window<<std::endl;
    }
    else if (arguments.read("--xcb"))
    {
        windowTraits->instanceExtensionNames.emplace_back("VK_KHR_surface");
        windowTraits->instanceExtensionNames.emplace_back("VK_KHR_xcb_surface");

        windowXcb = new WindowXcb( windowTraits->width, windowTraits->height );
        window = WindowAdapter::create( windowTraits, windowXcb->connection, windowXcb->window );
        std::cout<<"Create Widnow insg XCB "<<window<<std::endl;
    }
    else
    {
        windowTraits->instanceExtensionNames.emplace_back("VK_KHR_surface");
        windowTraits->instanceExtensionNames.emplace_back("VK_KHR_xlib_surface");

        windowXlib = new WindowXlib( windowTraits->width, windowTraits->height );
        window = WindowAdapter::create( windowTraits, windowXlib->dpy, windowXlib->window);
        std::cout<<"Create Widnow insg Xlib "<<window<<std::endl;
    }

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
    viewer->assignRecordAndSubmitTaskAndPresentation({commandGraph});

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
