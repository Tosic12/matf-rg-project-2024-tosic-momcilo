#include <app/AsteroidApp.hpp>
#include <app/MainController.hpp>
#include <app/GUIController.hpp>

namespace engine::asteroid::app {
void AsteroidApp::app_setup() {
    auto main_controller = register_controller<MainController>();
    auto gui_controller = register_controller<GUIController>();
    main_controller->after(core::Controller::get<core::EngineControllersEnd>());
    gui_controller->after(main_controller);
}
}