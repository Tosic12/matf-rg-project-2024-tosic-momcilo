#include <engine/core/Engine.hpp>
#include <app/AsteroidApp.hpp>

int main(int argc, char** argv) {
    return std::make_unique<engine::asteroid::app::AsteroidApp>()->run(argc, argv);
}
