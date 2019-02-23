#pragma once
#include <Process/Dataflow/Port.hpp>
#include <Process/GenericProcessFactory.hpp>
#include <Process/Process.hpp>

#include <vmo/Metadata.hpp>

namespace vmo
{
class Model final : public Process::ProcessModel
{
  SCORE_SERIALIZE_FRIENDS
  PROCESS_METADATA_IMPL(vmo::Model)
  W_OBJECT(Model)

public:
  Model(
      const TimeVal& duration,
      const Id<Process::ProcessModel>& id,
      QObject* parent);

  Model(DataStreamWriter& vis, QObject* parent);
  Model(JSONObjectWriter& vis, QObject* parent);

  ~Model() override;

  Process::Inlet input;
  Process::Inlet regen;
  Process::Inlet bang;
  Process::Inlet sequence_length;
  Process::Outlet output;

private:
  QString prettyName() const noexcept override;
};

using ProcessFactory = Process::ProcessFactory_T<vmo::Model>;
}
