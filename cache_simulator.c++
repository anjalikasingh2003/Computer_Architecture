//Let us take 4-Way Set-Associative cache of size 32 KB which can be adderessed using a 40 bit address. Block Size of cache is 64Bytes. 
//Implementing Write Back Cache with Write Allocate Policy

#include <bits/stdc++.h>
using namespace std;

class Block{
    public:
    string state; 
    bool isFull; 
    int tag;
    int data[64];

    int lru_counter;

    Block() : lru_counter(0) {}

   
    
};

unordered_map<int, int> memory;

int memory_fetch(int address){
    return memory[address];
}



void update_lru_counters(Block block[128][4], int index, int accessed_block) {
    for (int i = 0; i < 4; i++) {
        if (i != accessed_block && block[index][i].isFull) {
            block[index][i].lru_counter++;
        }
    }
    block[index][accessed_block].lru_counter = 0;
}

int read(Block block[128][4], int physical_address, int blockoffset, int index, int cpu_tag ){
 bool tag_match;
    int block_no;
    for(int i=0; i<4; i++){
        if(block[index][i].tag==cpu_tag){
            tag_match=1;
            block_no=i;

            break;

        }
    }

    int ans;

    if(tag_match==1){
        if(block[index][block_no].state=="V"){
            //hit
            ans=block[index][block_no].data[blockoffset];
            update_lru_counters(block, index, block_no);
        
        }

        else if (block[index][block_no].state=="MP"){
            //wait
        }


    }

    else{
        //miss
        
        int lru_block_no = -1;
        int min_lru_counter = INT_MAX;
        for(int i=0; i<4; i++){
            if(!block[index][i].isFull){
                block_no=i;
                break;


            }

             if (block[index][i].lru_counter < min_lru_counter) {
                    min_lru_counter = block[index][i].lru_counter;
                    lru_block_no = i;
                }         
        }

        if(block_no==-1){
            //lru-block replacement
             block_no = lru_block_no;
        }
        else{

            if(block[index][block_no].state=="MD"){
                int start_address=block[index][block_no].tag*8192+index*128+blockoffset*64;
                 for (int i = 0; i < 64; i++) {
                    memory[start_address + i] = block[index][block_no].data[i];
                }
            }
            block[index][block_no].tag=cpu_tag;
            block[index][block_no].state="MP";
            ans=memory_fetch(physical_address);
            for(int i=0; i<64; i++){
                block[index][block_no].data[i]=memory_fetch((physical_address/64)*64 + i);
            }
            block[index][block_no].isFull=true;
            block[index][block_no].state="V";

            update_lru_counters(block, index, block_no);

        }
    }
    return ans;
}

int main(){
Block block[128][4];
string com;
cout<<"Read or Write";
cin>>com;
int physical_address;
cin>>physical_address;
int blockoffset=physical_address%64;
int index=(physical_address/64)%128;
int cpu_tag=physical_address/8192;
if(com=="Read")
{
  int ans =read(block, physical_address, blockoffset, index, cpu_tag); 

}
else{
//Write
int value;
cin>>value;
bool tag_match=0;
    int block_no;
    for(int i=0; i<4; i++){
        if(block[index][i].tag==cpu_tag){
            tag_match=1;
            block_no=i;

            break;

        }
    }

    if(tag_match==1){
        //hit
        block[index][block_no].state="MD";   //Modified State
        block[index][block_no].data[blockoffset]=value;


    }
    else{
        //miss
        int ans =read(block, physical_address, blockoffset, index, cpu_tag);
        block[index][block_no].state="MD";   //Modified State
        block[index][block_no].data[blockoffset]=value;

    }


}



}