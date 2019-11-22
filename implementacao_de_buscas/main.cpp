#include <iostream>
#include <vector>
#include <queue>
#include <stdio.h>
#include <string.h>
#define MAX 11234

using namespace std;

priority_queue<int, std::vector<int>, std::greater<int>> pq;
bool visited[MAX];
int queue_begin, queue_end;
int G[MAX][MAX], queue_aux[MAX], v, e, n, matrix_type; // n == number_of_vertix, if matrix_type == 0 is adjacency, else is incidence
void buscaLargura(int begining, int end);
void buscaProfundidade(int begining, int end);
int dequeue();
void enqueue(int i);
void clear();

int main(){
    int op, i, j;
    char a='a';
    bool noth=false;
    memset(visited, 0, MAX);

    do{
        cout << "1) Adjacency matrix." << endl << "2) Incidence matrix." << endl << "0) Finish execution." << endl << "> ";
        cin >> op;
        if(op == 0){
            clear();
            cout << "Exiting." << endl;
            return 0;
        }else if(op == 1 || op == 2){
            matrix_type = op == 1 ? 0 : 1;break;
        }else{
            clear();
            cout << "Invalid option." << endl;
        }
    }while(op != 0);

    cout << "Type the number of vertix: ";
    cin >> v;
    if(op == 2){
        cout << "Type the number of edges: ";
        cin >> e;
    }else{
        e = v;
    }
    cout << "  ";
    if(matrix_type == 1)
        for(i=0; i<e; i++)
            cout << a++ << " ";
    else
        for(i=0; i<e; i++)
            cout << i+1 << " ";
    cout << endl;

    for(i=0; i < v; i++){
        cout << i+1 << " ";
        for(j=0; j < e; j++){
            cin >> G[i][j];
        }
    }
    do{
        cout << "To exit press -1 to start and destination vertix" << endl << "Type the start vertix: ";
        cin >> i;
        cout << "Type the destination vertix: ";
        cin >> j;
        if(i != -1 && j != -1){
            while(!pq.empty())
                pq.pop();
            cout << "Depth-first search:" << endl;
            buscaProfundidade(i, j);
            memset(visited, 0, MAX);
            while(!pq.empty())
                pq.pop();
            cout << endl << "Breadth-first search:" << endl;
            buscaLargura(i, j);
            memset(visited, 0, MAX);
        }
    }while(i != -1 && j != -1);

    return 0;
}

void clear() {
    // CSI[2J clears screen, CSI[H moves the cursor to top-left corner
    std::cout << "\x1B[2J\x1B[H";
}

void buscaLargura(int begining, int end){
    int i, j;
    memset(visited, false, MAX);

    queue_begin = 0;
    queue_end = 0;

    for(i = 0; i < MAX; i++)
        visited[i] = 0;

    visited[begining] = 1;

    // Print the current node
    cout << "Actual node: " << begining << "\nFrontier: {";

    //enqueue visited vertex
    enqueue(begining);

    //continue till queue is not empty
    while(queue_begin <= queue_end) {
        //dequeue first element from the queue
        i = dequeue();

        for(j = 0; j < v; j++) {
            if(visited[j] == 0 && G[i][j] == 1) {
                //mark vertex as visited
                visited[j] = 1;
                if(j == end)
                    printf("(%d), ", j);
                else
                    printf("%d, ", j);
                //push vertex into stack
                enqueue(j);
            }
        }
    }
    printf("}.\n");
}

void enqueue(int i){
    queue_aux[queue_end] = i;
    queue_end++;
}

int dequeue(){
    int index = queue_begin;
    queue_begin++;
    return queue_aux[index];
}

void buscaProfundidade(int begining, int end){
    int j, i;

    visited[begining]=true;

    if(matrix_type==0){
        if(begining == end){
            printf("(%d), ", begining);
        }else{
            printf("%d, ", begining);
        }

        for(i=0; i < v; i++)
           if(!visited[i] && G[begining][i]==1){
                buscaProfundidade(i, end);
            }
    }else{
        if(begining == end){
            printf("(%d), ", begining);
        }else{
            printf("%d, ", begining);
        }

        for(i=0; i < e; i++){
            if(G[begining][i]){
                for(j=0; j < v; j++)
                    if(G[j][i] && j != begining){
                        pq.push(j);
                    }
            }
        }
        while(!pq.empty()){
            if (visited[pq.top()]){
                pq.pop();
            }else if(!visited[pq.top()])
                buscaProfundidade(pq.top(), end);
        }
    }
}