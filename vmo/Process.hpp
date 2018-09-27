#pragma once
#include <Process/GenericProcessFactory.hpp>
#include <Process/Process.hpp>

#include <vmo/Metadata.hpp>

#include <Process/Dataflow/Port.hpp>

namespace vmo
{
class Model final : public Process::ProcessModel
{
  SCORE_SERIALIZE_FRIENDS
  PROCESS_METADATA_IMPL(vmo::Model)
  W_OBJECT(Model)

public:
  Model(
      const TimeVal& duration, const Id<Process::ProcessModel>& id,
      QObject* parent);

  template <typename Impl>
  Model(Impl& vis, QObject* parent)
    : Process::ProcessModel{vis, parent}
    , input{vis, this}
    , regen{vis, this}
    , bang{vis, this}
    , sequence_length{vis, this}
    , output{vis, this}
  {
    vis.writeTo(*this);
  }

  ~Model() override;

  Process::Inlet input;
  Process::Inlet regen;
  Process::Inlet bang;
  Process::Inlet sequence_length;
  Process::Outlet output;

private:
  QString prettyName() const override;
};

using ProcessFactory = Process::ProcessFactory_T<vmo::Model>;
}
