#ifndef GH_COMMANDS_H
#define GH_COMMANDS_H

/**
 * A utility class which reads input from the serial monitor and interprets
 * the input as commands. Commands are registered with the class.
 */
class CommandManager
{
public:

  /**
   * A named function that can be executed by typing its name into the serial
   * monitor.
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
