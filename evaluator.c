#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* subString(char* string, int start, int length){
    char* subString = malloc(length);
    int count = 0;
    for (size_t i = start; i < start + length; i++)
    {
        subString[count] = string[i];
        count++;
    }
    return subString;
}

double exponential(double value, double power){
    double output = 1;
    for (size_t i = 0; i < power; i++)
    {
        output *= value;
    }
    return output;
}

typedef enum TokenType{
    VALUE = '$',
    OPERATOR = '|',
    FUNCTION = '!'
}TokenType;

typedef enum NodeType{
    NUMBER,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    EXPONENT,
    OPENBRACKET,
    CLOSEBRACKET,
    VARIABLE,
    EMPTY
}NodeType;


NodeType getTypeFromSymbol(char symbol){
    switch (symbol)
    {
    case '+':
        return ADD;
    case '-':
        return SUBTRACT;
        
    case '*':
        return MULTIPLY;
        
    case '/':
        return DIVIDE;
        
    case '^':
        return EXPONENT;
    case '(':
        return OPENBRACKET;
    case ')':
        return CLOSEBRACKET;
    case 'x':
        return VARIABLE;
    default:
    break;
        }
}

char getSymbolFromType(NodeType type){
    switch (type)
    {
    case ADD:
        return '+';
    case SUBTRACT:
        return '-';
    case MULTIPLY:
        return '*';
    case DIVIDE:
        return '/';
    case EXPONENT:
        return '^';
    case OPENBRACKET:
        return '(';
    case CLOSEBRACKET:
        return ')';
    case VARIABLE:
        return 'x';
    default:
        return ' ';
    }
}

int getPrecedence(NodeType type){
    switch (type)
    {
    case ADD:
        return 1;
    case SUBTRACT:
        return 1;
    case MULTIPLY:
        return 2;
    case DIVIDE:
        return 2;
    case EXPONENT:
        return 3;
    case OPENBRACKET:
        return 4;
    case CLOSEBRACKET:
        return 4;
    default:
        return 0;
    }
}

int isOperator(char symbol){
    if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '^' || symbol == '(' || symbol == ')' || symbol == 'x') return 1;
    return 0;
}

int isNumber(NodeType type){
    switch (type)
    {
    case NUMBER:
        return 1;
    case VARIABLE:
        return 1;
    default:
        return 0;
    }
}

int typeIsOperator(NodeType type){
    switch (type)
    {
    case ADD:
        return 1;
    case SUBTRACT:
        return 1;
    case MULTIPLY:
        return 2;
    case DIVIDE:
        return 2;
    case EXPONENT:
        return 3;
    default:
        return 0;
    }
}

typedef struct Node Node;
struct Node{
    Node* left;
    Node* right;

    double value;

    NodeType type;

};

typedef struct OutputQueue OutputQueue;
struct OutputQueue
{
    int length;
    int head;
    int tail;
    Node* queue;
};

OutputQueue* createOutputQueue(int size){
    OutputQueue* queue = malloc(sizeof(OutputQueue));
    queue->length = size;
    queue->head = 0;
    queue->tail = -1;
    queue->queue = malloc(sizeof(Node) * size);
    Node empty;
    empty.type = EMPTY;
    for (size_t i = 0; i < size; i++)
    {
        queue->queue[i] = empty;
    }
    return queue;
}

void printQueue(OutputQueue* this){
    int length = this->length;
    //printf("%u",length);
    //printf("\n");
    for (size_t i = 0; i < length; i++)
    {
        NodeType type = this->queue[i].type;
        if (type == EMPTY) return;
        if (type == NUMBER)
        {
            printf("%f",this->queue[i].value);
            printf(" ");
        }
        else {
            printf("%c",getSymbolFromType(type));
            printf(" ");
        }
    }
}

void enqueue(OutputQueue* this, Node node){
    this->tail++;
    this->queue[this->tail] = node;
}
void requeue(OutputQueue* this, Node node){
    this->head--;
    this->queue[this->head] = node;
}

void eqOperator(OutputQueue* this, char operator){
    Node node;
    node.type = getTypeFromSymbol(operator);
    enqueue(this,node);
}

void eqNumber(OutputQueue* this, int value){
    Node node;
    node.type = NUMBER;
    node.value = value;
    enqueue(this,node);
}

Node dequeue(OutputQueue* this){
    Node output;
    output = this->queue[this->head];
    this->head++;
    return output;
}

int countOperators(char* string){
    int length = strlen(string);
    int opcount = 0;
    for (size_t i = 0; i < length; i++)
    {
        if (isOperator(string[i]) == 1)
        {
            opcount++;
        }
    }
    return opcount;
}

OutputQueue* createInfixQueue(char* expression){
    int opCount = countOperators(expression);
    int expressionlength = strlen(expression);
    OutputQueue* queue = createOutputQueue(opCount * 2 + 1);
    char* buffer = malloc(2);
    char* token = malloc(2);
    buffer[1] = '\0';
    buffer[0] = '0';
    token[1] = '\0';
    for (size_t i = 0; i < expressionlength; i++)
    {
        if (isOperator(expression[i]))
        {
            eqOperator(queue,expression[i]);
        }
        else{
            token[0] = expression[i];
            buffer = realloc(buffer,strlen(buffer) + 2);
            buffer = strcat(buffer,token);
            if (i + 1 == expressionlength)
            {
                eqNumber(queue,atof(buffer));
                free(token);
                free(buffer);
            }
            else if (isOperator(expression[i+1]))
            {
                eqNumber(queue,atof(buffer));
                free(token);
                free(buffer);
                buffer = malloc(2);
                token = malloc(2);
                buffer[1] = '\0';
                buffer[0] = '0';
                token[1] = '\0';

            }
            
            
        }
        
    }
    return queue;
}

typedef struct NodeStack NodeStack;
 struct NodeStack{
    int length;
    int head;
    Node* stack;
};

NodeStack* createNodeStack(int size){
    NodeStack* stack = malloc(sizeof(NodeStack));
    stack->head = -1;
    stack->length = size;
    stack->stack = malloc(sizeof(Node) * size);
    Node empty;
    empty.type = EMPTY;
    for (size_t i = 0; i < size; i++)
    {
        stack->stack[i] = empty;
    }
    
    return stack;
}

void nsPush(NodeStack* this,char operator){
    this->head++;
    Node node;
    node.type = getTypeFromSymbol(operator);
    this->stack[this->head] = node;
}

void Push(NodeStack* this,Node node){
    this->head++;
    this->stack[this->head] = node;
}

Node nsPop(NodeStack* this){
    Node node;
    node = this->stack[this->head];
    this->head--;
    return node;
}

NodeType nsPeek(NodeStack* this){
    return this->stack[this->head].type;
}

void printStack(NodeStack* this){
    int length = this->head + 1;
    if (length == 0) printf("STACK IS EMPTY");
    
    for (size_t i = 0; i < length; i++)
    {
        NodeType type = this->stack[i].type;
        if (type == EMPTY) return;
        if (type == NUMBER)
        {
            printf("%f",this->stack[i].value);
            printf(" ");
        }
        else {
            printf("%c",getSymbolFromType(type));
            printf(" ");
        }
    }
}

double Evaluate(Node* root, double* x){
    switch (root->type)
    {
    case NUMBER:
        return root->value;
    case ADD:
        return Evaluate(root->left,x) + Evaluate(root->right,x);
    case SUBTRACT:
        return Evaluate(root->left,x) - Evaluate(root->right,x);
    case MULTIPLY:
        return Evaluate(root->left,x) * Evaluate(root->right,x);
    case DIVIDE:
        return Evaluate(root->left,x) / Evaluate(root->right,x);
    case EXPONENT:
        return exponential(Evaluate(root->left,x),Evaluate(root->right,x));
    case VARIABLE:
        return *x;
    default:
        break;
    }
}

Node* copyNode(Node node){
    Node* newnode = malloc(sizeof(node));
    *newnode = node;
    return newnode; 
}

Node* newNumber(double value){
    Node* node = malloc(sizeof(Node));
    node->type = NUMBER;
    node->value = value;
    return node;
}

Node* newOperator(NodeType type, Node* left, Node* right){
    Node* node = malloc(sizeof(Node));
    node->type = type;
    node->left = left;
    node->right = right;
    return node;
}

char* Strip(char expression[]){
    int expressionlength = strlen(expression);
    int strippedlength = expressionlength;
    for (size_t i = 0; i < strlen(expression); i++)
    {
        if (expression[i] == '\n' || expression[i] =='\0')
        {
            strippedlength = i;
            expression[i] = '\0';
            break;
        }
        
        
    }
        for (size_t i = 0; i < strlen(expression); i++)
    {
        if (expression[i] == ' ')
        {
            strippedlength--;
        }
    }
    char* strippedexpression = malloc(strippedlength + 1);
    strippedexpression[strippedlength] = '\0';
    int count = 0;
    for (size_t i = 0; i < expressionlength; i++)
    {
        if (expression[i] == ' ') continue;
        strippedexpression[count] = expression[i];
        count++;
        
    }
    
    
    return strippedexpression;
}

OutputQueue* infixToPostfix(OutputQueue* queueInfix){
    OutputQueue* queueOutput = createOutputQueue(queueInfix->length);
    NodeStack* stackOperators = createNodeStack(queueInfix->length);
    while (queueInfix->head != queueInfix->tail + 1)
    {
        Node node = dequeue(queueInfix);
        if (node.type == OPENBRACKET)
        {
            nsPush(stackOperators,getSymbolFromType(node.type));
        }
        else if (node.type == CLOSEBRACKET)
        {
            while (nsPeek(stackOperators) != OPENBRACKET)
            {
                enqueue(queueOutput,nsPop(stackOperators));
            }
            nsPop(stackOperators);
        }
        
        else if (node.type == NUMBER || node.type == VARIABLE)
        {
            enqueue(queueOutput,node);
        }
        else{
            if (typeIsOperator(nsPeek(stackOperators)))
            {
                if (getPrecedence(nsPeek(stackOperators)) >= getPrecedence(node.type))
                {
                    enqueue(queueOutput,nsPop(stackOperators));
                    nsPush(stackOperators,getSymbolFromType(node.type));
                    continue;
                }
                else
                {
                    nsPush(stackOperators, getSymbolFromType(node.type));
                }
            }
            else
            {
                nsPush(stackOperators, getSymbolFromType(node.type));
            }

        }
        //printf("\n Queue: ");
        //printQueue(queueOutput);
        //printf("\n Stack: ");
        //printStack(stackOperators);
    }
    while (stackOperators->head > -1)
    {
        enqueue(queueOutput,nsPop(stackOperators));
    }

    //printf("\n Output Queue: ");
    //printQueue(queueOutput);

    int finalLength = queueOutput->length;

    for (size_t i = 0; i < queueOutput->length; i++)
    {
        if (queueOutput->queue[i].type == EMPTY)
        {
            finalLength = i;
            break;
        }
        
    }
    OutputQueue* finalQueue = createOutputQueue(finalLength);
    for (size_t i = 0; i < finalLength; i++)
    {
        enqueue(finalQueue,queueOutput->queue[i]);
    }
    free(queueOutput->queue);
    free(stackOperators->stack);
    free(queueOutput);
    free(stackOperators);
    
    printQueue(finalQueue);
    printf("\n %u",finalQueue->length);
    printf("\n");
    return finalQueue;
}

Node* postfixToTree(OutputQueue* queuePostfix){
    Node *stack[queuePostfix->length];
    int head = -1;
    for (size_t i = 0; i < queuePostfix->length; i++)
    {
        Node* node = malloc(sizeof(Node));
        *node = queuePostfix->queue[i];
        if (isNumber(node->type))
        {
            head++;
            stack[head] = node;
        }
        else{
            Node* left;
            left = stack[head];
            head--;

            Node* right;
            right = stack[head];
            head--;

            node->left = left;
            node->right = right;
            head++;
            stack[head] = node;
        }
    }
    return stack[head];
}

double evaluatePostfix(OutputQueue* queuePostfix, double x){
    Node operand1;
    Node operand2;
    Node operator;
    operator.left = &operand1;
    operator.right = &operand2;
    Node result;
    result.type = NUMBER;
    NodeStack* stack = createNodeStack(queuePostfix->length);

    for (size_t i = 0; i < queuePostfix->length; i++)
    {
        if (queuePostfix->queue[i].type == VARIABLE)
        {
            queuePostfix->queue[i].type = NUMBER;
            queuePostfix->queue[i].value = x;
        }
    }
    for (size_t i = 0; i < queuePostfix->length; i++)
    {
        //Node node = queuePostfix->queue[i];
        if (queuePostfix->queue[i].type == NUMBER)
        {
            Push(stack,queuePostfix->queue[i]);
            printf("\n");
            printStack(stack);
        }
        else if (typeIsOperator(queuePostfix->queue[i].type))
        {
            operator = queuePostfix->queue[i];
            operand2 = nsPop(stack);
            operand1 = nsPop(stack);
            operator.left = &operand1;
            operator.right = &operand2;
            double res = Evaluate(&operator,&x);
            printf("\n");
            printf("%f",res);
            result.value = res;
            Push(stack,result);
        }
    }
    
    
    return result.value;
}

double evaluateExpression(char* expression,double x){
    char* stripped = Strip(expression);
    OutputQueue* infixQueue = createInfixQueue(stripped);
    printQueue(infixQueue);
    free(stripped);
    OutputQueue* postfixQueue = infixToPostfix(infixQueue);
    printQueue(postfixQueue);
    free(infixQueue->queue);
    free(infixQueue);
    double output = evaluatePostfix(postfixQueue,1);
    free(postfixQueue->queue);
    free(postfixQueue);
    return output;
}

int main(void){
    char expression[200];
    printf("Enter expression to evaluate: ");
    fgets(expression,sizeof(expression) - 1,stdin);
    double result = evaluateExpression(expression,1);
    printf("\n Result of expression: %f",result);
    printf("\n");
    /*
    char* stripped = Strip(expression);
    printf("%s", stripped);
    int opCount = countOperators(stripped);
    printf("\nNumber of Operators in expression: ");
    printf("%u", opCount);
    printf("\nNumber of Values in expression: ");
    printf("%u", opCount + 1);
    printf("\n");
    OutputQueue* queueInfix = createInfixQueue(stripped);
    printQueue(queueInfix);
    printf("\n\n");
    OutputQueue* queuePostfix = infixToPostfix(queueInfix);

    //Node* tree = postfixToTree(queuePostfix);
    double x = 1;
    double output = evaluatePostfix(queuePostfix,x);
    */
    return 0;
}
