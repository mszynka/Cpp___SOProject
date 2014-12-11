#include ".\sop\processes\module.h"
#include ".\sop\processes\class_process.h"
#include ".\sop\system\kernel.h"
#include ".\sop\logger\logger.h"
#include ".\sop\string_converter.h"

sop::processes::Module::Module(sop::system::Kernel *kernel):
sop::system::Module(kernel)
{
  
}

sop::processes::Module::~Module()
{

}

std::string sop::processes::Module::getClassName() const
{
  return "sop::processes::Module";
}

//rejestrowanie komend shellowskich
void sop::processes::Module::initialize()
{
  _kernel->getShell()->registerCommand("showprocess",&sop::processes::Module::cH_showprocess,this); //zarejestrowanie komendy showprocess
  _kernel->getShell()->registerCommand("kill",&sop::processes::Module::cH_kill,this); //zarejestrowanie komendy kill
  _kernel->getShell()->registerCommand("fork",&sop::processes::Module::cH_fork,this); //zarejestrowanie komendy fork
  _kernel->getShell()->registerCommand("exec",&sop::processes::Module::cH_exec,this); //zarejestrowanie komendy exec
  sop::processes::Module::CreateShellInit(); //zainicjowanie stworzenia procesu INIT
  fillQueue(); //wypelnienie kolejki z PID'ami
}
//polecenie shelllowskie wypisujace na ekran informacje o procesie
void sop::processes::Module::cH_showprocess(const std::vector<const std::string> & params)
{  
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()==1)
  {
    std::cout<<"showprocess [-h] PID"<<std::endl;
    std::cout<<"Displays informations about process"<<std::endl;
    return;
  }
  uint16_t PID = sop::StringConverter::convertStringTo<uint16_t>(params[params.size()-1]);
  boost::shared_ptr<sop::process::Process> ProcesToShow = sop::processes::Module::findProcess(PID);
  if(ProcesToShow)
  {
    std::cout << "PID number: " << ProcesToShow->getPID() << std::endl;
    std::cout << "PPID number: " << ProcesToShow->getPPID() << std::endl;
    std::cout << "UID number: " << ProcesToShow->getUID() << std::endl;
    std::cout << "GID number: " << ProcesToShow->getGID() << std::endl;
    std::cout << "isRunning: " << ProcesToShow->getIsActuallyRunning() << std::endl;
    std::cout << "isTrue: " << ProcesToShow->getIsTrueProcess() << std::endl;
    std::cout << "Array Table: " << ProcesToShow->getArrayPages() << std::endl;
    std::cout << "Register A: " << ProcesToShow->getRejestrA() << std::endl;
    std::cout << "Register B: " << ProcesToShow->getRejestrB() << std::endl;
    std::cout << "Register C: " << ProcesToShow->getRejestrC() << std::endl;
    std::cout << "Register D: " << ProcesToShow->getRejestrD() << std::endl <<std::endl;
  }  
  else
  {
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process not found\n");
  } 
}
//polecenie shellowskie zabijajace proces o podanym PID
void sop::processes::Module::cH_kill(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()==1)
  {
    std::cout<<"kill [-h] PID"<<std::endl;
    std::cout<<"Killing process with the specified PID number"<<std::endl;
    return;
  }
  uint16_t PID = sop::StringConverter::convertStringTo<uint16_t>(params[params.size()-1]);
  boost::shared_ptr<sop::process::Process> ProcesToKill = sop::processes::Module::findProcess(PID);
  if (ProcesToKill->getPID() != 0)
  {
  sop::processes::Module::kill(ProcesToKill);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Shell command executed: Process killed");
  }
  else
  {
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Shell command executed: Init process cannot be killed");
  }
}
//polecenie shellowskie tworzacy proces potomka
void sop::processes::Module::cH_fork(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()==1)
  {
    std::cout<<"fork [-h] PID"<<std::endl;
    std::cout<<"Creating child process"<<std::endl;
    return;
  }
  uint16_t PID = sop::StringConverter::convertStringTo<uint16_t>(params[params.size()-1]);
  boost::shared_ptr<sop::process::Process> ProcesToFork = sop::processes::Module::findProcess(PID);
  sop::processes::Module::fork(ProcesToFork);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Shell command executed: Child process created");
}
//polecenie wywolujace funkcje exec
void sop::processes::Module::cH_exec(const std::vector<const std::string> & params)
{
  if(sop::system::Shell::hasParam(params,"-h" )|| params.size()==1)
  {
    std::cout<<"exec [-h] name_file PID"<<std::endl;
    std::cout<<"Run file and write code to memory"<<std::endl;
    return;
  }
  if (params.size()==3)
  {
    uint16_t PID = sop::StringConverter::convertStringTo<uint16_t>(params[params.size()-1]);
    boost::shared_ptr<sop::process::Process> ProcesForExec = sop::processes::Module::findProcess(PID);
    sop::processes::Module::exec(params[1], ProcesForExec);
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"shell command executed: file has written to memory");
  }
}
//funkcja wstawiajaca proces do wektora procesow
void sop::processes::Module::addToVector(boost::shared_ptr<sop::process::Process> object)
{
  sop::processes::Module::ProcessVector.push_back(object);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process added to the vector of processes");
}
//funkcja usuwajaca proces z wektora procesow
void sop::processes::Module::removeFromVector(uint16_t PID)
{
  std::vector<boost::shared_ptr<sop::process::Process>>::iterator it=ProcessVector.begin();
  for(;it!=ProcessVector.end();++it)
  {
    if((*it)->getPID() == PID) break;
  }
  if(it!=ProcessVector.end())
  {
    ProcessVector.erase(it);
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process deleted");
  }
  else
  {
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"I didn't find process so i don't know what to delete");
  }
}
//funkcja wyszukujaca proces o podanym PID
boost::shared_ptr<sop::process::Process> sop::processes::Module::findProcess(uint16_t PID)
{
  boost::shared_ptr<sop::process::Process> process_found;
  std::vector<boost::shared_ptr<sop::process::Process>>::iterator it;
  for (it= ProcessVector.begin(); it!= ProcessVector.end(); ++it)
  {
    if ((*it)->getPID() == PID)
    {
      process_found = (*it);
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process found");
      break;
    }
  }
  return process_found;
}
//funkcja tworzaca proces init/shell
void sop::processes::Module::CreateShellInit()
{
  boost::shared_ptr<sop::process::Process> Procesik (new sop::process::Process());
  Procesik->setPID(0);
  Procesik->setPPID(0);
  Procesik->setUID(0);
  Procesik->setGID(0);
  Procesik->setMemoryFlagStatus(0);
  Procesik->setProcessorFlagStatus(0);
  Procesik->setEndingFlagStatus(0);
  Procesik->setProcessIsInScheduler(0);
  Procesik->setIsActuallyRunning(1);
  sop::processes::Module::addToVector(Procesik);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::SEVERE,"INIT created and added to vector of processes");
}
//funkcja tworzaca nowy proces
boost::shared_ptr<sop::process::Process> sop::processes::Module::createNewProcess()
{
  boost::shared_ptr<sop::process::Process> Procesik (new sop::process::Process());
  Procesik->setPID(sop::processes::Module::getPIDfromList());
  Procesik->setPPID(0);
  Procesik->setUID(0);
  Procesik->setGID(0);
  Procesik->setMemoryFlagStatus(0);
  Procesik->setProcessorFlagStatus(0);
  Procesik->setEndingFlagStatus(0);
  Procesik->setProcessIsInScheduler(0);
  Procesik->setIsActuallyRunning(0);
  sop::processes::Module::addToVector(Procesik);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"New process created and added to vector of processes");
  return Procesik;
}
//funkcja tworzaca nowego potomka
void sop::processes::Module::fork(boost::shared_ptr<sop::process::Process> Parent)
{
  boost::shared_ptr<sop::process::Process> child;
  child = sop::processes::Module::createNewProcess();
  child->setPPID(Parent->getPID());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Parent's PID setted to child's PPID");
  Parent->setRejestrA(child->getPID());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"PID of child has written to Register A");
  child->setRejestrB(Parent->getRejestrB());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Register B copied from parent to child");
  child->setRejestrC(Parent->getRejestrC());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Register C copied from parent to child");
  child->setRejestrD(Parent->getRejestrD());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Register D copied from parent to child");
  child->setGID(Parent->getGID());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"GID copied from parent to child");
  child->setUID(Parent->getUID());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"UID copied from parent to child");
  child->setIsTrueProcess(Parent->getIsTrueProcess());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"IsTrueProcess Flag copied from parent to child");
  child->setIsActuallyRunning(0); //ustawienie na 1 to zaslepka do sprawdzenia waita. ta flaga jest w trakcie wykonywania normalnie ustawiana przez procesor
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"IsActuallyRunning Flag is setted on 0");
  child->setMemoryFlagStatus(0);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory Flag is setted on 0");
  child->setProcessorFlagStatus(0);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Processor Flag is setted on 0");
  child->setProcessIsInScheduler(0);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"ProcessIsInScheduler Flag is setted on 0");
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"New child process created");
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::CONFIG,"PROCESS GET STATUS NEW");
  if (Parent->getIsTrueProcess() == 0)
  {    
    return;
  } 
  else if (Parent->getIsTrueProcess() == 1)
  {
    child->setArrayPages(/*allocate(Parent->getArrayPages(), child->getPID())*/60); //wartosc 60 to zaslepka symulujaca tablice stronic normlanie jest to wskaznik na strukture
    if (/*child->getArrayPages().getPageTableSize()*/200 > 0) //wartosc 200 to zaslepka symulujaca ze pamiec o danej ilosci ramek zostala zaalokowana, ustawic 0 zeby nie moglo utworzyc procesu ze wzgledu na brak pamieci
    {
      child->setMemoryFlagStatus(1);
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory is allocated");
    }
    else
    {
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::WARNING,"Process creation failed because no memory allocated");
      kill(child);
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::WARNING,"Process is killed because creation failed");
      return;      
    }
    //_kernel->getProcessorModule()->addProcess(child); //skomentowanie to zaslepka spowodowana brakiem funkcji z innego modulu. Przyjmuje sie ze proces zostal dodany do schedulera.
    //linedokasacji//child->setProcessIsInScheduler(1); //zasymulowanie dodania procesu do schedulera
    //linedokasacji//_kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"ProcessIsInScheduler Flag is setted on 1"); //pokazanie ze flaga obecnosci w schedulerze jest ustawiona na 1
    if (child->getProcessIsInScheduler() == 1)
    {
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process added to scheduler");
    _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::CONFIG,"PROCESS GET STATUS: WAITING");
    //linedokasacji//child->setProcessorFlagStatus(1); //zasymulowanie przydzielenie procesora
    }
    if(child->getProcessorFlagStatus() == 1)
    {
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Processor flag is setted on 1"); //Pokazanie ze flaga procesora ustawiona jest na 1
      if (child->getIsActuallyRunning() == 1)
      {
        _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::CONFIG,"PROCESS GET STATUS: RUNNING");
      }
    }
    if (Parent->getIsActuallyRunning()==1 && child->getIsActuallyRunning()==1) //jesli parent jak i potomek flagi running ustawione na 1 to wywoluje wait
    {
      sop::processes::Module::wait(Parent, child); //wywolanie wait
      if (child->getEndingFlagStatus()==1 || child->getIsKilled()==1) //jesli potomek zostal wykonany standardowo albo zostal zabity ponownie alokuje pamiec dla parenta i wrzuca go do schedulera
      {
        Parent->setArrayPages(/*allocate(Parent->getArrayPages(), Parent->getPID())*/70); //zasymulwanie zaalokowania pamieci po raz kolejny do procesu ktory sie wybudzil
        if (/*Parent->getArrayPages().getPageTableSize()*/200 > 0) //wartosc 200 to zaslepka symulujaca ze pamiec o danej ilosci ramek zostala zaalokowana.
          Parent->setMemoryFlagStatus(1);
          _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory is allocated again to Parent process");
        }
        //_kernel->getProcessorModule()->addproces(Parent); // skomentowanie to zaslepka i przyjmujemy ze parent zostanie dodany do schedulera
        _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Parent process is added to scheduler again");
      }
    }
  }  
//funkcja wpisujaca kod programu do wykonania
void sop::processes::Module::exec(std::string filename, boost::shared_ptr<sop::process::Process> Proces)
{
  //sop::files::File* filepointer = sop::files::Filesystem::openFile(Proces, getPathFromParam(filename), "x");
  //std::string data = _kernel->getFilesModule()->readFile(filepointer);
  //_kernel->getFilesModule()->closeFile(filepointer);
  //delete filepointer;
  if (/*Proces->getArrayPages().getPageTableSize()*Physical_drive.getFrameSize()*/500 < /*data.size()*/ 300) //porownuje czy rozmiar pliku jest mniejszy od wielkosci zalokowanej pamieci (ilosc ramek * rozmiar rameki) //zasymulowanie, ze wielkosc przypisanej pamieci jest mniejsza od ilosci miejsca jakiego potrzebuja dane
  {
    //_kernel->getMemoryModule()->deallocate(Proces->getArrayPages()); //dealokacja juz przypisanej pamieci
    //_kernel->getMemoryModule()->allocate(data.size(), Proces->getPID()); //alokacja ponownie pamieci
    Proces->setArrayPages( /*_kernel->getMemoryModule()->allocate(data.size(), Proces->getPID())*/ 4); //przypisanie do pola gdzie zaalokowana jest pamiec, normlanie powinien byc to wskaznik na strukture
    if (/*Proces->getArrayPages().getPageTableSize()*/ 3 > 0) //jesli zostala zaalokowana pamiec (3 ramki tutaj) ustaw flage na 1 jesli nie to na 0
    {
      Proces->setMemoryFlagStatus(1);
      _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory is allocated");
     // _kernel->getMemoryModule()->write(Proces->getArrayPages(), data); //zapisanie kodu z pliku do pamieci
      Proces->setIsTrueProcess(1); //proces dostal kod wiec ustawiamy flage ze jest prawdziwy
    }
    else
    {
      Proces->setMemoryFlagStatus(0);
    }
  }
  else
  {
    //linedokasacji//Proces->setArrayPages(40); //tylko tymczasowo na czas testow w miejscu 40 powinien byc wskaznik na strukture
    //_kernel->getMemoryModule()->write(Proces->getArrayPages(), data); //zapisanie kodu z pliku do pamieci
    Proces->setIsTrueProcess(1); //proces dostal kod wiec ustwiamy flage ze jest prawdziwy
  }  
}
//funkcja wstrzymujaca proces macierzysty
void sop::processes::Module::wait(boost::shared_ptr<sop::process::Process> Parent, boost::shared_ptr<sop::process::Process> Child)
{
  Parent->setIsActuallyRunning(0);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Because of wait() process is stopped");
  Parent->setProcessIsInScheduler(0);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Because of wait() process is deleted from scheduler");
  //_kernel->getProcessorModule()->removeProcess(Parent, Parent->getPID()); //skomentowanie to zaslepka sugerujaca ze proces zostal usuniety
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process is deleted from Scheduler");
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::CONFIG,"PROCESS GET STATUS: WAITING ON CHILD");
}
//funkcja zabijajaca proces o podanym PID
void sop::processes::Module::kill(boost::shared_ptr<sop::process::Process> Proces)
{
  boost::shared_ptr<sop::process::Process> p = Proces;
  //_kernel->getMemoryModule()->deallocate(p->getArrayPages()); //skomentowanie to zaslepka przyjmuje ze pamiec zostala zdealokowana
  p->setMemoryFlagStatus(0);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory is deallocated");
  //_kernel->getProcessorModule()->removeProces(); // skomentowanie to zaslepka przyjmuje ze proces zostal usuniety ze schedulera
  p->setProcessorFlagStatus(0);
  p->setEndingFlagStatus(1);
  p->setIsKilled(1);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process is deleted from scheduler");
  sop::processes::Module::removeFromVector(p->getPID());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process is deleted from vector of processes");
}
//funkcja zamykajaca proces
void sop::processes::Module::exit(boost::shared_ptr<sop::process::Process> Proces, uint8_t ExitCode)
{
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::CONFIG,"PROCESS GET STATUS EXECUTED");
  boost::shared_ptr<sop::process::Process> p = Proces;
  //_kernel->getMemoryModule()->deallocate(p->getArrayPages()); // skomentowanie to zaslepka przyjmuje ze pamiec zostala zdealokowana
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Memory is deallocated");
  Proces->setEndingFlagStatus(1);
  sop::processes::Module::removeFromVector(p->getPID());
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process is deleted from vector of processes");
  Proces->setExitCode(ExitCode);
  std::string ExitCodestr = sop::StringConverter::convertToString<uint16_t>(ExitCode);
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Process has exit with:" + ExitCodestr + "ExitCode");
}
//funkcja wypelniajaca kolejke pidow
void sop::processes::Module::fillQueue()
 {
   for (uint16_t i = 1; i<=100; i++)
   {
     sop::processes::Module::PIDlist.push(i);
   }
   _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"PID's queue is filled");
 }
//funkcja pobierajaca PID z listy
uint16_t sop::processes::Module::getPIDfromList()
{
  uint16_t PIDnumber = PIDlist.front();
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Downloaded new PID from queue");
  PIDlist.pop();
  _kernel->getLogger()->logProcesses(sop::logger::Logger::Level::INFO,"Used PID is deleted from queue");
  return PIDnumber;
}
