#pragma once
#include <Process/GenericProcessFactory.hpp>

#include <vmo/Presenter.hpp>
#include <vmo/Process.hpp>
#include <vmo/View.hpp>

namespace vmo
{
using LayerFactory = Process::LayerFactory_T<
    vmo::Model, vmo::Presenter, vmo::View>;
}
