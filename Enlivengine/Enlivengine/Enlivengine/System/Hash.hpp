#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#include <string>


namespace en
{

class Hash
{
public:
	Hash() = delete;

	// TODO : constepxr
	static U32 Meow32(const void* buffer, U32 size);
	static U32 Meow32(const char* key);
	static U32 Meow32(const std::string& key);
	static U32 Meow32(std::string_view key);

	// TODO : constepxr
	static U64 Meow64(const void* buffer, U32 size);
	static U64 Meow64(const char* key);
	static U64 Meow64(const std::string& key);
	static U64 Meow64(std::string_view key);

	// TODO : Temp while Meow32 isn't constexpr
	static constexpr U32 ConstexprHash(const void* buffer, U32 size)
	{
		if (size == 0 || buffer != nullptr)
		{
			const U8* bytesBuffer = (const U8*)buffer;
			return static_cast<U32>(bytesBuffer[0]) + 33 * ConstexprHash((const void*)(bytesBuffer + 1), size - 1);
		}
		else
		{
			return 5381;
		}
	}
	static constexpr U32 ConstexprHash(const char* key) { return ConstexprHash((const void*)key, static_cast<U32>(strlen(key))); }
	static constexpr U32 ConstexprHash(const std::string& key) { return ConstexprHash((const void*)key.data(), static_cast<U32>(key.size())); }
	static constexpr U32 ConstexprHash(std::string_view key) { return ConstexprHash((const void*)key.data(), static_cast<U32>(key.size())); }

	static constexpr U32 Combine32(U32 hash1, U32 hash2) { return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2)); }
	static constexpr U64 Combine64(U64 hash1, U64 hash2) { return hash1 ^ (hash2 + 0x9e3779b9 + (hash1 << 6) + (hash1 >> 2)); }
};

} // namespace en
