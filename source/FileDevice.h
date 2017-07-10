#pragma once

#include <stdlib.h>
#include <stdio.h>

template<typename T>
void FileRead(void* pDevice, size_t index, size_t count, void* pDestination)
{
  FILE* pFile = reinterpret_cast<FILE*>(pDevice);
  fseek(pFile, index * sizeof(T), SEEK_SET);
  fread(pDestination, sizeof(T), count, pFile);
}

template<typename T>
void FileWrite(void* pDevice, size_t index, size_t count,
                const void* pSource)
{
  FILE* pFile = reinterpret_cast<FILE*>(pDevice);
  fseek(pFile, index * sizeof(T), SEEK_SET);
  fwrite(pSource, sizeof(T), count, pFile);
}
