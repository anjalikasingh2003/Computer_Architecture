#include <bits/stdc++.h>
using namespace std;

struct control
{
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
};

 

struct IFID
{
    int dpc;
    string instruction;
};

struct IDEX
{
    int dpc;
    int imm1;
    int imm2;
    int rs1;
    int rs2;
    int rdl;
   control ctrl;

};

struct EXMO
{
    control ctrl;
    int ALUResult;
    int rs2;
    int rd;;
};


struct MOWB
{
    control ctrl;
    int ALUResult;
    int LDResult;
    int rdl;
 
};



void controlUnit(IDEX &idex, EXMO &exmo, MOWB &mowb,  string opcode)
{
    // Set control signals based on opcode for IDEX
    if (opcode == "0110011")
    {
        idex.ctrl.ALUSrc = false;
        idex.ctrl.regWrite = true;
        idex.ctrl.regRead = true;
        idex.ctrl.ALUOp = 10;
    }
    else if (opcode == "0010011")
    {
        idex.ctrl.ALUSrc = true;
        idex.ctrl.regRead = true;
        idex.ctrl.regWrite = true;
        idex.ctrl.ALUOp = 11;
    }
    else if (opcode == "1100011")
    {
        idex.ctrl.ALUSrc = false;
        idex.ctrl.branch = true;
        idex.ctrl.ALUOp = 01;
        idex.ctrl.regRead = true;
    }

    // Set control signals based on opcode for EXMO
    if (opcode == "0110011")
    {
        exmo.ctrl.ALUSrc = false;
        exmo.ctrl.regWrite = true;
        exmo.ctrl.regRead = true;
        exmo.ctrl.ALUOp = 10;
    }
    else if (opcode == "0010011")
    {
        exmo.ctrl.ALUSrc = true;
        exmo.ctrl.regRead = true;
        exmo.ctrl.regWrite = true;
        exmo.ctrl.ALUOp = 11;
    }
    else if (opcode == "1100011")
    {
        exmo.ctrl.ALUSrc = false;
        exmo.ctrl.branch = true;
        exmo.ctrl.ALUOp = 01;
        exmo.ctrl.regRead = true;
    }

    // Set control signals based on opcode for MOWB
    if (opcode == "0110011")
    {
        mowb.ctrl.ALUSrc = false;
        mowb.ctrl.regWrite = true;
        mowb.ctrl.regRead = true;
        mowb.ctrl.ALUOp = 10;
    }
    else if (opcode == "0010011")
    {
        mowb.ctrl.ALUSrc = true;
        mowb.ctrl.regRead = true;
        mowb.ctrl.regWrite = true;
        mowb.ctrl.ALUOp = 11;
    }
    else if (opcode == "1100011")
    {
        mowb.ctrl.ALUSrc = false;
        mowb.ctrl.branch = true;
        mowb.ctrl.ALUOp = 01;
        mowb.ctrl.regRead = true;
    }
}

string ALUControl(control &ctrl, string f7, string f3){
    if(ctrl.ALUOp==10){
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

control ctrl;
IFID ifid;
IDEX idex;
EXMO exmo;
MOWB mowb;

idex.ctrl = control();
exmo.ctrl = control();
mowb.ctrl = control();

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
    ifid.dpc=i;
    ifid.instruction=v[i];
    string opcode=v[i].substr(25,7);
    idex.dpc=i;
    if(opcode=="0010011" )//i
    {
        idex.imm1=bin2imm(instruction.substr(0, 12));
        idex.rs1=bin2imm(instruction.substr(12, 5));
        string f3=ifid.instruction.substr(17, 3);
        idex.rdl=bin2imm(ifid.instruction.substr(20, 5));
        string f7="3";
        
        controlUnit(idex, exmo, mowb, opcode);
        int r1, r2;
        if(ctrl.regRead==1){
        r1=bin2imm(gpr[idex.rs1]);
        }
        if(ctrl.ALUSrc==0 && ctrl.regRead==1){
        r2=bin2imm(gpr[idex.rdl]);
        }
        if(ctrl.ALUSrc==1){
            r2=idex.imm1;
        }
        exmo.ctrl.ALUSelect=ALUControl(ctrl, f7, f3);
        ctrl.ALUResult=ALU(ctrl.ALUSelect, r1, r2);
        mowb.ctrl.ALUResult=exmo.ctrl.ALUResult;
        mowb.rdl=idex.rdl;
        if(r1==r2){
            ctrl.flag=1;
        }
        if(ctrl.regWrite==1){
            if(ctrl.mem2Reg==0){
                
                gpr[idex.rdl]=intToBinary(ctrl.ALUResult);

            }

        }




    }
  
    else if(opcode=="0110011"){//r
        string f7=instruction.substr(0, 7);
        idex.rs2=bin2imm(instruction.substr(7, 5));
        idex.rs1=bin2imm(instruction.substr(12, 5));
        string f3=instruction.substr(17, 3);
        idex.rdl=bin2imm(instruction.substr(20, 5));
        exmo.rd=idex.rdl;
        mowb.rdl=idex.rdl;
        controlUnit(idex, exmo, mowb, opcode);
        int r1, r2;
        if(ctrl.regRead==1){
        r1=bin2imm(gpr[idex.rs1]);
        }
        if(ctrl.regRead==1){
        r2=bin2imm(gpr[idex.rs2]);
        }
        ctrl.ALUSelect=ALUControl(ctrl, f7, f3);
        ctrl.ALUResult=ALU(ctrl.ALUSelect, r1, r2);
        exmo.ALUResult=ctrl.ALUResult;
        mowb.ALUResult=ctrl.ALUResult;
        if(r1==r2){
            ctrl.flag=1;
        }
        if(ctrl.regWrite==1){
            if(ctrl.mem2Reg==0){
                   gpr[mowb.rdl]=intToBinary(ctrl.ALUResult);

            }

        }

    }

    else if(opcode=="1100011"){ //branch
        string imm=instruction.substr(0, 7)+instruction.substr(20, 5);
        idex.imm1=bin2imm(imm);
        idex.rs2=bin2imm(instruction.substr(7, 5));
        idex.rs1=bin2imm(instruction.substr(12, 5));
        string f3=instruction.substr(17, 3);        
        controlUnit(idex, exmo, mowb, opcode);
         int r1, r2;
        if(ctrl.regRead==1){
        r1=bin2imm(gpr[idex.rs1]);
        }
        if(ctrl.regRead==1){
        r2=bin2imm(gpr[idex.rs2]);
        }
        if(r1==r2){
            ctrl.flag=1;
        }
        if(ctrl.flag==1){
            i=i+idex.imm1;

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