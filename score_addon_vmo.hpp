#pragma once
#include <score/application/ApplicationContext.hpp>
#include <score/command/Command.hpp>
#include <score/command/CommandGeneratorMap.hpp>
#include <score/plugins/InterfaceList.hpp>
#include <score/plugins/qt_interfaces/CommandFactory_QtInterface.hpp>
#include <score/plugins/qt_interfaces/FactoryFamily_QtInterface.hpp>
#include <score/plugins/qt_interfaces/FactoryInterface_QtInterface.hpp>
#include <score/plugins/qt_interfaces/GUIApplicationPlugin_QtInterface.hpp>
#include <score/plugins/qt_interfaces/PluginRequirements_QtInterface.hpp>

#include <QObject>

#include <utility>
#include <vector>

class score_addon_vmo final : public score::Plugin_QtInterface,
                              public score::FactoryInterface_QtInterface,
                              public score::CommandFactory_QtInterface
{
  SCORE_PLUGIN_METADATA(1, "1a496965-5b44-4d80-9ba1-1136adc20f12")

public:
  score_addon_vmo();
  ~score_addon_vmo() override;

private:
  std::vector<std::unique_ptr<score::InterfaceBase>> factories(
      const score::ApplicationContext& ctx,
      const score::InterfaceKey& key) const override;

  std::pair<const CommandGroupKey, CommandGeneratorMap>
  make_commands() override;
};
