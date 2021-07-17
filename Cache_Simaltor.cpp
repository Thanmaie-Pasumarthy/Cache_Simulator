#include <iostream>
#include <fstream>
#include<cstring>
#include<cmath>
using namespace std;
void conversion(string a,bool* b){
    int p = 0;
    int q = 1;
    int r = 2;
    int s = 3;
    for (int i = 0; i < 8;i++){
        if(a[i+2]=='0'){
            b[i * 4 + p] = 0;
            b[i * 4 + q] = 0;
            b[i * 4 + r] = 0;
            b[i * 4 + s] = 0;
        }
        else if(a[i+2]=='1'){
            b[i * 4 + p] = 0;
            b[i * 4 + q] = 0;
            b[i * 4 + r] = 0;
            b[i * 4 + s] = 1;
        }
        else if(a[i+2]=='2'){
            b[i * 4 + p] = 0;
            b[i * 4 + q] = 0;
            b[i * 4 + r] = 1;
            b[i * 4 + s] = 0;
        }
        else if(a[i+2]=='3'){
            b[i * 4 + p] = 0;
            b[i * 4 + q] = 0;
            b[i * 4 + r] = 1;
            b[i * 4 + s] = 1;
        }
        else if(a[i+2]=='4'){
            b[i * 4 + p] = 0;
            b[i * 4 + q] = 1;
            b[i * 4 + r] = 0;
            b[i * 4 + s] = 0;
        }
        else if(a[i+2]=='5'){
            b[i * 4 + p] = 0;
            b[i * 4 + q] = 1;
            b[i * 4 + r] = 0;
            b[i * 4 + s] = 1;
        }
        else if(a[i+2]=='6'){
            b[i * 4 + p] = 0;
            b[i * 4 + q] = 1;
            b[i * 4 + r] = 1;
            b[i * 4 + s] = 0;
        }
        else if(a[i+2]=='7'){
            b[i * 4 + p] = 0;
            b[i * 4 + q] = 1;
            b[i * 4 + r] = 1;
            b[i * 4 + s] = 1;
        }
        else if(a[i+2]=='8'){
            b[i * 4 + p] = 1;
            b[i * 4 + q] = 0;
            b[i * 4 + r] = 0;
            b[i * 4 + s] = 0;
        }
        else if(a[i+2]=='9'){
            b[i * 4 + p] = 1;
            b[i * 4 + q] = 0;
            b[i * 4 + r] = 0;
            b[i * 4 + s] = 1;
        }
        else if(a[i+2]=='a'||a[i+2]=='A'){
            b[i * 4 + p] = 1;
            b[i * 4 + q] = 0;
            b[i * 4 + r] = 1;
            b[i * 4 + s] = 0;
        }
        else if(a[i+2]=='b'||a[i+2]=='B'){
            b[i * 4 + p] = 1;
            b[i * 4 + q] = 0;
            b[i * 4 + r] = 1;
            b[i * 4 + s] = 1;
        }
        else if(a[i+2]=='c'||a[i+2]=='C'){
            b[i * 4 + p] = 1;
            b[i * 4 + q] = 1;
            b[i * 4 + r] = 0;
            b[i * 4 + s] = 0;
        }
        else if(a[i+2]=='d'||a[i+2]=='D'){
            b[i * 4 + p] = 1;
            b[i * 4 + q] = 1;
            b[i * 4 + r] = 0;
            b[i * 4 + s] = 1;
        }
        else if(a[i+2]=='e'||a[i+2]=='E'){
            b[i * 4 + p] = 1;
            b[i * 4 + q] = 1;
            b[i * 4 + r] = 1;
            b[i * 4 + s] = 0;
        }
        else if(a[i+2]=='f'||a[i+2]=='F'){
            b[i * 4 + p] = 1;
            b[i * 4 + q] = 1;
            b[i * 4 + r] = 1;
            b[i * 4 + s] = 1;
        }
    }
}

struct cache_block{
    bool tag[32];
    bool valid_bit;
    bool dirty_bit;
    cache_block *next;
};

cache_block *cache;

class directMapped{
    public:
        int writeMisses = 0;
        int readMisses = 0;
        int dirtyBitEvicted = 0;
        int blockSize;
        int blockOffset;
        int numSet;
        void process(bool address[32],bool);
};

void directMapped::process(bool address[32],bool RorW){
    int blockOffset = log2(blockSize);
    int setIndexBits = log2(numSet);
    int tagBit = 32 - blockOffset - setIndexBits;

    bool  tag[tagBit];
    for (int i = 0; i < tagBit;i++){
        tag[i] = address[i];
    }

    int setIndex=0;
    for (int i = 0; i < setIndexBits;i++){
        setIndex = (setIndex * 2 )+ address[tagBit + i];
    }
    setIndex = setIndex % numSet;

    cache_block *present;
    present = cache;
    for (int i = 0; i < setIndex; i++) {
        present = present->next;
    }
    int i = 0;
    while (i < tagBit) {
        if (tag[i] != present->tag[i]) {
            break;
        }
        i++;
    }
    if (i==tagBit) {
        if (RorW) {
            present->dirty_bit = 1;
        }
        return;
    }
    else{
        if(!present->valid_bit){
            if(RorW)
                writeMisses++;
            else
                readMisses++;
            present->valid_bit = 1;
            if (RorW) {
                present->dirty_bit = 1;
             }
            for (int j = 0; j < tagBit;j++){
                present->tag[i] = tag[i];
            }
            return;
          }
          else{
              if(RorW)
                  writeMisses++;
              else
                  readMisses++;
              if(present->dirty_bit){
                  dirtyBitEvicted++;
                  if(!RorW){
                      present->dirty_bit = 0;
                  }
              }
              for (int j = 0; j < tagBit;j++){
                  present->tag[i] = tag[i];
              }
              return;
          }
        }
}

class SetAssociative{
    public:
        int setAssociativity;
        int blockSize;
        int numBlocks;
        int numSet;
        int ReadMisses=0;
        int WriteMisses=0;
        int numDirtyBlocks=0;

        void random(bool[],bool);
        long int** lrupolicy(bool address[32],long int**, bool);
        bool** pseudoLRU(bool address[32],bool**, bool);
};

void SetAssociative::random(bool address[32],bool RorW){
    int blockOffset = log2(blockSize);
    int setIndexBits = log2(numSet);
    int tagBit = 32 - blockOffset - setIndexBits;

    bool  tag[tagBit];
    for (int i = 0; i < tagBit;i++){
        tag[i] = address[i];
    }
    int setIndex=0;
    for (int i = 0; i < setIndexBits;i++){
        setIndex = (setIndex * 2 )+ address[tagBit + i];
    }
    setIndex = setIndex % numSet;
    bool presentTag[tagBit];
    cache_block* present;
    cache_block *checkVal;
    present = cache;

    for (int i = 0; i < (setIndex) * setAssociativity;i++){
        present = present->next;
    }
    checkVal = present;
    cache_block* conflictmissesptr=present;
    int a = 0;

    for (int i = (setIndex) * setAssociativity; i < (setIndex+1) * setAssociativity;i++){
        if(conflictmissesptr->valid_bit==1){
            a++;
	      }
	      conflictmissesptr=conflictmissesptr->next;
    }
    if(a==setAssociativity){
        if (!RorW)
            ReadMisses++;
        else
            WriteMisses++;
    }
	  a=0;
    while (a < setAssociativity) {
        bool presentTag[tagBit];
        for (int j = 0; j < tagBit; j++) {
            presentTag[j] = present->tag[j];
        }
        int i = 0;
        while (i < tagBit) {
            if (presentTag[i] != tag[i]) {
                break;
            }
            i++;
        }
        if (i == tagBit) {
            if (RorW) {
                present->dirty_bit = 1;
            }
            return;
        }
        present = present->next;
        a++;
    }

    present = checkVal;
    a = 0;

    while(a<setAssociativity){
        if(present->valid_bit==1){
            present=present->next;
            a++;
        }
        else {
            if (!RorW)
                ReadMisses++;
            else
                WriteMisses++;
            present->valid_bit = 1;
            for (int i = 0; i < tagBit; i++) {
                present->tag[i] = tag[i];
            }
	          if(RorW)
	             present->dirty_bit=1;
            return;
        }
      present=present->next;
      a++;
    }
    if(present==NULL){
        if(!RorW){
            ReadMisses++;
        }
        else{
            WriteMisses++;
        }
        if(cache->dirty_bit==1){
            if(!RorW)
                cache->dirty_bit=0;
            numDirtyBlocks++;
        }
        for(int i=0;i<tagBit;i++){
            cache->tag[i]=tag[i];
        }
    }
    return;
}

long int**  SetAssociative::lrupolicy(bool address[32],long int** recent, bool RorW) {
    int blockOffset = log2(blockSize);
    int setIndexBits = log2(numSet);
    int tagBit = 32 - blockOffset - setIndexBits;

    bool  tag[tagBit];
    for (int i = 0; i < tagBit;i++){
        tag[i] = address[i];
    }
    int setIndex=0;
    for (int i = 0; i < setIndexBits;i++){
        setIndex = (setIndex * 2 )+ address[tagBit + i];
    }
    setIndex = setIndex % numSet;
    cache_block* present;
    present=cache;
    int b=0;
    bool presentTag[tagBit];
    cache_block *checkVal;

    for (int i = 0; i < (setIndex) * setAssociativity;i++){
        present = present->next;
    }
    checkVal = present;
    cache_block* conflictmissesptr=present;
    int a = 0;

    for (int i = (setIndex) * setAssociativity; i < (setIndex+1) * setAssociativity;i++){
        if(conflictmissesptr->valid_bit==1)
            a++;
	      conflictmissesptr=conflictmissesptr->next;
    }
    if(a==setAssociativity){
			if(RorW)
				  WriteMisses++;
			else
				  ReadMisses++;
		}

    while(b<setAssociativity){
        bool presentTag[tagBit];
        for(int j=0;j<tagBit;j++){
            presentTag[j]=present->tag[j];
        }
        int i=0;
        while(i<tagBit){
            if(presentTag[i]!=tag[i])
                break;
            i++;
        }
        if(i==tagBit){
            if(RorW)
                present->dirty_bit=1;
            for(int j=0;j<numBlocks;j++)
                recent[setIndex][j]++;
            recent[setIndex][b]=0;
            return recent;
        }
        present=present->next;
        b++;
    }

    present=checkVal;
    int r=0;
    while(r<setAssociativity){
        if(present->valid_bit==1)
            present=present->next;
        else{
            for(int j=0;j<numBlocks;j++)
                recent[setIndex][j]++;
            recent[setIndex][r]=0;
            if(!RorW)
                ReadMisses++;
            else
                WriteMisses++;
            present->valid_bit=1;
            for(int i=0;i<tagBit;i++)
                present->tag[i]=tag[i];
            if(RorW)
      	        present->dirty_bit=1;
            return recent;
        }
        present=present->next;
        r++;
    }

    if(r==setAssociativity){
        if(!RorW)
            ReadMisses++;
        else
            WriteMisses++;
        int leastrecentblock=0;
        for(int i=0;i<numBlocks;i++){
            if(recent[setIndex][leastrecentblock]<recent[setIndex][i])
                leastrecentblock=i;
        }

        int k=0;
        present=checkVal;
        while(k<leastrecentblock){
            present=present->next;
            k++;
        }
        if(present->dirty_bit==1){
            if(!RorW)
                cache->dirty_bit=0;
            numDirtyBlocks++;
        }
        for(int i=0;i<tagBit;i++)
            present->tag[i]=tag[i];

        for(int j=0;j<numBlocks;j++)
            recent[setIndex][j]++;

        recent[setIndex][leastrecentblock]=0;
    }
    return recent;
}


bool** SetAssociative::pseudoLRU(bool address[32],bool** pseudo, bool RorW){
    int blockOffset = log2(blockSize);
    int setIndexBits = log2(numSet);
    int tagBit = 32 - blockOffset - setIndexBits;

    bool  tag[tagBit];
    for (int i = 0; i < tagBit;i++){
        tag[i] = address[i];
    }
    int setIndex=0;
    for (int i = 0; i < setIndexBits;i++){
        setIndex = (setIndex * 2 )+ address[tagBit + i];
    }
    setIndex = setIndex % numSet;
    cache_block *present;
    present = cache;
    bool presentTag[tagBit];
    cache_block *checkVal;

    for (int i = 0; i < (setIndex) * setAssociativity;i++){
        present = present->next;
    }
    checkVal = present;
    cache_block* conflictmissesptr=present;
    int a = 0;

    for (int i = (setIndex) * setAssociativity; i < (setIndex+1) * setAssociativity;i++){
        if(conflictmissesptr->valid_bit==1){
            a++;
	      }
	      conflictmissesptr=conflictmissesptr->next;
    }
    if(a==setAssociativity){
  			if(RorW)
  				WriteMisses++;
  			else
  				ReadMisses++;
		}

    int pos=0;
    while(pos<setAssociativity){
        for(int j=0;j<tagBit;j++)
            presentTag[j]=present->tag[j];
        int i=0;
        while(i<tagBit){
            if(presentTag[i]!=tag[i]){
                break;
            }
            i++;
        }
        if(i==tagBit){
          if(RorW)
              present->dirty_bit=1;
          int ArrIndex=pos+numBlocks-1;
          while(ArrIndex>0){
              int parent=(ArrIndex-1)/2;
              if(ArrIndex==2*parent+1)
                  pseudo[setIndex][parent]=1;
              else
                  pseudo[setIndex][parent]=0;
              ArrIndex=parent;
          }
          return pseudo;
      }
      present=present->next;
      pos++;
    }

    present=checkVal;
    int index=0;
    while(index<setAssociativity){
        if(present->valid_bit==1)
            present=present->next;
        else{
            if(!RorW){
                ReadMisses++;
            }
            else{
                WriteMisses++;
            }
            present->valid_bit=1;
            for(int i=0;i<tagBit;i++){
                present->tag[i]=tag[i];
            }
            if(RorW){
    	         present->dirty_bit=1;
            }
            int ArrIndex=index+numBlocks-1;
            int parent=(ArrIndex-1)/2;
            while(parent>=0){
                if(pseudo[setIndex][parent]==1){
                    pseudo[setIndex][parent] = 0;
                }
                else{
                    pseudo[setIndex][parent] = 1;
                }
                parent=(parent-1)/2;
            }
            return pseudo;
        }
        present=present->next;
        index++;
    }
    if(index==setAssociativity){
        if(!RorW){
            ReadMisses++;
        }
        else{
            WriteMisses++;
        }
        int findevict=0;
        while(findevict<numBlocks-1){
            if(pseudo[setIndex][findevict]==0){
                findevict=2*findevict+1;
            }
            else if(pseudo[setIndex][findevict]==1){
                findevict=2*findevict+2;
            }
        }
        findevict=findevict-numBlocks+1;
        int f=0;
        present=cache;
        while(f<findevict){
            present=present->next;
            f++;
        }
        if(present->dirty_bit==1){
  	       if(!RorW)
              present->dirty_bit=0;
          numDirtyBlocks++;
        }
        for(int i=0;i<tagBit;i++){
            present->tag[i]=tag[i];
        }
        int ArrIndex=findevict+numBlocks-1;
        int parent=(ArrIndex-1)/2;
        while(parent>=0){
            if(pseudo[setIndex][parent]==1){
                pseudo[setIndex][parent] = 0;
            }
            else{
                pseudo[setIndex][parent] = 1;
            }
            parent=(parent-1)/2;
        }
    }
    return pseudo;
}

class fullyAssociative{
    public:
        void random(bool[],bool);
        long int* lrupolicy(bool[],long int *,bool);
        bool* pseudoLRU(bool[],bool*,bool);
        int blockSize;
        int numBlocks;
        int ReadMisses=0;
        int WriteMisses=0;
        int numDirtyBlocks=0;
};

void fullyAssociative::random(bool address[32],bool RorW){
    int blockOffsetBits=log2(blockSize);
    int tagBits=32-blockOffsetBits;
    bool tagAdd[tagBits];
    for(int i=0;i<tagBits;i++){
        tagAdd[i]=address[i];
    }
    cache_block* present;
    present=cache;
    while(present->next!=NULL){
        bool presentTag[tagBits];
        for(int j=0;j<tagBits;j++){
            presentTag[j]=present->tag[j];
        }
        int i=0;
        while(i<tagBits){
            if(presentTag[i]!=tagAdd[i]){
                break;
            }
            i++;
        }
        if(i==tagBits){
            if(RorW){
                present->dirty_bit=1;
            }
            return;
        }
        present=present->next;
    }
    present=cache;
    while(present->next!=NULL){
        if(present->valid_bit==1){
            present=present->next;
        }
        else{
            if(!RorW){
                ReadMisses++;
            }
            else{
                WriteMisses++;
            }
            present->valid_bit=1;
            for(int i=0;i<tagBits;i++){
                present->tag[i]=tagAdd[i];
            }
    	      if(RorW)
    	          present->dirty_bit=1;
            return;
        }
        present=present->next;
    }
    if(present==NULL){
        if(!RorW){
            ReadMisses++;
        }
        else{
            WriteMisses++;
        }
        if(cache->dirty_bit==1){
  	       if(!RorW)
              cache->dirty_bit=0;
          numDirtyBlocks++;
        }
        for(int i=0;i<tagBits;i++){
            cache->tag[i]=tagAdd[i];
        }
    }
    return;
}


long int*  fullyAssociative::lrupolicy(bool address[32],long int* recent,bool RorW) {
    int blockOffsetBits=log2(blockSize);
    int tagBits=32-blockOffsetBits;
    bool tagAdd[tagBits];
    for(int i=0;i<tagBits;i++){
        tagAdd[i]=address[i];
    }
    cache_block* present;
    present=cache;
    int b=0;
    while(present->next!=NULL){
        bool presentTag[tagBits];
        for(int j=0;j<tagBits;j++){
            presentTag[j]=present->tag[j];
        }
        int i=0;
        while(i<tagBits){
            if(presentTag[i]!=tagAdd[i]){
                break;
            }
            i++;
        }
        if(i==tagBits){
            if(RorW){
                present->dirty_bit=1;
            }
            for(int j=0;j<numBlocks;j++){
                recent[j]++;
            }
            recent[b]=0;
            return recent;
        }
        present=present->next;
        b++;
    }
    present=cache;
    int r=0;
    while(present->next!=NULL){
        if(present->valid_bit==1){
            present=present->next;
        }
        else{
            for(int j=0;j<numBlocks;j++){
                recent[j]++;
            }
            recent[r]=0;
            if(!RorW){
                ReadMisses++;
            }
            else{
                WriteMisses++;
            }
            present->valid_bit=1;
            for(int i=0;i<tagBits;i++){
                present->tag[i]=tagAdd[i];
            }
            if(RorW)
      	        present->dirty_bit=1;
            return recent;
        }
        present=present->next;
        r++;
    }
    if(present==NULL){
        if(RorW){
            ReadMisses++;
        }
        else{
            WriteMisses++;
        }
        int leastrecentblock=0;
        for(int i=0;i<numBlocks;i++){
            if(recent[leastrecentblock]<recent[i]){
                leastrecentblock=i;
            }
        }
        int k=0;
        present=cache;
        while(k<leastrecentblock){
            present=present->next;
            k++;
        }
        if(present->dirty_bit==1){
            if(!RorW)
                cache->dirty_bit=0;
            numDirtyBlocks++;
        }
        for(int i=0;i<tagBits;i++){
            present->tag[i]=tagAdd[i];
        }
        for(int j=0;j<numBlocks;j++){
            recent[j]++;
        }
        recent[leastrecentblock]=0;
    }
    return recent;
}


bool* fullyAssociative::pseudoLRU(bool address[32],bool* pseudo,bool RorW){
    int blockOffsetBits=log2(blockSize);
    int tagBits=32-blockOffsetBits;
    bool tagAdd[tagBits];
    for(int i=0;i<tagBits;i++){
        tagAdd[i]=address[i];
    }
    cache_block* present;
    present=cache;
    int pos=0;
    while(present->next!=NULL){
        bool presentTag[tagBits];
        for(int j=0;j<tagBits;j++){
            presentTag[j]=present->tag[j];
        }
        int i=0;
        while(i<tagBits){
            if(presentTag[i]!=tagAdd[i]){
              break;
            }
            i++;
        }
        if(i==tagBits){
            if(RorW){
                present->dirty_bit=1;
            }
            int ArrIndex=pos+numBlocks-1;
            while(ArrIndex>0){
                int parent=(ArrIndex-1)/2;
                if(ArrIndex==2*parent+1){
                    pseudo[parent]=1;
                }
                else{
                    pseudo[parent]=0;
                }
                ArrIndex=parent;
            }
            return pseudo;
        }
        present=present->next;
        pos++;
    }
    present=cache;
    int index=0;
    while(present->next!=NULL){
        if(present->valid_bit==1){
            present=present->next;
        }
        else{
            if(!RorW){
                ReadMisses++;
            }
            else{
                WriteMisses++;
            }
            present->valid_bit=1;
            for(int i=0;i<tagBits;i++){
                present->tag[i]=tagAdd[i];
            }
    	      if(RorW)
    	         present->dirty_bit=1;
            int ArrIndex=index+numBlocks-1;
            int parent=(ArrIndex-1)/2;
            while(parent>=0){
                if(pseudo[parent]==1){
                    pseudo[parent] = 0;
                }
                else{
                    pseudo[parent] = 1;
                }
                parent=(parent-1)/2;
            }
            return pseudo;
        }
        present=present->next;
        index++;
    }
    if(present==NULL){
        if(!RorW){
            ReadMisses++;
        }
        else{
            WriteMisses++;
        }
        int findevict=0;
        while(findevict<numBlocks-1){
            if(pseudo[findevict]==0){
                findevict=2*findevict+1;
            }
            else if(pseudo[findevict]==1){
                findevict=2*findevict+2;
            }
        }
        findevict=findevict-numBlocks+1;
        int f=0;
        present=cache;
        while(f<findevict){
            present=present->next;
            f++;
        }
        if(present->dirty_bit==1){
  	        if(!RorW)
                present->dirty_bit=0;
            numDirtyBlocks++;
        }
        for(int i=0;i<tagBits;i++){
            present->tag[i]=tagAdd[i];
        }
        int ArrIndex=findevict+numBlocks-1;
        int parent=(ArrIndex-1)/2;
        while(parent>=0){
            if(pseudo[parent]==1){
                pseudo[parent] = 0;
              }
            else{
                pseudo[parent] = 1;
            }
            parent=(parent-1)/2;
        }
    }
    return pseudo;
}

int  main(){
    int cachesize;
    cin >> cachesize;
    int blocksize;
    cin >> blocksize;
    int numBlocks=cachesize/blocksize;
    cache=new cache_block;
    cache_block* block=cache;
    for(int i=0;i<numBlocks;i++){
        block->next=new cache_block;
        block=block->next;
    }

    int associativity;
    cout << "Please enter 0 for fully associative, 1 for direct mapped and corresponding associativity for set associative\n";
    cin >> associativity;
    int replacementpolicy;
    cout << "Please enter 0 for random replacement policy, 1 for LRU and 2 for pseudo LRU\n";
    cin >> replacementpolicy;
    string address;
    int CacheAccesses = 0;
    int readAccesses = 0;
    int writeAccesses = 0;
    int numDirtyBlocks=0;
    int ReadMisses=0;
    int WriteMisses=0;
    int cacheMisses = 0;
    string inputfilename;
    cin>>inputfilename;
    if (associativity == 0) {
        fullyAssociative fa;
        fa.blockSize = blocksize;
        fa.numBlocks = cachesize / blocksize;
        ifstream ifile(inputfilename);
        if (ifile.is_open()) {
            while (getline(ifile, address)) {
                CacheAccesses++;
		            bool RorW;
                if (address[11] == 'r'||address[11]=='R'){
                    readAccesses++;
		                RorW=false;
		            }
                else if(address[11]=='w'||address[11]=='W'){
                    writeAccesses++;
		                RorW=true;
		            }
                bool b[32];
                conversion(address, b);
                if (replacementpolicy == 0)
                    fa.random(b,RorW);
                else if (replacementpolicy == 1) {
                    long int *recent = new long int[fa.numBlocks];
                    recent = fa.lrupolicy(b, recent, RorW);
                }
                else {
                    bool *pseudo = new bool[2 * fa.numBlocks - 1];
                    pseudo = fa.pseudoLRU(b, pseudo, RorW);
                }
            }
            ifile.close();
            numDirtyBlocks = fa.numDirtyBlocks;
            ReadMisses = fa.ReadMisses;
            WriteMisses = fa.WriteMisses;
            cacheMisses = ReadMisses + WriteMisses;
        }

    }
    else if (associativity == 1) {
        directMapped dm;
        dm.blockSize = blocksize;
        dm.numSet = cachesize / blocksize;

        ifstream ifile(inputfilename);
        if (ifile.is_open()) {
            while (getline(ifile, address)) {
                CacheAccesses++;
		            bool RorW;
                if (address[11] == 'r'||address[11]=='R'){
                    readAccesses++;
		                RorW=false;
		            }
                else if(address[11]=='w'||address[11]=='W'){
                    writeAccesses++;
		                RorW=true;
		            }
                bool b[32];
                conversion(address, b);
                dm.process(b,RorW);
            }
            ifile.close();

            numDirtyBlocks = dm.dirtyBitEvicted;
            ReadMisses = dm.readMisses;
            WriteMisses = dm.writeMisses;
            cacheMisses = ReadMisses + WriteMisses;
        }
    }

    else {
        SetAssociative sa;
        sa.blockSize = blocksize;
        sa.numBlocks = cachesize / blocksize;
        sa.numSet = sa.numBlocks / associativity;
        sa.setAssociativity = associativity;
        ifstream ifile(inputfilename);
        if (ifile.is_open()) {
            while (getline(ifile, address)) {

                CacheAccesses++;
                bool RorW;
                if (address[11] == 'r'||address[11]=='R'){
                    readAccesses++;
		                RorW=false;
		            }
                else if(address[11]=='w'||address[11]=='W'){
                    writeAccesses++;
	                  RorW=true;
		            }
                bool b[32];
                conversion(address, b);
                if (replacementpolicy == 0)
                    sa.random(b,RorW);
                else if (replacementpolicy == 1) {
                    long int **recent = new long int *[sa.numSet];
                    for (int i = 0; i < sa.numSet; i++) {
                        recent[i] = new long int[sa.numBlocks];
                    }
                    recent = sa.lrupolicy(b, recent, RorW);
                }
                else {
                    bool **pseudo = new bool *[sa.numSet];
                    for (int i = 0; i < sa.numSet; i++) {
                        pseudo[i] = new bool[2 * sa.numBlocks - 1];
                    }
                    pseudo = sa.pseudoLRU(b, pseudo, RorW);
                }
            }
            ifile.close();
            numDirtyBlocks = sa.numDirtyBlocks;
            ReadMisses = sa.ReadMisses;
            WriteMisses = sa.WriteMisses;
            cacheMisses = ReadMisses + WriteMisses;
        }
    }
        cout << cachesize << endl;
        cout << blocksize << endl;
        if(associativity==0){
            cout << "Fully associative cache" << endl;
        }
        else if(associativity==1){
            cout << "Direct-mapped cache" << endl;
        }
        else {
            cout << "Set-associative cache" << endl;
        }
        if(replacementpolicy==0){
            cout << "Random Replacement" << endl;
        }
        else if(replacementpolicy==1){
            cout << "LRU Replacement" << endl;
        }
        else if(replacementpolicy==2){
            cout << "Pseudo-LRU Replacement" << endl;
        }
        cout << CacheAccesses << endl;
        cout << readAccesses << endl;
        cout << writeAccesses << endl;
        cout << cacheMisses << endl;
        cout << ReadMisses << endl;
        cout << WriteMisses << endl;
        cout << numDirtyBlocks << endl;
    return 0;
}
