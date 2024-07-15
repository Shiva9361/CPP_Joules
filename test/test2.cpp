#include <stdio.h>
#include <CPP_Joules/LangJoulesWrapper.h>
#include <unistd.h> // For sleep function

int main()
{
  printf("Sleeping for 2 seconds...\n");
  sleep(2); // Sleep for 2 seconds
  printf("Awake now!\n");
  return 0;
}