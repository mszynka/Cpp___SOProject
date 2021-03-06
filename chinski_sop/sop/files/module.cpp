#include ".\sop\files\module.h"
#include ".\sop\system\kernel.h"
#include ".\sop\files\filesystem.h"

sop::files::Module::Module(sop::system::Kernel *kernel):
  sop::system::Module(kernel)
{
  this->fsxxxx = new Filesystem(kernel->getLogger(), "disk.txt");
}

sop::files::Module::~Module()
{

}

std::string sop::files::Module::getClassName() const
{
  return "sop::files::Module";
}

void sop::files::Module::initialize()
{
  sop::system::Shell* shell = this->_kernel->getShell();
  sop::logger::Logger* logger = this->_kernel->getLogger();
  shell->registerCommand("cd",&Filesystem::changeDirectoryHandler,(this->fsxxxx));
  shell->registerCommand("rm",&Filesystem::removeFileHandler,this->fsxxxx);
  shell->registerCommand("vi",&Filesystem::viHandler,this->fsxxxx);
  shell->registerCommand("touch",&Filesystem::createFileHandler,this->fsxxxx);
  shell->registerCommand("mkdir",&Filesystem::createDirectoryHandler,this->fsxxxx);
  shell->registerCommand("rmdir",&Filesystem::removeDirectoryHandler,this->fsxxxx);
  shell->registerCommand("l",&Filesystem::listHandler,this->fsxxxx);
  shell->registerCommand("ll",&Filesystem::listHandler,this->fsxxxx);
  shell->registerCommand("cat",&Filesystem::catHandler,this->fsxxxx);
  shell->registerCommand("diskstat",&Filesystem::statHandler,this->fsxxxx);
  shell->registerCommand("format",&Filesystem::formatHandler,this->fsxxxx);
}