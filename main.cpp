#include <stdio.h>
#include <stdlib.h>
#include "dump.h"
#include "log.h"

const size_t listSize = 10;
const int INIT_SIZE = 5;

struct list_elem {
    char data;
    int next;
    int prev;
};

struct LIST {
    struct list_elem list_arr[listSize];
    int head;
    int tail;
    int free;
    size_t lSize;
};

void ListCtor(struct LIST* List);
void ListDtor(struct LIST* List);

void ConsoleListDump(struct LIST* List);
void printList(struct LIST* List);

char getHead(struct LIST* List);
char getTail(struct LIST* List);
char getNext(struct LIST* List, int index);
char getPrev(struct LIST* List, int index);

int frontInsert(struct LIST* List, char elem);
int backInsert(struct LIST* List, char elem);
int insertBeforeGiven(struct LIST* List, int index ,char elem);
int insertAfterGiven(struct LIST* List, int index ,char elem);

int getPhysicalByLogical(struct LIST* List, int index);
int getLogicalByPhysical(struct LIST* List, int index);
int getLogicalByData(struct LIST* List, char data);

void Delete(struct LIST* List, int index);

void GraphListDump(struct LIST* List, const char* str);
int AssertOk(struct LIST* List);

int main() {
    struct list_elem list_arr[listSize] = {NULL};
    struct LIST List = {list_arr[listSize], 0, 0, 0, 0};
    ListCtor(&List);
    //ConsoleListDump(&List);

    char head = getHead(&List);
    char tail = getTail(&List);
    printf("head is %c, tail is %c\n", head, tail);

    char gotNext = getNext(&List, 3);
    printf("gotNext is %c\n", gotNext);

    char gotPrev = getPrev(&List, 3);
    printf("gotPrev is %c\n", gotPrev);

    frontInsert(&List, 'L');
    backInsert(&List, 'Z');
    insertBeforeGiven(&List, 3, 'F');
    insertAfterGiven(&List, 2, 'X');
    Delete(&List, 2);
    insertAfterGiven(&List, 1, 'B');
    /*int gotByLogical = getPhysicalByLogical(&List, 9);
    int logByPhys = getLogicalByPhysical(&List, 9);
    int logByData = getLogicalByData(&List, 'S');
    printf("get phys by log %d\n", gotByLogical);
    printf("got log by phys %d\n", logByPhys);
    printf("log by data is %d\n", logByData);*/

    //ConsoleListDump(&List);
    printList(&List);

    //GraphListDump(&List, "aa");

    return 0;
}

char getHead(struct LIST* List){
    ASSERT(List != nullptr);
    return List->list_arr[List->head].data;
}

char getTail(struct LIST* List){
    ASSERT(List != nullptr);
    return List->list_arr[List->tail].data;
}

char getNext(struct LIST* List, int index){
    ASSERT(List != nullptr);
    return List->list_arr[List->list_arr[index].next].data;
}

char getPrev(struct LIST* List, int index){
    ASSERT(List != nullptr);
    return List->list_arr[List->list_arr[index].prev].data;
}

int frontInsert(struct LIST* List, char elem){
    ASSERT(List != nullptr);

    int curIndex = List->free;
    List->free = List->list_arr[curIndex].next;

    List->list_arr[curIndex].data = elem;

    List->list_arr[curIndex].next = List->head;
    List->list_arr[List->head].prev = curIndex;
    List->list_arr[curIndex].prev = 0;

    List->head = curIndex;

    List->lSize++;

    ASSERT_OK(List);

    return curIndex;
}

int backInsert(struct LIST* List, char elem){
    ASSERT(List != nullptr);

    int curIndex = List->free;
    List->free = List->list_arr[curIndex].next;

    List->list_arr[curIndex].data = elem;

    List->list_arr[curIndex].prev = List->tail;
    List->list_arr[List->tail].next = curIndex;
    List->list_arr[curIndex].next = 0;

    List->tail = curIndex;
    List->lSize++;

    ASSERT_OK(List);

    return curIndex;
}

int insertBeforeGiven(struct LIST* List, int index, char elem){
    ASSERT(List != nullptr);

    int curIndex = List->free;
    List->free = List->list_arr[curIndex].next;
    int oldPrev = List->list_arr[index].prev;

    List->list_arr[curIndex].data = elem;
    List->list_arr[curIndex].prev = oldPrev;
    List->list_arr[curIndex].next = index;

    List->list_arr[oldPrev].next = curIndex;
    List->list_arr[index].prev = curIndex;

    List->lSize++;

    ASSERT_OK(List);

    return curIndex;
}

int insertAfterGiven(struct LIST* List, int index ,char elem){
    ASSERT(List != nullptr);

    int curIndex = List->free;
    List->free = List->list_arr[curIndex].next;
    int oldNext = List->list_arr[index].next;

    List->list_arr[curIndex].data = elem;
    List->list_arr[curIndex].prev = index;
    List->list_arr[curIndex].next = oldNext;

    List->list_arr[oldNext].prev = curIndex;
    List->list_arr[index].next = curIndex;

    List->lSize++;

    ASSERT_OK(List);

    return curIndex;
}

void Delete(struct LIST* List, int index){
    ASSERT(List != nullptr);

    int prev = List->list_arr[index].prev;
    int next = List->list_arr[index].next;
    List->list_arr[index].data = ' ';
    List->list_arr[index].next = List->free;
    List->list_arr[index].prev = -1;
    List->free = index;

    List->list_arr[prev].next = next;
    List->list_arr[next].prev = prev;

    List->lSize--;

    ASSERT_OK(List);
}

void ListCtor(struct LIST* List){
    ASSERT(List != nullptr);

    int i = 0;
    for ( i = 1; i < INIT_SIZE+1; i++) {
        List->list_arr[i].data = 'A'+i-1;
        if (i < INIT_SIZE) {
            List->list_arr[i].next = i+1;
        }
        List->list_arr[i].prev = i-1;
    }
    List->list_arr[0].data = ' ';
    List->list_arr[0].next = 0;
    List->list_arr[0].prev = 0;

    List->head = 1;
    List->tail = i - 1;
    List->free = i;
    List->lSize = INIT_SIZE;

    for (i; i < listSize; i++) {
        List->list_arr[i].data = ' ';
        List->list_arr[i].prev = -1;
        if (i + 1 == listSize)
            List->list_arr[i].next = 0;
        else List->list_arr[i].next = i + 1;
    }

    ASSERT_OK(List);
}

void ListDtor(struct LIST* List){
    ASSERT(List != nullptr);

    for (int i = 0; i < listSize; i++){
        List->list_arr[i].data = '0';
        List->list_arr[i].next = 0;
        List->list_arr[i].prev = 0;
    }
    List->head = 0;
    List->tail = 0;
    List->free = 0;
    List->lSize = 0;
}

void ConsoleListDump(struct LIST* List){
    ASSERT(List != nullptr);

    printf("\n\t##LIST_DUMP##\n");
    printf("indx is: ");
    for (int i = 0; i < listSize; i++){
        printf("%3d ", i);
    }
    printf("\n");
    for (int i = 0; i < listSize; i++){
        printf("-----");
    }
    printf("\ndata is: ");
    for (int i = 0; i < listSize; i++) {
        printf("%3c ",  List->list_arr[i].data);
    }
    printf("\nnext is: ");
    for (int i = 0; i < listSize; i++) {
        printf("%3d ",  List->list_arr[i].next);
    }
    printf("\nprev is: ");
    for (int i = 0; i < listSize; i++) {
        printf("%3d ",  List->list_arr[i].prev);
    }
    printf("\nhead is: %d\n", List->head);
    printf("tail is: %d\n", List->tail);
    printf("free is: %d\n", List->free);
    printf("size is: %d\n", List->lSize);
    printf("\t##END_DUMP##\n\n");
}

void printList(struct LIST* List){
    ASSERT(List != nullptr);

    printf("\n\t##PRINT_LIST##\n");
    int curIndex = List->head;
    while(curIndex){
        printf("%c ", List->list_arr[curIndex].data);
        curIndex = List->list_arr[curIndex].next;
    }
    printf("\n\t##END_PRINT##\n\n");
}

int getPhysicalByLogical(struct LIST* List, int index){
    ASSERT(List != nullptr);

    if (index > List->lSize){
        printf("there is no such logical index\n");
        return 0;
    }
    int logicalIndex = 1;
    int curIndex = List->head;
    while (logicalIndex < index){
            curIndex = List->list_arr[curIndex].next;
            logicalIndex++;
    }
    return curIndex;
}

int getLogicalByPhysical(struct LIST* List, int index){
    int logicalIndex = 1;
    int curIndex = List->head;
    while (curIndex != index){
            curIndex = List->list_arr[curIndex].next;
            if (curIndex == 0) {
                printf("there is no such physical index\n");
                return 0;
            }
            logicalIndex++;
    }

    return logicalIndex;
}

int getLogicalByData(struct LIST* List, char data){
    int logicalIndex = 1;
    int curIndex = List->head;
    while(List->list_arr[curIndex].data != data){
        curIndex = List->list_arr[curIndex].next;
        if (curIndex == 0) {
            printf("couldn't find such character\n");
            return 0;
        }
        logicalIndex++;
    }

    return logicalIndex;
}

int AssertOk(struct LIST* List){
    ASSERT(List->head != 0);
    ASSERT(List->tail != 0);
    ASSERT(List->lSize != 0);

    int curIndex = List->list_arr[List->head].next;
    while (curIndex != 0){
        int next = List->list_arr[curIndex].next;
        if (next == 0) return 0;

        int prev = List->list_arr[curIndex].prev;
        char data = List->list_arr[curIndex].data;

        if ((List->list_arr[List->list_arr[prev].next].data != data) || List->list_arr[List->list_arr[next].prev].data != data){
            return 1;
        }
        curIndex = List->list_arr[curIndex].next;
    }
    return 0;
}

void GraphListDump(struct LIST* List, const char* str){
    dumpPrint("digraph {\n");
    dumpPrint("rankdir=LR;\n");
    dumpPrint("node [ shape=record ];\n");


    dumpPrint("LIST [label = \"LIST|size: %d\"]\n", List->lSize);
    dumpPrint("LIST -> struct0 [style=\"invis\" weight = 1000]\n");

    for (int i = 0; i < listSize; i++){
        dumpPrint("index%d[label = \"index: %d\"];\n", i, i);
        if (i == 0) continue;
        dumpPrint("index%d -> index%d [style = \"invis\" weight = 200];\n", i-1, i);
    }

    dumpPrint("INDEX -> index0[style = \"invis\" weight = 900]\n");

    for (int i = 0; i < listSize; i++){
        char data = List->list_arr[i].data;
        int next = List->list_arr[i].next;
        char prev = List->list_arr[i].prev;
        char* color = NULL;
        if (i == 0){
            dumpPrint("struct%d [\nlabel = \"<data>data: nil|<next>next: %d|<prev>prev: %d\", style = \"filled\", fillcolor = \"cyan\" \n];\n", i, next, prev);
            continue;
        }
        if (prev != -1)
            color = "green";
        else
            color = "lightgrey";

        dumpPrint("struct%d [\nlabel = \"<data>data: %c|<next>next: %d|<prev>prev: %d\", style = \"filled\", fillcolor = \"%s\" \n];\n", i, data, next, prev, color);
        dumpPrint("struct%d -> struct%d [dir=none weight=100 style=\"invis\"];\n", i-1, i);
    }

    for (int i = 1; i < listSize; i++) {
        int next = List->list_arr[i].next;
        dumpPrint("struct%d:<next> -> struct%d:<prev> [weight = 2] [dir = both]\n", i, next);
    }

    dumpPrint("structH [\nlabel = \"<head>head: %d|<tail>tail: %d|<free>free: %d\"]\n", List->head, List->tail, List->free);
    dumpPrint("structH:<head> -> struct%d:<data> [weight = 0]\n", List->head);
    dumpPrint("structH:<tail> -> struct%d:<data>[weight = 0]\n", List->tail);
    dumpPrint("structH:<free> -> struct%d:<data>[weight = 0]\n", List->free);

    dumpPrint("}");
    fclose(DumpFile);

    printf("%d\n", system("cd C:\\Users\\alexm\\Documents\\List"));
    printf("%d\n", system("dot -T png -o graphCode.png dump.dot"));

}
