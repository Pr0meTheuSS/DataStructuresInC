  // "Copyright [2022] <Olimpiev Y.>"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define pass


typedef int HeapKeyType;


struct THeapElement {
  void* Content;
  size_t ContentSize;
  HeapKeyType Key;
};


typedef enum EHeapMode {
  MinInRoot,
  MaxInRoot
} EHeapMode;


struct THeap {
  THeapElement* Elements;
  size_t Size;
  size_t EndIndex;
  EHeapMode Mode;
};


void swap(THeapElement* left, THeapElement* right) {
  THeapElement temp = *left;
  *left = *right;
  *right = temp;
}

THeap* InitHeap(size_t heap_size) {
  THeap* heap = (THeap*)calloc(1, sizeof(THeap));
  assert(heap);
  heap->Size = heap_size;
  heap->Elements = (THeapElement*)calloc(heap->Size, sizeof(THeapElement));
  assert(heap->Elements);
  return heap;
}


HeapKeyType HeapCondition(HeapKeyType left,
                          HeapKeyType right,
                          EHeapMode mode) {
  switch (mode) {
    case MinInRoot:
      return (left < right) ? left : right;
    case MaxInRoot:
      return (left > right) ? left : right;
    default:
      return (left < right) ? left : right;
  }
}

void Heapify(THeap* heap, size_t index) {
  assert(heap);
  while (index != 0) {
    size_t parentIndex = index / 2 - (1 - index % 2);
    HeapKeyType confirmKey = HeapCondition(heap->Elements[parentIndex].Key,
                                           heap->Elements[index].Key,
                                           heap->Mode);
    if (heap->Elements[index].Key == confirmKey) {
      swap(&heap->Elements[index], &heap->Elements[parentIndex]);
    }
    index = parentIndex;
  }
}


int AddToHeap(THeap* heap,
              void* content,
              size_t content_size,
              HeapKeyType key) {
  assert(heap);
  if (heap->EndIndex >= heap->Size) {
    return EXIT_FAILURE;
  }

  heap->Elements[heap->EndIndex].Content = (void*)calloc(1, content_size);
  assert(heap->Elements[heap->EndIndex].Content);
  heap->Elements[heap->EndIndex].ContentSize = content_size;

  memcpy(heap->Elements[heap->EndIndex].Content, content, content_size);
  heap->Elements[heap->EndIndex].Key = key;

  Heapify(heap, heap->EndIndex);
  heap->EndIndex++;
}


void GetRoot(THeap* heap, void* content, size_t content_size) {
  assert(heap);
  assert(content);
  assert(heap->Size);
  assert(heap->Elements[0].ContentSize == content_size);

  if (heap->EndIndex < 1) {
    return;
  }
  memcpy(content, heap->Elements[0].Content, content_size);

  swap(&heap->Elements[0], &heap->Elements[heap->EndIndex - 1]);

  // free content part
  free(heap->Elements[heap->EndIndex - 1].Content);

  heap->EndIndex--;
  if (heap->EndIndex >= 1)
    Heapify(heap, heap->EndIndex - 1);
}


void FreeHeap(THeap* heap) {
  if (heap) {
    free(heap->Elements);
    heap->EndIndex = heap->Size = 0;
    free(heap);
  }
}


typedef struct Person {
  char Name[128];
  unsigned int Age;
} Person;


/**/
int main(void) {
  const size_t arrSize = 10;
  Person persons[] = { {"Yuriy", 21},
                       {"Mark", 0},
                       {"Mary", 26},
                       {"Egor", 20},
                       {"Boria", 30},
                       {"Vika", 18},
                       {"Nikita", 18},
                       {"Serj", 20},
                       {"Yanina", 21},
                       {"Polina", 15} };
  THeap* heap = InitHeap(arrSize);

  for (size_t i = 0; i < arrSize; i++) {
     AddToHeap(heap, &persons[i], sizeof(persons[i]) , persons[i].Age);
  }
  for (size_t i = 0; i < arrSize; i++) {
    Person currPers;
    GetRoot(heap, &currPers, sizeof(currPers));
    printf("Name: %s, Age: %d\n", currPers.Name, currPers.Age);
  }
  return EXIT_SUCCESS;
}
/*

// Test for detect mem leak
int main(void) {
  const size_t arrSize = 10'000;
  THeap* heap = InitHeap(arrSize);

  for (size_t i = 0; i < arrSize; i++) {
    Person person = { "Name", 10 };
    AddToHeap(heap, &person, sizeof(person), person.Age);
  }
  for (size_t i = 0; i < arrSize; i++) {
    Person currPers;
    GetRoot(heap, &currPers, sizeof(currPers));
  }
  FreeHeap(heap);
  return EXIT_SUCCESS;
}
*/
