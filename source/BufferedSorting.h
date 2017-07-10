#ifndef BUFFERED_SORTING_H
#define BUFFERED_SORTING_H

#include <stdlib.h>

// Выгружает из массива элемент index по переданному указателю
typedef void (fpRead)(void* pDevice, size_t index, size_t count,
                      void* pDestination);
// Записывает некоторый элемент из указанного участка памяти в указанную
// ячейку массива
typedef void (fpWrite)(void* pDevice, size_t index, size_t count,
                       const void* pSource);
// Сравнивает два элемента массива
typedef int (fpGreater)(const void* pLeft, const void *pRight);

struct Task
{
  void*  m_pDevice;
  size_t m_nElementSize;
  size_t m_nLength;

  fpRead*     m_pReader;
  fpWrite*    m_pWriter;
  fpGreater*  m_pCompare;
};
typedef struct Task Task;

void BufferedSort(Task* pTask);

#endif // #ifndef BUFFERED_SORTING_H
