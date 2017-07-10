#include "BufferedSorting.h"

#include <math.h>
#include <stdint.h>
#include <memory.h>
#include "LoggingMacroses.h"

void ReadElementFromEachBlock(Task* pTask, uint8_t* pBuffer,
                              size_t nTotalBlocks, size_t nBlockLength,
                              size_t nOffset)
{
  for(size_t nBlock = 0; nBlock < nTotalBlocks; nBlock++)
    pTask->m_pReader(pTask->m_pDevice,
                     nBlock * nBlockLength + nOffset, 1,
                     pBuffer + nBlock * pTask->m_nElementSize);
}

void WriteElementToEachBlock(Task* pTask, const uint8_t* pBuffer,
                             size_t nTotalBlocks, size_t nBlockLength,
                             size_t nOffset)
{
  for(size_t nBlock = 0; nBlock < nTotalBlocks; nBlock++)
    pTask->m_pWriter(pTask->m_pDevice,
                     nBlock * nBlockLength + nOffset, 1,
                     pBuffer + nBlock * pTask->m_nElementSize);
}

void BufferedSort(Task* pTask)
{
  // Диск нужно разбить на nTotalBlocks блоков, по nBlockLength элементов в
  // каждом блоке
  size_t nTotalBlocks     = sqrt(pTask->m_nLength);
  size_t nBlockLength     = pTask->m_nLength  / nTotalBlocks;
  size_t nBlockSize       = nBlockLength      * pTask->m_nElementSize;
  size_t nLastBlockLength = pTask->m_nLength  % nBlockLength;
  nTotalBlocks++;

  // Выделим буфер на два блока
  size_t nBufferLength  = 2 * nBlockLength;
  //int pDeubg[10];
  //uint8_t* pBuffer  = (uint8_t*)pDeubg;
  uint8_t* pBuffer  = (uint8_t*)malloc(2 * nBlockSize);

  // Сортируем между собой соответствующие элементы каждого блока
  LOG("Sorting layers...\n");
  for(size_t nOffset = 0; nOffset < nBlockLength; nOffset++)
  {
    LOG(".");
    LOG_PERSENT(nOffset, nOffset + 1, nBlockLength, " \t%d%%\n");

    size_t nBlocks = (nOffset < nLastBlockLength) ? nTotalBlocks
                                                  : nTotalBlocks - 1;
    ReadElementFromEachBlock(pTask, pBuffer, nBlocks, nBlockLength, nOffset);
    qsort(pBuffer, nBlocks, pTask->m_nElementSize, pTask->m_pCompare);
    WriteElementToEachBlock(pTask, pBuffer, nBlocks, nBlockLength, nOffset);
  }
  LOG("\nAll layers has been sorted\n");

  // Выгружаем в буфер по два соседних блока, сортируем их между собой
  // и записываем обратно на диск. Первый блок вычитывается во вторую половину
  // буфера, а все последующие - в первую
  uint8_t* pLeftBlock   = pBuffer;
  uint8_t* pRightBlock  = pBuffer + nBlockSize;
  LOG("Sorting blocks\n");
  pTask->m_pReader(pTask->m_pDevice, 0, nBlockLength, pRightBlock);
  for(size_t nBlock = 1; nBlock < nTotalBlocks - 1; nBlock++) {
    LOG_PERSENT(nBlock, nBlock + 1, nTotalBlocks - 1, "%d%% -> ");

    pTask->m_pReader(pTask->m_pDevice, nBlock * nBlockLength,
                     nBlockLength, pLeftBlock);
    qsort(pBuffer, nBufferLength, pTask->m_nElementSize, pTask->m_pCompare);
    // Левый блок пишем на диск
    pTask->m_pWriter(pTask->m_pDevice, (nBlock - 1) * nBlockLength,
                     nBlockLength, pLeftBlock);
  }

  if(nLastBlockLength) {
    // В конце есть один не полный блок, поэтому его обработаем отдельно
    memcpy(pLeftBlock, pRightBlock, nBlockSize);
    pTask->m_pReader(pTask->m_pDevice, (nTotalBlocks - 1) * nBlockLength,
                     nLastBlockLength, pRightBlock);
    qsort(pBuffer, nBlockLength + nLastBlockLength,
          pTask->m_nElementSize, pTask->m_pCompare);
    // Пишем два последних блока
    pTask->m_pWriter(pTask->m_pDevice, (nTotalBlocks - 2) * nBlockLength,
                     nBlockLength + nLastBlockLength, pLeftBlock);
  } else {
    // Последний блок на диске имеет нулевую длину (его не существует), поэтому
    // заполняем предпоследний блок диска, т.к. фактически, он является
    // последним
    pTask->m_pWriter(pTask->m_pDevice, (nTotalBlocks - 2) * nBlockLength,
                     nBlockLength, pRightBlock);
  }
  LOG("100%\n");
  LOG("COMPLETE!");
  free(pBuffer);
}
