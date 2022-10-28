#ifndef DUMP_H_INCLUDED
#define DUMP_H_INCLUDED

//#define NDEBUG
#define DUMP_MODE 1

#ifndef NDEBUG
    #define ASSERT_OK(List)                                                             \
        if (AssertOk(List) || DUMP_MODE)                                                \
        {                                                                               \
            printf("in %s(%d)\n", __PRETTY_FUNCTION__, __LINE__);                       \
            GraphListDump(List, __PRETTY_FUNCTION__);                                   \
            ConsoleListDump(List);                                                      \
        }
#else
    #define ASSERT_OK(List) ;
#endif

#ifndef DUMP_NAME
    #define DUMP_NAME "dump.dot"
#endif

#define dumpPrint(str...)                                                 \
    fprintf(DumpFile, str);                                               \
    fflush(DumpFile)                                                      \

FILE *openDump();
FILE* DumpFile = openDump();

FILE* openDump() {
    FILE *Dump = fopen(DUMP_NAME, "w");
    return Dump;
}

#endif // DUMP_H_INCLUDED
