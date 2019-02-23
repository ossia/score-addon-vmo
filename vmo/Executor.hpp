#pragma once
#include <Process/Execution/ProcessComponent.hpp>

#include <ossia/dataflow/node_process.hpp>

namespace vmo
{
class Model;
class ProcessExecutorComponent final
    : public Execution::ProcessComponent_T<vmo::Model, ossia::node_process>
{
  COMPONENT_METADATA("320c4a41-e557-4de2-b30c-d6189f7de512")
public:
  ProcessExecutorComponent(
      Model& element,
      const Execution::Context& ctx,
      const Id<score::Component>& id,
      QObject* parent);
};

using ProcessExecutorComponentFactory
    = Execution::ProcessComponentFactory_T<ProcessExecutorComponent>;
}
