#ifndef ECS_COMPONENT_HPP
#define ECS_COMPONENT_HPP

#include "Base.hpp"
#include "Entity.hpp"

namespace ecs {

class Component {
public:
protected:
  Component() = default;
};

template <typename ComponentType>
concept CValidComponent = std::copy_constructible<ComponentType> &&
                          std::derived_from<ComponentType, Component> &&
                          !std::is_same_v<ComponentType, Component>;

class ComponentTable final {
public:
  ComponentTable() = default;
  ~ComponentTable() = default;
  ComponentTable(const ComponentTable &) = delete;
  ComponentTable(ComponentTable &&) = default;
  ComponentTable &operator=(const ComponentTable &) = delete;
  ComponentTable &operator=(ComponentTable &&) = default;

  u64 getNComponents() const noexcept { return this->mComponentData.size(); }

  template <typename ComponentType> //
  void reg()
    requires CValidComponent<ComponentType>
  {
    this->mComponentData.try_emplace(
        typenameStr<ComponentType>(),
        std::make_shared<SparseVector<ComponentType>>());
  }

  // template <typename ComponentType> //
  // ComponentTable &dereg()
  //   requires CValidComponent<ComponentType>
  // {
  //   componentTable.erase(typenameStr<ComponentType>());
  // }

  template <typename ComponentType, typename EntityType> //
  void add(const EntityType &entity, ComponentType component = {})
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    this->getArray<ComponentType>()->add(entity.getID(), std::move(component));
  }

  template <typename ComponentType, typename EntityType> //
  void remove(const EntityType &entity)
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    this->getArray<ComponentType>()->remove(entity.getID());
  }

  template <typename EntityType> //
  void remove(const EntityType &entity)
    requires CValidEntity<EntityType>
  {
    for (auto &pair : this->mComponentData) {
      pair.second->remove(entity.getID());
    }
  }

  template <typename ComponentType> //
  std::shared_ptr<SparseVector<ComponentType>> getArray()
    requires CValidComponent<ComponentType>
  {
    try {
      return std::static_pointer_cast<SparseVector<ComponentType>>(
          this->mComponentData.at(typenameStr<ComponentType>()));
    } catch (std::out_of_range &e) {
      throw std::runtime_error(
          "ComponentTable::getArray: unregistered component");
    }
  }

  template <typename ComponentType, typename EntityType> //
  ComponentType &get(const EntityType &entity)
    requires CValidComponent<ComponentType> && CValidEntity<EntityType>
  {
    return this->getArray<ComponentType>()->at(entity.getID());
  }

  void clear() { this->mComponentData.clear(); }

private:
  std::unordered_map<cString, std::shared_ptr<ISparseVector>> mComponentData =
      {};
};

} // namespace ecs

#endif // ECS_COMPONENT_HPP