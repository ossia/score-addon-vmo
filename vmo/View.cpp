#include "View.hpp"

#include <Process/Style/ScenarioStyle.hpp>

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
namespace vmo
{

View::View(QGraphicsItem* parent) : LayerView{parent}
{
}

View::~View()
{
}

void View::paint_impl(QPainter* painter) const
{
}
}
