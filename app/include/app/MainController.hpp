#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

namespace engine::asteroid::app {
class MainPlatformEventObserver final : public engine::platform::PlatformEventObserver {
public:
    void on_key(engine::platform::Key key) override;

    void on_mouse_move(engine::platform::MousePosition position) override;
};

class MainController final : public engine::core::Controller {
public:
    std::string_view name() const override {
        return "asteroid::app::MainController";
    }

private:
    void initialize() override;

    bool loop() override;

    void poll_events() override;

    void update() override;

    void begin_draw() override;

    void draw() override;

    void end_draw() override;

    void draw_skybox();

    void draw_asteroid();

    void draw_diamond();

    void update_camera();

    engine::resources::Shader* init_shader_with_lights();

    float m_asteroid_scale{0.2f};

    glm::vec3 m_diamond_rot{0.1f, 0.2f, -0.3f};
    glm::vec3 m_diamond_pos{-0.5f, -0.1f, 0.6f};
    glm::vec3 m_diamond_scale{0.15f};

    glm::vec3 m_dir_light_direction{0, 1, -1};
    glm::vec3 m_dir_light_ambient{0.3f, 0.3f, 0.3f};
    glm::vec3 m_dir_light_diffuse{1.0f, 1.0f, 1.0f};
    glm::vec3 m_dir_light_specular{1.0f, 1.0f, 1.0f};

    float m_camera_negz{0.25f};
    float m_camera_speed{0.1f};
    bool m_draw_gui{false};
    bool m_cursor_enabled{true};

    static constexpr glm::vec3 AXES[3] = {glm::vec3{1, 0, 0}, glm::vec3{0, 1, 0}, glm::vec3{0, 0, 1}};
};
}
#endif //MAINCONTROLLER_HPP
