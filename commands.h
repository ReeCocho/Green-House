#ifndef GH_COMMANDS_H
#define GH_COMMANDS_H

/**
 * Command manager.
 */
class CommandManager
{
public:

  /**
   * A command that can be executed.
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
   * Poll for commands.
   */
  void poll();

  /**
   * Add a command.
   * @param New command.
   */
  void add_command(const Command& c);

private:

  /** List of commands. */
  Command* m_commands;

  /** Number of commands. */
  size_t m_length;
};

#endif
