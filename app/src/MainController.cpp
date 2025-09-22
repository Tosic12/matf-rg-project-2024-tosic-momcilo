#include <memory>
#include <spdlog/spdlog.h>
#include <imgui.h>
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
    camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, m_camera_neg_z);
    camera->move_camera(engine::graphics::Camera::Movement::DOWN, m_camera_neg_y);
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
    update_spaceship();
}

void MainController::begin_draw() {
    engine::graphics::OpenGL::clear_buffers();
}

void MainController::draw() {
    draw_asteroid();
    draw_diamond();
    draw_spaceship();
    draw_fuel_ball();
    draw_skybox();
    draw_gui_settings();
}

void MainController::end_draw() {
    engine::core::Controller::get<engine::platform::PlatformController>()->swap_buffers();
}

engine::resources::Shader* MainController::init_shader_with_lights(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& model_scale, const std::string& name) {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader(name);
    shader->use();
    shader->set_mat4("projection", graphics->projection_matrix());
    shader->set_mat4("view", graphics->camera()
                                     ->view_matrix());
    shader->set_vec3("viewPos", graphics->camera()->Position);
    shader->set_vec3("color", glm::vec3{0.0f});

	shader->set_vec3("dirLight.direction", m_dir_light_direction);
    shader->set_vec3("dirLight.ambient", m_dir_light_ambient);
    shader->set_vec3("dirLight.diffuse", m_dir_light_diffuse);
    shader->set_vec3("dirLight.specular", m_dir_light_specular);

    shader->set_vec3("pointLight.position",  m_spaceship_pos);
    shader->set_vec3("pointLight.ambient",   m_point_light_ambient);
    shader->set_vec3("pointLight.diffuse",   m_point_light_diffuse);
    shader->set_vec3("pointLight.specular",  m_point_light_specular);

    shader->set_float("pointLight.constant",  m_point_light_constant);
    shader->set_float("pointLight.linear",    m_point_light_linear);
    shader->set_float("pointLight.quadratic", m_point_light_quadratic);


    glm::mat4 model = glm::mat4(1.0);
    model = rotate(model, rot[0], AXES[0]);
    model = rotate(model, rot[1], AXES[1]);
    model = rotate(model, rot[2], AXES[2]);
    model = translate(model, pos);
    model = scale(model, model_scale);
    shader->set_mat4("model", model);

    return shader;
}

void MainController::draw_asteroid() {
    auto shader = init_shader_with_lights(glm::vec3{0.0f}, glm::vec3{0.0f}, glm::vec3{m_asteroid_scale});
    auto asteroid = engine::core::Controller::get<engine::resources::ResourcesController>()->model("asteroid");
	auto texture = engine::core::Controller::get<engine::resources::ResourcesController>()->texture("diffuse", "./resources/models/asteroid/diffuse.png");
    texture->bind_index(0);
    shader->set_sampler("material.diffuse", 0);
    asteroid->draw(shader);
}

void MainController::draw_diamond() {
    auto shader = init_shader_with_lights(m_diamond_pos, m_diamond_rot, glm::vec3{m_diamond_scale});
    auto diamond = engine::core::Controller::get<engine::resources::ResourcesController>()->model("diamond");
    auto texture = engine::core::Controller::get<engine::resources::ResourcesController>()->texture("diffuse_diamond", "./resources/textures/green_gem.jpg");
    texture->bind_index(1);
    shader->set_sampler("material.diffuse", 1);
    diamond->draw(shader);
}

void MainController::draw_spaceship() {
	auto shader = init_shader_with_lights(m_spaceship_pos, glm::vec3{0}, glm::vec3{m_spaceship_scale});
    auto spaceship = engine::core::Controller::get<engine::resources::ResourcesController>()->model("spaceship");
    auto texture = engine::core::Controller::get<engine::resources::ResourcesController>()->texture("diffuse_spaceship", "./resources/models/spaceship/IntergalacticSpaceship_color_4.jpg");
    texture->bind_index(2);
    shader->set_sampler("material.diffuse", 2);
    spaceship->draw(shader);
}

void MainController::draw_fuel_ball() {
	auto shader = init_shader_with_lights(m_spaceship_pos + glm::vec3{0, 0, m_fuelball_delta_z}, glm::vec3{0}, glm::vec3{m_fuelball_scale});
    auto fuelball = engine::core::Controller::get<engine::resources::ResourcesController>()->model("fuelball");
    shader->set_vec3("color", m_point_light_diffuse);
    fuelball->draw(shader);
}

void MainController::draw_skybox() {
    auto shader = engine::core::Controller::get<engine::resources::ResourcesController>()->shader("skybox");
    auto skybox_cube = engine::core::Controller::get<engine::resources::ResourcesController>()->skybox("skybox");
    engine::core::Controller::get<engine::graphics::GraphicsController>()->draw_skybox(shader, skybox_cube);
}

void MainController::draw_gui_settings() {
    auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
    graphics->begin_gui();

    {
    ImGui::Begin("Lighting & Camera Settings");

    ImGui::Text("Directional Light");
    ImGui::InputFloat("Direction X", &m_dir_light_direction.x, m_light_settings_step);
    ImGui::InputFloat("Direction Y", &m_dir_light_direction.y, m_light_settings_step);
    ImGui::InputFloat("Direction Z", &m_dir_light_direction.z, m_light_settings_step);

    ImGui::InputFloat("Ambient R", &m_dir_light_ambient.x, m_light_settings_step);
    ImGui::InputFloat("Ambient G", &m_dir_light_ambient.y, m_light_settings_step);
    ImGui::InputFloat("Ambient B", &m_dir_light_ambient.z, m_light_settings_step);

    ImGui::InputFloat("Diffuse R", &m_dir_light_diffuse.x, m_light_settings_step);
    ImGui::InputFloat("Diffuse G", &m_dir_light_diffuse.y, m_light_settings_step);
    ImGui::InputFloat("Diffuse B", &m_dir_light_diffuse.z, m_light_settings_step);

    ImGui::InputFloat("Specular R", &m_dir_light_specular.x, m_light_settings_step);
    ImGui::InputFloat("Specular G", &m_dir_light_specular.y, m_light_settings_step);
    ImGui::InputFloat("Specular B", &m_dir_light_specular.z, m_light_settings_step);

    ImGui::Separator();

    ImGui::Text("Point Light");

    ImGui::InputFloat("Ambient R", &m_point_light_ambient.x, m_light_settings_step);
    ImGui::InputFloat("Ambient G", &m_point_light_ambient.y, m_light_settings_step);
    ImGui::InputFloat("Ambient B", &m_point_light_ambient.z, m_light_settings_step);

    ImGui::InputFloat("Diffuse R", &m_point_light_diffuse.x, m_light_settings_step);
    ImGui::InputFloat("Diffuse G", &m_point_light_diffuse.y, m_light_settings_step);
    ImGui::InputFloat("Diffuse B", &m_point_light_diffuse.z, m_light_settings_step);

    ImGui::InputFloat("Specular R", &m_point_light_specular.x, m_light_settings_step);
    ImGui::InputFloat("Specular G", &m_point_light_specular.y, m_light_settings_step);
    ImGui::InputFloat("Specular B", &m_point_light_specular.z, m_light_settings_step);

    ImGui::InputFloat("Constant",  &m_point_light_constant, m_light_settings_step);
    ImGui::InputFloat("Kl Component",    &m_point_light_linear, m_light_settings_step);
    ImGui::InputFloat("Kq Component", &m_point_light_quadratic, m_light_settings_step);
    ImGui::Separator();

    ImGui::InputFloat("Camera Speed",    &m_camera_speed, m_speed_settings_step);
    ImGui::InputFloat("Spaceship Speed", &m_spaceship_speed, m_speed_settings_step);

    ImGui::End();
    }

    graphics->end_gui();
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

void MainController::update_spaceship(){
    auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
    glm::vec3 delta_pos{0};
	float dt_space = platform->dt() * m_spaceship_speed;
    if (platform->key(engine::platform::KEY_LEFT)
                .state() == engine::platform::Key::State::Pressed) {
        delta_pos.x -= dt_space;
    }
    if (platform->key(engine::platform::KEY_RIGHT)
                .state() == engine::platform::Key::State::Pressed) {
        delta_pos.x += dt_space;
    }
    if (platform->key(engine::platform::KEY_Q)
                .state() == engine::platform::Key::State::Pressed) {
        delta_pos.y -= dt_space;
    }
    if (platform->key(engine::platform::KEY_E)
                .state() == engine::platform::Key::State::Pressed) {
        delta_pos.y += dt_space;
    }
    if (platform->key(engine::platform::KEY_UP)
                .state() == engine::platform::Key::State::Pressed) {
        delta_pos.z -= dt_space;
    }
    if (platform->key(engine::platform::KEY_DOWN)
                .state() == engine::platform::Key::State::Pressed) {
        delta_pos.z += dt_space;
    }
    if (glm::length(m_spaceship_pos + delta_pos) >= 0.245f) { // distance from the asteroid
    	m_spaceship_pos += delta_pos;
    }
    m_fuelball_scale += m_fuel_sign*dt_space/10000;
    if (m_fuelball_scale.x > m_fuelball_init_scale*1.1f){
    	m_fuel_sign = -1;
    }
    if (m_fuelball_scale.x < 0.9f*m_fuelball_init_scale) {
    	m_fuel_sign = 1;
    }
}
}



