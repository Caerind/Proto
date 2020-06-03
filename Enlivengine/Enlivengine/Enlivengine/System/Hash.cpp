#include <Enlivengine/System/Hash.hpp>

#include <cstring> // strlen
#include <meow_hash/meow_hash_x64_aesni.h>

namespace en
{

U32 Hash::Meow32(const void* buffer, U32 size)
{
	return MeowU32From(MeowHash(MeowDefaultSeed, size, (void*)buffer), 0);
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
	return MeowU64From(MeowHash(MeowDefaultSeed, size, (void*)buffer), 0);
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

U32 Hash::Combine32(U32 hash1, U32 hash2)
{
	return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
}

U64 Hash::Combine64(U64 hash1, U64 hash2)
{
	return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2));
}

} // namespace en
