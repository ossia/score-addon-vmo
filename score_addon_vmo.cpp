#include "score_addon_vmo.hpp"

#include <score/plugins/customfactory/FactorySetup.hpp>

#include <vmo/CommandFactory.hpp>
#include <vmo/Executor.hpp>
#include <vmo/Inspector.hpp>
#include <vmo/Layer.hpp>
#include <vmo/LocalTree.hpp>
#include <vmo/Process.hpp>
#include <score_addon_vmo_commands_files.hpp>
#include <dlfcn.h>

static void* vmo_dll{};
score_addon_vmo::score_addon_vmo()
{
  vmo_dll = dlopen("libpython2.7.so", RTLD_LAZY |  RTLD_GLOBAL);
}

score_addon_vmo::~score_addon_vmo()
{
  if(vmo_dll)
    dlclose(vmo_dll);
}

std::vector<std::unique_ptr<score::InterfaceBase>>
score_addon_vmo::factories(
    const score::ApplicationContext& ctx, const score::InterfaceKey& key) const
{
  return instantiate_factories<
      score::ApplicationContext,
      FW<Process::ProcessModelFactory, vmo::ProcessFactory>,
      FW<Process::LayerFactory, vmo::LayerFactory>,
      FW<Process::InspectorWidgetDelegateFactory, vmo::InspectorFactory>,
      FW<Execution::ProcessComponentFactory,
         vmo::ProcessExecutorComponentFactory>,
      FW<LocalTree::ProcessComponentFactory,
         vmo::LocalTreeProcessComponentFactory>>(ctx, key);
}

std::pair<const CommandGroupKey, CommandGeneratorMap>
score_addon_vmo::make_commands()
{
  using namespace vmo;
  std::pair<const CommandGroupKey, CommandGeneratorMap> cmds{
      CommandFactoryName(), CommandGeneratorMap{}};

  ossia::for_each_type<
#include <score_addon_vmo_commands.hpp>
      >(score::commands::FactoryInserter{cmds.second});

  return cmds;
}

#include <score/plugins/PluginInstances.hpp>
SCORE_EXPORT_PLUGIN(score_addon_vmo)
