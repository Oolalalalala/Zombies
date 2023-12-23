#pragma once

#include <unordered_map>

class AssetHandle
{
public:
	AssetHandle()
		: m_Handle(s_Counter++)
	{
	}

	AssetHandle(uint64_t handle)
		: m_Handle(handle)
	{
	}

	operator uint64_t() const { return m_Handle; }
	bool operator==(const AssetHandle& other) const { return m_Handle == other.m_Handle; }

private:
	uint64_t m_Handle;

	// Uses simple increment counter
	static uint64_t s_Counter;
};


namespace std {

	template<>
	struct hash<AssetHandle>
	{
		inline std::size_t operator()(const AssetHandle& handle) const
		{
			return hash<uint64_t>()((uint64_t)handle);
		}
	};

}
