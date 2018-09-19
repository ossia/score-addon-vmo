#pragma once
#include <LocalTree/LocalTreeComponent.hpp>
#include <LocalTree/Scenario/ProcessComponent.hpp>

namespace vmo
{
class Model;

class LocalTreeProcessComponent : public LocalTree::ProcessComponent_T<Model>
{
  COMPONENT_METADATA("e731cb6a-d649-4d75-b48e-286b5616e0f7")

public:
  LocalTreeProcessComponent(
      const Id<score::Component>& id, ossia::net::node_base& parent,
      vmo::Model& scenario, LocalTree::DocumentPlugin& doc,
      QObject* parent_obj);

  ~LocalTreeProcessComponent() override;
};

using LocalTreeProcessComponentFactory
    = LocalTree::ProcessComponentFactory_T<LocalTreeProcessComponent>;
}
