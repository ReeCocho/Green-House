#ifndef GH_STATE_MACHINE_H
#define GH_STATE_MACHINE_H

/* Includes. */
#include <stdlib.h>

/**
 * State machine utility. A state machine is a directed graph, where nodes
 * in the graph are functions and edges are how each node flows to another.
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
   * Run the state machine.
   * @param dt Time in milliseconds since last execution.
   */
  void execute(const unsigned long dt);

  /**
   * Change the active node.
   * @param n New active node.
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
