#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

long int memory_total = -1, memory_free = -1;


static void get_mem( void ) 
{

  char buffer[ 1024 ];
  const char *delimeter=":";
  char *string, *string1, *ptr;
  long int memory_total, memory_free, memory_avail;
  long int swap_free, swap_total;
  
  
  FILE *fp;

  if ( ( fp = fopen( "/proc/meminfo", "r" ) ) == NULL) 
    {

      perror("fopen()");

      exit( EXIT_FAILURE );

    } 

  while ( fgets( buffer, 1024, fp ) != NULL )
    {

      if ( strstr ( buffer, "MemTotal:" ) )
	{

	  string = strtok ( buffer, delimeter );

	  while ( string != NULL )
	    {

	      if ( ( string1 = strstr(string, "kB") ) != NULL )
		{

		  strncpy ( string1, "", 2 );
	       
		  memory_total = strtol( string, &ptr, 10);
		 
		  break;

		}
	       
	      string = strtok ( NULL, delimeter );

	    }

	  continue;

	}
      
      
      if ( strstr ( buffer, "MemFree:" ) )
      	{
	  
      	  string = strtok ( buffer, delimeter );
	  
      	  while ( string != NULL )
      	    {

      	      if ( ( string1 = strstr( string, "kB" ) ) != NULL )
      		{

      		  strncpy ( string1, "", 2 );
	       
      		  memory_free = strtol( string, &ptr, 10);
		 
      		  break;

      		}
	       
      	      string = strtok ( NULL, delimeter );

      	    }

	  continue;

      	}


      if ( strstr ( buffer, "MemAvailable:" ) )
      	{
	  
      	  string = strtok ( buffer, delimeter );
	  
      	  while ( string != NULL )
      	    {

      	      if ( strstr(string, "kB") != NULL )
      		{

      		  string1 = strstr( string, "kB" );
      		  strncpy ( string1, "", 2 );
	       
      		  memory_avail = strtol( string, &ptr, 10);
		 
      		  break;

      		}
	       
      	      string = strtok ( NULL, delimeter );

      	    }

	  continue;

      	}


      if ( strstr ( buffer, "SwapTotal:" ) )
      	{
	  
      	  string = strtok ( buffer, delimeter );
	  
      	  while ( string != NULL )
      	    {

      	      if ( strstr(string, "kB") != NULL )
      		{

      		  string1 = strstr( string, "kB" );
      		  strncpy ( string1, "", 2 );
	       
      		  swap_total = strtol( string, &ptr, 10);
		 
      		  break;

      		}
	       
      	      string = strtok ( NULL, delimeter );

      	    }

	  continue;

      	}



      if ( strstr ( buffer, "SwapFree:" ) )
      	{
	  
      	  string = strtok ( buffer, delimeter );
	  
      	  while ( string != NULL )
      	    {

      	      if ( strstr(string, "kB") != NULL )
      		{

      		  string1 = strstr( string, "kB" );
      		  strncpy ( string1, "", 2 );
	       
      		  swap_free = strtol( string, &ptr, 10);
		 
      		  break;

      		}
	       
      	      string = strtok ( NULL, delimeter );

      	    }

	  continue;

      	}

      
    }

  printf( "Memory\n" );
  printf( "------\n" );
  printf( "Memory total: %ld\n", memory_total );
  printf( "Memory free: %ld\n", memory_free);
  printf( "Memory available: %ld\n", memory_avail);
  printf( "Swap total: %ld\n", swap_total);
  printf( "Swap free: %ld\n", swap_free);
  printf( "\n\n" );
  
  
  fclose( fp );

  return;

}




static void get_load_avg( void ) 
{

  char buffer[ 1024 ];
  const char *delimeter=" ";
  char *string, *ptr;

  double loadavg[ 3 ];

  FILE *fp;

  if ( ( fp = fopen( "/proc/loadavg", "r" ) ) == NULL) 
    {

      perror("fopen()");

      exit( EXIT_FAILURE );

    } 

  fgets( buffer, 1024, fp );

  string = strtok ( buffer, delimeter );
  
  for ( int i = 0; i <= 2; i++ )
    {

      loadavg[ i ] = strtod( string, &ptr );

      string = strtok ( NULL, delimeter );

    }

  fclose( fp );

  return;

}


void get_cpuinfo()
{

    char buffer[ 1024 ];
    const char *delimeter = ":", *delimeter_flags = " ";
    char *string, *ptr;

    long int num_cores = 0, l3_cache_size = 0;
    float *array_core_frequency = NULL;
  
  
    FILE *fp;

    if ( ( fp = fopen( "/proc/cpuinfo", "r" ) ) == NULL) 
    {

	perror("fopen()");

	exit( EXIT_FAILURE );

    } 


    while( fgets( buffer, 1024, fp ) ) 
    {

	if ( strstr ( buffer, "model name" ) )
      	{

	    string = strtok ( buffer, delimeter );
	    string = strtok ( NULL, delimeter );
	    printf( "Modellname: %s\n", string );
	  
      	}


	if ( strstr ( buffer, "cache size" ) )
      	{

	    string = strtok ( buffer, delimeter );
	    string = strtok ( NULL, delimeter );
	    l3_cache_size = strtol( string, &ptr, 10);
	    printf( "L3 Cache Size: %ld kB\n", l3_cache_size );
	  
      	}


	if ( strstr ( buffer, "cpu cores" ) )
      	{

	    string = strtok ( buffer, delimeter );
	    string = strtok ( NULL, delimeter );
	    num_cores = strtol( string, &ptr, 10);

	    array_core_frequency = (float *) malloc( sizeof( float ) * num_cores );
	  
	    printf( "Number of CPU cores: %ld\n", num_cores );

	}

      
    }
  
    rewind( fp );


    /* while( fgets( buffer, 1024, fp ) )  */
    /* { */
//	for ( int i = 0; i < num_cores; i++ )
//	{
    int i = 0;
    
    while( fgets( buffer, 1024, fp ) ) 
	    {	  
		if ( strstr ( buffer, "cpu MHz" ) )
		{

		    string = strtok ( buffer, delimeter );
		    string = strtok ( NULL, delimeter );

		    array_core_frequency[ i ] = strtof( string, &ptr);
		    i++;
		    
		    printf( "CPU clock rate CPU %f: \n", array_core_frequency[ i ] );
		  
		}
	  
//	    }
  
	}
      
      
//    }

    fclose( fp );

    return;

}


int main ( int argc, char **argv ) 
{

  double intervall = 1.0;
  long int steps = 1;
 
  char *c_ptr;
  char sleep_intervall[ 100 ];

  if ( argc > 1 )
    {
      
      intervall = strtod( argv[1], &c_ptr );
      steps = strtol( argv[2], &c_ptr, 10 );
      
    } else{
    
    intervall = 1;
    steps = LONG_MAX;
    
  }

  sprintf( sleep_intervall, "sleep %f", intervall );

  for ( int i = 0; i < steps ; i++ )
    {

      get_mem(); 
//      get_load_avg();
      get_cpuinfo();

      system( sleep_intervall );
 
    }

  return EXIT_SUCCESS;

}
