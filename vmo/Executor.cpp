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
#include <ossia/detail/hash_map.hpp>
#include <ossia/network/value/value_hash.hpp>
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
  ossia::value_inlet input;
  ossia::value_inlet regen;
  ossia::value_inlet bang;
  ossia::value_inlet sequence_length;
  ossia::value_outlet output;

  std::vector<ossia::value> input_sequence;
  std::vector<int> output_sequence;
  std::size_t sequence_idx{};
  int32_t m_sequence_length{16};

  ossia::fast_hash_map<ossia::value, int> m_value_cache;
  std::vector<int> m_sequence_cache;

public:
  node()
  {
    input.target<ossia::value_port>()->is_event = true;
    regen.target<ossia::value_port>()->is_event = true;
    bang.target<ossia::value_port>()->is_event = true;
    m_inlets.push_back(&input);
    m_inlets.push_back(&regen);
    m_inlets.push_back(&bang);
    m_inlets.push_back(&sequence_length);
    m_outlets.push_back(&output);
  }

  void
  run(const ossia::token_request& tk, ossia::exec_state_facade s) noexcept override
  {
    interpreter();
    try
    {
      auto& i = *input.target<ossia::value_port>();
      auto& r = *regen.target<ossia::value_port>();
      auto& b = *bang.target<ossia::value_port>();
      auto& sl = *sequence_length.target<ossia::value_port>();
      auto& o = *output.target<ossia::value_port>();

      for (const auto& v : i.get_data())
      {
        input_sequence.push_back(v.value);
      }

      for (const auto& v : sl.get_data())
      {
        if (auto i = v.value.target<int32_t>())
          m_sequence_length = ossia::clamp(*i, 0, 10000);
      }

      if (!r.get_data().empty())
      {
        // Create a dictionary from the received values
        m_value_cache.clear();
        // VMO fails if alphabet starts at 0
        int max = 1;
        m_sequence_cache.clear();
        for (auto& val : input_sequence)
        {
          if (auto it = m_value_cache.find(val); it != m_value_cache.end())
          {
            m_sequence_cache.push_back(it->second);
          }
          else
          {
            m_value_cache.insert({val, max});
            m_sequence_cache.push_back(max);
            max++;
          }
        }

        // Create a python oracle
        using namespace py::literals;
        auto p = oracle().attr("build_oracle")(m_sequence_cache, "a", 0.01);

        py::list res = generate().attr("improvise")(p, m_sequence_length);

        output_sequence.clear();
        for (auto item : res)
        {
          output_sequence.push_back(item.cast<int>() - 1);
        }

        sequence_idx = 0;
      }

      if (!b.get_data().empty())
      {
        if (sequence_idx < output_sequence.size())
        {
          o.write_value(
              input_sequence[output_sequence[sequence_idx]], tk.physical_start(s.modelToSamples()));
        }
        sequence_idx = (sequence_idx + 1) % output_sequence.size();
      }
    }
    catch (std::exception& e)
    {
      std::cerr << e.what() << std::endl;
    }
  }

  std::string label() const noexcept override { return "VMO"; }
};

ProcessExecutorComponent::ProcessExecutorComponent(
    vmo::Model& element,
    const Execution::Context& ctx,
    const Id<score::Component>& id,
    QObject* parent)
    : ProcessComponent_T{element, ctx, id, "vmoExecutorComponent", parent}
{
  try
  {
    auto n = std::make_shared<vmo::node>();
    this->node = n;
    m_ossia_process = std::make_shared<ossia::node_process>(n);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }
}
}
