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
void printBusLine(JRB tree);
void addEdge(Graph graph, int v1, int v2, double weight);
double getEdgeValue(Graph graph, int v1, int v2);
int indegree(Graph graph, int v, int *output);
int outdegree(Graph graph, int v, int *output);
int addBusLine(Graph graph, char *name, int idVertex);
int checkVertex(JRB tree, char *token);
void readDataMap(Graph graph);
int trim(char *s);
void printVertex(int v);
void Case1(Graph graph);
void Case2(Graph graph);
void Case3(Graph graph);
void Case4(Graph graph);
void Case5(Graph graph);
void Case6(Graph graph);
double shortesPath(Graph graph, int s, int t, int *path, int *length);

int menu()
{
    int choose;
    printf("\n\n");
    printf("------------------ Quan Ly BUS Ha Noi -----------------\n");
    printf("-------------------------------------------------------\n");
    printf("- %-50s -\n", "1: Find location througn ID");
    printf("- %-50s -\n", "2: Check adjacet 2 location");
    printf("- %-50s -\n", "3: Show all adjacet location");
    printf("- %-50s -\n", "4: Sortest path between 2 location");
    printf("- %-50s -\n", "5: Show all buses through a location");
    printf("- %-50s -\n", "6: Show all locations in a Buses");
    printf("- %-50s -\n", "7: Exit");
    printf("\nYour choose: ");
    fflush(stdin);
    scanf("%d", &choose);
    return choose;
}

int main()
{

    Graph g = createGraph();
    readDataMap(g);
    // //in toan bo diem bus
    // printJRB(g.vertices);
    int output[100], lenght;
    shortesPath(g, getIdVertex(g, "Ben xe Gia Lam"), getIdVertex(g, "Tran Hung Dao"), output, &lenght);
    for (int i = 0; i < lenght; i++)
    {
        printf("%d\t", output[i]);
    }

    while (1)
    {
        int check = menu();
        switch (check)
        {
        case 1:
            Case1(g);
            break;
        case 2:
            Case2(g);
            break;
        case 3:
            Case3(g);
            break;
        case 4:
            Case4(g);
            break;
        case 5:
            Case5(g);
            break;
        case 6:
            Case6(g);
            break;
        case 7:
            return 0;
        default:
            return 0;
            break;
        }
    }
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
        if (jrb_find_int(tree, v2) == NULL)
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
    JRB tmp = make_jrb();
    int max = 0;
    if (jrb_find_str(graph.busLine, name) == NULL)
    {
        JRB tree = make_jrb();
        jrb_insert_str(graph.busLine, name, new_jval_v(tree));
        if (jrb_find_int(graph.vertices, idVertex) != NULL)
        {
            JRB node = jrb_find_str(graph.busLine, name);
            JRB node2 = (JRB)jval_v(node->val);
            jrb_traverse(tmp, node2)
            {
                max++;
            }
            jrb_insert_int(node2, max, new_jval_i(idVertex));
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
            jrb_traverse(tmp, node2)
            {
                max++;
            }
            jrb_insert_int(node2, max, new_jval_i(idVertex));
            return 1;
        }
        else
        {
            printf("Chua co diem bus: %d, ban phai them diem bus\n", idVertex);
            return 0;
        }
    }
}
void printVertex(int v)
{
    printf("%4d", v);
}
void DFS(Graph graph, int start, int stop, void (*func)(int))
{
    int visited[100];
    for (int i = 0; i < 100; i++)
        visited[i] = 0;
    Dllist stack = new_dllist();
    dll_append(stack, new_jval_i(start));

    while (!dll_empty(stack))
    {
        Dllist node = dll_last(stack);
        int u = jval_i(node->val);
        dll_delete_node(node);
        if (u == stop)
            break;
        if (visited[u] == 0)
        {
            func(u);
            visited[u] = 1;
            int output[100];

            for (int i = outdegree(graph, u, output) - 1; i >= 0; i--)
            {
                if (visited[output[i]] == 0)
                {
                    dll_append(stack, new_jval_i(output[i]));
                }
            }
        }
    }
    func(stop);
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
int checkVertex(JRB tree, char *token)
{
    JRB node = make_jrb();
    jrb_traverse(node, tree)
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
    f = fopen("dataBus2.txt", "r");
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
        if (checkVertex(graph.vertices, token) == 0)
            addVertex(graph, idVertex++, token);
        addBusLine(graph, All[i].tuyen, getIdVertex(graph, token));
        while (token != NULL)
        {
            token = strtok(NULL, "-");
            if (token != NULL)
            {
                trim(token);
                if (checkVertex(graph.vertices, token) == 0)
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
void Case1(Graph graph)
{
    char *name;
    int id;
    name = (char *)malloc(100 * sizeof(char));
    int output[100];
    printf("Input the ID BusStation: ");
    fflush(stdin);
    scanf("%d", &id);
    if (getVertex(graph, id) == NULL)
        printf("ID don't exist: %d\n", id);
    else
        printf("\n\nThe BusStation | ID: %d - Name: %s |\n", id, getVertex(graph, id));
}
void Case2(Graph graph)
{
    char *name1, *name2;
    int output[100], total;
    name1 = (char *)malloc(100 * sizeof(char));
    name2 = (char *)malloc(100 * sizeof(char));
    printf("Input Name BusStation 1: ");
    fflush(stdin);
    scanf("%[^\n]s", name1);
    if (!checkVertex(graph.vertices, name1))
    {
        printf("%s isn't exist\n", name1);
        return;
    }
    printf("Input Name BusStation 2: ");
    fflush(stdin);
    scanf("%[^\n]s", name2);
    if (!checkVertex(graph.vertices, name2))
    {
        printf("%s isn't exist\n", name2);
        return;
    }
    total = outdegree(graph, getIdVertex(graph, name1), output);
    for (int i = 0; i < total; i++)
    {
        if (output[i] == getIdVertex(graph, name2))
        {
            printf("\n\nBusStation %s is adjacet %s\n", name1, name2);
            return;
        }
    }
    printf("\nBusStation %s isn't adjacet %s\n", name1, name2);
}

void Case3(Graph graph)
{
    char *name;
    int output[100];
    name = (char *)malloc(100 * sizeof(char));
    printf("Input name of BusStation: ");
    fflush(stdin);
    scanf("%[^\n]s", name);
    if (checkVertex(graph.vertices, name))
    {
        int total = outdegree(graph, getIdVertex(graph, name), output);
        printf("\nBusStations adjacet %s are\n\n", name);
        printf("- %-5s | %-50s |\n\n", "ID", "Name BusStation");
        for (int i = 0; i < total; i++)
        {
            printf("- %-5d | %-50s |\n", output[i], getVertex(graph, output[i]));
        }
    }
    else
        printf("\nBusStaion Don't exist\n");
}
void Case4(Graph graph)
{
    char *name1, *name2;
    name1 = (char *)malloc(100 * sizeof(char));
    name2 = (char *)malloc(100 * sizeof(char));
    printf("Input Name BusStation Start: ");
    fflush(stdin);
    scanf("%[^\n]s", name1);
    if (!checkVertex(graph.vertices, name1))
    {
        printf("%s isn't exist\n", name1);
        return;
    }
    printf("Input Name BusStation Stop: ");
    fflush(stdin);
    scanf("%[^\n]s", name2);
    if (!checkVertex(graph.vertices, name1))
    {
        printf("%s isn't exist\n", name2);
        return;
    }
    int output[100], lenght;
    shortesPath(graph, getIdVertex(graph, name1), getIdVertex(graph, name2), output, &lenght);
    printf("Sort Path\n");
    JRB node1 = make_jrb();
    JRB node2 = make_jrb();
    JRB node3 = make_jrb();
    JRB tmp = make_jrb();
    JRB tree = make_jrb();
    JRB next = make_jrb();
    JRB prev = make_jrb();
    //cay luu cac tuyen qua qua 2 diem lien tiep
    JRB root = make_jrb();
    for (int i = lenght; i > 0; i--)
    {
        printf("%s -> %s:", getVertex(graph, output[i]), getVertex(graph, output[i - 1]));
        jrb_traverse(node1, graph.busLine)
        {

            node2 = (JRB)jval_v(node1->val);
            JRB root2 = make_jrb();
            jrb_traverse(tree, node2)
            {
                if (jval_i(tree->val) == output[i])
                {
                    next = jrb_next(tree);
                    prev = jrb_prev(tree);
                    if (next != NULL && prev != NULL)
                    {
                        if (jval_i(next->val) == output[i - 1] || jval_i(prev->val) == output[i - 1])
                        {
                            printf("%s\t", jval_s(node1->key));
                            jrb_insert_int(root, lenght - i, new_jval_v(root2));
                            jrb_insert_str(root2, jval_s(node1->key), new_jval_i(1));
                        }
                    }
                }
            }
        }
        printf("\n");
    }
    int check, i = 0;
    char path[lenght + 1][100];
    jrb_traverse(node1, root)
    {
        node2 = jrb_next(node1);
        node3 = jrb_next(node2);
        if (node3 == NULL)
        {

            jrb_traverse(tmp, node2)
            {
                check = 0;
                if (jrb_find_str(node1, jval_s(tmp->key)) != NULL)
                {
                    printf("%s", jval_s(tmp->key));
                    check = 1;
                    break;
                }else if (jrb_find_str(jrb_next(node2), jval_s(tmp->key)) != NULL)
                {
                    printf("%s", jval_s(tmp->key));
                    check = 1;
                    break;
                }
            }
        }
        if (check == 0)
            printf("%s", jval_s(jrb_first(node1)->key));
    }
    for (int j = lenght; j > 0; j--)
    {
        printf("%d\t",output[i]);
    }
    printf("\n");
    for (int j = 0; j <=i; j++)
    {
        printf("%s\t",path[i]);
    }
}
void Case5(Graph graph)
{
    char *name;
    int output[100];
    name = (char *)malloc(100 * sizeof(char));
    printf("Input name of BusStation: ");
    fflush(stdin);
    scanf("%[^\n]s", name);
    if (checkVertex(graph.vertices, name))
    {
        JRB node = make_jrb();
        JRB node2 = make_jrb();
        printf("\nThe BusLine through\n\n");
        jrb_traverse(node, graph.busLine)
        {
            if (node != NULL)
            {
                JRB tree = make_jrb();
                tree = (JRB)jval_v(node->val);
                jrb_traverse(node2, tree)
                {
                    if (getIdVertex(graph, name) == jval_i(node2->val))
                    {
                        printf("- %s\n", jval_s(node->key));
                        break;
                    }
                }
            }
        }
    }
    else
        printf("\nBusStaion Don't exist\n");
}

void Case6(Graph graph)
{
    char *name;
    int output[100];
    name = (char *)malloc(100 * sizeof(char));
    printf("Input name of BusLine: ");
    fflush(stdin);
    scanf("%[^\n]s", name);
    JRB node = make_jrb();
    node = jrb_find_str(graph.busLine, name);
    if (node == NULL)
        printf("Don't exist the BusLine: %s\n", name);
    else
    {
        JRB tree = (JRB)jval_v(node->val);
        JRB node2 = make_jrb();
        printf("- %-5s | %-50s |\n", "ID", "Name");
        jrb_traverse(node2, tree)
            printf("- %-5d | %-50s |\n", jval_i(node2->val), getVertex(graph, jval_i(node2->val)));
    }
}

double shortesPath(Graph graph, int s, int t, int *path, int *length)
{
    double distance[1000], min;
    int previous[1000], u, visited[1000], output[100], number;
    for (int i = 0; i < 1000; i++)
    {
        distance[i] = INFINITIVE_VALUE;
        visited[i] = 0;
        previous[i] = 0;
    }
    distance[s] = 0;
    previous[s] = s;
    visited[s] = 1;
    Dllist ptr, queue, node;
    queue = new_dllist();
    dll_append(queue, new_jval_i(s));
    while (!dll_empty(queue))
    {
        node = dll_first(queue);
        int u = jval_i(node->val);
        dll_delete_node(node);
        number = outdegree(graph, u, output);
        for (int i = 0; i < number; i++)
        {
            if (visited[output[i]] == 0)
            {
                visited[output[i]] = 1;
                dll_append(queue, new_jval_i(output[i]));
            }
            if ((getEdgeValue(graph, u, output[i]) + distance[u]) < distance[output[i]])
            {
                distance[output[i]] = getEdgeValue(graph, u, output[i]) + distance[u];
                previous[output[i]] = u;
            }
        }
    }
    path[0] = t;
    *length = 1;
    int cur = t;
    while (previous[cur] != s)
    {
        path[*length] = previous[cur];
        *length = *length + 1;
        cur = previous[cur];
    }

    path[*length] = s;
    return distance[t];
}