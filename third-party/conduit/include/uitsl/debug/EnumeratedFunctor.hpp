#pragma once
#ifndef UITSL_DEBUG_ENUMERATEDFUNCTOR_HPP_INCLUDE
#define UITSL_DEBUG_ENUMERATEDFUNCTOR_HPP_INCLUDE

#include <unordered_map>

#include "../third-party/Empirical/include/emp/base/vector.hpp"

namespace uitsl {

/**
 * This class holds an unordered map from Input to Output.
 * This is done so they can be passed around in template arguments,
 * or wherever else a class might be used, wihtout having to rebuild
 * the map every time.
*/
template <typename Input, typename Output>
class EnumeratedFunctor {
    std::unordered_map<Input, Output> map;
public:
    EnumeratedFunctor() = default;
    EnumeratedFunctor(const std::unordered_map<Input, Output>& map_) : map(map_) { ; }
    EnumeratedFunctor(const emp::vector<Output>& vect) {
        for (size_t i = 0; i < vect.size(); ++i) map[i] = vect[i];
    }

    Output operator()(const Input& a) const { return map.at(a); }
    size_t GetSize() const { return map.size(); }
};

} // namespace uitsl
#endif // #ifndef UITSL_DEBUG_ENUMERATEDFUNCTOR_HPP_INCLUDE
