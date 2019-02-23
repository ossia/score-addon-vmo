#include "Inspector.hpp"

#include <score/document/DocumentContext.hpp>

#include <QFormLayout>
#include <QLabel>
#include <QPushButton>

namespace vmo
{
InspectorWidget::InspectorWidget(
    const vmo::Model& object,
    const score::DocumentContext& context,
    QWidget* parent)
    : InspectorWidgetDelegate_T{object, parent}
    , m_dispatcher{context.commandStack}
{
}

InspectorWidget::~InspectorWidget() {}
}
