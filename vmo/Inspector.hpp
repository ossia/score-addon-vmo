#pragma once
#include <Process/Inspector/ProcessInspectorWidgetDelegate.hpp>
#include <Process/Inspector/ProcessInspectorWidgetDelegateFactory.hpp>

#include <score/command/Dispatchers/CommandDispatcher.hpp>

#include <vmo/Process.hpp>

namespace vmo
{
class InspectorWidget final
    : public Process::InspectorWidgetDelegate_T<vmo::Model>
{
public:
  explicit InspectorWidget(
      const vmo::Model& object, const score::DocumentContext& context,
      QWidget* parent);
  ~InspectorWidget() override;

private:
  CommandDispatcher<> m_dispatcher;
};

class InspectorFactory final
    : public Process::InspectorWidgetDelegateFactory_T<Model, InspectorWidget>
{
  SCORE_CONCRETE("7be47ef2-5135-4cd1-b2ab-237017f79265")
};
}
