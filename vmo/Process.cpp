#include "Process.hpp"

#include <wobjectimpl.h>

W_OBJECT_IMPL(vmo::Model)
namespace vmo
{

Model::Model(
    const TimeVal& duration, const Id<Process::ProcessModel>& id,
    QObject* parent)
    : Process::ProcessModel{duration, id, "vmoProcess", parent}
    , input{Id<Process::Port>(0), this}
    , regen{Id<Process::Port>(1), this}
    , bang{Id<Process::Port>(2), this}
    , sequence_length{Id<Process::Port>(3), this}
    , output{Id<Process::Port>(4), this}
{
  metadata().setInstanceName(*this);
  m_inlets.push_back(&input);
  m_inlets.push_back(&regen);
  m_inlets.push_back(&bang);
  m_inlets.push_back(&sequence_length);
  m_outlets.push_back(&output);
}

Model::~Model()
{
}

QString Model::prettyName() const
{
  return tr("VMO Process");
}

}
template <>
void DataStreamReader::read(const vmo::Model& proc)
{
  m_stream << proc.input << proc.regen << proc.bang << proc.sequence_length << proc.output;
  insertDelimiter();
}

template <>
void DataStreamWriter::write(vmo::Model& proc)
{
  checkDelimiter();
}

template <>
void JSONObjectReader::read(const vmo::Model& proc)
{
}

template <>
void JSONObjectWriter::write(vmo::Model& proc)
{
}
