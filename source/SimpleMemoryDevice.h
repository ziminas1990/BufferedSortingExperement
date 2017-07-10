#pragma once

#include <stdlib.h>

template<typename T>
void ArrayRead(void* pDevice, size_t index, size_t count, void* pDestination)
{
  T* pTypedArray = reinterpret_cast<T*>(pDevice);
  memcpy(pDestination, pTypedArray + index, count * sizeof(T));
}

template<typename T>
void ArrayWrite(void* pDevice, size_t index, size_t count,
                const void* pSource)
{
  T* pTypedArray = reinterpret_cast<T*>(pDevice);
  memcpy(pTypedArray + index, pSource, count * sizeof(T));
}
