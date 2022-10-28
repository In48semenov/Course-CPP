#pragma once

#include <numeric>
#include <string>

namespace mse {

[[nodiscard]] std::size_t sum_digits(std::int32_t number);

[[nodiscard]] std::size_t count_primes(std::uint32_t limit);

[[nodiscard]] std::int32_t divide(std::int32_t a, std::int32_t b);

[[nodiscard]] std::int32_t roman_to_integer(const std::string &number);

} // namespace mse
