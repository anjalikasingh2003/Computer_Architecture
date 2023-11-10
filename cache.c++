#include <bits/stdc++.h>
using namespace std;

vector<int> state(100); //1=V 0=I 2=MP
vector<int> tag (100);
vector<int> data(100);
vector<int> mshr;
unordered_map<int,int>memory(1000);

int checkMemory(int address){
    int data=memory[address];
    return data;

}



int main(){
    int address;
    cin>>address; //suppose cpu ne address bheja check karne ko
    
    int j;//offset selector
    int flag=0;

    for(int i=0; i<100; i++){
        if(address==tag[i]){
            j=i;
            flag=1;
            break;
        }
    }

    int ans;
    int flag2=0;
    if(flag==1 && state[j]==1){
        //hit
        ans=data[j];
    




    }

    if(flag==1 && state[j]==2){
        //wait till memory response

    }

    else{

        //miss
        state[j]=2;
        mshr.push_back(address);
      ans=checkMemory(address);
      flag2=1;
        

       
    }

    if(flag2==1){
        state[j]=1;
    }

    cout<<ans<<endl;
    return 0;

}