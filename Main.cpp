#include "ECS.hpp"

#include <glm/glm.hpp>

struct Motion : public ecs::Component {
  glm::vec3 position;
  glm::vec3 velocity;
  glm::vec3 acceleration;
};

struct Mesh : public ecs::Component {
  glm::vec4 color;
};

DECLARE_SIMPLE_ENTITY(NPC);
DECLARE_SIMPLE_ENTITY(Player);

struct Renderable : public ecs::System {
  void function(ecs::World &world) {
    for (auto entityID : entityIDs) {
      auto &motion = world.get<Motion>(entityID);
      auto &mesh = world.get<Mesh>(entityID);
      motion.position = {3, 0, 5};
      mesh.color = {1, 1, 1, 1};
    }
  }
};

int protected_main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  using namespace ecs;
  World scence;

  scence.mComponentTable->reg<Motion>();
  scence.mComponentTable->reg<Mesh>();

  scence.mEntityManager->setNComponents(
      scence.mComponentTable->getNComponents());

  auto npc1 = scence.mEntityManager->createEntity<NPC>(scence);
  scence.mComponentTable->add<Motion>(npc1);
  scence.mEntityManager->setSignature(npc1, 1, true);
  scence.mEntityManager->destroyEntity(npc1);

  Renderable sRender1;
  // sRender1.function(5);

  return EXIT_SUCCESS;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  try {
    return protected_main(argc, argv);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Uncaught exception" << std::endl;
  }
  return EXIT_FAILURE;
}