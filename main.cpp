#include <iostream>
#include <fstream>
#include <string>
#include "Memory.h"
Memory* initMemory();
void readFromFile();
void displayMainMenu();
void loadIntoMemory(Memory* storageMem);
int getInput();
vector <Record> recordList;
Memory* storageMem;

int main()
{
    int selection = 0;
    do {
        displayMainMenu();
        cin >> selection;

        switch (selection) {
        case 1:
            cout << "\n Initialise Memory + Read File + Load to Memory\n";
            storageMem = initMemory();
            readFromFile();
            loadIntoMemory(storageMem);
            break;
        case 2:
            cout << "\n Display Record Statistics\n";
            storageMem->getRecordStats(storageMem->getRootBlockPointer()->rootRecord);
            //storageMem->iterMemory();
            //initBPlusTree(storageMem);
            break;
        case 3:
        {
			cout << "\n Insert the memory data into B+ Tree...\n";
			cout << "number of Pointers: ";
            int numberOfPointers;
			cin >> numberOfPointers;
            storageMem->setNOP(numberOfPointers);
            storageMem->readMemory();
			break;
        }
        case 4:
            cout << "\n Print B+ Tree...\n";
            storageMem->resultPrint();
            break;
        case 5:
            cout << "\n Search for value...\n";
            storageMem->searchEqual();
            break;
        case 6:
            cout << "\n Search for range between 7 and 9 ..\n";
            storageMem->range_search();
            break;
        case 7:
            cout << "\n Delete the memory data from the B+ Tree...\n";
            cout << "delete movies with averageRating = 7.0: ";
            storageMem->readMemoryUpdated();
            break;
        case 0:
            cout << "\n To exit the menu";
            return 0;
            break;
        default:
            cout << "\n Invalid selection";
        }
    } while (selection > 0 && selection <= 11);


    cout << "THATS ALL FOLKS!" << endl;
    return 0;
}

Memory* initMemory() {
    bool initSuccess = false;

    cout << "\n-- Initialising Memory --" << endl;
    size_t memSize = 1024 * 1024 * 500;
    cout << "Memory Size Checked!" << endl;
    size_t blockSize = 500;
    cout << "Record Block Size Checked!" << endl;
    Memory* memory = new Memory(memSize, blockSize);
    cout << "Memory Database Created!" << endl;

    cout << "\n-- Mem Data Check --" << endl;
    cout << "Memory Size: " << memory->getMemorySize() << endl;
    cout << "Block Size: " << memory->getBlockSize() << endl;

    initSuccess = true;
    if (initSuccess == true) {
        cout << "Memory Successfully Initialised!\n" << endl;
        cout << "Max Block Count: " << memory->getMaxBlockCount() << endl;
        return memory;
    }
    else {
        cout << "Error! Memory Initialisation Failed!\n" << endl;
        return NULL;
    }
}

void readFromFile() {
    int recCount = 0;

    ifstream dataFile("data.tsv");
    if (!dataFile.is_open())
    {
        cout << "Failed to open file 'data.tsv'" << endl;
    }
    else
    {
        Record record;

        string line;
        getline(dataFile, line); // reading first line and skipping

        while (dataFile >> record.tconst >> record.averageRating >> record.numVotes)
        {
            /* FOR TESTING PURPOSES
            cout << "\n Record Check: \n";
            cout << record.tconst << endl;
            cout << record.averageRating << endl;
            cout << record.numVotes << endl;
            */
            record.indexing = recCount+1;
            recordList.push_back(record);
            recCount++;
        }

        cout << "\n-- Read Data Check --" << endl;
        cout << "Record Count: " << recCount << endl;
    }
}

void displayMainMenu() {
    cout << "\n Menu";
    cout << "\n====Database Storage====";
    cout << "\n 1 - Initialise Memory + Read File + Load to Memory";
    cout << "\n 2 - Display Record Statistics";
	cout << "\n========Insertion=======";
	cout << "\n 3 - Insertion";
	cout << "\n 4 - Print Tree";
	cout << "\n========Search===========";
	cout << "\n 5 - Search one value";
	cout << "\n 6 - Search within range";
    cout << "\n========Delete===========";
    cout << "\n 7 - Deletion";
    cout << "\n 0 - Exit";

    cout << "\n Enter selection: ";
}

int getInput() {
    int selection;
    cin >> selection;
    return selection;
}

void loadIntoMemory(Memory* storageMem) {
    cout << "\n-- Load into Memory --" << endl;
    cout << "Record List Count: " << recordList.size() << endl;

    for (int i = 0; i < recordList.size(); i++) {
        if (storageMem->memIsFull()) {
            break;
        }
        else {
            /* FOR TESTING PURPOSES
            std::cout << "\n-- Test (Reading Record Prior to Memory Allocation) --" << std::endl;
            cout << "TCONST: " << recordList[i].tconst << endl;
            cout << "AVG RATING: " << recordList[i].averageRating << endl;
            cout << "NUM VOTE: " << recordList[i].numVotes << endl;
             */
            storageMem->allocateRecordToMem(recordList[i]);
        }
    }
    storageMem->displayStats();
}
