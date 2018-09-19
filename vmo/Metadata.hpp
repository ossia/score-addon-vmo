#pragma once
#include <Process/ProcessMetadata.hpp>

namespace vmo
{
class Model;
}

PROCESS_METADATA(
    , vmo::Model, "fe02e504-964a-479c-a1fb-4e602b777c18",
    "vmo",                                   // Internal name
    "vmo",                                   // Pretty name
    Process::ProcessCategory::Other,              // Category
    "Other",                                      // Category
    "Description",                                // Description
    "Author",                                     // Author
    (QStringList{"Put", "Your", "Tags", "Here"}), // Tags
    {},                                           // Inputs
    {},                                           // Outputs
    Process::ProcessFlags::SupportsAll            // Flags
)
