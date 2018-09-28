#pragma once
#include <Process/ProcessMetadata.hpp>

namespace vmo
{
class Model;
}

PROCESS_METADATA(
    , vmo::Model, "fe02e504-964a-479c-a1fb-4e602b777c18",
    "vmo",                                           // Internal name
    "Variable Markov Oracle",                        // Pretty name
    Process::ProcessCategory::Other,                 // Category
    "Impro",                                         // Category
    "Improvise using the VMO algorithm",             // Description
    "Shlomo Dubnov, Ge Wang, Jean-Michaël Celerier", // Author
    (QStringList{}),                                 // Tags
    {},                                              // Inputs
    {},                                              // Outputs
    Process::ProcessFlags::SupportsAll               // Flags
)
