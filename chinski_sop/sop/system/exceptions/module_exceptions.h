#ifndef _SOP_SYSTEM_EXCEPTIONS_MODULE_EXCEPTIONS_H_
#define _SOP_SYSTEM_EXCEPTIONS_MODULE_EXCEPTIONS_H_

#include <string>
#include ".\sop\exception.h"

namespace sop{
  namespace system{
    namespace exceptions{

      /*
        Exception for module exceptions.
        Every module exception should inherit from it.
      */
      class ModuleException : public sop::Exception{
        public:
          ModuleException();
          ModuleException(const std::string & value);
          virtual ~ModuleException();
          virtual std::string getClassName();

        protected:

        private:

      };

      /*
        Exception thrown when module initialization fails.
      */
      class ModuleInitializationException : public sop::system::exceptions::ModuleException{
        public:
          ModuleInitializationException();
          ModuleInitializationException(const std::string & value);
          virtual ~ModuleInitializationException();
          virtual std::string getClassName();

        protected:

        private:

      };
    }
  }
}

#endif