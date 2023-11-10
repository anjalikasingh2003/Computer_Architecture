#include <bits/stdc++.h>
#include <fstream>
using namespace std;

unordered_map<string, char> inst_type;
unordered_map<string, int> register_no;
unordered_map<char, int> opc;
unordered_map<string, int> func3;
unordered_map<string, int> func7;

void  naming(){

//R-Type
inst_type ["add"]='R';
inst_type ["sub"]='R';

//I-Type
inst_type ["addi"]='I';
inst_type ["slli"]='I'; 
inst_type ["srai"]='I';

//L-Type
inst_type ["lw"]='L';

//S-Type
inst_type["sw"]='S';

//B-Type
inst_type["beq"]='B';
inst_type["bne"]='B';
inst_type["bge"]='B';
inst_type["blt"]='B';

//J-Type
inst_type["jal"]='J';
inst_type["jalr"]='J';

//Registers
register_no["zero"]=0;
register_no["ra"]=1;
register_no["sp"]=2;
register_no["gp"]=3;
register_no["tp"]=4;
register_no["t0"]=5;
register_no["t1"]=6;
register_no["t2"]=7;
register_no["s0"]=8;
register_no["s1"]=9;
register_no["a0"]=10;
register_no["a1"]=11;
register_no["a2"]=12;
register_no["a3"]=13;
register_no["a4"]=14;
register_no["a5"]=15;
register_no["a6"]=16;
register_no["a7"]=17;
register_no["s2"]=18;
register_no["s3"]=19;
register_no["s4"]=20;
register_no["s5"]=21;
register_no["s6"]=22;
register_no["s7"]=23;
register_no["s8"]=24;
register_no["s9"]=25;
register_no["s10"]=26;
register_no["s11"]=27;
register_no["t3"]=28;
register_no["t4"]=29;
register_no["t5"]=30;
register_no["t6"]=31;



    opc['R']=51;
    opc['I']=19;
    opc['B']=99;
    opc['J']=111;
    opc['S']=35;
    opc['L']=3;

   func3["add"]=0;
   func3["sub"]=0;
   func3["addi"]=0;
   func3["lw"]=3;
   func3["sw"]=2;
   func3["beq"]=0;
   func3["bge"]=5;
   func3["blt"]=4;
   func3["bne"]=1;
   func3["slli"]=1;
   func3["srai"]=5;

   func7["add"]=0;
   func7["sub"]=32;


}

string encode_L(vector<string> value){
    naming();
int i=stoi(value[2]);
int rs1=register_no[value[3]];
int f3=func3[value[0]];
int rd=register_no[value[1]];
int op=opc['L'];
string s1=bitset<12>(i).to_string();
string s2=bitset<5>(rs1).to_string();
string s3=bitset<3>(f3).to_string();
string s4=bitset<5>(rd).to_string();
string s5=bitset<7>(op).to_string();

return (s1+s2+s3+s4+s5);

}

string encode_S(vector<string> value){
    naming();
int i=stoi(value[2]);
int rs1=register_no[value[3]];
int f3=register_no[value[0]];
int rd=register_no[value[1]];
int op=opc['L'];
string s1=bitset<12>(i).to_string();
string s2=bitset<5>(rs1).to_string();
string s3=bitset<3>(f3).to_string();
string s4=bitset<5>(rd).to_string();
string s5=bitset<7>(op).to_string();

return (s1+s2+s3+s4+s5);

}
string encode_I(vector<string> value){
    naming();
int i=stoi(value[3]);
int rs1=register_no[value[2]];
int f3=func3[value[0]];
int rd=register_no[value[1]];
int op=opc['I'];
string s1=bitset<12>(i).to_string();
string s2=bitset<5>(rs1).to_string();
string s3=bitset<3>(f3).to_string();
string s4=bitset<5>(rd).to_string();
string s5=bitset<7>(op).to_string();

return (s1+s2+s3+s4+s5);

}

string encode_R(vector<string> value){
    naming();

int rs1=register_no[value[3]];
int rs2=register_no[value[2]];
int f3=func3[value[0]];
int rd=register_no[value[1]];
int f7=func7[value[2]];
int op=opc['R'];
string s1=bitset<7>(f7).to_string();
string s2=bitset<5>(rs2).to_string();
string s3=bitset<5>(rs1).to_string();
string s4=bitset<3>(f3).to_string();
string s5=bitset<5>(rd).to_string();
string s6=bitset<7>(op).to_string();

return (s1+s2+s3+s4+s5+s6);

}

string encode_B(vector<string> value, int imm){
    naming();

int rs1=register_no[value[1]];
int rs2=register_no[value[2]];
int f3=func3[value[0]];
int op=opc['B'];
string s=bitset<12>(imm).to_string();
string s1=s.substr(0, 7);
string s2=bitset<5>(rs2).to_string();
string s3=bitset<5>(rs1).to_string();
string s4=bitset<3>(f3).to_string();
string s5=s.substr(7, 5);
string s6=bitset<7>(op).to_string();

return (s1+s2+s3+s4+s5+s6);

}
string encode_J(vector<string> value){
    naming();
int i=stoi(value[2]);
int rs1=register_no[value[3]];
int f3=func3[value[0]];
int rd=register_no[value[1]];
int op=opc['L'];
string s1=bitset<12>(i).to_string();
string s2=bitset<5>(rs1).to_string();
string s3=bitset<3>(f3).to_string();
string s4=bitset<5>(rd).to_string();
string s5=bitset<7>(op).to_string();

return (s1+s2+s3+s4+s5);

}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    
    if (start == std::string::npos || end == std::string::npos) {
        // The string contains only whitespace characters
        return "";
    }
    
    return str.substr(start, end - start + 1);
}


int main(){
    naming();
string s;
ifstream in("CSN221project_program.txt");
ofstream out("encoding.txt");
int pc=0;

unordered_map<string, int> symbol_table;

while(getline(in, s)){
pc++;

vector<string>val;
int j=0;
string e="";

s = trim(s);


for(int i=0; i<s.length(); i++){
    

    if(s[i]!=' ' && s[i]!=',' && s[i]!='(' && s[i]!=')'){
        e=e+s[i];
       
    }
    else{
         
        val.push_back(e);
     
        e="";
    }
}
val.push_back(e);
if(val.size()==1){
symbol_table[val[0]]=pc;
}
}
pc=0;
ifstream inorder("CSN221project_program.txt");
while(getline(inorder, s)){
pc++;

vector<string>value;
int j=0;
string exp="";

s = trim(s);


// unordered_map<string, int> symbol_table;

for(int i=0; i<s.length(); i++){
    

    if(s[i]!=' ' && s[i]!=',' && s[i]!='(' && s[i]!=')'){
        exp=exp+s[i];
       
    }
    else{
      
        value.push_back(exp);
     
        exp="";
    }
}
value.push_back(exp);
if(value.size()==1){
// symbol_table[value[0]]=pc;
continue;
}

// for(int i=0; i<value.size(); i++){
//     cout<<value[i]<<" ";
// }
cout<<endl;
string ans;

if(inst_type[value[0]]=='R'){
 
    ans=encode_R(value);
}
else if(inst_type[value[0]]=='I'){
    
    ans=encode_I(value);
}
else if(inst_type[value[0]]=='L'){
    
    ans=encode_L(value);
}
else if(inst_type[value[0]]=='S'){
    
    ans=encode_S(value);
}
else if(inst_type[value[0]]=='B'){
    
    int imm=pc-symbol_table[value[3]];
    ans=encode_B(value, imm);
}
else if(inst_type[value[0]]=='J'){
  
    ans=encode_J(value);
}


cout<<ans<<endl;


out<<ans<<endl;

}
out.close();

return 0;
}