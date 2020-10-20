#include "Memory.h"
#include <fstream>
#include <math.h>
#include <cstring>
#include <set>
#include <stdlib.h>
#include <fstream>
#define INT_MAX 2147483647

using namespace std;
float epsilon = 0.005f;
int range_search_node = 0;
int numberOfMerges = 0;
Memory::Memory(){
}

Memory::Memory(std:: size_t memorySize, std::size_t blockSize){
    this->memorySize = memorySize;
    this->blockSize = blockSize;

    this->totalMemSizeUsed = 0;

    this->maxBlockCount = (int)memorySize/blockSize;
    this->allocatedBlockCount = 0;
    //this->usedBlockCount = 0;
    this->freeBlockCount = maxBlockCount;

    this->rootBlockPtr = NULL;
    this->currentBlockPtr = NULL;
    rootMass = new Mass();
}

Memory::~Memory(){
    //dtor
    delete rootBlockPtr;
    rootBlockPtr = NULL;
    currentBlockPtr = NULL;
}

std::size_t Memory::getMemorySize(){
    return memorySize;
}

std::size_t Memory::getBlockSize(){
    return blockSize;
}

std::size_t Memory::getTotalMemSizeUsed(){
    return totalMemSizeUsed;
}

int Memory::getMaxBlockCount(){
    return maxBlockCount;
}

int Memory::getAllocatedBlockCount(){
    return allocatedBlockCount;
}

int Memory::getUsedBlockCount(){
    return usedBlockCount;
}

int Memory::getFreeBlockCount(){
    return freeBlockCount;
}

struct Block* Memory::getRootBlockPointer(){
    return rootBlockPtr;
}

bool Memory::allocateBlockStruct(){
    // Allocate a new block and move free pointer to start of block.

    if(allocatedBlockCount < maxBlockCount){
        Block* newBlock = new Block();
        newBlock->recordCount = 0;
        newBlock->rootRecord = NULL;
        newBlock->currentRecord = NULL;
        newBlock->nextFreeRecord = NULL;
        newBlock->nextBlock = NULL;

        if(rootBlockPtr == NULL){
            rootBlockPtr = newBlock;
            currentBlockPtr = rootBlockPtr;
        }
        else{
            //currentBlockPtr = rootBlockPtr + (blockSize * allocatedBlockCount);
            Block* currBlock = currentBlockPtr;
            currBlock->nextBlock = newBlock;
            currentBlockPtr = newBlock;
        }


        allocatedBlockCount++;
        freeBlockCount--;
        totalMemSizeUsed = totalMemSizeUsed + blockSize;
        return true;
    }
    else{
        std::cout << "--- Error, out of memory for allocation ---\n" ;
        std::cout << "Total Memory Size: " << memorySize << "\n";
        std::cout << "Total Memory Size Used: " << totalMemSizeUsed << "\n";
        return false;
    }
}

struct Block* Memory::allocateRecordToMem(Record record){

    // Checking if the size of record exceeds the memory block size before allocation
    if(sizeof(record) > blockSize){
        std::cout << "--- Error, Record size exceeds memory block size ---\n";
        std::cout << "Record Size: " << sizeof(record) << "\n";
        std::cout << "Memory Block Size: " << blockSize << "\n";
        return NULL;
    }
    else if((allocatedBlockCount == 0 && freeBlockCount != 0)||(isCurrentBlockFull(sizeof(record)))){
        // no allocated blocks && there are still possible blocks to be allocated
        // current block not full
        bool success = allocateBlockStruct();

        if(!success){
            return NULL;
        }
    }

    // Add record
    if(currentBlockPtr->recordCount == 0){
        // root record
        currentBlockPtr->rootRecord = new struct Record();
        memcpy(currentBlockPtr->rootRecord, &record, sizeof(record));
        currentBlockPtr->currentRecord = currentBlockPtr->rootRecord;
        currentBlockPtr->nextFreeRecord = currentBlockPtr->currentRecord->next;
        (currentBlockPtr->recordCount)++;
    }
    else{
        // non root records
        currentBlockPtr->nextFreeRecord = new struct Record();
        memcpy(currentBlockPtr->nextFreeRecord, &record, sizeof(record));
        currentBlockPtr->currentRecord->next = currentBlockPtr->nextFreeRecord;
        currentBlockPtr->currentRecord = currentBlockPtr->nextFreeRecord;
        currentBlockPtr->nextFreeRecord = currentBlockPtr->currentRecord->next;
        (currentBlockPtr->recordCount)++;
    }
    return currentBlockPtr;

}
void Memory::deallocateRecord(Record record){
}

void Memory::displayStats(){
    std::cout << "\n--- Display Statistics ---\n";
    std::cout << "Memory Statistics\n";
    std::cout << "Total Memory Size: " << memorySize << "B\n";
    std::cout << "Total Memory Size Used: " << totalMemSizeUsed << "B\n";

    std::cout << "\n";

    std::cout << "Blocks Statistics\n";
    std::cout << "Max Blocks: " << maxBlockCount << "\n";
    std::cout << "Free Blocks: " << freeBlockCount << "\n";
    std::cout << "Total Allocated Blocks: " << allocatedBlockCount << "\n";
    //std::cout << "Used Blocks: " << usedBlockCount << "\n";
}

bool Memory::memIsFull(){
    if(totalMemSizeUsed + blockSize > memorySize){
        return true;
    }
    else{
        return false;
    }
}
struct Block* Memory::getCurrentBlockPointer(){
    return currentBlockPtr;
}

bool Memory::isCurrentBlockFull(std::size_t recordSize){

    if((currentBlockPtr->recordCount * 20) >= ((int)blockSize)){
    //if (currentBlockPtr->recordCount==5){
        return true;
    }
    else{
        return false;
    }

//    void* tmp1 = thisCurrentBlockPtr-> + recordSize;
//    std::cout<< tmp1 << std::endl;
//    void* tmp2 = thisCurrentBlockPtr + blockSize;
//    std::cout<< tmp2 << std::endl;
//
//    if(tmp1<=tmp2){
//        return false;
//    }
//    else{
//        return true;
//    }
}

void Memory::getRecordStats(Record* rec){
    std::cout << "Record Statistics: \n" << std::endl;
    std::cout << "Tconst (char[10]) Size: " << sizeof(rec->tconst) << std::endl;
    std::cout << "averageRating (float) Size: " << sizeof(rec->averageRating) << std::endl;
    std::cout << "numVotes (int) Size: " << sizeof(rec->numVotes) << std::endl;
    std::cout << "*next (struct Record) Size: " << sizeof(rec->next) << std::endl;
}

void Memory::iterMemory(){
    Block* rootBlock = rootBlockPtr;
    Block* currBlock = rootBlock;
    do{
        Record* rootRecord = currBlock->rootRecord;
        Record* currRecord = rootRecord;
        do{
            std::cout << "Tconst: " << currRecord->tconst << std::endl;
            currRecord = currRecord->next;
        }while(currRecord != NULL);
        std::cout << "\n" << endl;
        currBlock = currBlock->nextBlock;
    }while(currBlock != NULL);


//    for(int i=0; i<currBlock->recordCount;i++){
//
//    }

}

void Memory::printBlock(set <int> indexes, bool isEqual){
    Block* rootBlock = rootBlockPtr;
    Block* currBlock = rootBlock;
    //currBlock = currBlock->nextBlock;

    int i = 0;
    ofstream myfile;
    if (isEqual){
        myfile.open("datablock_1.txt");
    }else{
        myfile.open("datablock_2.txt");
    }
    while(currBlock!=NULL){
        Record* rootRecord = currBlock->rootRecord;
        Record* currRecord = rootRecord;
        i++;
        if(indexes.count(i)){
            while(currRecord!=NULL){
                myfile << "Tconst: " << currRecord->tconst;
                myfile << " averageRating: " << currRecord->averageRating;
                myfile << " numVote: " << currRecord->numVotes << "\n";
                currRecord = currRecord->next;
            }
            myfile << "\n";
        }
        currBlock = currBlock->nextBlock;
    }
    myfile.close();
}

void Memory::splitLeaf(Mass* curMass) {
	int x, i, j;

	//split the greater half to the left when numberOfPointer is odd
	//else split equal equal when numberOfPointer is even
	if (numberOfPointers % 2)
		x = (numberOfPointers + 1) / 2;
	else x = numberOfPointers / 2;

	//we don't declare another Mass for leftMass, rather re-use curMass as leftMass and
	//take away the right half keys to the rightMass
	Mass* rightMass = new Mass();

	//so leftMass has x number of nodes
	curMass->tNodes = x;
	//and rightMass has numberOfPointers-x
	rightMass->tNodes = numberOfPointers - x;
	//so both of them have their common parent [even parent may be null, so both of them will have null parent]
	rightMass->parentMass = curMass->parentMass;

	for (i = x, j = 0; i < numberOfPointers; i++, j++) {
		//take the right-half keys from curMasses and put in the rightMass
		rightMass->key[j] = curMass->key[i];
		rightMass->index[j] = curMass->index[i];
		//and erase right-half keys from curMass to make it real leftMass
		//so that it does not contain all keys only contains left-half keys
		curMass->key[i] = INT_MAX;
		curMass->index[i] = NULL;
	}
	//for splitting the leaf Masses we copy the first item from the rightMass to their parentMass
	//and val contains that key
	float val = rightMass->key[0];
	Record* id = rightMass->index[0];

	// add by zc
	Mass* tmp = curMass->nextMass;
	curMass->nextMass = rightMass;
	rightMass->nextMass = tmp;

	//if the leaf itself is a parent then
	if (curMass->parentMass == NULL) {
		//it has null parent, so create a new parent
		Mass* parentMass = new Mass();
		//and new parent should have a null parent
		parentMass->parentMass = NULL;
		//new parent will have only one member
		parentMass->tNodes = 1;
		//and that member is val
		parentMass->key[0] = val;
		parentMass->index[0] = id;
		//so the parent has two child, so assign them (don't forget curMass is actually the leftMass)
		parentMass->childMass[0] = curMass;
		parentMass->childMass[1] = rightMass;
		//their parent of the left and right Masses is no longer null, so assign their parent
		curMass->parentMass = rightMass->parentMass = parentMass;
		//from now on this parentMass is the rootMass
		rootMass = parentMass;
		return;
	}
	else {   //if the splitted leaf Mass is not rootMass then

		// we have to put the val and assign the rightMass to the right place in the parentMass
		// so we go to the parentMass and from now we consider the curMass as the parentMass of the splitted Mass

		curMass = curMass->parentMass;

		//for the sake of insertNodeion sort to put the rightMass and val in the exact position
		//of th parentMass [here curMass] take a new child Mass and assign rightMass to it
		Mass* newChildMass = new Mass();
		newChildMass = rightMass;

		//simple insertion sort to put val at the exact position of keys[] in the parentMass [here curMass]

		for (i = 0; i <= curMass->tNodes; i++) {
			if (val < curMass->key[i]) {
				swap(curMass->key[i], val);
				swap(curMass->index[i], id);
			}
		}

		//after putting val number of nodes gets increase by one
		curMass->tNodes++;

		//simple insertNodeion sort to put rightMass at the exact position
		//of childMass[] in the parentMass [here curMass]

		for (i = 0; i < curMass->tNodes; i++) {
			if (newChildMass->key[0] < curMass->childMass[i]->key[0]) {
				swap(curMass->childMass[i], newChildMass);
			}
		}
		curMass->childMass[i] = newChildMass;

		//we reordered some Masses and pointers, so for the sake of safety
		//all childMasses' should have their parent updated
		for (i = 0; curMass->childMass[i] != NULL; i++) {
			curMass->childMass[i]->parentMass = curMass;
		}
	}
	// add by zc not sure correct
	// curMass->nextMass = rightMass;



}

//function to split the non leaf nodes
void Memory::splitNonLeaf(Mass* curMass) {
	int x, i, j;

	//split the less half to the left when numberOfPointer is odd
	//else split equal equal when numberOfPointer is even.  n/2 does it nicely for us

	x = numberOfPointers / 2;

	//declare rightMass and we will use curMass as the leftMass
	Mass* rightMass = new Mass();

	//so leftMass has x number of nodes
	curMass->tNodes = x;
	//rightMass has numberOfPointers-x-1 children, because we won't copy and paste
	//rather delete and paste the first item of the rightMass
	rightMass->tNodes = numberOfPointers - x - 1;
	//both children have their common parent
	rightMass->parentMass = curMass->parentMass;


	for (i = x, j = 0; i <= numberOfPointers; i++, j++) {
		//copy the right-half members to the rightMass
		rightMass->key[j] = curMass->key[i];
		rightMass->index[j] = curMass->index[i];
		//and also copy their children
		rightMass->childMass[j] = curMass->childMass[i];
		//erase the right-half keys from curMass to make it perfect leftMass
		//which won't contain only left-half keys and their children
		curMass->key[i] = INT_MAX;
		curMass->index[i] = NULL;
		//erase all the right-half childMasses from curMass except the x one
		//because if left child has 3 nodes then it should have 4 childMasses, so don't delete that child
		if (i != x)curMass->childMass[i] = NULL;
	}

	//we will take a copy of the first item of the rightMass
	//as we will delete that item later from the list
	float val = rightMass->key[0];
	Record* id = rightMass->index[0];
	//just right-shift key[] and childMass[] by one from rightMass
	//to have no repeat of the first item for non-leaf Mass
	memcpy(&rightMass->key, &rightMass->key[1], sizeof(float) * (rightMass->tNodes + 1));
	memcpy(&rightMass->index, &rightMass->index[1], sizeof(long) * (rightMass->tNodes + 1));
	memcpy(&rightMass->childMass, &rightMass->childMass[1], sizeof(rootMass) * (rightMass->tNodes + 1));

	//we reordered some keys and positions so don't forget
	//to assign the children's exact parent

	for (i = 0; curMass->childMass[i] != NULL; i++) {
		curMass->childMass[i]->parentMass = curMass;
	}
	for (i = 0; rightMass->childMass[i] != NULL; i++) {
		rightMass->childMass[i]->parentMass = rightMass;
	}


	//if the splitted Mass itself a parent
	if (curMass->parentMass == NULL) {
		//create a new parent
		Mass* parentMass = new Mass();
		//parent should have a null parent
		parentMass->parentMass = NULL;
		//parent will have only one node
		parentMass->tNodes = 1;
		//the only key is the val
		parentMass->key[0] = val;
		parentMass->index[0] = id;
		//it has two children, leftMass and rightMass
		parentMass->childMass[0] = curMass;
		parentMass->childMass[1] = rightMass;

		//and both rightMass and leftMass has no longer null parent, they have their new parent
		curMass->parentMass = rightMass->parentMass = parentMass;

		//from now on this new parent is the root parent
		rootMass = parentMass;
		return;
	}
	else {   //if the splitted leaf Mass is not rootMass then

		// we have to put the val and assign the rightMass to the right place in the parentMass
		// so we go to the parentMass and from now we consider the curMass as the parentMass of the splitted Mass
		curMass = curMass->parentMass;

		//for the sake of insertNodeion sort to put the rightMass and val in the exact position
		//of th parentMass [here curMass] take a new child Mass and assign rightMass to it

		Mass* newChildMass = new Mass();
		newChildMass = rightMass;

		//simple insertion sort to put val at the exact position of keys[] in the parentMass [here curMass]


		for (i = 0; i <= curMass->tNodes; i++) {
			if (val < curMass->key[i]) {
				swap(curMass->key[i], val);
				swap(curMass->index[i], id);
			}
		}

		//after putting val number of nodes gets increase by one
		curMass->tNodes++;

		//simple insertNodeion sort to put rightMass at the exact position
		 //of childMass[] in the parentMass [here curMass]

		for (i = 0; i < curMass->tNodes; i++) {
			if (newChildMass->key[0] < curMass->childMass[i]->key[0]) {
				swap(curMass->childMass[i], newChildMass);
			}
		}
		curMass->childMass[i] = newChildMass;

		//we reordered some Masses and pointers, so for the sake of safety
		//all childMasses' should have their parent updated
		for (i = 0; curMass->childMass[i] != NULL; i++) {
			curMass->childMass[i]->parentMass = curMass;
		}
	}

}

void Memory::insertNode(Mass* curMass, float val, Record* id) {

	for (int i = 0; i <= curMass->tNodes; i++) {
		if (val < curMass->key[i] && curMass->childMass[i] != NULL) {
			insertNode(curMass->childMass[i], val, id);
			if (curMass->tNodes == numberOfPointers)
				splitNonLeaf(curMass);
			return;
		}
		else if (val < curMass->key[i] && curMass->childMass[i] == NULL) {
			swap(curMass->key[i], val);
			swap(curMass->index[i], id);
			if (i == curMass->tNodes) {
				curMass->tNodes++;
				break;
			}
		}
	}

	if (curMass->tNodes == numberOfPointers) {
		splitLeaf(curMass);
	}
}

void Memory::printResult(vector<Mass*> Masses, bool bPrint) {
	vector < Mass* > newMasses;
	for (int i = 0; i < Masses.size(); i++) { //for every Mass
		Mass* curMass = Masses[i];

		if(bPrint) cout << "[|";
		int j;
		for (j = 0; j < curMass->tNodes; j++) {  //traverse the childMasses, print keys and save all the childMasses

			if (bPrint) cout << " " << curMass->index[j]->tconst << "," << curMass->key[j] << ","
				<< curMass->index[j]->numVotes << " |";

			if (curMass->childMass[j] != NULL) {
				newMasses.push_back(curMass->childMass[j]);
			}
		}
		if (curMass->key[j] == INT_MAX && curMass->childMass[j] != NULL)
			newMasses.push_back(curMass->childMass[j]);

		if (bPrint) cout << "]  ";
		if (!bPrint) numberOfNodes++;
	}

	if (bPrint) cout << endl << endl;
	if (Masses.size() > 0) if (!bPrint) numberOfLevels++;

	if (newMasses.size() == 0) { //if there is no childMass Mass left to send out then just the end of the recursion
		Masses.clear();
	}
	else {                    //else send the childMasses to the recursion to continue to the more depth
		Masses.clear();
		printResult(newMasses, bPrint);
	}
}

void Memory::search_range(vector < Mass* > Masses) {
    float x = 7.0;
    float y = 9.0;
    bool found = false;
    int equal_search_node = 0;
    cout << endl << endl;
	cout << "Displaying results for search averageRating between 7.0 and 9.0" << endl;
	Mass* curMass = Masses[0];
	// WHILE NON leaf node
	while (curMass->childMass[0]!=NULL){
	    printNode(curMass, false);
		for (int i = 0; i < curMass->tNodes; i++){
		    if (curMass->parentMass!=NULL){
                if (curMass->key[i] >= x){
                    curMass = curMass->childMass[i];
                    equal_search_node++;
                    break;
                }
            }else{
                if (curMass->key[i] > x){
                    curMass = curMass->childMass[i];
                    equal_search_node++;
                    break;
                }
            }
			if (i==curMass->tNodes-1){
				curMass = curMass->childMass[i+1];
				equal_search_node++;
				break;
			}
		}
		//cout << curBlock->key[0] <<endl;
	}
	printNode(curMass, false);
	vector <string> ttconsts;
	equal_search_node++;
	vector <int> indexings;
	set <int> blockIndexs;
	int i = 0;
	while (i<curMass->tNodes){
        if (curMass->key[i] > y){
            break;
        }
        if (curMass->key[i] >= x){
            found = true;
            ttconsts.push_back(curMass->index[i]->tconst);
            indexings.push_back(curMass->index[i]->indexing);
		}
		i++;
		if (i==curMass->tNodes){
            equal_search_node++;
            curMass = curMass->nextMass;
            printNode(curMass, false);
            i = 0;
		}
	}
	if(!found){
        cout << "Not found\n";
	}else{
        for(int i=0;i<ttconsts.size();i++){
            int blockIndex = ceil(indexings[i]/5.0);

            blockIndexs.insert(blockIndex);
            cout << ttconsts[i] <<endl;
        }
	}
	cout << "number of index nodes accessed: " << equal_search_node << endl;
	cout << "number of data blocks accessed: " << blockIndexs.size() << endl;
	cout << "content of index nodes and data blocks are saved into txt files respectively!" << endl;
	cout << "please refer to datablock_2.txt and indexnode_2.txt" << endl;
	printBlock(blockIndexs, false);
}

void Memory::printNode(Mass* curMass, bool isEqual){
    ofstream myfile;
    if(isEqual){
        myfile.open("indexnode_1.txt", ios_base::app);
    }else{
        myfile.open("indexnode_2.txt", ios_base::app);
    }
    myfile << "accessing node: " << endl;
    for(int i = 0; i < curMass->tNodes; i++){
        myfile << curMass->index[i]->tconst << ' ' << curMass->index[i]->averageRating << ' ' << curMass->index[i]->numVotes << endl;
    }
    myfile.close();
}

void Memory::searchEqual(){
    float x = 8.0;
    bool found = false;
    int equal_search_node = 0;
    cout << endl << endl;
	cout << "Displaying results for search averageRating=8.0" << endl;
	Mass* curMass = Masses[0];
	// WHILE NON leaf node
	while (curMass->childMass[0]!=NULL){
	    printNode(curMass, true);
		for (int i = 0; i < curMass->tNodes; i++){
		    if (curMass->parentMass!=NULL){
                if (curMass->key[i] >= x){
                    curMass = curMass->childMass[i];
                    equal_search_node++;
                    break;
                }
            }else{
                if (curMass->key[i] > x){
                    curMass = curMass->childMass[i];
                    equal_search_node++;
                    break;
                }
            }
			if (i==curMass->tNodes-1){
				curMass = curMass->childMass[i+1];
				equal_search_node++;
				break;
			}
		}
		//cout << curBlock->key[0] <<endl;
	}
	printNode(curMass, true);
	vector <string> ttconsts;
	vector <int> indexings;
	equal_search_node++;
	set <int> blockIndexs;
	int i = 0;
	while (i<curMass->tNodes){
        if (curMass->key[i] > x){
            break;
        }
        if (fabs(curMass->key[i]-x) < epsilon){
            ttconsts.push_back(curMass->index[i]->tconst);
            indexings.push_back(curMass->index[i]->indexing);
			found = true;
		}
		i++;
		if (i==curMass->tNodes){
            equal_search_node++;
            curMass = curMass->nextMass;
            if (curMass == NULL){
                break;
            }
            printNode(curMass, true);
            i = 0;
		}
	}
	if(!found){
        cout << "Not found\n";
	}else{
        // cout << ttconsts.size() << endl;
        cout << "values of tconst found: "<<endl;
        for(int i=0;i<ttconsts.size();i++){
            //int tconstint = stoi(ttconsts[i].substr(2,10));
            //int blockIndex = ceil(tconstint/5.0);
            int blockIndex = ceil(indexings[i]/5.0);
            blockIndexs.insert(blockIndex);
            cout << ttconsts[i] <<endl;
        }
	}

	cout << "number of index nodes accessed: " << equal_search_node << endl;
	cout << "number of data blocks accessed: " << blockIndexs.size() << endl;
	cout << "content of index nodes and data blocks are saved into txt files respectively!" << endl;
	cout << "please refer to datablock_1.txt and indexnode_1.txt" << endl;
	printBlock(blockIndexs, true);
}


void Memory::range_search(){
    Masses.clear();
	Masses.push_back(rootMass);
	search_range(Masses);
}

void Memory::setNOP(int n) {
	numberOfPointers = n;
}

void Memory::resultPrint() {
	Masses.clear();
	Masses.push_back(rootMass);
	printResult(Masses);
}

void Memory::readMemory() {
	Block* rootBlock = rootBlockPtr;
	Block* currBlock = rootBlock;
	do {
		Record* rootRecord = currBlock->rootRecord;
		Record* currRecord = rootRecord;
		do {
			insertNode(rootMass, currRecord->averageRating, currRecord);
			currRecord = currRecord->next;
		} while (currRecord != NULL);
		currBlock = currBlock->nextBlock;
	} while (currBlock != NULL);

	numberOfLevels = 0;
	numberOfNodes = 0;
	Masses.clear();
	Masses.push_back(rootMass);
	printResult(Masses, false);
	cout << endl << endl;
	cout << "- the parameter n of the B+ tree : " << numberOfPointers - 1 << endl;
	cout << "- the number of nodes of the B+ tree : " << numberOfNodes << endl;
	cout << "- the height of the B+ tree : " << numberOfLevels << endl << "- root node : ";
	vector < Mass* > newMasses;
	Mass* curMass = rootMass;
	cout << "[|";
	int j;
	for (j = 0; j < curMass->tNodes; j++) {  //traverse the childMasses, print keys and save all the childMasses

		cout << " " << curMass->index[j]->tconst << "," << curMass->key[j] << ","
			<< curMass->index[j]->numVotes << " |";

		if (curMass->childMass[j] != NULL) {
			newMasses.push_back(curMass->childMass[j]);
		}
	}
	if (curMass->key[j] == INT_MAX && curMass->childMass[j] != NULL)
		newMasses.push_back(curMass->childMass[j]);
	cout << "]  " << endl << "- it's children : ";
	for (int i = 0; i < newMasses.size(); i++) { //for every Mass
		Mass* curMass = newMasses[i];
		cout << "[|";
		int j;
		for (j = 0; j < curMass->tNodes; j++) {  //traverse the childMasses, print keys and save all the childMasses
			cout << " " << curMass->index[j]->tconst << "," << curMass->key[j] << ","
				<< curMass->index[j]->numVotes << " |";
		}
		cout << "]  ";
	}
	cout << endl << endl;
}

void  Memory::redistributeMass(Mass* leftMass, Mass* rightMass, bool isLeaf, int posOfLeftMass, int whichOneisCurMass) {

	//re-distribution will affect the first key of the rightMass, so remember it
	//for later replacement of the copy of this key somewhere in ancestor Mass
	int RightFirstValue = rightMass->key[0];

	if (whichOneisCurMass == 0) { //leftMass is curMass

		//if the Mass are not leaf node
		if (!isLeaf) {
			//bring down the key from which it is left child in parentMass
			leftMass->key[leftMass->tNodes] = leftMass->parentMass->key[posOfLeftMass];
			leftMass->index[leftMass->tNodes] = leftMass->parentMass->index[posOfLeftMass];
			//the right child of newly added key should be the left child of first key of rightMass
			leftMass->childMass[leftMass->tNodes + 1] = rightMass->childMass[0];
			//increase leftMass's number of nodes by one
			leftMass->tNodes++;
			//send up a the first key of the rightMass to the parentMass
			leftMass->parentMass->key[posOfLeftMass] = rightMass->key[0];
			leftMass->parentMass->index[posOfLeftMass] = rightMass->index[0];
			//shift left by one in rightMass
			memcpy(&rightMass->key[0], &rightMass->key[1], sizeof(float) * (rightMass->tNodes + 1));
			memcpy(&rightMass->index[0], &rightMass->index[1], sizeof(long) * (rightMass->tNodes + 1));
			memcpy(&rightMass->childMass[0], &rightMass->childMass[1], sizeof(rootMass) * (rightMass->tNodes + 1));
			rightMass->tNodes--;

		}
		else {
			//borrow the first key of rightMass to the last position of leftMass
			leftMass->key[leftMass->tNodes] = rightMass->key[0];
			leftMass->index[leftMass->tNodes] = rightMass->index[0];
			leftMass->tNodes++;
			//shift by one node to left of the rightMass
			memcpy(&rightMass->key[0], &rightMass->key[1], sizeof(float) * (rightMass->tNodes + 1));
			memcpy(&rightMass->index[0], &rightMass->index[1], sizeof(long) * (rightMass->tNodes + 1));
			//decrease number of nodes by one
			rightMass->tNodes--;

			leftMass->parentMass->key[posOfLeftMass] = rightMass->key[0];
			leftMass->parentMass->index[posOfLeftMass] = rightMass->index[0];
		}



	}
	else { //rightMass is curMass

		if (!isLeaf) {

			//shift right by one in rightMass so that first position becomes free
			memcpy(&rightMass->key[1], &rightMass->key[0], sizeof(float) * (rightMass->tNodes + 1));
			memcpy(&rightMass->index[1], &rightMass->index[0], sizeof(long) * (rightMass->tNodes + 1));
			memcpy(&rightMass->childMass[1], &rightMass->childMass[0], sizeof(rootMass) * (rightMass->tNodes + 1));
			//bring down the key from which it is left child in parentMass to first pos of rightMass
			rightMass->key[0] = leftMass->parentMass->key[posOfLeftMass];
			rightMass->index[0] = leftMass->parentMass->index[posOfLeftMass];
			//and the left child of the newly first key of right child will be the last child of leftMass
			rightMass->childMass[0] = leftMass->childMass[leftMass->tNodes];

			rightMass->tNodes++;

			//send up a the last key of the leftMass to the parentMass
			leftMass->parentMass->key[posOfLeftMass] = leftMass->key[leftMass->tNodes - 1];
			leftMass->parentMass->index[posOfLeftMass] = leftMass->index[leftMass->tNodes - 1];
			//erase the last element and pointer of leftMass
			leftMass->key[leftMass->tNodes - 1] = INT_MAX;
			leftMass->index[leftMass->tNodes - 1] = NULL;
			leftMass->childMass[leftMass->tNodes] = NULL;
			leftMass->tNodes--;

		}
		else {

			//shift by one node to right of the rightMass so that we can free the first position
			memcpy(&rightMass->key[1], &rightMass->key[0], sizeof(float) * (rightMass->tNodes + 1));
			memcpy(&rightMass->index[1], &rightMass->index[0], sizeof(long) * (rightMass->tNodes + 1));
			//borrow the last key of leftMass to the first position of rightMass
			rightMass->key[0] = leftMass->key[leftMass->tNodes - 1];
			rightMass->index[0] = leftMass->index[leftMass->tNodes - 1];
			//increase number of nodes by one
			rightMass->tNodes++;

			leftMass->key[leftMass->tNodes - 1] = INT_MAX;
			leftMass->index[leftMass->tNodes - 1] = NULL;
			leftMass->tNodes--;

			leftMass->parentMass->key[posOfLeftMass] = rightMass->key[0];
			leftMass->parentMass->index[posOfLeftMass] = rightMass->index[0];
		}
	}
}


void  Memory::mergeMass(Mass* leftMass, Mass* rightMass, bool isLeaf, int posOfRightMass) {

	//cout << "leftMass " << leftMass->key[0] << " rightMass " << rightMass->key[0] << endl;
	//cout << "size " << leftMass->tNodes << " size " << rightMass->tNodes << endl;
	if (!isLeaf) {

		leftMass->key[leftMass->tNodes] = leftMass->parentMass->key[posOfRightMass - 1];
		leftMass->index[leftMass->tNodes] = leftMass->parentMass->index[posOfRightMass - 1];
		leftMass->tNodes++;
	}

	memcpy(&leftMass->key[leftMass->tNodes], &rightMass->key[0], sizeof(float) * (rightMass->tNodes + 1));
	memcpy(&leftMass->index[leftMass->tNodes], &rightMass->index[0], sizeof(long) * (rightMass->tNodes + 1));
	memcpy(&leftMass->childMass[leftMass->tNodes], &rightMass->childMass[0], sizeof(rootMass) * (rightMass->tNodes + 1));


	leftMass->tNodes += rightMass->tNodes;


	//cout << "before: " << leftMass->parentMass->key[1] << endl;
	memcpy(&leftMass->parentMass->key[posOfRightMass - 1], &leftMass->parentMass->key[posOfRightMass], sizeof(float) * (leftMass->parentMass->tNodes + 1));
	memcpy(&leftMass->parentMass->index[posOfRightMass - 1], &leftMass->parentMass->index[posOfRightMass], sizeof(long) * (leftMass->parentMass->tNodes + 1));
	memcpy(&leftMass->parentMass->childMass[posOfRightMass], &leftMass->parentMass->childMass[posOfRightMass + 1], sizeof(rootMass) * (leftMass->parentMass->tNodes + 1));
	//cout << "after merging " << leftMass->parentMass->childMass[posOfRightMass-2]->key[0] << " and ";// << leftMass->parentMass->childMass[posOfRightMass]->key[0] << endl;
	leftMass->parentMass->tNodes--;

	//we reordered some Masss and pointers, so for the sake of safety
	//all childMasss' should have their parent updated
	for (int i = 0;leftMass->childMass[i] != NULL;i++) {
		leftMass->childMass[i]->parentMass = leftMass;
	}


}
bool dataFound;
void  Memory::deleteNode(Mass* curMass, float val, int curMassPosition) {

	//to check if the current Mass is a leaf or not
	bool isLeaf;
	if (curMass->childMass[0] == NULL)
		isLeaf = true;
	else isLeaf = false;

	//left most key could be changed due to merge or re-distribution later,
	//so keep it to replace it's copy from it's ancestor
	int LeftMostValue = curMass->key[0];



	for (int i = 0;dataFound == false && i <= curMass->tNodes; i++) {
		if (val < curMass->key[i] && curMass->childMass[i] != NULL) {
			deleteNode(curMass->childMass[i], val, i);
			numberOfMerges++;

		}
		//if we could find the target key at any leafMass then
		else if (val == curMass->key[i] && curMass->childMass[i] == NULL) {

			//delete the node by shifting all values and pointers  by one to the left
			memcpy(&curMass->key[i], &curMass->key[i + 1], sizeof(float) * (curMass->tNodes + 1));
			memcpy(&curMass->index[i], &curMass->index[i + 1], sizeof(long) * (curMass->tNodes + 1));
			//decrease number of nodes by one
			curMass->tNodes--;
			dataFound = true;
			break;
		}
	}

	//if the root is the only leaf
	if (curMass->parentMass == NULL && curMass->childMass[0] == NULL) {
		return;
	}


	//if the curMass is rootMass and it has one pointers only
	if (curMass->parentMass == NULL && curMass->childMass[0] != NULL && curMass->tNodes == 0) {
		rootMass = curMass->childMass[0];
		rootMass->parentMass = NULL;
		return;
	}


	//now check if the curMass has less than half of the number of maximum node
	//cout << curMassPosition << endl;
	//if less than half we will try to re-distribute first

	//cout << curMass->childMass[0]->key[0] << " "<< curMassPosition << endl;
	if (isLeaf && curMass->parentMass != NULL) {

		if (curMassPosition == 0) {
			Mass* rightMass = new Mass();
			rightMass = curMass->parentMass->childMass[1];

			//if we the right one has more than half nodes of maximum capacity than re-distribute
			if (rightMass != NULL && rightMass->tNodes > (numberOfPointers + 1) / 2) {

				redistributeMass(curMass, rightMass, isLeaf, 0, 0);
			}
			//else there is nothing to re-distribute, so we can merge them
			else if (rightMass != NULL && curMass->tNodes + rightMass->tNodes < numberOfPointers) {

				mergeMass(curMass, rightMass, isLeaf, 1);
				numberOfMerges++;
			}
		}

		else {


			Mass* leftMass = new Mass();
			Mass* rightMass = new Mass();


			leftMass = curMass->parentMass->childMass[curMassPosition - 1];

			rightMass = curMass->parentMass->childMass[curMassPosition + 1];


			//if we see that left one has more than half nodes of maximum capacity then try to re-distribute
			if (leftMass != NULL && leftMass->tNodes > (numberOfPointers + 1) / 2) {
				redistributeMass(leftMass, curMass, isLeaf, curMassPosition - 1, 1);
			}
			else if (rightMass != NULL && rightMass->tNodes > (numberOfPointers + 1) / 2) {
				redistributeMass(curMass, rightMass, isLeaf, curMassPosition, 0);
			}
			else if (leftMass != NULL && curMass->tNodes + leftMass->tNodes < numberOfPointers) {
				mergeMass(leftMass, curMass, isLeaf, curMassPosition);
				numberOfMerges++;
			}
			else if (rightMass != NULL && curMass->tNodes + rightMass->tNodes < numberOfPointers) {
				mergeMass(curMass, rightMass, isLeaf, curMassPosition + 1);
				numberOfMerges++;

			}
		}
	}
	else if (!isLeaf && curMass->parentMass != NULL) {

		if (curMassPosition == 0) {
			Mass* rightMass = new Mass();
			rightMass = curMass->parentMass->childMass[1];

			//if we see the right one has more than half nodes of maximum capacity than re-distribute
			if (rightMass != NULL && rightMass->tNodes - 1 >= ceil((numberOfPointers - 1) / 2)) {
				redistributeMass(curMass, rightMass, isLeaf, 0, 0);
			}
			//else there is nothing to re-distribute, so we can merge them
			else if (rightMass != NULL && curMass->tNodes + rightMass->tNodes < numberOfPointers - 1) {
				mergeMass(curMass, rightMass, isLeaf, 1);
				numberOfMerges++;
			}
		}
		//for any other case we can safely take the left one to try for re-distribution
		else {


			Mass* leftMass = new Mass();
			Mass* rightMass = new Mass();


			leftMass = curMass->parentMass->childMass[curMassPosition - 1];

			rightMass = curMass->parentMass->childMass[curMassPosition + 1];


			//if we see that left one has more than half nodes of maximum capacity then try to re-distribute
			if (leftMass != NULL && leftMass->tNodes - 1 >= ceil((numberOfPointers - 1) / 2)) {
				redistributeMass(leftMass, curMass, isLeaf, curMassPosition - 1, 1);
			}
			else if (rightMass != NULL && rightMass->tNodes - 1 >= ceil((numberOfPointers - 1) / 2)) {
				redistributeMass(curMass, rightMass, isLeaf, curMassPosition, 0);
			}
			//else there is nothing to re-distribute, so we merge them

			else if (leftMass != NULL && curMass->tNodes + leftMass->tNodes < numberOfPointers - 1) {
				mergeMass(leftMass, curMass, isLeaf, curMassPosition);
				numberOfMerges++;
			}
			else if (rightMass != NULL && curMass->tNodes + rightMass->tNodes < numberOfPointers - 1) {
				mergeMass(curMass, rightMass, isLeaf, curMassPosition + 1);
				numberOfMerges++;
			}
		}

	}

	//delete the duplicate if any in the ancestor Mass
	Mass* tempMass = new Mass();
	tempMass = curMass->parentMass;
	while (tempMass != NULL) {
		for (int i = 0; i < tempMass->tNodes;i++) {
			if (tempMass->key[i] == LeftMostValue) {
				tempMass->key[i] = curMass->key[0];
				tempMass->index[i] = curMass->index[0];
				break;
			}
		}
		tempMass = tempMass->parentMass;
	}

}


void Memory::printResultUpdated(vector<Mass*> Masses, bool bPrint) {
	vector < Mass* > newMasses;
	for (int i = 0; i < Masses.size(); i++) { //for every Mass
		Mass* curMass = Masses[i];

		if (bPrint) cout << "[|";
		int j;
		for (j = 0; j < curMass->tNodes; j++) {  //traverse the childMasses, print keys and save all the childMasses

			if (bPrint) cout << " " << curMass->index[j]->tconst << "," << curMass->key[j] << ","
				<< curMass->index[j]->numVotes << " |";

			if (curMass->childMass[j] != NULL) {
				newMasses.push_back(curMass->childMass[j]);
			}
		}
		if (curMass->key[j] == INT_MAX && curMass->childMass[j] != NULL)
			newMasses.push_back(curMass->childMass[j]);

		if (bPrint) cout << "]  ";
		if (!bPrint) numberOfNodes++;
	}

	if (bPrint) cout << endl << endl;
	if (Masses.size() > 0) if (!bPrint) numberOfLevels++;

	if (newMasses.size() == 0) { //if there is no childMass Mass left to send out then just the end of the recursion
		Masses.clear();
	}
	else {                    //else send the childMasses to the recursion to continue to the more depth
		Masses.clear();
		printResult(newMasses, bPrint);
	}
}

void Memory::readMemoryUpdated() {
	Mass* curMass = Masses[0];
	float val = 7.0;
	int curMassPosition = 0;

	deleteNode(curMass, val, curMassPosition);

	numberOfLevels = 0;
	numberOfNodes = 0;
	Masses.clear();
	Masses.push_back(rootMass);
	printResultUpdated(Masses, false);
	cout << endl << endl;
	cout << "- number of times that a node is deleted(or two nodes are merged) during updating : " << numberOfMerges << endl;
	cout << "- the number of nodes of the updated B+ tree : " << numberOfNodes << endl;
	cout << "- the height of the updated B+ tree : " << numberOfLevels << endl << "- root node of the updated B+ tree: ";
	vector < Mass* > newMasses;
	//Mass* curMass = rootMass;
	cout << "[|";
	int j;
	for (j = 0; j < curMass->tNodes; j++) {  //traverse the childMasses, print keys and save all the childMasses

		cout << " " << curMass->index[j]->tconst << "," << curMass->key[j] << ","
			<< curMass->index[j]->numVotes << " |";

		if (curMass->childMass[j] != NULL) {
			newMasses.push_back(curMass->childMass[j]);
		}
	}
	if (curMass->key[j] == INT_MAX && curMass->childMass[j] != NULL)
		newMasses.push_back(curMass->childMass[j]);
	cout << "]  " << endl << "- it's children of the updated B+ tree: ";
	for (int i = 0; i < newMasses.size(); i++) { //for every Mass
		Mass* curMass = newMasses[i];
		cout << "[|";
		int j;
		for (j = 0; j < curMass->tNodes; j++) {  //traverse the childMasses, print keys and save all the childMasses
			cout << " " << curMass->index[j]->tconst << "," << curMass->key[j] << ","
				<< curMass->index[j]->numVotes << " |";
		}
		cout << "]  ";
	}
	cout << endl << endl;
}
