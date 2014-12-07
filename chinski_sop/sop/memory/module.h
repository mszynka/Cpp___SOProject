#ifndef _SOP_MEMORY_MODULE_H_
#define _SOP_MEMORY_MODULE_H_

#include <string>
#include ".\sop\system\module.h"
#include ".\sop\memory\logical_memory.h"
#include ".\sop\memory\physical_memory.h"
#include ".\sop\memory\swap.h"

namespace sop
{
  namespace memory
  {
    /*
      Memory module class.
    */
    class Module : public sop::system::Module
    {
      public:
        explicit Module(sop::system::Kernel *kernel);
        virtual ~Module();
        virtual std::string getClassName() const;
        virtual void initialize();
        LogicalMemory* allocate(uint16_t program_size,uint16_t pid);
      

      protected:

      private:
        PhysicalMemory physical_drive;//nasza pami�� fizyczna, pierwszy argument to rozmair w bajtach, a drugi rozmiar ramki
        uint8_t calculatePages(uint16_t program_size); //zwraca liczb� stron niezb�dn� dla danego programu
        SwapFile swap_drive;

    };
  }
}

#endif