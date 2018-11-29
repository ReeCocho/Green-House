#ifndef GH_COMMANDS_H
#define GH_COMMANDS_H

/**
 * A utility class which reads input from the serial monitor and interprets
 * the input as commands. To use a command manager you first construct an
 * instance of the class as normal. Then, you fill 'Command' objects with a
 * string containing the name of the command, and a function pointer which
 * points to the function that will be executed when the command is run. You
 * submit these 'Command' objects to the 'CommandManager' by calling
 * 'add_command'. Then, whenever you want, you can call 'poll' which will
 * check if any commands were executed.
 */
class CommandManager
{
public:

  /**
   * Information about a command that the command manager can execute.
   */
  struct Command
  {
    /** Function to execute. */
    void(*func)();

    /** Name of the command. */
    const char* str;
  };

  /**
   * Constructor.
   */
  CommandManager();

  /**
   * Destructor.
   */
  ~CommandManager();

  /**
   * Checks to see if any command have been input. If they have, it 
   * executes them.
   */
  void poll();

  /**
   * Add a new command.
   * @param c The new command.
   */
  void add_command(const Command& c);

private:

  /** List of commands. */
  Command* m_commands;

  /** Number of commands. */
  size_t m_length;
};

#endif
