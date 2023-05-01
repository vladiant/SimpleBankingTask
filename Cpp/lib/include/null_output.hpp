#pragma once

#include <ostream>

namespace sbt {
struct NullOutput : public std::ostream {};
}  // namespace sbt
