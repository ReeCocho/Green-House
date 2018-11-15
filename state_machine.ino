/* Includes. */
#include "state_machine.h"

StateMachine::StateMachine() : 
  m_length(0),
  m_active_node(0),
  m_last_active_node(0)
{
  // Allocate only 1 byte so we can realloc it when adding a new node
  m_nodes = (Node*)malloc(1);
}

StateMachine::~StateMachine()
{
  // Free the node list
  free(m_nodes);
}

void StateMachine::execute(const unsigned long dt)
{
  // Call the function accosiated with the active node
  m_nodes[m_active_node].func(dt, *this);
}

void StateMachine::set_active_node(const size_t n)
{
  // Set the last active node to the active node
  m_last_active_node = m_active_node;

  // And update the active node
  m_active_node = n;
}

void StateMachine::add_node(const Node& n)
{
  // Resize the node list to fit m_length + 1 nodes
  m_nodes = (Node*)realloc(m_nodes, sizeof(Node) * ++m_length);

  // And add the new node
  m_nodes[m_length - 1] = n;
}

size_t StateMachine::get_active_node() const
{
  return m_active_node;
}

size_t StateMachine::get_last_active_node() const
{
  return m_last_active_node;
}
