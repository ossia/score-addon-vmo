#include "Executor.hpp"

#include <Process/ExecutionContext.hpp>

#include <ossia/dataflow/port.hpp>

#include <vmo/Process.hpp>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wregister"
#include <pybind11/embed.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#pragma GCC diagnostic pop
#include <ossia/network/value/value_hash.hpp>
#include <ossia/detail/hash_map.hpp>
namespace py = pybind11;

namespace vmo
{

py::scoped_interpreter& interpreter()
{
  static py::scoped_interpreter guard{};
  return guard;
}

py::module& oracle()
{
  interpreter();
  static py::module oracle{py::module::import("vmo.VMO.oracle")};
  return oracle;
}
py::module& generate()
{
  interpreter();
  static py::module oracle{py::module::import("vmo.generate")};
  return oracle;
}

class node final : public ossia::nonowning_graph_node
{
  ossia::inlet input{ossia::value_port{}};
  ossia::inlet regen{ossia::value_port{}};
  ossia::inlet bang{ossia::value_port{}};
  ossia::inlet sequence_length{ossia::value_port{}};
  ossia::outlet output{ossia::value_port{}};

  bool m_is_active{};

  std::vector<ossia::value> input_sequence;
  std::vector<ossia::value> output_sequence;
  std::size_t sequence_idx{};
public:
  node()
  {
    input.data.target<ossia::value_port>()->is_event = true;
    regen.data.target<ossia::value_port>()->is_event = true;
    bang.data.target<ossia::value_port>()->is_event = true;
    m_inlets.push_back(&input);
    m_inlets.push_back(&regen);
    m_inlets.push_back(&bang);
    m_inlets.push_back(&sequence_length);
    m_outlets.push_back(&output);
  }

  void
  run(ossia::token_request tk, ossia::exec_state_facade s) noexcept override
  {
    interpreter();
    try
    {
      auto& i = *input.data.target<ossia::value_port>();
      auto& r = *regen.data.target<ossia::value_port>();
      auto& b = *bang.data.target<ossia::value_port>();
      auto& o = *output.data.target<ossia::value_port>();

      for(const auto& v : i.get_data())
      {
        input_sequence.push_back(v.value);
      }

      if (!r.get_data().empty())
      {
        // Create a dictionary from the received values
        ossia::fast_hash_map<ossia::value, int> v;
        int max = 0;
        std::vector<int> seq;
        for(auto& val : input_sequence)
        {
          if(auto it = v.find(val); it != v.end())
          {
            seq.push_back(it->second);
          }
          else
          {
            v.insert({val, max});
            seq.push_back(max);
            max++;
          }
        }

        // Create a python oracle
        using namespace py::literals;

        {
          auto locals = py::dict("seq"_a=seq);
          py::exec("print(seq)", py::globals(), locals);
        }
        auto p = oracle().attr("build_oracle")(seq, "a");

        auto res = generate().attr("improvise")(p, 5);
        auto locals = py::dict("res"_a=res);
        py::exec(R"_(
import sys
print(res)
sys.stdout.flush())_", py::globals(), locals);
        //self.sequence = self.oracle.make_rand_sequence(0.4, seq_len);

        // Create a sequence in the oracle
      }

                           /*
          std::vector<int> seq = {0,1,1,2,0,1,2,3,0,1,2};//"a", "b" , "b", "c", "a", "b", "c", "d", "a", "b", "c" };

          auto p = oracle().attr("build_oracle")(seq, "f");
          auto locals = py::dict("p"_a=p);
          py::exec(R"_(
                   print(str([0, 0, 0, 1, 0, 1, 2, 2, 0, 1, 2, 3]) + " == " + str(p.lrs))
                   print(str([None, 0, 0, 2, 0, 1, 2, 4, 0, 1, 2, 7]) + " == " + str(p.sfx))
                   )_", py::globals(), locals);
                                      */

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
