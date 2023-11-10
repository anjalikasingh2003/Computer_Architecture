#include <bits/stdc++.h>
using namespace std;

    bool ALUSrc=0;
    int ALUOp;
    bool branch=0;
    bool mem2Reg=0;
    bool jump=0;
    bool memWrite=0;
    bool memRead=0;
    string ALUSelect;
    bool regWrite=0;
    bool regRead=0;
    int ALUResult; 
    int flag=0;



void controlUnit(string opcode)
{
  
    if(opcode=="0110011"){
        ALUSrc=0;
        regWrite=1;
        regRead=1;
        ALUOp=10;
    }

    if(opcode=="0010011"){
        ALUSrc=1;
        regRead=1;
        regWrite=1;
        ALUOp=11;
    }

    if(opcode=="1100011"){
        ALUSrc=0;
        branch=1;
        ALUOp=01;
        regRead=1;

    }
}


string ALUControl(string f7, string f3){
    if(ALUOp==10){
        if(f7=="0000000" && f3=="000"){
            return "0010";
        }
        else if(f7=="0100000" && f3=="000"){
            return "0110";
        }
        else if(f7=="0000000" && f3=="111"){
            return "0000";
        }
        else if(f7=="0000000" && f3=="110"){
            return "0001";
        }

    }


}

int ALU(string ALUSelect, int r1, int r2){

    if(ALUSelect=="0010")
{
    return r1+r2;
}
else if(ALUSelect=="0110"){
    return r1-r2;
}
else if(ALUSelect=="0000"){
    return r1&r2;
}
else if(ALUSelect=="1111"){
    return r1|r2;
}
}


std::string intToBinary(int number) {
    // Use std::bitset to convert the integer to binary
    std::bitset<32> binary(number);
    
    // Convert the binary representation to a string
    return binary.to_string();
}

int bin2imm(string str){
    int j=1;
    int k=0;
    for(int i=str.length()-1;i>0;i--){
        if(str[i]=='1'){
            k+=j;
        }
        j*=2;
    }
    if(str[0]=='1'){
        k-=j;
    }
    return k;
}

int main(){


int i=0;
ifstream in("encoding.txt");
string instruction;
string gpr[32];
int arr[100000]={0};
for(int i=0; i<32; i++){
    gpr[i]="00000000000000000000000000000000";
}
vector <string> v;
int l=0;
while(getline(in, instruction)){

    
 
    l++;  //pc ko increment kar diya
    v.push_back(instruction);
    }
  
   
   
   for(int i=0; i < l ; i++){
    cout<<v[i];
    string opcode=v[i].substr(25,7);
    if(opcode=="0010011" )//i
    {
        int imm=bin2imm(instruction.substr(0, 12));
        int rs1=bin2imm(instruction.substr(12, 5));
        string f3=instruction.substr(17, 3);
        int rd=bin2imm(instruction.substr(20, 5));
        string f7="3";
        controlUnit(opcode);
        int r1, r2;
        if(regRead==1){
        r1=bin2imm(gpr[rs1]);
        }
        if(ALUSrc==0 && regRead==1){
        r2=bin2imm(gpr[rd]);
        }
        if(ALUSrc==1){
            r2=imm;
        }
        ALUSelect=ALUControl(f7, f3);
        ALUResult=ALU(ALUSelect, r1, r2);
        if(r1==r2){
            flag=1;
        }
        if(regWrite==1){
            if(mem2Reg==0){
                
                gpr[rd]=intToBinary(ALUResult);

            }

        }




    }
  
    else if(opcode=="0110011"){//r
        int f7=bin2imm(instruction.substr(0, 7));
        int rs2=bin2imm(instruction.substr(7, 5));
        int rs1=bin2imm(instruction.substr(12, 5));
        int f3=bin2imm(instruction.substr(17, 3));
        int rd=bin2imm(instruction.substr(20, 5));
        controlUnit(opcode);
        int r1, r2;
        if(regRead==1){
        r1=bin2imm(gpr[rs1]);
        }
        if(regRead==1){
        r2=bin2imm(gpr[rs2]);
        }
        ALUSelect=ALUControl(f7, f3);
        ALUResult=ALU(ALUSelect, r1, r2);
        if(r1==r2){
            flag=1;
        }
        if(regWrite==1){
            if(mem2Reg==0){
                   gpr[rd]=intToBinary(ALUResult);

            }

        }

    }

    else if(opcode=="1100011"){
        string imm=instruction.substr(0, 7)+instruction.substr(20, 5);
        int immediate=bin2imm(imm);
        int rs2=bin2imm(instruction.substr(7, 5));
        int rs1=bin2imm(instruction.substr(12, 5));
        int f3=bin2imm(instruction.substr(17, 3));        
        controlUnit(opcode);
         int r1, r2;
        if(regRead==1){
        r1=bin2imm(gpr[rs1]);
        }
        if(regRead==1){
        r2=bin2imm(gpr[rs2]);
        }
        if(r1==r2){
            flag=1;
        }
        if(flag==1){
            i=i+immediate;

        }
        else{
            continue;
        }



    }
}
cout<<"hello"<<endl;

for(int i=0; i<32; i++){
  cout<< " pp"<< gpr[i]<<endl;
}
       

    return 0;
}