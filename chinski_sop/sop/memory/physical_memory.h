#ifndef _SOP_MEMORY_PHYSICAL_MEMORY_H_
#define _SOP_MEMORY_PHYSICAL_MEMORY_H_

#include <stdint.h>
#include <boost\shared_ptr.hpp>
#include <list>
#include <deque>
#include ".\sop\memory\logical_memory.h"
#include ".\sop\memory\swap.h"
namespace sop
{
namespace memory
{
  struct frame
  {
    uint16_t pid;
    uint8_t page_number;
  };
  
class PhysicalMemory
{
public:	
  PhysicalMemory(uint16_t storage_size,uint8_t frame_size);//kontstruktor tworz�cy pami�� i ustawiajacy liczb� ramek
  ~PhysicalMemory();//destruktor, stworzono dla mo�liwo�ci zapisania do loggera ze co� zosta�o zniszczone
  uint8_t getFrameSize();//zwraca rozmiar ramki
  uint16_t getStorageSize();//zwraca rozmiar pami�ci
  uint8_t* getStorage();//zwraca wska�nik na pami�� fizyczn�
  uint16_t getNumberOfFrames();//zwraca liczbe ramek
  uint16_t getNumberOfFreeFrames();//zwraca liczb� wolnych ramek
  uint16_t getNumberOfNotFreeFrames();//zwraca liczb� zaj�tych ramek
  void setNubmerOfFreeFrames(uint16_t change);//ustawia liczb� wolnych ramek
  void setNumberOfNotFreeFrames(uint16_t change);//ustawia liczb� zaj�tych ramek
  uint8_t getGuardianLevelofFreeFrames();//zwraca krytyczn� liczb� wolnych ramek
  void getFreeFrames(uint8_t pages_needed,sop::memory::LogicalMemory* table_of_pages,uint16_t pid, SwapFile* file_swap);//funkcja odpowiedzialna za przydzielenie ramek stronom, na podstawie liczby poz�danych stron
  void setFrame(uint16_t pid,uint8_t page_number,uint16_t frame_number);//ustawia rekord w tabeli ramek na podstawie podanych argument�w
  frame* getFrame(uint16_t frame_number);//zwraca opis danej ramki z tabeli ramek
  void popFrontListOfFreeFrames();//usuwa ramke bedaca z przodu listy wolnych ramek
  void pushEndListOfFreeFrames(uint16_t frame_nr);//wstawia na koniec listy nr wolnej ramki
  void FindAndEraseFromDeque(uint16_t frame_nr);//usuni�cie ramki z kolejki ramek
protected:
  
private:
  bool isEnoughFrames(uint8_t pages_needed);//funkcja zwracaj�ca czy jest wystarczaj�co ramek, aby mo�na by�o je przydzieli� "z marszu" stronom, czy trzeba wykona� swapa
  uint8_t frame_size;//rozmiar ramki
  uint16_t storage_size; //zmienna odpowiadaj�ca za rozmiar pami�ci
  uint16_t number_of_frames;//liczba ramek
  uint16_t number_of_free_frames;//liczba wolnych ramek
  uint16_t number_of_not_free_frames;//liczba zaj�tych ramek
  uint8_t guardian_of_free_frames;
  boost::shared_ptr<uint8_t[]> storage;//w�a�ciwa pami�� fizyczna
  boost::shared_ptr<frame[]> frame_table;//tabela ramek
  std::list <uint16_t> list_of_free_frames;//lista wolnych ramek
  std::deque <uint16_t>assigned_frames_deque;//kolejka FIFO przydzielanych ramek, potrzebna przy wymianie stron(algorytm FIFO)
  void swap(SwapFile* file_swap,LogicalMemory* table_of_pages,int8_t victim,int8_t fram);//funkcja wymiataj�ca strone na plik wymiany
 
};

}

}


#endif //_SOP_MEMORY_PHYSICAL_MEMORY_H_