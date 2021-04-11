#pragma once
#include <Process/Dataflow/Port.hpp>
#include <Process/GenericProcessFactory.hpp>
#include <Process/Process.hpp>
#include <score/serialization/JSONVisitor.hpp>

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

  Model(DataStream::Deserializer& vis, QObject* parent);
  Model(JSONObject::Deserializer& vis, QObject* parent);

  ~Model() override;

  Process::ValueInlet input;
  Process::ValueInlet regen;
  Process::ValueInlet bang;
  Process::ValueInlet sequence_length;
  Process::ValueOutlet output;

private:
  QString prettyName() const noexcept override;
};

using ProcessFactory = Process::ProcessFactory_T<vmo::Model>;
}
