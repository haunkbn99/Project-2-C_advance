#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jrb.h"
#include "dllist.h"
#include "jval.h"
#define INFINITIVE_VALUE 9999999
typedef struct
{
    JRB vertices;
    JRB edges;
    JRB busLine;
} Graph;
//
typedef struct
{
    char tuyen[50];
    char line[1000];
} Data;
Graph createGraph();
void dropGraph(Graph graph);
void addVertex(Graph graph, int id, char *name);
char *getVertex(Graph graph, int id);
int getIdVertex(Graph graph, char *name);
void printJRB(JRB graph);
void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);
int addBusLine(Graph graph, char *name, int idVertex);
int checkVertex(Graph graph, char *token);
void readDataMap(Graph graph);
int trim(char *s);

int menu()
{
    int choose;
    printf("------- Quan Ly BUS Ha Noi -------\n");
    printf("----------------------------------\n");
    printf("- %-30s -\n", "1: Tim mot dia diem");
    printf("- %-30s -\n", "2: Kiem tra hai diem");
    printf("- %-30s -\n", "3: Cac dia diem noi tiep");
    printf("- %-30s -\n", "4: Tim duong di ngan nhat");
    printf("- %-30s -\n", "5: Liet ke tuyen bus");
    printf("- %-30s -\n", "6: Liet ke diem bus mot tuyen");
    printf("- %-30s -\n", "7: Thoat");
    scanf("%d", &choose);
    return choose;
}

int main()
{

    Graph g = createGraph();
    readDataMap(g);
    printJRB(g.vertices);
    JRB node = make_jrb();
    node = jrb_find_str(g.busLine, "88");
    printJRB((JRB)jval_v(node->val));
    printf("do dai cua %s -> %s la: %.2lf\n","","",getEdgeValue(g,getIdVertex(g,"Pho Nhon"),getIdVertex(g,"Quoc lo 32")));
    node = jrb_find_int(g.edges,getIdVertex(g,"Pho Nhon"));
    printf("%d\n",node->key);
    node = jrb_find_int(g.edges,getIdVertex(g,"Quoc lo 32"));
    printf("%d\n",node->key);
    int output[100];
    int total = outdegree(g,2,output);
    printf("%s lien ke:\n",getVertex(g,2));
    for (int i = 0; i < total; i++)
    {
        printf("%s: -> ",getVertex(g,output[i]));
    }
    
    
    // while (1)
    // {
    //     int check = menu();
    //     switch (check)
    //     {
    //     case 1:
    //         break;
    //     case 2:

    //         break;
    //     case 3:

    //         break;
    //     case 4:

    //         break;
    //     case 7:
    //         return 0;
    //     default:
    //         return 0;
    //         break;
    //     }
    // }
}

Graph createGraph()
{
    Graph g;
    g.edges = make_jrb();
    g.vertices = make_jrb();
    g.busLine = make_jrb();
    return g;
}
void dropGraph(Graph graph)
{
    jrb_free_tree(graph.edges);
    jrb_free_tree(graph.vertices);
    jrb_delete_node(graph.busLine);
}
void addVertex(Graph graph, int id, char *name)
{
    if (jrb_find_int(graph.vertices, id) == NULL)
    {
        jrb_insert_int(graph.vertices, id, new_jval_s(name));
    }
}
char *getVertex(Graph graph, int id)
{
    JRB node;
    node = jrb_find_int(graph.vertices, id);
    if (node == NULL)
    {
        return NULL;
    }
    else
    {
        return jval_s(node->val);
    }
}
int getIdVertex(Graph graph, char *name)
{
    JRB node = make_jrb();
    jrb_traverse(node, graph.vertices)
    {
        if (strcmp(name, jval_s(node->val)) == 0)
        {
            return jval_i(node->key);
        }
    }
    return -1;
}

void printJRB(JRB graph)
{
    printf("- %-5s | %-50s |\n", "ID", "Name");
    JRB node = make_jrb();
    jrb_traverse(node, graph)
    {
        printf("- %-5d | %-50s |\n", jval_i(node->key), jval_s(node->val));
    }
}
void addEdge(Graph graph, int v1, int v2, double weight)
{
    JRB tree = make_jrb();
    if (jrb_find_int(graph.edges, v1) == NULL)
    {
        jrb_insert_int(graph.edges, v1, new_jval_v(tree));
        jrb_insert_int(tree, v2, new_jval_d(weight));
    }
    else
    {
        JRB node = jrb_find_int(graph.edges, v1);
        tree = (JRB)jval_v(node->val);
        jrb_insert_int(tree, v2, new_jval_d(weight));
    }
}
double getEdgeValue(Graph graph, int v1, int v2)
{
    JRB node1 = jrb_find_int(graph.edges, v1);
    if (node1 == NULL)
    {
        return INFINITIVE_VALUE;
    }
    else
    {
        JRB tree;
        tree = (JRB)jval_v(node1->val);
        JRB node2 = jrb_find_int(tree, v2);
        if (node2 == NULL)
        {
            return INFINITIVE_VALUE;
        }
        else
        {
            return jval_d(node2->val);
        }
    }
}
int indegree(Graph graph, int v, int *output)
{
    JRB tree, node;
    int total = 0;
    jrb_traverse(node, graph.edges)
    {
        tree = (JRB)jval_v(node->val);
        if (jrb_find_int(tree, v))
        {
            output[total] = jval_i(node->key);
            total++;
        }
    }
    return total;
}

int outdegree(Graph graph, int v, int *output)
{
    int total = 0;
    JRB treeNode = jrb_find_int(graph.edges, v);
    JRB tree = (JRB)jval_v(treeNode->val);
    if (tree == NULL)
        return 0;

    JRB node;
    jrb_traverse(node, tree)
    {
        output[total] = jval_i(node->key);
        total++;
    }

    return total;
}
int addBusLine(Graph graph, char *name, int idVertex)
{
    if (jrb_find_str(graph.busLine, name) == NULL)
    {
        JRB tree = make_jrb();
        jrb_insert_str(graph.busLine, name, new_jval_v(tree));
        if (jrb_find_int(graph.vertices, idVertex) != NULL)
        {
            JRB node = jrb_find_str(graph.busLine, name);
            JRB node2 = (JRB)jval_v(node->val);
            jrb_insert_int(node2, idVertex, new_jval_s(getVertex(graph, idVertex)));
            return 1;
        }
        else
        {
            printf("Chua co diem bus: %d, ban phai them diem bus\n", idVertex);
            return 0;
        }
    }
    else
    {
        if (jrb_find_int(graph.vertices, idVertex) != NULL)
        {
            JRB node = jrb_find_str(graph.busLine, name);
            JRB node2 = (JRB)jval_v(node->val);
            jrb_insert_int(node2, idVertex, new_jval_s(getVertex(graph, idVertex)));
            return 1;
        }
        else
        {
            printf("Chua co diem bus: %d, ban phai them diem bus\n", idVertex);
            return 0;
        }
    }
}

int trim(char *s)
{
    int d = 0, i = 0, prev = -1;
    while (s[i])
    {
        if (s[i] == ' ' && (prev < 0 || s[prev] == ' '))
        {
            d++;
        }
        else
        {
            s[i - d] = s[i];
            prev = i;
        }
        i++;
    }
    i -= d;
    if (i && s[i - 1] == ' ')
        s[i - 1] = 0, d++;
    else
        s[i] = 0;

    return d;
}
int checkVertex(Graph graph, char *token)
{
    JRB node = make_jrb();
    jrb_traverse(node, graph.vertices)
    {
        if (strcmp(token, jval_s(node->val)) == 0)
        {
            return 1;
        }
    }
    return 0;
}
void readDataMap(Graph graph)
{
    Data All[1000];
    int count = 0, idVertex = 0, line = 0;
    char *token, *preveous;
    token = (char *)malloc(100 * sizeof(char));
    preveous = (char *)malloc(100 * sizeof(char));

    FILE *f;
    f = fopen("dataBus.txt", "r");
    if (f == NULL)
    {
        printf(" Error\n");
        exit(1);
    }

    while (fgets(All[count].tuyen, 20, f) != NULL)
    {
        All[count].tuyen[strlen(All[count].tuyen) - 1] = '\0';
        fgets(All[count].line, 1000, f);
        All[count].line[strlen(All[count].line) - 1] = '\0';
        count++;
    }
    for (int i = 0; i < count; i++)
    {
        token = strtok(All[i].line, "-");
        trim(token);
        strcpy(preveous, token);
        if (checkVertex(graph, token) == 0)
            addVertex(graph, idVertex++, token);
        addBusLine(graph, All[i].tuyen, getIdVertex(graph, token));
        while (token != NULL)
        {
            token = strtok(NULL, "-");
            if (token != NULL)
            {
                trim(token);
                if (checkVertex(graph, token) == 0)
                    addVertex(graph, idVertex++, token);
                addBusLine(graph, All[i].tuyen, getIdVertex(graph, token));
                addEdge(graph, getIdVertex(graph, preveous), getIdVertex(graph, token), 1);
                addEdge(graph, getIdVertex(graph, token), getIdVertex(graph, preveous), 1);
                strcpy(preveous, token);
            }
        }
    }

    fclose(f);
}