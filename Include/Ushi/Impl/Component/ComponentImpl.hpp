#ifndef USHI_INCLUDE_USHI_IMPL_COMPONENT_IMPL_HPP
#define USHI_INCLUDE_USHI_IMPL_COMPONENT_IMPL_HPP

#include "Ushi/Core/Core.hpp"

namespace ushi {

struct Component {
  virtual ~Component() noexcept = default;
};

template <typename T_Component>
concept IsComponent = !std::same_as<T_Component, Component> &&
                      std::derived_from<T_Component, Component>;

} // namespace ushi

#endif // USHI_INCLUDE_USHI_IMPL_COMPONENT_IMPL_HPP