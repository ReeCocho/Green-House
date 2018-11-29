#ifndef GH_STATE_MACHINE_H
#define GH_STATE_MACHINE_H

/* Includes. */
#include <stdlib.h>

/**
 * State machine utility. A state machine is a directed graph, where nodes
 * in the graph are functions and edges are how each node flows to another.
 * To initialize the state machine, you first construct it like normal. Then,
 * you create instances of the 'Node' class. This class contains a function
 * pointer, which points to a function that takes in a 'const unsigned long',
 * which represents the time in milliseconds since the state machine was last
 * updated, and a reference to the state machine. These nodes are registered
 * with the state machine by calling 'add_node'. The 'index' of a node is it's 
 * index in the 'm_nodes' array. Indices start at 0, and increase by 1 after 
 * each call to 'add_node'. To use the system, you call 'execute' and pass in 
 * the time in milliseconds since 'execute' was last called.
 */
class StateMachine
{
public:

  /**
   * A state machine node.
   */
  struct Node
  {
    /** 
     * Function that the state machine will execute when running the node. 
     * @param Time in milliseconds since the last execution of the state machine.
     * @param State machine the node belongs to.
     */
    void(*func)(const unsigned long, StateMachine&);
  };

  /**
   * Constructor.
   */
  StateMachine();

  /**
   * Destructor.
   */
  ~StateMachine();

  /**
   * Run the state machine. This function will execute the active nodes function.
   * @param dt Time in milliseconds since last execution.
   */
  void execute(const unsigned long dt);

  /**
   * Change the active node.
   * @param n Index of the new active node.
   * @note This function doesn't do any bounds checking, so don't fuck it up.
   * 'n' should be less than 'm_length'.
   */
  void set_active_node(const size_t n);

  /**
   * Add a new node.
   * @param n Node to add.
   */
  void add_node(const Node& n);

  /**
   * Get the active node.
   * @return Active node.
   */
  size_t get_active_node() const;

  /**
   * Get the last active node.
   * @return Last active node.
   */
  size_t get_last_active_node() const;

private:

  /** State machine nodes. */
  Node* m_nodes;

  /** Number of nodes. */
  size_t m_length;

  /** Last active node. */
  size_t m_last_active_node;

  /** Active node. */
  size_t m_active_node;
};

#endif
