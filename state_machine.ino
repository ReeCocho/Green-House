/** Includes. */
#include "state_machine.h"

StateMachine::StateMachine() : 
  m_length(0),
  m_active_node(0),
  m_last_active_node(0)
{
  // Allocate only 1 byte so re can call realloc
  m_nodes = (Node*)malloc(1);
}

StateMachine::~StateMachine()
{
  free(m_nodes);
}

void StateMachine::execute(const unsigned long dt)
{
  m_nodes[m_active_node].func(dt, *this);
}

void StateMachine::set_active_node(const size_t n)
{
  m_last_active_node = m_active_node;
  m_active_node = n;
}

void StateMachine::add_node(const Node& n)
{
  m_nodes = (Node*)realloc(m_nodes, sizeof(Node) * ++m_length);
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
