#include "LocalTree.hpp"

#include <LocalTree/Property.hpp>
#include <vmo/Process.hpp>

namespace vmo
{
LocalTreeProcessComponent::LocalTreeProcessComponent(
    const Id<score::Component>& id,
    ossia::net::node_base& parent,
    vmo::Model& proc,
    const score::DocumentContext& doc,
    QObject* parent_obj)
    : LocalTree::ProcessComponent_T<vmo::Model>{parent,
                                                proc,
                                                doc,
                                                id,
                                                "vmoComponent",
                                                parent_obj}
{
}

LocalTreeProcessComponent::~LocalTreeProcessComponent() {}
}
