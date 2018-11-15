/* Includes. */
#include <stdlib.h>
#include "commands.h"

/**
 * Read a string sent by the user over the serial monitor.
 * @param wait Forces the program to wait for input.
 */
static char* read_user_input(const bool wait)
{
  // String which holds the users input
  char* str = NULL;

  /* We use a do while loop here so we can gurantee that it atleast loops once.
   * If we didn't request the function to wait for a command, it will return NULL
   * if no input was provided. Otherwise, it will loop until we input something.
   */
  do
  {
    // Check if we are getting an input
    if(Serial.available() > 0)
    {
      // Store the number of bytes available
      auto len = Serial.available();

      // Loop until the number of bytes available doesn't change
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
    }
  } while(wait);

  return str;
}



CommandManager::CommandManager() : m_length(0)
{
  // Allocate 1 byte so we can realloc it when we add a command
  m_commands = (Command*)malloc(1);
}

CommandManager::~CommandManager()
{
  // Free the command list
  free(m_commands);
}

void CommandManager::poll()
{
  // Read a string from the user
  char* str = read_user_input(false);

  // If it isn't NULL...
  if(str != NULL)
  {
    // Loop over every command and check if the user input that one
    for(size_t i = 0; i < m_length; ++i)
      // If they did...
      if(strcmp(str, m_commands[i].str) == 0)
      {
        // Call the commands accosiated function
        m_commands[i].func();

        // And free the string
        free(str);

        // And return from the function
        return;
      }

    // If we looped over everything and didn't find a command, it must be unkown
    Serial.print("Unkown command \"");
    Serial.print(str);
    Serial.print("\"\n");    
    free(str);
  }
}

void CommandManager::add_command(const Command& c)
{
  // Resize the command list so it can fit m_length + 1 commands
  m_commands = (Command*)realloc(m_commands, sizeof(Command) * ++m_length);

  // And then add the command
  m_commands[m_length - 1] = c;
}
