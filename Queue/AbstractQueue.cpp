#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#define pass 

typedef struct TQueueNode TQueueNode;
typedef struct TQueue TQueue;


struct TQueueNode {
	TQueueNode* Next;
	void* Content;
	size_t ContentSize;
};


struct TQueue {
	TQueueNode* Head;
	TQueueNode* Tail;
};


TQueueNode* InitQueueNode(void* content, size_t content_size) {
	assert(content);
	assert(content_size != 0);

	TQueueNode* newNode = (TQueueNode*)calloc(1, sizeof(TQueueNode));
	assert(newNode);

	newNode->Content = (void*)calloc(1, content_size);
	assert(newNode->Content);

	newNode->ContentSize = content_size;
	memcpy(newNode->Content, content, content_size);

	return newNode;
}


int EnQueue(TQueue* q, void* content, size_t content_size) {
	assert(q);
	assert(content);
	assert(content_size != 0);

	TQueueNode* newNode = InitQueueNode(content, content_size);
	if (!newNode) {
		return EXIT_FAILURE;
	}
	if (!q->Head) {
		q->Head = newNode;
		q->Tail = newNode;
		return EXIT_SUCCESS;
	}

	q->Tail->Next = newNode;
	q->Tail = newNode;

	return EXIT_SUCCESS;
}


int DeQueue(TQueue* q, void* content, size_t content_size) {
	assert(q);
	assert(content);
	if (!q->Head) {
		return EXIT_FAILURE;
	}
	if (q->Head->ContentSize != content_size) {
		return EXIT_FAILURE;
	}

	TQueueNode* toDel = q->Head;
	memcpy(content, q->Head->Content, q->Head->ContentSize);
	q->Head = q->Head->Next;

	free(toDel->Content);
	free(toDel);

	if (!q->Head) {
		q->Tail = NULL;
	}

	return EXIT_SUCCESS;
}


// For Tests
typedef struct TCostomContent TCostomContent;
struct TCostomContent {
	char Name[128];
	size_t Age;
};


/*
// Simple Test
int main(void) {
	TQueue q = { NULL, NULL };

	size_t personsAmount = 4;
	TCostomContent Yuriy = { "Yuriy", 21 };
	TCostomContent Egor = { "Egor", 20 };
	TCostomContent VikaGenius = { "VikaGenius", 18 };
	TCostomContent Mark = { "Mark", 0 };

	TCostomContent persons[] = { Yuriy, Egor, VikaGenius, Mark };

	for (size_t i = 0; i < personsAmount; i++) {
		EnQueue(&q, &persons[i], sizeof(persons[i]));
	}

	// personsAmount + 1 : for check DeQueue from empty queue
	for (size_t i = 0; i < personsAmount + 1; i++) {
		TCostomContent content = { 0, 0 };
		if (DeQueue(&q, &content, sizeof(content)) == EXIT_SUCCESS)
			printf("#%d is person: %s, %d y.o.\n", i, content.Name, content.Age);
	}
	// assert that q is ready to work after use
	assert(!q.Head);
	assert(!q.Tail);

	return EXIT_SUCCESS;
}*/


// Test for check memory leak
// Checked by Visual Studio profiler - no memory leaks were detected
/*
int main(void) {
	TQueue q = { NULL, NULL };

	size_t personsAmount = 1'000'000;

	TCostomContent p = { "Yuriy", 21 };
	for (size_t i = 0; i < personsAmount; i++) {
		EnQueue(&q, &p, sizeof(p));
	}

	// personsAmount + 1 : for check DeQueue from empty queue
	for (size_t i = 0; i < personsAmount + 1; i++) {
		TCostomContent content = { 0, 0 };
		if (DeQueue(&q, &content, sizeof(content)) == EXIT_SUCCESS)
			pass;
	}
	// assert that q is ready to work after use
	assert(!q.Head);
	assert(!q.Tail);

	return EXIT_SUCCESS;
}
*/
