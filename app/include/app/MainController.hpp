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

        void draw_model(engine::resources::Shader *shader,
                        const std::string &name,
                        const std::string &texture,
                        const std::string &path,
                        const glm::vec3 &pos,
                        const glm::vec3 &rot,
                        const glm::vec3 &scale,
                        int32_t texture_index);

        void draw_mined_diamond(engine::resources::Shader *shader);

        void set_shader_model(engine::resources::Shader *shader, const glm::vec3 &pos, const glm::vec3 &rot,
                              const glm::vec3 &model_scale);

        void draw_fuel_ball(engine::resources::Shader *shader);

        void update_spaceship();

        void update_camera();

        void draw_gui_settings();

        engine::resources::Shader *init_shader_with_lights();

        float m_asteroid_scale{0.2f};

        glm::vec3 m_diamond_rot{0.1f, 0.2f, -0.3f};
        glm::vec3 m_diamond_pos{-0.12f, -0.015f, 0.115f};
        glm::vec3 m_diamond_scale{0.03f};

        glm::vec3 m_spaceship_pos{0.0f, -0.25f, 0.0f};
        glm::vec3 m_spaceship_rot{0.0f, 0.0f, 0.0f};
        glm::vec3 m_spaceship_scale{0.01f};

        glm::vec3 m_fuelball_scale{0.0005f};
        float m_fuelball_init_scale = 0.0005f;
        float m_fuelball_delta_z    = 0.01f;

        glm::vec3 m_dir_light_direction{0, 1, -1};
        glm::vec3 m_dir_light_ambient{0.3f, 0.3f, 0.3f};
        glm::vec3 m_dir_light_diffuse{1.0f, 1.0f, 1.0f};
        glm::vec3 m_dir_light_specular{1.0f, 1.0f, 1.0f};

        glm::vec3 m_point_light_ambient  = glm::vec3(0.0f);
        glm::vec3 m_point_light_diffuse  = glm::vec3{2.0f, 2.0f, 0.0f};
        glm::vec3 m_point_light_specular = glm::vec3{2.0f, 2.0f, 0.0f};

        glm::vec3 m_mined_pos   = glm::vec3{-0.25f, 0.27f, 0.1f};
        glm::vec3 m_mined_scale = glm::vec3{0.01f};
        bool m_mined            = false;
        bool m_app              = false;

        int m_fuel_sign = 1;
        int m_mined_cnt = 0;

        float m_point_light_constant  = 1.0f;
        float m_point_light_linear    = 1.5f;
        float m_point_light_quadratic = 2.0f;

        float m_camera_neg_z{0.25f};
        float m_camera_neg_y{0.02f};
        float m_camera_speed{0.1f};
        float m_mined_speed{0.2f};
        float m_spaceship_speed{0.5f};
        bool m_draw_gui{false};
        bool m_cursor_enabled{true};

        float m_mine_to_app{2.0f};
        float m_app_to_steal{4.0f};

        float m_light_settings_step{0.05f};
        float m_speed_settings_step{0.1f};

        float m_mat_specular_comp{0.1f};
        float m_mat_shininess_comp{0.2f};

        float m_current_time{0.0f};

        static constexpr glm::vec3 AXES[3] = {glm::vec3{1, 0, 0}, glm::vec3{0, 1, 0}, glm::vec3{0, 0, 1}};
    };
}
#endif //MAINCONTROLLER_HPP
