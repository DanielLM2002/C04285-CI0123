#include "openFiles.h"
#define amount_of_files 128

OpenFiles::OpenFiles() {
    openFileCount = new int[amount_of_files];
    openFilesMap = new BitMap(amount_of_files);
    this->openFilesMap->Mark(0);
    this->openFilesMap->Mark(1);
    this->openFilesMap->Mark(2);
    ThreadsInUse = 0;
}

OpenFiles::~OpenFiles() {
    delete openFileCount;
    delete openFilesMap;
}

void OpenFiles::addThread() {
    this->ThreadsInUse++;
}

void OpenFiles::removeThread() {
    this->ThreadsInUse--;
}

int OpenFiles::Open(int fileHandle) {
        int nachOSHandle = this->openFilesMap->Find();
        printf("nachOSHandle: %d\n", nachOSHandle);
        this->openFileCount[nachOSHandle] = fileHandle;
        return nachOSHandle;
}

int OpenFiles::Close(int fileHandle) {
    if (this->ThreadsInUse == 0) {
        return -1;
    }
    if (this->openFilesMap->Test(fileHandle)) {
        this->openFileCount[fileHandle]--;
        if (this->openFileCount[fileHandle] == 0) {
            this->openFilesMap->Clear(fileHandle);
        }
        return 0;
    } else {
        return -1;
    }
}

bool OpenFiles::isOpen(int fileHandle) {
    return this->openFilesMap->Test(fileHandle);
}

int OpenFiles::getOpenCount(int fileHandle) {
    int unixHandle = fileHandle;
    if (isOpen(fileHandle)) 
        unixHandle = this->openFileCount[fileHandle];
    return unixHandle;
}

void OpenFiles::Print() {
    std::cout << "OpenFiles:" << std::endl;
    for(int i = 0; i < amount_of_files; i++) {
        std::cout << "Index" << this->openFileCount[i] << "\n";
    }
    std::cout << std::endl;
}