#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <iostream>
#include <set>
using namespace std;
#define INT_MAX 2147483647
#define MAX 50
struct Record{
    char tconst[10];
    float averageRating;
    int numVotes;
    int indexing;
    struct Record *next;
    Record(){
        next = NULL;
    }

};

struct Block{
    int recordCount;

    struct Record *rootRecord; // root record node
    struct Record *currentRecord; // current record node
    struct Record *nextFreeRecord; // memcpy into this

    struct Block *nextFreeBlock;
    struct Block *nextBlock;
};

struct Mass {
	//number of nodes
	int tNodes;
	//for parent Mass and index
	Mass* parentMass;
	//keys
	float key[MAX];
	Record* index[MAX];
	//child Masss
	Mass* childMass[MAX];
	Mass() { //constructor to initialize a Mass
		tNodes = 0;
		parentMass = NULL;
		for (int i = 0; i < MAX; i++) {
			key[i] = INT_MAX;
            index[i] = NULL;
			childMass[i] = NULL;
		}
	}
	Mass* nextMass = NULL;
};

class Memory
{
    private:

    std::size_t memorySize;
    std::size_t blockSize;

    std::size_t totalMemSizeUsed;
    int maxBlockCount;
    int allocatedBlockCount;
    int usedBlockCount;
    int freeBlockCount;

    struct Block *rootBlockPtr;
    struct Block *currentBlockPtr;
    struct Block *nextFreeBlockPtr;

    Mass* rootMass;
	int numberOfPointers;
	int numberOfNodes;
	int numberOfLevels;
    vector<Mass*> Masses;
    public:
        Memory();
        Memory(std::size_t memorySize, std::size_t blockSize);
        virtual ~Memory();

        bool allocateBlockStruct();
        struct Block *allocateRecordToMem(Record record);
        void deallocateRecord(Record record);

        void displayStats();

        bool memIsFull();

    std::size_t getMemorySize();
    std::size_t getBlockSize();
    std::size_t getTotalMemSizeUsed();

    int getMaxBlockCount();
    int getAllocatedBlockCount();
    int getUsedBlockCount();
    int getFreeBlockCount();

    struct Block* getRootBlockPointer();
    struct Block* getCurrentBlockPointer();

    bool isCurrentBlockFull(std::size_t recordSize);
    void getRecordStats(Record* rec);

    void iterMemory();
    void printBlock(set<int> indexes, bool isEqual);
    void splitLeaf(Mass* curMass);
    void splitNonLeaf(Mass* curMass);
    void insertNode(Mass* curMass, float val, Record * id);
    void printResult(vector<Mass*> Masses, bool bPrint = true);
    void setNOP(int n);
    void resultPrint();
    void readMemory();
    void searchEqual();
    void printNode(Mass* curMass, bool isEqual);
    void search_range(vector<Mass*> Masses);
    void range_search();

    void redistributeMass(Mass* leftMass, Mass* rightMass, bool isLeaf, int posOfLeftMass, int whichOneisCurMass);
    void mergeMass(Mass* leftMass, Mass* rightMass, bool isLeaf, int posOfRightMass);
    void deleteNode(Mass* curMass, float val, int curMassPosition);
    void printResultUpdated(vector<Mass*> Masses, bool bPrint = true);
    void readMemoryUpdated();

};


#endif // MEMORY_H
