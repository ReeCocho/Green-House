/** Includes. */
#include <stdlib.h>
#include "commands.h"

/**
 * Read a string sent by the user over serial.
 * @param Forces the program to wait for input.
 */
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



CommandManager::CommandManager() : m_length(0)
{
  // Allocate 1 byte so we can use realloc
  m_commands = (Command*)malloc(1);
}

CommandManager::~CommandManager()
{
  free(m_commands);
}

void CommandManager::poll()
{
  // Read a string from the user
  char* str = read_user_input(false);

  // If it isn't NULL process it as a command and free it
  if(str != NULL)
  {
    // Loop over every command and check if the user input that one
    for(size_t i = 0; i < m_length; ++i)
      if(strcmp(str, m_commands[i].str) == 0)
      {
        m_commands[i].func();
        free(str);
        return;
      }

    // Unkown command
    Serial.print("Unkown command \"");
    Serial.print(str);
    Serial.print("\"\n");    

    free(str);
  }
}

void CommandManager::add_command(const Command& c)
{
  m_commands = (Command*)realloc(m_commands, sizeof(Command) * ++m_length);
  m_commands[m_length - 1] = c;
}
