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
    int flag=0;
};

 struct PC{
    int pc;
    int valid;
 };

struct IFID
{
    int dpc;
    string instruction;
    int valid;
    int stall;
};

struct IDEX
{
    int dpc;
    int imm1;
    int imm2;
    int rsl1;
    int rsl2;
    int rs1;
    int rs2;
    int rdl;
    int func3;
    int func7;
    int valid;
    int stall;
   control ctrl;

};

struct EXMO
{
    control ctrl;
    int ALUResult;
    int rs2;
    int rdl;
    int rd;
    int tpc;
    int valid;
    int stall;
};


struct MOWB
{
    control ctrl;
    int ALUResult;
    int LDResult;
    int rdl;
    int valid;
    int stall;
 
};

unordered_map<int, int> memory;

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


int forwarder(int rsl,  int rs, int Erdl, int Mrdl, IDEX &idex){
    if(rsl==Erdl && rsl==idex.rsl1){
        return idex.rs1;
    }
    if(rsl==Erdl && rsl==idex.rsl2){
        return idex.rs2;
    }
    if(rsl==Mrdl && rsl==idex.rsl1){
        return idex.rs1;
    }
     if(rsl==Mrdl && rsl==idex.rsl2){
        return idex.rs2;
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


void IF_stage(IFID &ifid, PC &pc, IDEX &idex, string &instruction) {

    if(ifid.stall || !pc.valid){
        return;
    }
    ifid.dpc=pc.pc;
    ifid.instruction = instruction;
       pc.valid=true;
}

void ID_stage(IFID &ifid, IDEX &idex, EXMO &exmo, MOWB &mowb, control &ctrl, string &opcode, string* gpr) {

    if(idex.stall || !ifid.valid){
        return;
    }

    idex.dpc = ifid.dpc;
    
    if(opcode=="0010011" )//i
    {
        idex.imm1=bin2imm(ifid.instruction.substr(0, 12));
        idex.rs1=bin2imm(ifid.instruction.substr(12, 5));
        idex.func3=bin2imm(ifid.instruction.substr(17, 3));
        idex.rdl=bin2imm(ifid.instruction.substr(20, 5));
        idex.func7=3;
    }
    else if(opcode=="1100011"){ //branch
        string imm=ifid.instruction.substr(0, 7)+ifid.instruction.substr(20, 5);
        idex.imm1=bin2imm(imm);
        idex.rs2=bin2imm(ifid.instruction.substr(7, 5));
        idex.rs1=bin2imm(ifid.instruction.substr(12, 5));
        idex.func3=bin2imm(ifid.instruction.substr(17, 3)); 
    }
    else if(opcode=="0110011"){//r
        idex.func7=bin2imm(ifid.instruction.substr(0, 7));
        idex.rs2=bin2imm(ifid.instruction.substr(7, 5));
        idex.rs1=bin2imm(ifid.instruction.substr(12, 5));
        idex.func3=bin2imm(ifid.instruction.substr(17, 3));
        idex.rdl=bin2imm(ifid.instruction.substr(20, 5));
        

        }

    
        controlUnit(idex, exmo, mowb, opcode);
        int r1, r2;
    
        if(idex.ctrl.regRead==1){
        r1=bin2imm(gpr[idex.rs1]);
        }
        if(idex.ctrl.ALUSrc==0 && idex.ctrl.regRead==1){
        r2=bin2imm(gpr[idex.rdl]);
        }
        if(idex.ctrl.ALUSrc==1){
            r2=idex.imm1;
        }

   idex.valid=true;
   ifid.stall=false;

}

void EX_stage(IDEX &idex, EXMO &exmo, MOWB &mowb, control &ctrl,  string* gpr, IFID &ifid, PC &pc) {

     if(exmo.stall || !idex.valid){
        return;
    }
    
    int aluinput1, aluinput2;
    aluinput1=forwarder(idex.rsl1, idex.rs1, exmo.rdl, mowb.rdl, idex );
    aluinput2=forwarder(idex.rsl2, idex.rs2, exmo.rdl, mowb.rdl, idex);
    exmo.ALUResult = ALU(ctrl.ALUSelect, aluinput1, aluinput2);
     if(aluinput1==aluinput2){
            ctrl.flag=1;
        }
    exmo.ctrl=idex.ctrl;
      if(ctrl.flag==1){
            exmo.tpc=idex.dpc+idex.imm1;
            idex.valid=false;
            ifid.valid=false;
            pc.valid=false;
        }
        exmo.valid=true;
        idex.stall=false;


}

void MEM_stage(EXMO &exmo, MOWB &mowb, IDEX &idex, PC &pc, control &ctrl, string *gpr) {
      if(mowb.stall || !exmo.valid){
        return;
    }
   
    mowb.ALUResult = exmo.ALUResult;
    mowb.rdl = exmo.rdl;
    if (exmo.ctrl.memWrite){
        memory[exmo.ALUResult]=idex.rs2;
    }
    if (exmo.ctrl.memRead){
       mowb.LDResult= memory[exmo.ALUResult];
    }
    mowb.ALUResult = exmo.ALUResult;
    mowb.ctrl=exmo.ctrl;
    if(ctrl.flag==1){
        pc.pc=exmo.tpc;
        pc.valid=true;
    }
    exmo.stall=false;
    mowb.valid=true;
}

void WB_stage(MOWB &mowb, string* gpr) {
    if(!mowb.valid){
        return;
    }
    if(!mowb.ctrl.regWrite){
        return;
    }
    if (mowb.ctrl.regWrite == 1) {
        if (mowb.ctrl.mem2Reg == 0) {
            gpr[mowb.rdl] = intToBinary(mowb.ALUResult);
        }
        else{
             gpr[mowb.rdl] = intToBinary(mowb.LDResult);
        }
    }
   mowb.stall=false;
}


int main(){

control ctrl;
PC pc;
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


     string opcode=v[i].substr(25,7);
        // IF stage
        IF_stage(ifid,pc, idex, v[i]);
        i+=4;

        // ID stage
        ID_stage(ifid, idex, exmo, mowb, ctrl, opcode, gpr);

        // EX stage
        EX_stage(idex, exmo, mowb, ctrl, gpr, ifid, pc);

        // MEM stage
        MEM_stage(exmo, mowb, idex, pc, ctrl, gpr);

        // WB stage
        WB_stage(mowb, gpr);


    }



    return 0;
}