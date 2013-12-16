#include "stats.h"

Stats::Stats() {
	this->mallocCount = 0;
	this->freeCount = 0;
	this->invalidReadCount = 0;
	this->invalidWriteCount = 0;
	this->fenceHitCount = 0;
}

void Stats::reset() {
	this->mallocCount = 0;
	this->freeCount = 0;
	this->invalidReadCount = 0;
	this->invalidWriteCount = 0;
	this->fenceHitCount = 0;	
}

unsigned int Stats::getMallocCount() {
	return this->mallocCount;
}
unsigned int Stats::getFreeCount() {
	return this->freeCount;
}
unsigned int Stats::getInvalidReadCount() {
	return this->invalidReadCount;
}

unsigned int Stats::getInvalidWriteCount() {
	return this->invalidWriteCount;
}

unsigned int Stats::getFenceHitCount() {
	return this->fenceHitCount;
}

void Stats::setMallocCount(unsigned int count) {
	this->mallocCount = count;
}

void Stats::setFreeCount(unsigned int count) {
	this->freeCount = count;
}

void Stats::setInvalidReadCount(unsigned int count) {
	this->invalidReadCount = count;
}

void Stats::setInvalidWriteCount(unsigned int count) {
	this->invalidWriteCount = count;
}

void Stats::setFenceHitCount(unsigned int count) {
	this->fenceHitCount = count;
}

void Stats::incMallocCount() {
	this->mallocCount++;
}

void Stats::incFreeCount() {
	this->freeCount++;
}

void Stats::incInvalidReadCount() {
	this->invalidReadCount++;
}

void Stats::incInvalidWriteCount() {
	this->invalidWriteCount++;
}

void Stats::incFenceHitCount() {
	this->fenceHitCount++;
}

void Stats::displayResults(MemList memlist, FILE *fp) {
	if(fp == NULL) {
		fp = stdin;
	}
	// Print out the initial object
	fprintf(fp, "=== RESULTS ===\n");
	fprintf(fp, "%-20s %d\n", "allocations: ", this->mallocCount);
	fprintf(fp, "%-20s %d\n", "deallocations: ", this->freeCount);
	fprintf(fp, "%-20s %d\n", "invalid reads: ", this->invalidReadCount);
	fprintf(fp, "%-20s %d\n", "invalid writes: ", this->invalidWriteCount);
	fprintf(fp, "%-20s %d\n", "fence hit: ", this->fenceHitCount);
	// Print out the contents of memlist if it was provided.
	fprintf(fp, "=== MEMLIST CONTENTS ===\n");
	char buffer[2048];
	int lostMemory = 0;
	for(int i = 0; i < memlist.size(); i++) {
		MemoryAlloc alloc = memlist.get(i); 
		fprintf(fp, "%s\n", alloc.toString(buffer, 2048));
		lostMemory += alloc.getTotalSize();
	}
	fprintf(fp, "The memlist contains %d items.\nTotal loss of %d bytes\n", memlist.size(), lostMemory);
}