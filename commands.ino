/** INCLUDES **/
#include <EEPROM.h>
#include "state_machine.h"

// A command that can be executed
struct Command
{
  // Function to execute
  void(*func)();

  // Name of the function
  const char* str;
};

// List of commands
static Command commands[] =
{
  {
    &force_stop_pump,
    "force_stop_pump"
  },
  {
    &force_start_pump,
    "force_start_pump"
  },
  {
    &resume_pump,
    "resume_pump"
  }
};

// Read a string sent by the user
// @param Forces the program to wait for input
static char* read_user_input(const bool wait)
{
  char* str = NULL;
  
  do
  {
    // Check if we are getting an input
    if(Serial.available() > 0)
    {
      // Store the number of bytes available
      auto len = Serial.available();
      
      while(true)
      {
        // Wait ten miliseconds to allow more bytes to arrive
        delay(10);
  
        // Get the new number of bytes
        auto new_len = Serial.available();
  
        // If we didn't get any more bytes, it's safe to say we've
        // reached the end of the signal.
        if(new_len == len) break; 
  
        // Update length
        len = new_len;
      }
  
      // Allocate some memory to store the string
      str = (char*)malloc(len + 1);
  
      // Read every character
      for(int i = 0; i < len; ++i)
        str[i] = Serial.read();
  
      // Add the null terminator
      str[len] = '\0';
  
      return str;
    }
  } while(wait);

  return str;
}

// Process the users input as a command
// @param Command string to process
static void process_command(const char* cmd)
{
  for(size_t i = 0; i < sizeof(commands)/sizeof(Command); ++i)
    if(strcmp(cmd, commands[i].str) == 0)
    {
      commands[i].func();
      return;
    }

  // Unkown command
  Serial.print("Unkown command \"");
  Serial.print(cmd);
  Serial.print("\"\n");
}

void process_input()
{
  // Read a string from the user
  char* str = read_user_input(false);

  // If it isn't NULL process it as a command and free it
  if(str != NULL)
  {
    process_command(str);
    free(str);
  }
}
