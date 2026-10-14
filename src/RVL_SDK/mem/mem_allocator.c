#include <revolution/mem.h>
#include <revolution/os.h>

static void* AllocatorAllocForExpHeap_(MEMAllocator* allocator, u32 size) {
    return MEMAllocFromExpHeapEx(allocator->heap, size, allocator->heapParam1);
}

static void AllocatorFreeForExpHeap_(MEMAllocator* allocator, void* block) {
    MEMFreeToExpHeap(allocator->heap, block);
}

static void* AllocatorAllocForFrmHeap_(MEMAllocator* allocator, u32 size) {
    return MEMAllocFromFrmHeapEx(allocator->heap, size, allocator->heapParam1);
}

static void AllocatorFreeForFrmHeap_(MEMAllocator* allocator, void* block) {
#pragma unused(allocator)
#pragma unused(block)
}

// The OS heap allocator below is not in mscharged-decomp's file; no reference gives
// the names of these three functions.
static void* fn_80260714(MEMAllocator* allocator, u32 size) {
    return OSAllocFromHeap((OSHeapHandle)allocator->heap, size);
}

static void fn_8026071C(MEMAllocator* allocator, void* block) {
    OSFreeToHeap((OSHeapHandle)allocator->heap, block);
}

void* MEMAllocFromAllocator(MEMAllocator* allocator, u32 size) {
    return allocator->func->allocFunc(allocator, size);
}

void MEMFreeToAllocator(MEMAllocator* allocator, void* block) {
    allocator->func->freeFunc(allocator, block);
}

void MEMInitAllocatorForExpHeap(MEMAllocator* allocator, MEMHeapHandle heap, s32 align) {
    static const MEMAllocatorFunc sAllocatorFunc = {AllocatorAllocForExpHeap_, AllocatorFreeForExpHeap_};
    allocator->func = &sAllocatorFunc;
    allocator->heap = heap;
    allocator->heapParam1 = align;
    allocator->heapParam2 = 0;
}

void MEMInitAllocatorForFrmHeap(MEMAllocator* allocator, MEMHeapHandle heap, s32 align) {
    static const MEMAllocatorFunc sAllocatorFunc = {AllocatorAllocForFrmHeap_, AllocatorFreeForFrmHeap_};
    allocator->func = &sAllocatorFunc;
    allocator->heap = heap;
    allocator->heapParam1 = align;
    allocator->heapParam2 = 0;
}

void fn_8026077C(MEMAllocator* allocator, OSHeapHandle heap) {
    static const MEMAllocatorFunc sAllocatorFunc = {fn_80260714, fn_8026071C};
    allocator->func = &sAllocatorFunc;
    allocator->heap = (MEMHeapHandle)heap;
    allocator->heapParam1 = 0;
    allocator->heapParam2 = 0;
}
