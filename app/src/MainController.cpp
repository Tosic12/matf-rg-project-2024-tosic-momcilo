#include <memory>
#include <spdlog/spdlog.h>
#include <app/MainController.hpp>
#include <app/GUIController.hpp>

namespace engine::asteroid::app {
void MainPlatformEventObserver::on_key(engine::platform::Key key) {
    spdlog::info("Keyboard event: key={}, state={}", key.name(), key.state_str());
}

void MainPlatformEventObserver::on_mouse_move(engine::platform::MousePosition position) {
    spdlog::info("MousePosition: {} {}", position.x, position.y);
}

void MainController::initialize() {
    // User initialization
    engine::graphics::OpenGL::enable_depth_testing();

    auto observer = std::make_unique<MainPlatformEventObserver>();
    engine::core::Controller::get<engine::platform::PlatformController>()->register_platform_event_observer(
            std::move(observer));
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, m_camera_negz);
}

bool MainController::loop() {
    const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KeyId::KEY_ESCAPE)
                .state() == engine::platform::Key::State::JustPressed) {
        return false;
    }
    return true;
}

void MainController::poll_events() {
    const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    if (platform->key(engine::platform::KEY_F1)
                .state() == engine::platform::Key::State::JustPressed) {
        m_cursor_enabled = !m_cursor_enabled;
        platform->set_enable_cursor(m_cursor_enabled);
    }
}

void MainController::update() {
    update_camera();
}

void MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MainController::draw() {
    draw_asteroid();
    draw_diamond();
    draw_skybox();
}

void MainController::end_draw() {
    engine::core::Controller::get<engine::platform::PlatformController>()->swap_buffers();
}

engine::resources::Shader* MainController::init_shader_with_lights(const glm::vec3& pos, float rot_x, float rot_y, float rot_z, const glm::vec3& model_scale) {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("basic");
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()
                                     ->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);
	shader->set_vec3("dirLight.direction", m_dir_light_direction);
    shader->set_vec3("dirLight.ambient", m_dir_light_ambient);
    shader->set_vec3("dirLight.diffuse", m_dir_light_diffuse);
    shader->set_vec3("dirLight.specular", m_dir_light_specular);

    glm::mat4 model;
	model = scale(glm::mat4(1.0f), model_scale);
    model = rotate(model, rot_x, AXES[0]);
    model = rotate(model, rot_y, AXES[1]);
    model = rotate(model, rot_z, AXES[2]);
    model = translate(model, pos);
    shader->set_mat4("model", model);

    return shader;
}

void MainController::draw_asteroid() {
    auto shader = init_shader_with_lights(glm::vec3{0.0f}, 0.0f, 0.0f, 0.0f, glm::vec3{m_asteroid_scale});
    auto asteroid = engine::core::Controller::get<engine::resources::ResourcesController>()->model("asteroid");
	auto texture = engine::core::Controller::get<engine::resources::ResourcesController>()->texture("diffuse", "./resources/models/asteroid/diffuse.png");
    texture->bind_index(0);
    shader->set_sampler("material.diffuse", 0);
    asteroid->draw(shader);
}

void MainController::draw_diamond() {
    auto shader = init_shader_with_lights(m_diamond_pos, m_diamond_rot[0], m_diamond_rot[1], m_diamond_rot[2], glm::vec3{m_diamond_scale});
    auto diamond = engine::core::Controller::get<engine::resources::ResourcesController>()->model("diamond");
    auto texture = engine::core::Controller::get<engine::resources::ResourcesController>()->texture("diffuse_diamond", "./resources/textures/green_gem.jpg");
    texture->bind_index(1);
    shader->set_sampler("material.diffuse", 1);
    diamond->draw(shader);
}

void MainController::draw_skybox() {
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("skybox");
    auto skybox_cube = engine::core::Controller::get<engine::resources::ResourcesController>()->skybox("skybox");
    engine::core::Controller::get<engine::graphics::GraphicsController>()->draw_skybox(shader, skybox_cube);
}

void MainController::update_camera() {
    auto gui = engine::core::Controller::get<GUIController>();
    if (gui->is_enabled()) {
        return;
    }
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();
    float dt = platform->dt() * m_camera_speed;
    if (platform->key(engine::platform::KEY_W)
                .state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt);
    }
    if (platform->key(engine::platform::KEY_S)
                .state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt);
    }
    if (platform->key(engine::platform::KEY_A)
                .state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt);
    }
    if (platform->key(engine::platform::KEY_D)
                .state() == engine::platform::Key::State::Pressed) {
        camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt);
    }
    auto mouse = platform->mouse();
    if (platform->key(engine::platform::MOUSE_BUTTON_RIGHT)
                .state() == engine::platform::Key::State::Pressed) {
        camera->rotate_camera(mouse.dx, mouse.dy);
    }
    camera->zoom(mouse.scroll);
}
}



