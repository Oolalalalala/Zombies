#pragma once

#include <cstdint>
#include <memory>


template<typename TData>
struct Buffer
{
	TData* Data = nullptr;
	uint32_t Count = 0;

	Buffer() = default;

	Buffer(uint32_t count)
	{
		Allocate(count);
	}

	Buffer(const Buffer&) = default;

	template<typename T>
	static Buffer<T> Copy(const Buffer<T> source)
	{
		Buffer<T> dest(source.Count);
		memcpy(dest.Data, source.Data, source.Count * sizeof(T));
		return dest;
	}

	void Allocate(uint32_t count)
	{
		Data = new TData[count];
		Count = count;
	}

	void Release()
	{
		delete[] Data;
		Data = nullptr;
		Count = 0;
	}

	TData* Begin() const
	{
		return Data;
	}

	TData* End() const
	{
		return Data + Count;
	}

	operator bool() const
	{
		return (bool)Data;
	}

	TData& operator[](uint32_t idx)
	{
		return *(Data + idx);
	}
	
};