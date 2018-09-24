#include "Executor.hpp"

#include <Process/ExecutionContext.hpp>

#include <ossia/dataflow/port.hpp>

#include <vmo/Process.hpp>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wregister"
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <dlfcn.h>
#pragma GCC diagnostic pop
namespace py = pybind11;
namespace vmo
{
void vmo_context()
{
}

class node final : public ossia::nonowning_graph_node
{
  ossia::inlet input{ossia::audio_port{}};
  ossia::inlet active{ossia::value_port{}};
  ossia::outlet output{ossia::audio_port{}};

  bool m_is_active{};

  std::vector<std::vector<float>> data;
public:
  node()
  {
    vmo_context();
  }

  void
  run(ossia::token_request tk, ossia::exec_state_facade s) noexcept override
  {
    try
    {
      auto& i = *input.data.target<ossia::audio_port>();
      auto& ok = *active.data.target<ossia::value_port>();
      auto& o = *output.data.target<ossia::audio_port>();

      for(const auto& v : ok.get_data())
      {
        m_is_active = ossia::convert<bool>(v.value);
      }

      if(m_is_active)
      {

      }
      else
      {
        static bool ok = false;
        if(!ok)
        {
          ok = true;
          using namespace py::literals;

          dlopen("libpython2.7.so", RTLD_LAZY |  RTLD_GLOBAL);
          py::scoped_interpreter guard{};
          py::module oracle{py::module::import("vmo.VMO.oracle")};

          std::vector<std::string> seq = {"a", "b" , "b", "c", "a", "b", "c", "d", "a", "b", "c" };

          auto p = oracle.attr("build_oracle")(seq, "f");
          auto locals = py::dict("p"_a=p);
          py::exec(R"_(
                   print(str([0, 0, 0, 1, 0, 1, 2, 2, 0, 1, 2, 3]) + " == " + str(p.lrs))
                   print(str([None, 0, 0, 2, 0, 1, 2, 4, 0, 1, 2, 7]) + " == " + str(p.sfx))
                   )_", py::globals(), locals);

        }

      }
    }
    catch(std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
  }

  std::string label() const noexcept override
  {
    return "vmo";
  }

private:
};

ProcessExecutorComponent::ProcessExecutorComponent(
    vmo::Model& element, const Execution::Context& ctx,
    const Id<score::Component>& id, QObject* parent)
    : ProcessComponent_T{element, ctx, id, "vmoExecutorComponent", parent}
{
  try
  {
    auto n = std::make_shared<vmo::node>();
    this->node = n;
    m_ossia_process = std::make_shared<ossia::node_process>(n);
  }
  catch(std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  /** Don't forget that the node executes in another thread.
   * -> handle live updates with the in_exec function, e.g. :
   *
   * connect(&element.metadata(), &score::ModelMetadata::ColorChanged,
   *         this, [=] (const QColor& c) {
   *
   *   in_exec([c,n=std::dynamic_pointer_cast<vmo::node>(this->node)] {
   *     n->set_color(c);
   *   });
   *
   * });
   */
}
}
