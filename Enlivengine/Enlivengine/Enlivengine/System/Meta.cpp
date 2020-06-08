#include "Meta.hpp"

namespace en::Meta
{

std::unordered_map<U32, std::function<void*()>> ClassFactory::mFactory;

} // namespace en::Meta