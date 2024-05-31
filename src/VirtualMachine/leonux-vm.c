#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// DEFINE SIMPLE DATA TYPES

enum DataTypes {
	INT,
	FLOAT,
	CHAR,
	PTR,
};

// DEFINE ERROR TYPES

enum ErrorSources {
	OPERATION_PROCESSOR_ERROR,
	OPERATION_STACK_ERROR,
	RETURN_STACK_ERROR,
	ADDRESS_STACK_ERROR,
	TEXT_SEGMENT_ERROR,
	STACK_ERROR,
	ADDRESS_SEGMENT_ERROR
};

enum ErrorTypes {
	STACK_UNDERFLOW_ERROR,
	STACK_OVERFLOW_ERROR,
	UNKNOWN_DATA_TYPE_ERROR,
	UNABLE_TO_ALLOCATE_MEMORY_ERROR,
	UNABLE_TO_LOAD_COMMAND_ERROR
};

void printErrorMessage(enum ErrorSources source,enum ErrorTypes type) {
	printf("--------------------------------------------------\n");
	printf("NEW ERROR MESSAGE : LEONUX VIRTUAL MACHINE\n");
	printf("ERROR SOURCE:\n");
	switch(source) {
		case OPERATION_PROCESSOR_ERROR:
			printf("OPERATION_PROCESSOR_ERROR\n");
			break;
		case OPERATION_STACK_ERROR:
			printf("OPERATION_STACK_ERROR\n");
			break;
		case RETURN_STACK_ERROR:
			printf("RETURN_STACK_ERROR\n");
			break;
		case ADDRESS_STACK_ERROR:
			printf("ADDRESS_STACK_ERROR\n");
			break;
		case TEXT_SEGMENT_ERROR:
			printf("TEXT_SEGMENT_ERROR\n");
			break;
		case STACK_ERROR:
			printf("STACK_ERROR\n");
			break;
		case ADDRESS_SEGMENT_ERROR:
			printf("ADDRESS_SEGMENT_ERROR\n");
			break;
	}
	printf("ERROR TYPE:\n");
	switch(type) {
		case STACK_UNDERFLOW_ERROR:
			printf("STACK_UNDERFLOW_ERROR\n");
			break;
		case STACK_OVERFLOW_ERROR:
			printf("STACK_OVERFLOW_ERROR\n");
			break;
		case UNKNOWN_DATA_TYPE_ERROR:
			printf("UNKNOWN_DATA_TYPE_ERROR\n");
			break;
		case UNABLE_TO_ALLOCATE_MEMORY_ERROR:
			printf("UNABLE_TO_ALLOCATE_MEMORY_ERROR\n");
			break;
		case UNABLE_TO_LOAD_COMMAND_ERROR:
			printf("UNABLE_TO_LOAD_COMMAND_ERROR\n");
			break;
	}
	printf("--------------------------------------------------\n");
}

// DATA TYPE

union DataValue {
	int int_val;
	double float_val;
	char char_val;

	struct DataType *ptr_val;
};

struct DataType {
	enum DataTypes type;
	union DataValue *value;
};

// DECLARE OPERATION AND RETURN STACK

struct OperationNode {
	struct DataType *value;
	struct OperationNode *prev;
};

struct ReturnNode {
	struct DataType *value;
	struct ReturnNode *prev;
};

struct OperationStack {
	struct OperationNode *top;
};

struct ReturnStack {
	struct ReturnNode *top;
};

typedef struct OperationStack OperationStack;
typedef struct ReturnStack ReturnStack;

// DECLARE TEXT SEGMENT

struct CommandNode{
	char *command;
	int id;
	struct CommandNode *next;
	struct CommandNode *prev;
};

struct TextSegment {
	int id_count;
	struct CommandNode *head;
	struct CommandNode *tail;
	struct CommandNode *current;
};

typedef struct TextSegment TextSegment;

// DECLARE ADDRESS STACK

struct AddressNode{
	struct CommandNode *value;
	struct AddressNode *prev;
};

struct AddressStack{
	struct AddressNode *top;
};

typedef struct AddressStack AddressStack;

OperationStack *createOperationStack() {
	OperationStack *result = malloc(sizeof(OperationStack));
	if (result == NULL) {printErrorMessage(OPERATION_STACK_ERROR,UNABLE_TO_ALLOCATE_MEMORY_ERROR);
				printf("FAILED TO CREATE OPERATION STACK : FATAL ERROR!\n");
				return NULL;}
	result->top = NULL;

	return result;
}

ReturnStack *createReturnStack() {
	ReturnStack *result = malloc(sizeof(ReturnStack));
	if (result == NULL) {printErrorMessage(RETURN_STACK_ERROR,UNABLE_TO_ALLOCATE_MEMORY_ERROR);
				printf("FAILED TO CREATE RETURN STACK : FATAL ERROR!\n");
				return NULL;}
	result->top = NULL;

	return result;
}

struct DataType *push_OperationStack(struct DataType *data, OperationStack *stack) {
	struct OperationNode *node = malloc(sizeof(struct OperationNode));
	if (node == NULL) {printErrorMessage(OPERATION_STACK_ERROR,UNABLE_TO_ALLOCATE_MEMORY_ERROR);
		       		printf("FAILED TO PUSH ELEMENT ON THE OPERATION STACK : FATAL ERROR!\n");
					return NULL;}
	node->value = data;

	if (stack->top == NULL) {
		stack->top = node;
	} else {
		struct OperationNode *temp = stack->top;
		stack->top = node;
		node->prev = temp;
	}

	return data;
}

struct DataType *push_ReturnStack(struct DataType *data, ReturnStack *stack) {
	struct ReturnNode *node = malloc(sizeof(struct ReturnNode));
	if (node == NULL) {printErrorMessage(RETURN_STACK_ERROR,UNABLE_TO_ALLOCATE_MEMORY_ERROR);
				printf("FAILED TO PUSH ELEMENT ON THE RETURN STACK : FATAL ERROR!\n");
				return NULL;}
	node->value = data;

	if (stack->top == NULL) {
		stack->top = node;
	} else{
		struct ReturnNode *temp = stack->top;
		stack->top = node;
		node->prev = temp;
	}
	return data;
}

struct DataType *pop_OperationStack(OperationStack *stack) {
	if (stack->top == NULL) {printErrorMessage(OPERATION_STACK_ERROR,STACK_UNDERFLOW_ERROR);
					printf("FAILED TO POP ELEMENT FROM THE OPERATION STACK : FATAL ERROR!\n");
					return NULL;}
	struct OperationNode *temp = stack->top;
	struct DataType *result = temp->value;
	stack->top = stack->top->prev;
	free(temp);
	return result;
}

struct DataType *pop_ReturnStack(ReturnStack *stack) {
	if (stack->top == NULL) {printErrorMessage(RETURN_STACK_ERROR,STACK_UNDERFLOW_ERROR);
					printf("FAILED TO POP ELEMENT FROM THE RETURN STACK : FATAL ERROR!\n");
					return NULL;}
	struct ReturnNode *temp = stack->top;
	struct DataType *result = temp->value;
	stack->top = stack->top->prev;
	free(temp);
	return result;
}

struct DataType *top_OperationStack(OperationStack *stack) {
	if (stack->top == NULL) {printErrorMessage(OPERATION_STACK_ERROR,STACK_UNDERFLOW_ERROR);
					printf("FAILED TO RETURN TOP ELEMENT FROM THE OPERATION STACK : FATAL ERROR!\n");
					return NULL;}
	return stack->top->value;
}

struct DataType *top_ReturnStack(ReturnStack *stack) {
	if (stack->top == NULL) {printErrorMessage(RETURN_STACK_ERROR,STACK_UNDERFLOW_ERROR);
					printf("FAILED TO RETURN TOP ELEMENT FROM THE RETURN STACK : FATAL ERROR!\n");
					return NULL;}
	return stack->top->value;
}

int is_emptyOperationStack(OperationStack *stack) {
	if (stack->top == NULL) {return 1;} else {return 0;}
}

int is_emptyReturnStack(ReturnStack *stack) {
	if (stack->top == NULL) {return 1;} else {return 0;}
}

AddressStack *createAddressStack() {
	AddressStack *result = malloc(sizeof(AddressStack));
	if (result == NULL) {printErrorMessage(ADDRESS_STACK_ERROR,UNABLE_TO_ALLOCATE_MEMORY_ERROR);
				printf("FAILED TO CREATE ADDRESS STACK : FATAL ERROR!\n");
				return NULL;}
	result->top = NULL;

	return result;
}

struct CommandNode *push_AddressStack(struct CommandNode *addr, AddressStack *stack) {
	struct AddressNode *node = malloc(sizeof(struct AddressNode));
	if (node == NULL) {printErrorMessage(ADDRESS_STACK_ERROR,UNABLE_TO_ALLOCATE_MEMORY_ERROR);
				printf("FAILED TO PUSH ELEMENT ON THE ADDRESS STACK : FATAL ERROR!\n");
				return NULL;}
	node->value = addr;
	
	if (stack->top == NULL) {
		stack->top = node;
	} else {
		struct AddressNode *temp = stack->top;
		stack->top = node;
		node->prev = temp;
	}

	return addr;
}

struct CommandNode *pop_AddressStack(AddressStack *stack) {
		if (stack->top == NULL) {printErrorMessage(ADDRESS_STACK_ERROR,STACK_UNDERFLOW_ERROR);
						printf("FAILED TO POP ELEMENT FROM THE ADDRESS STACK : FATAL ERROR!\n");
						return NULL;}
		struct AddressNode *temp = stack->top;
		struct CommandNode *result = temp->value;
		stack->top = stack->top->prev;
		free(temp);
		return result;
}

struct CommandNode *top_AddressStack(AddressStack *stack) {
	if (stack->top == NULL) {printErrorMessage(ADDRESS_STACK_ERROR,STACK_UNDERFLOW_ERROR);
					printf("FAILED TO RETURN TOP ELEMENT FROM THE ADDRESS STACK : FATAL ERROR!\n");
					return NULL;}
	return stack->top->value;
}

int is_emptyAddressStack(AddressStack *stack) {
	if (stack->top == NULL) {return 1;} else {return 0;}
}

TextSegment *createTextSegment() {
	TextSegment *result = malloc(sizeof(TextSegment));
	if (result == NULL) {printErrorMessage(TEXT_SEGMENT_ERROR,UNABLE_TO_ALLOCATE_MEMORY_ERROR);
				printf("FAILED TO CREATE TEXT SEGMENT : FATAL ERROR!\n");
				return NULL;}
	result->head = NULL;
	result->tail = NULL;
	result->current = NULL;
	result->id_count = 0;

	return result;
}

struct CommandNode *enqueue_TextSegment(TextSegment *segment,char *command) {
	if (segment->id_count == 0) {
		struct CommandNode *result = malloc(sizeof(struct CommandNode));
		if (result == NULL) {printErrorMessage(TEXT_SEGMENT_ERROR,UNABLE_TO_ALLOCATE_MEMORY_ERROR);
					printf("FAILED TO CREATE NEW COMMAND NODE : FATAL ERROR!\n");
					return NULL;}
		result->id = segment->id_count;
		segment->id_count++;
		result->command = strdup(command);
		result->prev = NULL;
		result->next = NULL;
		segment->head = result;
		segment->tail = result;
		segment->current = result;
		return result;
	} else {
		struct CommandNode *result = malloc(sizeof(struct CommandNode));
		if (result == NULL) {printErrorMessage(TEXT_SEGMENT_ERROR,UNABLE_TO_ALLOCATE_MEMORY_ERROR);
					printf("FAILED TO CREATE NEW COMMAND NODE : FATAL ERROR!\n");
					return NULL;}
		result->id = segment->id_count;
		segment->id_count++;
		result->command = strdup(command);
		result->prev = segment->tail;
		result->next = NULL;
		segment->tail->next = result;
		segment->tail = result;
		return result;
	}
}

struct CommandNode *current_TextSegment(TextSegment *segment) {
	if (segment->id_count == 0) {printErrorMessage(TEXT_SEGMENT_ERROR,UNABLE_TO_LOAD_COMMAND_ERROR);
					printf("FAILED TO LOAD CURRENT COMMAND OF TEXT SEGMENT : FATAL ERROR!\n");
					return NULL;}
	return segment->current;
}

struct CommandNode *next_TextSegment(TextSegment *segment) {
	if (segment->id_count == 0) {printErrorMessage(TEXT_SEGMENT_ERROR,UNABLE_TO_LOAD_COMMAND_ERROR);
					printf("FAILED TO LOAD NEXT COMMAND OF TEXT SEGMENT : FATAL ERROR!\n");
					return NULL;}
	if (segment->current->next == NULL) {printErrorMessage(TEXT_SEGMENT_ERROR,UNABLE_TO_LOAD_COMMAND_ERROR);
						printf("FAILED TO LOAD NEXT COMMAND OF TEXT SEGMENT : FATAL ERROR!\n");
						return NULL;}
	segment->current = segment->current->next;
	return segment->current;
}

// STACK DEVELOPMENT

struct Stack_DataNode {
	struct DataType *value;
	char *id;
	struct Stack_DataNode *next;
};

struct Stack_CollisionList {
	struct Stack_DataNode *head;
};

struct Stack_HashNode {
	char *table_id;
	int table_size;
	struct Stack_CollisionList *elements;
	struct Stack_HashNode *prev;
};

struct Stack {
	struct Stack_HashNode *top;
};

typedef struct Stack Stack;

Stack *createStack() {
	Stack *result = malloc(sizeof(Stack));
	if (result == NULL) {printErrorMessage(STACK_ERROR,UNABLE_TO_ALLOCATE_MEMORY_ERROR);
				printf("FAILED TO CREATE STACK : FATAL ERROR!\n");
				return NULL;}
	result->top = NULL;
	return result;
}

struct Stack_HashNode *push_Stack(char *table_id, int table_size, Stack *stack) {
	struct Stack_HashNode *node = malloc(sizeof(struct Stack_HashNode));
	if (node == NULL) {printErrorMessage(STACK_ERROR,UNABLE_TO_ALLOCATE_MEMORY_ERROR);
				printf("FAILED TO PUSH ELEMENT ON THE STACK : FATAL ERROR!\n");
				return NULL;}
	node->table_id = strdup(table_id);
	node->table_size = table_size;
	node->elements = malloc(sizeof(struct Stack_CollisionList)*(node->table_size));
	if (node->elements == NULL) {printErrorMessage(STACK_ERROR,UNABLE_TO_ALLOCATE_MEMORY_ERROR);
					printf("FAILED TO PUSH ELEMENT ON THE STACK : FATAL ERROR!\n");
					return NULL;}
	node->prev = NULL;
	for (int i = 0; i < node->table_size; i++) {
		node->elements[i].head = NULL;
	}
	if (stack->top == NULL) {
		stack->top = node;
	} else {
		struct Stack_HashNode *temp = stack->top;
		stack->top = node;
		node->prev = temp;
	}

	return node;
}

struct Stack_HashNode *top_Stack(Stack *stack) {
	if (stack->top == NULL) {printErrorMessage(STACK_ERROR,STACK_UNDERFLOW_ERROR);
					printf("FAILED TO RETURN TOP ELEMENT FROM THE STACK : FATAL ERROR! \n");
					return NULL;}
	return stack->top;
}

int pop_Stack(Stack *stack) {
	if (stack->top == NULL) {printErrorMessage(STACK_ERROR,STACK_UNDERFLOW_ERROR);
					printf("FAILED TO POP ELEMENT FROM THE STACK : FATAL ERROR!\n");
					return 0;}
	// NEED TO FREE THE HOLE TREE BEHIND STACK TOP NODE

}

int is_emptyStack(Stack *stack) {
	if (stack->top == NULL) {return 1;} else {return 0;}
}

// ADDRESS REGISTER DEVELOPMENT

struct AddressSegment_AddressNode {
	struct CommandNode *value;
	char *id;
	struct AddressSegment_AddressNode *next;
};

struct AddressSegment_CollisionList {
	struct AddressSegment_AddressNode *head;
};

struct AddressSegment_HashTable {
	int table_size;
	struct AddressSegment_CollisionList *elements;
};

struct AddressSegment {
	struct AddressSegment_HashTable *table;
};

typedef struct AddressSegment AddressSegment;

// MAIN FUNCTION

int main(int argc, char **argv) {
	printf("Filename: %s\n",argv[1]);
	printf("FINISHED WITH EXIT CODE 0\n");
	return 0;
}
