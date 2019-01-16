#ifndef GH_CPP_MEM_H
#define GH_CPP_MEM_H

// new
extern void* operator new (size_t size);

// placement new
extern void* operator new (size_t size, void* ptr);

// delete
extern void operator delete (void* ptr);

#endif
