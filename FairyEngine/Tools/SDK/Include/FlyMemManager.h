//------------------------------------------------------------------------
// Name: FlyMemManager.h
// Desc: This file define a useful tool for programming. A memory
//       Manager to alloc and free memory and keep some tracks.
//       It can check the heap memory leaks! When you wan't to use
//       in your project, Include the "FlyMemMacros.h" in your each
//       cpp file.
// ----------------------------------------------------------------------
// CopyRight(C) FlySoft Corp. All right Observed.
//------------------------------------------------------------------------


#ifndef __FLY_MEM_MANAGER_H__
#define __FLY_MEM_MANAGER_H__


// Headers of this file
#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include <string.h>
#include <windows.h>
#include <time.h>


//define some macros and structures.
typedef enum _MEM_ALLOC_TYPE
{
    m_alloc_unknown      = 0,
    m_alloc_new          = 1,
    m_alloc_new_array    = 2,
    m_alloc_malloc       = 3,
    m_alloc_calloc       = 4,
    m_alloc_realloc      = 5,
    m_alloc_delete       = 6,
    m_alloc_delete_array = 7,
    m_alloc_free         = 8,
} MEM_ALLOC_TYPE;

// Alloc unit for manager.
typedef struct _MEM_ALLOC_UNIT
{
    MEM_ALLOC_TYPE  allocationType;
    void*           Address;
    size_t          Size;
    char            sourceFile[MAX_PATH];
    UINT            sourceLine;
} MEM_ALLOC_UNIT;


//------------------------------------------------------------------------
// Write the debug version for the memoryManager
//------------------------------------------------------------------------
class __declspec(dllexport) FlyMemManager
{
protected:
    char m_szSourceFile[MAX_PATH];
    UINT m_nSourceLine;

    MEM_ALLOC_UNIT* m_pMemUnits;
    UINT m_nNumMemUnits;
    UINT m_nNumMaxMemUnits;

    // Add and remove the allocation track.
    void add_track( const char* file,UINT line,MEM_ALLOC_TYPE memType,void* address,size_t size );

    void remove_track( void* address );
    MEM_ALLOC_UNIT getAllocUnit( int index ) { return m_pMemUnits[index]; }
    void resetTracks(void);
    UINT getNumUnits(void) { return m_nNumMemUnits; }
    UINT getMaxNumUnits(void) { return m_nNumMemUnits; }

    const char* getAllocType( MEM_ALLOC_TYPE allocType );

public:
    /* Constructor and destructor */
    FlyMemManager();
    ~FlyMemManager();

    // Can be called directly by the overrided new,delete operator!
    void* op_new_sc( size_t Size );
    void* op_new_vc( size_t Size );
    void op_del_sc( void* Address );
    void op_del_vc( void* Address );

    // Use this function for the global macros
    // new and delete.
    void setOwner( const char* file,UINT line );

    /*
    ----------------------------------------------------------------------
        @This function is to used when allocate memory from heap.
        @remarks
            This function is the actual memory allocator and act as a
            bridge between FlyEngine and the C/C++ alloc/calloc funcs.
        @Param -> IN:
            While memory allocation requests are made through this func,
            the tracking of memory address is possible. Therefore, 
            attempting to deallocate a memory unit that was
            not allocated using this function will result in a warning
            given by the deAllocator or dllocMem.
    ----------------------------------------------------------------------
    */
    void* allocMem( const char* file,UINT line,const MEM_ALLOC_TYPE allocType,const size_t Size );

    /*
    ----------------------------------------------------------------------
        @This function is to used when realloc a memory unit.
        @remarks
            This function is the actual memory reallocator and acts as
            a bridge between FlyEngine and the C/C++ realloc funcs.
        @Param -> IN:
            While memory reallocation requests are made through this 
            function, the tracking of memory address is possible. 
            Therefore, attempting to deallocate a memory unit that are not
            reallocated by this function will result in a warning given
            by the deallocator or dllocMem.
    ----------------------------------------------------------------------
    */
    void* rllocMem( const char* file,UINT line,const MEM_ALLOC_TYPE allocType,const size_t Size,void* Address );

    /*
    ----------------------------------------------------------------------
        @This function is used when deallocate a memory unit.
        @remarks
            This function is the actual memory deallocator and acts as
            a bridge between FR Engine and the C/C++ free function.
        @Param
            While memory deallocation requests are made through this 
            function, The tracking of memory address is possible.
            Therefore, attempting to deallocate a memory unit that was
            not allocated using allocMem or rllocMem, trying to deallocate
            memory that was allocated with malloc or trying to deallocate
            from other process will result in a warning.
        @Param
            Tracking this kind of errors exists because users may want to
            write their own memory allocator later on or they'd like to
            get rid of this engine's memory allocator.
    ----------------------------------------------------------------------
    */
    void dllocMem( const char* file,UINT line,const MEM_ALLOC_TYPE deallocType,void* Address );

    // Unsed free RAM calculations
    unsigned int calcUnused( const MEM_ALLOC_UNIT* allocUnit );
    unsigned int calcAllUnused(void);

    // Write the report document.
    void dumpMemReport( const char* filename = "MemReport.log" );

    // Get the singleton object.
    static FlyMemManager& instance();
};

//////////////////////////////////////////////////////////////////////////

#endif  //#ifndef __FLY_MEM_MANAGER_H__