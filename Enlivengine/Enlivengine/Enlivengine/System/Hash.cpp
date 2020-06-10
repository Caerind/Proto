#include <Enlivengine/System/Hash.hpp>

#include <cstring> // strlen

#include <meow_hash/meow_hash_x64_aesni.h>

namespace en
{

U32 Hash::Meow32(const void* buffer, U32 size)
{
	return static_cast<U32>(MeowU32From(MeowHash(MeowDefaultSeed, size, (void*)buffer), 0));
}

U32 Hash::Meow32(const char* key)
{
	return Meow32((const void*)key, static_cast<U32>(strlen(key)));
}

U32 Hash::Meow32(const std::string& key)
{
	return Meow32((const void*)key.data(), static_cast<U32>(key.size()));
}

U32 Hash::Meow32(std::string_view key)
{
	return Meow32((const void*)key.data(), static_cast<U32>(key.size()));
}

U64 Hash::Meow64(const void* buffer, U32 size)
{
	return static_cast<U32>(MeowU64From(MeowHash(MeowDefaultSeed, size, (void*)buffer), 0));
}

U64 Hash::Meow64(const char* key)
{
	return Meow64((const void*)key, static_cast<U32>(strlen(key)));
}

U64 Hash::Meow64(const std::string& key)
{
	return Meow64((const void*)key.data(), static_cast<U32>(key.size()));
}

U64 Hash::Meow64(std::string_view key)
{
	return Meow64((const void*)key.data(), static_cast<U32>(key.size()));
}

} // namespace en
