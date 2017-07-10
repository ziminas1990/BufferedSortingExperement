#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <iostream>

extern "C" {
#include "BufferedSorting.h"
}

#include "SimpleMemoryDevice.h"
#include "FileDevice.h"

template<typename T>
int GreaterFirst(const void* pLeft, const void *pRight)
{
  const T* pTypedLeft   = static_cast<const T*>(pLeft);
  const T* pTypedRight  = static_cast<const T*>(pRight);
  if(*pTypedLeft > *pTypedRight)
    return -1;
  else if(*pTypedLeft < *pTypedRight)
    return 1;
  else
    return 0;
}

int main(int argc, char* argv[])
{
  FILE* pFile = fopen(argv[1], "rb+");
  if(!pFile) {
    std::cerr << "Can't open file " << argv[1] << std::endl;
    return 1;
  }

  fseek(pFile, 0, SEEK_END);
  size_t nSize = ftell(pFile);
  fseek(pFile, 0, SEEK_SET);

  Task task;
  task.m_pDevice      = pFile;
  task.m_nLength      = nSize / sizeof(int);
  task.m_nElementSize = sizeof(int);
  task.m_pReader      = FileRead<int>;
  task.m_pWriter      = FileWrite<int>;
  task.m_pCompare     = GreaterFirst<int>;

  BufferedSort(&task);

  fclose(pFile);
  return 0;
}
