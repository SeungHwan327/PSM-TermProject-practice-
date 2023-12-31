#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 30

// Data structure Define
typedef struct _Data
{
    int tag;
    char date[11];
    char fee_paid[5];
    char name[20];
    int age;
    char organization[50];
    char job[20];
} Data;

// Linked list node structure
typedef struct _Node
{
    Data data;
    struct _Node* next;
} Node;


// This function opens file and returns file pointer
FILE* open_file(const char* address, const char* mode)
{
    FILE* fp = fopen(address, mode);

    if (fp == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    return fp;
}

// This function splits raw data into data structure
void split_data(char raw_data[MAX][1000], Data data[MAX])
{
    for (int i = 0; i < MAX; i++)
    {
        int count = 0;
        char* token = strtok(raw_data[i], "/");
        char tokens[7][100] = { 0 };

        while (count < 7)
        {
            strcpy(tokens[count], token);
            token = strtok(NULL, "/");
            count++;
        }

        data[i].tag = atoi(tokens[0]);
        strcpy(data[i].date, tokens[1]);
        strcpy(data[i].fee_paid, tokens[2]);
        strcpy(data[i].name, tokens[3]);
        data[i].age = atoi(tokens[4]);
        strcpy(data[i].organization, tokens[5]);
        strcpy(data[i].job, tokens[6]);
    }
}

// This function sorts data by age using bubble sort
void sort_data(Data data[MAX])
{
    Data temp;
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX - i - 1; j++)
        {
            if (data[j].age > data[j + 1].age)
            {
                temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}


// This function finds fee-paid people
void is_paid(Node* head)
{
    printf("Fee Paid List\n_________________________________________________________________\n");

    Node* current = head->next;

    while (current != NULL)
    {
        if (strcmp(current->data.fee_paid, "yes") == 0)
        {
            printf("%d/%s/%s/%s/%d/%s/%s\n", current->data.tag, current->data.date, current->data.fee_paid,
                current->data.name, current->data.age, current->data.organization, current->data.job);
        }

        current = current->next;
    }

    printf("_________________________________________________________________\n");
}



// This function inserts a node into the linked list
Node* insert_node(Node* head, Data data)
{
    Node* new_node = NULL;
    new_node = (Node*)malloc(sizeof(Node));

    if (new_node == NULL)
    {
        printf("Memory allocation error\n");
        exit(1);
    }

    new_node->data = data;

    if (head == NULL)
    {
        head = (Node*)malloc(sizeof(Node));
        if (head == NULL)
        {
            printf("Memory allocation error\n");
            exit(1);
        }
        head->next = NULL;
    }

    new_node->next = head->next;
    head->next = new_node;

    return head;
}

// This function makes free memory allocated for the linked list
void free_linked_list(Node* head)
{
    Node* current = head;
    Node* next;

    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
}

int main()
{
    char raw_data[MAX][1000];
    Data data[MAX];
    FILE* fp = open_file("registration_data.txt", "r");
    FILE* sorted_fp = open_file("sorted_data.txt", "w");

    for (int i = 0; i < MAX; i++)
        fgets(raw_data[i], 1000, fp);

    split_data(raw_data, data);
    sort_data(data);

    for (int i = 0; i < MAX; i++)
        fprintf(sorted_fp, "%d/%s/%s/%s/%d/%s/%s\n", data[i].tag, data[i].date, data[i].fee_paid, data[i].name, data[i].age, data[i].organization, data[i].job);

    fclose(fp);
    fclose(sorted_fp);

    Node* head = NULL;
    for (int i = 0; i < MAX; i++)
        head = insert_node(head, data[i]);

    is_paid(head);

    free_linked_list(head);

    return 0;
}