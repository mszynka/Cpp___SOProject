#include ".\sop\memory\module.h"
#include <math.h>

sop::memory::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel),physical_drive(4096,32),swap_drive(4096,32)//stworzenie pamięci fizycznej i pliku wymiany
{

}

sop::memory::Module::~Module()
{

}

std::string sop::memory::Module::getClassName() const
{
  return "sop::memory::Module";
}

void sop::memory::Module::initialize()
{
  
}

uint8_t sop::memory::Module::calculatePages(uint16_t program_size)
{
  float number_of_pages=(float)physical_drive.getFrameSize();
  number_of_pages=ceil(number_of_pages/program_size);
  
  return (uint8_t)number_of_pages;
}

sop::memory::LogicalMemory* sop::memory::Module::allocate(uint16_t program_size,uint16_t pid)
{
  LogicalMemory table_of_pages(calculatePages(program_size));//tworzenie tabeli stron dla procesu, o określonej liczbie stron
  physical_drive.getFreeFrames(table_of_pages.getPageTableSize(),&table_of_pages,pid,&swap_drive);//wywołanie funkcji przydzielającej ramki stronom na podstawie liczby stron w tabeli stron
  //?dobrze przekazuje table of pages do getFreeFrames??
  return &table_of_pages;
}


