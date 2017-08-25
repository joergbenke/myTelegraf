#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>


struct process{
  unsigned int pid;
  unsigned int voluntary_ctxt_switches;
  unsigned int nonvoluntary_ctxt_switches;
  unsigned int threads;
  char *state;
  unsigned short int cpulast;
  //
  unsigned long int utime;
  unsigned long int stime;
};

struct cpu{
  char modelname[100];
  short int num_cores;
  float core_frequency[4];
  unsigned long int cachesize;
};

struct memory{
  unsigned long int memory_total;
  unsigned long int memory_avail;
  unsigned long int memory_free;
  //
  unsigned int swap_total;
  unsigned int swap_free;
  //
  unsigned long int minflt;
  unsigned long int majflt;
  //
  unsigned int vmpeak;
  unsigned int vmsize;
  unsigned int vmhwm;
  unsigned int vmrss;
};

struct system{
  float load_avg[ 3 ];
};

struct io{
  unsigned int read_bytes;
  unsigned int write_bytes;
};


struct cpu cpu_instance;
struct memory memory_instance; 
struct process process_instance;
struct system system_instance;
struct io io_instance;


void get_proc_cpuinfo()
{

  char buffer[ 1024 ];
  const char *delimeter = ":", *delimeter_flags = " ";
  char *string, *ptr;

  
  FILE *fp;

  if ( ( fp = fopen( "/proc/cpuinfo", "r" ) ) == NULL) 
    {

      perror( "Could not find /proc/cpuinfo!" );

      exit( EXIT_FAILURE );

    } 


  while( fgets( buffer, 1024, fp ) ) 
    {

      if ( strstr ( buffer, "model name" ) )
      	{

	  string = strtok ( buffer, delimeter );
	  string = strtok ( NULL, delimeter );
	  sprintf( cpu_instance.modelname, "%s", string ); 
      	}


      if ( strstr ( buffer, "cache size" ) )
      	{

	  string = strtok ( buffer, delimeter );
	  string = strtok ( NULL, delimeter );
	  cpu_instance.cachesize = strtol( string, &ptr, 10);
	  
      	}


      if ( strstr ( buffer, "cpu cores" ) )
      	{

	  string = strtok ( buffer, delimeter );
	  string = strtok ( NULL, delimeter );
	  cpu_instance.num_cores = strtol( string, &ptr, 10);
	  
	}

    }

  printf( "\nCPU (static data)\n" );
  printf( "--------------------\n" );
  printf( "Modellname: %s", cpu_instance.modelname );
  printf( "L3 Cache Size: %ld kB\n", cpu_instance.cachesize );
  printf( "Number of CPU cores: %ld\n", cpu_instance.num_cores );
  printf( "\n" );
    
  return;

}



static void get_proc_meminfo( void ) 
{

  char buffer[ 1024 ];
  const char *delimeter=":";
  char *string, *string1, *ptr;
  
  FILE *fp;

  if ( ( fp = fopen( "/proc/meminfo", "r" ) ) == NULL) 
    {

      perror( "Could not find /proc/meminfo!" );

      exit( EXIT_FAILURE );

    } 

  while ( fgets( buffer, 1024, fp ) != NULL )
    {

      if ( strstr ( buffer, "MemTotal" ) )
	{

	  string = strtok ( buffer, delimeter );

	  while ( string != NULL )
	    {

	      if ( ( string1 = strstr( string, "kB" ) ) != NULL )
		{

		  strncpy ( string1, "", 2 );
		  memory_instance.memory_total = strtol( string, &ptr, 10);
		 
		  break;

		}
	       
	      string = strtok ( NULL, delimeter );

	    }

	  continue;

	}
      

      if ( strstr ( buffer, "SwapTotal" ) )
      	{
	  
	  string = strtok ( buffer, delimeter );
	  
	  while ( string != NULL )
      	    {

	      if ( strstr(string, "kB" ) != NULL )
      		{

		  string1 = strstr( string, "kB" );
		  strncpy ( string1, "", 2 );
		  memory_instance.swap_total = strtol( string, &ptr, 10);
		 
		  break;

      		}
	       
	      string = strtok ( NULL, delimeter );

      	    }

	  continue;

      	}

    }

  printf( "\nMemory (static data)\n" );
  printf( "--------------------\n" );
  printf( "Memory total: %ld\n", memory_instance.memory_total );
  printf( "Swap total: %ld\n", memory_instance.swap_total);
  printf( "\n" );

  return;

}

static void get_mem_proc_meminfo( void ) 
{

  char buffer[ 1024 ];
  const char *delimeter=":";
  char *string, *string1, *ptr;
  
  
  FILE *fp;

  if ( ( fp = fopen( "/proc/meminfo", "r" ) ) == NULL) 
    {

      perror("Could not find /proc/cpuinfo!");

      exit( EXIT_FAILURE );

    } 

  while ( fgets( buffer, 1024, fp ) != NULL )
    {

      if ( strstr ( buffer, "MemFree" ) )
      	{
	  
	  string = strtok ( buffer, delimeter );
	  
	  while ( string != NULL )
      	    {

	      if ( ( string1 = strstr( string, "kB" ) ) != NULL )
      		{

		  strncpy ( string1, "", 2 );
	       
		  memory_instance.memory_free = strtol( string, &ptr, 10);
		 
		  break;

      		}
	       
	      string = strtok ( NULL, delimeter );

      	    }

	  continue;

      	}


      if ( strstr ( buffer, "MemAvailable" ) )
      	{
	  
	  string = strtok ( buffer, delimeter );
	  
	  while ( string != NULL )
      	    {

	      if ( strstr(string, "kB") != NULL )
      		{

		  string1 = strstr( string, "kB" );
		  strncpy ( string1, "", 2 );
	       
		  memory_instance.memory_avail = strtol( string, &ptr, 10);
		 
		  break;

      		}
	       
	      string = strtok ( NULL, delimeter );

      	    }

	  continue;

      	}


      if ( strstr ( buffer, "SwapFree" ) )
      	{
	  
	  string = strtok ( buffer, delimeter );
	  
	  while ( string != NULL )
      	    {

	      if ( strstr(string, "kB") != NULL )
      		{

		  string1 = strstr( string, "kB" );
		  strncpy ( string1, "", 2 );
	       
		  memory_instance.swap_free = strtol( string, &ptr, 10);
		 
		  break;

      		}
	       
	      string = strtok ( NULL, delimeter );

      	    }

	  continue;

      	}

    }

  printf( "\n\nMemory (dynamic data)\n" );
  printf( "---------------------\n" );
  printf( "Memory free: %ld\n", memory_instance.memory_free);
  printf( "Memory available: %ld\n", memory_instance.memory_avail);
  printf( "Swap free: %ld\n", memory_instance.swap_free);
  printf( "\n" );
  
  
  fclose( fp );

  return;

}



static void get_proc_status( void ) 
{

  char buffer[ 1024 ];
  const char *delimeter=":";
  char *string, *string1, *ptr;
  
  sprintf( buffer, "/proc/%d/status", process_instance.pid );

  
  FILE *fp;

  if ( ( fp = fopen( buffer, "r" ) ) == NULL) 
    {

      perror("Could not find /proc/PID/status!");

      exit( EXIT_FAILURE );

    } 

  while ( fgets( buffer, 1024, fp ) != NULL )
    {

      if ( strstr ( buffer, "VmPeak" ) )
	{

	  string = strtok ( buffer, delimeter );

	  while ( string != NULL )
	    {

	      if ( ( string1 = strstr( string, "kB" ) ) != NULL )
		{

		  strncpy ( string1, "", 2 );
		  memory_instance.vmpeak = strtol( string, &ptr, 10);
		 
		  break;

		}
	       
	      string = strtok ( NULL, delimeter );

	    }

	  //	  continue;

	}


      if ( strstr ( buffer, "VmSize" ) )
	{

	  string = strtok ( buffer, delimeter );

	  while ( string != NULL )
	    {

	      if ( ( string1 = strstr( string, "kB" ) ) != NULL )
		{

		  strncpy ( string1, "", 2 );
		  memory_instance.vmsize = strtol( string, &ptr, 10);
		 
		  break;

		}
	       
	      string = strtok ( NULL, delimeter );

	    }

	}


      if ( strstr ( buffer, "VmHWM" ) )
	{

	  string = strtok ( buffer, delimeter );

	  while ( string != NULL )
	    {

	      if ( ( string1 = strstr( string, "kB" ) ) != NULL )
		{

		  strncpy ( string1, "", 2 );
		  memory_instance.vmhwm = strtol( string, &ptr, 10);
		 
		  break;

		}
	       
	      string = strtok ( NULL, delimeter );

	    }

	}


      if ( strstr ( buffer, "VmRSS" ) )
	{

	  string = strtok ( buffer, delimeter );

	  while ( string != NULL )
	    {

	      if ( ( string1 = strstr( string, "kB" ) ) != NULL )
		{

		  strncpy ( string1, "", 2 );
		  memory_instance.vmrss = strtol( string, &ptr, 10);
		 
		  break;

		}
	       
	      string = strtok ( NULL, delimeter );

	    }

	}


      if ( strstr ( buffer, "Threads" ) )
	{

	  string = strtok ( buffer, delimeter );
	  string = strtok ( NULL, delimeter );

	  process_instance.threads = strtol( string, &ptr, 10);

	}
      

      if ( ( strstr ( buffer, "voluntary_ctxt_switches" ) ) && !( strstr ( buffer, "nonvoluntary_ctxt_switches" ) ) )
	{
	  
	  string = strtok ( buffer, delimeter );
	  string = strtok ( NULL, delimeter );

	  process_instance.voluntary_ctxt_switches = (unsigned int ) strtol( string, &ptr, 10);
	  
	  continue;
	  
	}
      

      if ( strstr ( buffer, "nonvoluntary_ctxt_switches" ) )
      	{
	  
      	  string = strtok ( buffer, delimeter );
      	  string = strtok ( NULL, delimeter );
	  
      	  process_instance.nonvoluntary_ctxt_switches = strtol( string, &ptr, 10);
	  
      	}
      
    }


  fclose( fp );

  printf( "\n\nMemory (proc-status)\n" );
  printf( "---------------------\n" );
  printf( "VmPeak: %ld\n", memory_instance.vmpeak);
  printf( "VmSize: %ld\n", memory_instance.vmsize);
  printf( "VmHWM: %ld\n", memory_instance.vmhwm);
  printf( "VmRSS: %ld\n", memory_instance.vmrss);
  printf( "Number of threads: %d\n", process_instance.threads );
  printf( "voluntary_ctxt_switches: %u\n", process_instance.voluntary_ctxt_switches );
  printf( "nonvoluntary_ctxt_switches: %u\n", process_instance.nonvoluntary_ctxt_switches );
  printf( "\n" );

  return;

}


static void get_proc_io( void ) 
{

  char buffer[ 1024 ];
  const char *delimeter=":";
  char *string, *string1, *ptr;
  
  sprintf( buffer, "/proc/%d/io", process_instance.pid );

  
  FILE *fp;

  if ( ( fp = fopen( buffer, "r" ) ) == NULL) 
    {

      perror("Could not find /proc/PID/io!");

      exit( EXIT_FAILURE );

    } 

  while ( fgets( buffer, 1024, fp ) != NULL )
    {

      if ( strstr ( buffer, "read_bytes" ) )
      	{

	  string = strtok ( buffer, delimeter );
	  string = strtok ( NULL, delimeter );
	  io_instance.read_bytes = strtol( string, &ptr, 10); 
      	}


      if ( ( strstr ( buffer, "write_bytes" ) ) && !( strstr ( buffer, "cancelled_write_bytes" ) ) )
      	{

	  string = strtok ( buffer, delimeter );
	  string = strtok ( NULL, delimeter );
	  io_instance.write_bytes = strtol( string, &ptr, 10); 
      	}

    }

  fclose ( fp );

  printf( "\nIO:\n" );
  printf( "=====\n" );
  printf( "read_bytes: %u\n", io_instance.read_bytes );
  printf( "write_bytes: %u\n", io_instance.write_bytes );
  printf( "\n\n" );


  return;

}


static void get_proc_stat( void ) 
{

  char buffer[ 2048 ];
  const char *delimeter=" ";
  char *string, *string1, *ptr;
  
  sprintf( buffer, "/proc/%d/stat", process_instance.pid );
  printf ("\nget_proc_stat\n" );
  
  FILE *fp;

  if ( ( fp = fopen( buffer, "r" ) ) == NULL) 
    {

      perror("Could not find /proc/PID/stat!");

      exit( EXIT_FAILURE );

    } 

  fgets( buffer, 1024, fp );

  string = strtok ( buffer, delimeter );
  printf( "String in stat: %s\n", string);
  
  for ( int i = 1; i < 42; i++ )
    {

      string = strtok ( NULL, delimeter );

      switch ( i ){ 
      case 2: 
	process_instance.state = string;
	break;
      case 9: 
	memory_instance.minflt = strtol( string, &ptr, 10 );
	break;
      case 11:
	memory_instance.majflt = strtol( string, &ptr, 10 );
	break;
      case 13:
	process_instance.utime = strtol( string, &ptr, 10 );
	break;
      case 14:
	process_instance.stime = strtol( string, &ptr, 10 );
	break;
      case 38:
	process_instance.cpulast = strtol( string, &ptr, 10 );
	break;
      default:
	break;
      }

    }

  fclose( fp );

   printf( "\nStat file \n" ); 
   printf( "--------------------\n" ); 
   printf( "State: %s\n", process_instance.state );
   printf( "Minflt: %u\n", memory_instance.minflt );
   printf( "Majflt: %u\n", memory_instance.majflt );
   printf( "utime: %u\n", process_instance.utime );
   printf( "stime: %u\n", process_instance.stime );
   printf( "last cpu: %u\n", process_instance.cpulast );

  return;

}



static void get_load_avg( void ) 
{

  char buffer[ 1024 ];
  const char *delimeter=" ";
  char *string, *ptr;

  float loadavg[ 3 ];

  FILE *fp;

  if ( ( fp = fopen( "/proc/loadavg", "r" ) ) == NULL) 
    {

      perror("Could not find /proc/cpuinfo!");

      exit( EXIT_FAILURE );

    } 

  fgets( buffer, 1024, fp );

  string = strtok ( buffer, delimeter );
  loadavg[ 0 ] = strtod( string, &ptr );
  
  for ( int i = 1; i <= 2; i++ )
    {

      string = strtok ( NULL, delimeter );
      loadavg[ i ] = strtod( string, &ptr );

    }

  fclose( fp );

  for ( int i = 0; i <= 2; i++ )
    system_instance.load_avg[ i ] = loadavg[ i ];


  printf( "\nSystem (load/dynamic data)\n" );
  printf( "--------------------\n" );
  for ( int i = 0; i <= 2; i++ )
    printf( "%f \n", system_instance.load_avg[ i ] ); 

  return;

}



void get_cpu_proc_cpuinfo()
{

  const char *delimeter = ":", *delimeter_flags = " ";

  char buffer[ 1024 ];
  char *string, *ptr;

  unsigned int i = 0;

  float *array_core_frequency = NULL;
  array_core_frequency = (float *) malloc( sizeof( float ) * cpu_instance.num_cores );
  
  
  FILE *fp;

  if ( ( fp = fopen( "/proc/cpuinfo", "r" ) ) == NULL) 
    {

      perror("Could not find /proc/cpuinfo!");

      exit( EXIT_FAILURE );

    } 


  i = 0;
  while( fgets( buffer, 1024, fp ) ) 
    {

      if ( strstr ( buffer, "cpu MHz" ) )
	{

	  string = strtok ( buffer, delimeter );
	  string = strtok ( NULL, delimeter );

	  cpu_instance.core_frequency[ i ] = strtof( string, &ptr);
	  i++;
		    
		  
	}
	  
    }


  fclose( fp );

    
  printf( "\nCPU (dynamic data)\n" );
  printf( "--------------------\n" );

  for ( int i = 0; i < cpu_instance.num_cores; i++ )
    printf( "CPU clock rate CPU: %f \n", cpu_instance.core_frequency[ i ] );

  printf( "\n" );

  return;

}


void push_data_to_db()
{

  char buffer[1024];

  sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s memory_total=%u,memory_free=%u,memory_avail=%u,swap_total=%u,swap_free=%u,min_flt=%u,maj_flt=%u,vmpeak=%u,vmsize=%u,vmhwm=%u,vmrss=%u,read_bytes=%u,write_bytes=%u'","memory","profit",memory_instance.memory_total,memory_instance.memory_free,memory_instance.memory_avail,memory_instance.swap_total, memory_instance.swap_free,memory_instance.minflt,memory_instance.majflt,memory_instance.vmpeak,memory_instance.vmsize,memory_instance.vmhwm, memory_instance.vmrss,io_instance.read_bytes,io_instance.write_bytes);
  system (buffer );


  sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s read_bytes=%u,write_bytes=%u'","io","profit",io_instance.read_bytes,io_instance.write_bytes);
  system (buffer );


  sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s load_average1=%u,load_average5=%u,load_average15=%u'","system","profit",system_instance.load_avg[0],system_instance.load_avg[1],system_instance.load_avg[2]);
  system (buffer );


  sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s cpufrequency_core0=%f,cpufrequency_core1=%f,cpufrequency_core2=%f,cpufrequency_core3=%f'","cpu","profit",cpu_instance.core_frequency[0], cpu_instance.core_frequency[1],cpu_instance.core_frequency[2],cpu_instance.core_frequency[3]);
  system (buffer );


  sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s pid=%u,state=%s,num_threads=%u,utime=%u,stime=%u,voluntary_ctxt_switches=%u,nonvoluntary_ctxt_switches=%u'","process","profit",process_instance.pid,process_instance.state,process_instance.threads,process_instance.utime,process_instance.stime,process_instance.voluntary_ctxt_switches,process_instance.nonvoluntary_ctxt_switches );
  system ( buffer );

  //
  // Memory

  /* sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s memory_total=%u'", "memory", "profit", memory_instance.memory_total ); */
  /* system ( buffer ); */

  /* sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s memory_free=%u'", "memory", "profit", memory_instance.memory_free ); */
  /* system ( buffer ); */

  /* sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s memory_avail=%u '", "memory", "profit", memory_instance.memory_avail ); */
  /* system ( buffer ); */


  /* sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s swap_total=%u '", "memory", "profit", memory_instance.swap_total ); */
  /* system ( buffer ); */

  /* sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s swap_free=%u '", "memory", "profit", memory_instance.swap_free ); */
  /* system ( buffer ); */


  /* sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s min_flt=%u '", "memory", "profit", memory_instance.minflt ); */
  /* system ( buffer ); */

  /* sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s maj_flt=%u '", "memory", "profit", memory_instance.majflt ); */
  /* system ( buffer ); */


  /* sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s vmpeak=%u '", "memory", "profit", memory_instance.vmpeak ); */
  /* system ( buffer ); */

  /* sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s vmsize=%u '", "memory", "profit", memory_instance.vmsize ); */
  /* system ( buffer ); */

  /* sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s vmhwm=%u '", "memory", "profit", memory_instance.vmhwm ); */
  /* system ( buffer ); */

  /* sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s vmrss=%u '", "memory", "profit", memory_instance.vmrss ); */
  /* system ( buffer ); */


  //
  // IO

  /* sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s read_bytes=%u 1434055562000000000'", "memory", "profit", io_instance.read_bytes ); */
  /* system ( buffer ); */

  /* sprintf( buffer, "curl -i -XPOST 'http://localhost:8086/write?db=myTelegraf' --data-binary '%s,location=%s write_bytes=%u 1434055562000000000'", "memory", "profit", io_instance.write_bytes ); */
  /* system ( buffer ); */


  return;

}





unsigned int get_pid( const char *prgname )
{

  char *c_ptr;
  char tmp[ 256 ];
  char buffer[1024];
  
  unsigned int pid;

  sprintf( tmp, "pgrep %s > /tmp/out.txt", prgname );
  system ( tmp );
  
  
  FILE *fp;
  
  if ( ( fp = fopen( "/tmp/out.txt", "r" ) ) == NULL) 
    {
      
      perror("Could not find /tmp/out.txt!");
      
      exit( EXIT_FAILURE );
      
    } 
  
  fgets( buffer, 1024, fp );
  
  pid = strtol( buffer, &c_ptr, 10);
  //  printf( "PID = %d\n", pid );

  return pid;
  
}

  
int main ( int argc, char **argv ) 
{
 
  double intervall = 1.0;
  long int steps = 1;
    
    
  char *c_ptr;
  char sleep_intervall[ 100 ];
  char *prgname;
  prgname = (char *) malloc( sizeof( char ) * 256 );
    
    
  printf( "agc: %d, argv 0 = %s; argv 1 = %s, argv 2 = %s\n\n", argc, argv[0], argv[1], argv[2] );
    
  switch ( argc )
    {

    case 2:
      intervall = strtod( argv[1], &c_ptr );
      break;

    case 3:
      intervall = strtod( argv[1], &c_ptr );
      steps = strtol( argv[2], &c_ptr, 10 );
      break;

    case 4:
      intervall = strtod( argv[1], &c_ptr );
      steps = strtol( argv[2], &c_ptr, 10 );
      prgname = strcpy(prgname, argv[3]);

      break;

    default:
      printf( "Falsche Anzahl an Parametern!\n" );
      break;
    }
  
  sprintf( sleep_intervall, "sleep %f", intervall );
    
  get_proc_cpuinfo();
  get_proc_meminfo(); 

  if ( argc == 2 )
    {

      for ( int i = 0; i < 1e6 ; i++ )
	{
	    
	  get_mem_proc_meminfo(); 
	  get_cpu_proc_cpuinfo();
	  get_load_avg();
	    
	  system( sleep_intervall );
	    
	}
    }


  if ( argc == 3 )
    {

      for ( int i = 0; i < steps ; i++ )
	{
	    
	  get_mem_proc_meminfo(); 
	  get_cpu_proc_cpuinfo();
	  get_load_avg();

	  system( sleep_intervall );
	    
	}

    }


  if ( argc > 3 )
    {

      process_instance.pid = get_pid( argv[ 3 ] );
      printf( "\nMisc\n" );
      printf( "------\n" );
      printf( "Programmname: %s\n", prgname);
      printf( "PID: %d\n", process_instance.pid ); 


      for ( int i = 0; i < steps ; i++ )
	{
	    
	  get_mem_proc_meminfo(); 
	  get_cpu_proc_cpuinfo();
	    
	  get_proc_status();
	  get_proc_stat();

	  get_load_avg();
	  get_proc_io();

	  push_data_to_db();

	  system( sleep_intervall );
	    
	}
	
    }
    
  return EXIT_SUCCESS;

}
