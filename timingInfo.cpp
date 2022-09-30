
#include <iostream>
#include <iomanip>
#include <string>

#include "main.h"

class timingInfo
{
 public:

  // Name of the intenty for which timing is desired

  string name;

  // Two methods of recording time:
  
  double       startSeconds ,  endSeconds ;  // (1)
  std::clock_t startCPUtime ,  endCPUtime ;  // (2)


  timingInfo()
    {
      name = "timingInfo name not set";
    }
    
  timingInfo(string _name)
    {
      name = _name;
    }
    
  void Start(int myPE)
  {

    // Record start time using two methods
    
    startSeconds = MPI_Wtime();              // (1)
    startCPUtime = std::clock();             // (2)
    
    if ( myPE == 0)
      {
	cout << "[timingInfo] [" << name << "] Start time (Wall): " << startSeconds << " Start time (CPU) : " << startCPUtime << endl;
      }

  }

  void Finish(int myPE)
  {

    // Record finish time using two methods
    
    endSeconds = MPI_Wtime();               // (1)
    endCPUtime = std::clock();              // (2)

    // Compute elapsed time
    
    double elapsedSeconds = endSeconds - startSeconds;  // (1)
    double elapsedCPUtime = endCPUtime - startCPUtime;  // (2)

    // Report results
    
    if ( myPE == 0)
      {

	// (1) results
	
	cout << "[timingInfo] [" << name << "] Finish time (Wall): " << endSeconds <<
	                                     " Finish time (CPU) : " << endCPUtime << endl;

	// (2) results
	
	cout << "[timingInfo] [" << name << "] Elapsed time (Wall, seconds): "  << elapsedSeconds <<
	                                     " Elapsed time (CPU, seconds) : "  << elapsedCPUtime/ (double) CLOCKS_PER_SEC << endl;
      }
    
    return ;
    
  }

};


// ====================================================================================
// ||                                                                                ||
// ||                      T i m i n g   U t i l i i t e s                           ||
// ||                                                                                ||
// ====================================================================================

void StartTimer(struct timespec &t0  )
{
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t0);
}

void EndTimer( string KernelName , struct timespec &t0 , struct timespec &t1 )
{
  // The timespec struct provides time information in this format:
  //
  // tv_sec:tv_nsec  
  //
  // which should be thought of as an anology to the more commonly understood
  // time format
  //
  //  HH:MM:SS
  //
  // So in order to compute elapsed time, one must subtract each place (e.g., HH, MM, SS),
  // and add the differences while including units conversion

  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1);

  int64_t sec_place     = t1.tv_sec  - t0.tv_sec ;
  int64_t ns_place      = t1.tv_nsec - t0.tv_nsec;
  int64_t sec_2_ns      = 1000000000;
  int64_t exeTime_in_ns = sec_place*sec_2_ns + ns_place;

  cout << "[ " << KernelName << " ] EXECUTION TIME  = " << exeTime_in_ns << " (ns)" << endl;
}