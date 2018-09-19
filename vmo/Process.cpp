#include "Process.hpp"

#include <wobjectimpl.h>

W_OBJECT_IMPL(vmo::Model)
namespace vmo
{

Model::Model(
    const TimeVal& duration, const Id<Process::ProcessModel>& id,
    QObject* parent)
    : Process::ProcessModel{duration, id, "vmoProcess", parent}
{
  metadata().setInstanceName(*this);
}

Model::~Model()
{
}

QString Model::prettyName() const
{
  return tr("vmo Process");
}

void Model::startExecution()
{
}

void Model::stopExecution()
{
}

void Model::reset()
{
}

void Model::setDurationAndScale(const TimeVal& newDuration)
{
}

void Model::setDurationAndGrow(const TimeVal& newDuration)
{
}

void Model::setDurationAndShrink(const TimeVal& newDuration)
{
}
}
template <>
void DataStreamReader::read(const vmo::Model& proc)
{
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
