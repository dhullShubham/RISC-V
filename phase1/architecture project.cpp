#include <bits/stdc++.h>
using namespace std;
int registers[32] = {0}; //for 32 bit 
unordered_map<long long int,string> data_memory; //We will not waste space by using hashmaps
unordered_map<int,string> instr_memory; //to store the instructions, also will help later in pipelining gives instruction when given PC
//just some utility functions which will be useful later 
long int binary_decimal(string bin, bool isimm)
{
    //isimm tells if it is immediate value
    long int ans =0;
    int j=1;
    for(int i=bin.length()-1;i>=0;i--)
    {
        if(i!= 0)
        {ans += ((bin[i]-48)*j);}
        else{
          if(isimm)  ans -= ((bin[i]-48)*j); //handled two's complement
          else ans += ((bin[i]-48)*j);
        }
        j = j<<1;
    }
    return ans;
}
string util_decimal_hexa(int n)
{
    if(n==0) return "0";
    else if(n==1) return "1";
    else if(n==2) return "2";
    else if(n==3) return "3";
    else if(n==4) return "4";
    else if(n==5) return "5";
    else if(n==6) return "6";
    else if(n==7) return "7";
    else if(n==8) return "8";
    else if(n==9) return "9";
    else if(n==10) return "A";
    else if(n==11) return "B";
    else if(n==12) return "C";
    else if(n==13) return "D";
    else if(n==14) return "E";
    else if(n==15) return "F";
    return "";
    
}

string decimal_hexa(int n)
{
    string ans="";
    if(n==0) return "0x00000000";
    while(n!=0)
    {
        ans = util_decimal_hexa(n%16) + ans;
        n=n/16;
    }
    while(ans.length()<8){
        ans.insert(0,"0");
    }
    ans.insert(0,"0x");
    return ans;
}

int util_hexa_decimal(char hexa)
{
    if(hexa== '0') return 0;
    else if(hexa == '1') return 1;
    else if(hexa == '2') return 2;
    else if(hexa == '3') return 3;
    else if(hexa == '4') return 4;
    else if(hexa == '5') return 5;
    else if(hexa == '6') return 6;
    else if(hexa == '7') return 7;
    else if(hexa == '8') return 8;
    else if(hexa == '9') return 9;
    else if(hexa == 'A' || hexa == 'a') return 10;
    else if(hexa == 'B' || hexa == 'b') return 11;
    else if(hexa == 'C' || hexa == 'c') return 12;
    else if(hexa == 'D' || hexa == 'd') return 13;
    else if(hexa == 'E' || hexa == 'e') return 14;
    else if(hexa == 'F' || hexa == 'f') return 15;
    return 0;
}

long long int hexa_decimal(string hexa)
{
    long int ans = 0;
    int j=1;
    for(int i=hexa.length()-1;i>=0;i--)
    {
        ans += util_hexa_decimal(hexa[i])*j;
        j = j<<4;
    }
    return ans;
}

string util_hexa_binary(string hexa)
{
    if(hexa=="0") return "0000";
    if(hexa=="1") return "0001";
    if(hexa=="2") return "0010";
    if(hexa=="3") return "0011";
    if(hexa=="4") return "0100";
    if(hexa=="5") return "0101";
    if(hexa=="6") return "0110";
    if(hexa=="7") return "0111";
    if(hexa=="8") return "1000";
    if(hexa=="9") return "1001";
    if(hexa=="A") return "1010";
    if(hexa=="B") return "1011";
    if(hexa=="C") return "1100";
    if(hexa=="D") return "1101";
    if(hexa=="E") return "1110";
    if(hexa=="F") return "1111";
    else return "";
}

string hexa_binary(string hexa)
{
    string ans = "";
    for(int i=0;i<8;i++)
        {
            string temp;
            temp=hexa[i];
            ans += util_hexa_binary(temp);
        }
    return ans;
}
//starting decode
//The instructions we are considerign is
int decode_Rtype(string instr); // for decoding r type instructions
int decode_SBtype(string instr);// for sb types
int decode_Itype(string instr);//for i types
int decode_Utype(string instr);//for U types
int decode_Stype(string instr);//for S types
int decode_UJtype(string instr);//for UJ type 

//global variable for registers
int reg1;// first operand register
int reg2;//second register
int dest_reg=0;// destination register
// for a constant value
long int immediate;

int decode_Rtype(string instr)//this instr will be in binary format initially
{
    cout<<"Decoding...."<<endl;
    string opcode = instr.substr(25,7);
    string funct3 = instr.substr(17,3);
    string funct7 = instr.substr(0,7);
    //writing the cases
    int indicator; 
    /*
    1 for add
    2 for sub
    3 for xor 
    4 for mul
    5 for div
    6 for modulo
    7 for and
    */
   if(funct3=="000"&&funct7=="0000000")
        indicator = 1;
    else if(funct3=="000"&&funct7=="0100000")
        indicator = 2;
    else if(funct3=="100"&&funct7=="0000000")
        indicator = 3;
    else if(funct3=="000"&&funct7=="0000001")
        indicator = 4;
    else if(funct3=="100"&&funct7=="0000001")
        indicator = 5;
    else if(funct3=="110"&&funct7=="0000001")
        indicator = 6;
    else if(funct3=="111"&&funct7=="0000001")
        indicator = 7;    
    string operand2 = instr.substr(7,5);
    string operand1 = instr.substr(12,5);
    string destination = instr.substr(20,5);
    reg1 = binary_decimal(operand1,false);
    reg2 = binary_decimal(operand2,false);
    dest_reg = binary_decimal(destination, false);
    cout<< "R type was decoded"<<endl;
    cout<< "The first register is "<<reg1<<" second register is "<<reg2<<endl;
    cout<<"The destination register is "<<dest_reg<<endl;
    if(indicator == 1) cout<<"It was an add instuction"<<endl;
    else if(indicator == 2) cout<<"It was a subtract instuction"<<endl;
    else if(indicator == 3) cout<<"It was a xor instuction"<<endl;
    else if(indicator == 4) cout<<"It was a multiply instuction"<<endl;
    else if(indicator == 5) cout<<"It was a division instuction"<<endl;
    else if(indicator == 6) cout<<"It was a modulo instuction"<<endl;
    else if(indicator == 7) cout<<"It was and instruction"<<endl;
    cout<<"Reading the registers"<<endl;
    cout<<"Initial values in x"<<reg1<< " is "<<registers[reg1]<<" value in" <<reg2<<"register is "<<registers[reg2]<<endl;
    cout<<"value in destination register x"<<dest_reg<<" is "<<registers[dest_reg]<<endl;
    return indicator;
}

int decode_Itype(string instr)
{
    cout<<"Decoding...."<<endl;
    cout<< "I type was decoded"<<endl;
    string imm_temp = instr.substr(0,12);
    string operand1 = instr.substr(12,5);
    string funct3 = instr.substr(17,3);
    string destination = instr.substr(20,5);
    string opcode = instr.substr(25,7);
    int indicator;
    /*
    1 for addi
    2 for jalr
    3 for ori 
    4 for lw
    */
    if(opcode=="0010011"&&funct3=="000")
        {indicator = 1;
        cout<< "It was an addi instruction"<<endl;
        }
    else if(opcode=="1100111"&&funct3=="000")
        {indicator = 2;
        cout<< "It was a jalr instruction"<<endl;
        }
    else if(opcode=="0010011"&&funct3=="110")
        {indicator = 3;
        cout<<"It was a ori instruction"<<endl;
        }
    else if(opcode=="0000011"&&funct3=="010")
        {indicator = 4;
        cout<<"It was a w instruction"<<endl;
        }
    reg1 = binary_decimal(operand1,false);
    dest_reg= binary_decimal(destination,false);
    //Handling two's complement
    immediate = binary_decimal(imm_temp,true);
    cout<<"Operand register is "<<reg1<<" and the immediate value is "<<immediate<<endl;
    cout<<"The destination is x"<<dest_reg<<" and the value stored in it is "<<registers[dest_reg]<<endl;
    cout<<"Initial values in x"<<reg1<< " is "<<registers[reg1]<<endl;
    return indicator;
}

int decode_SBtype(string instr)
{
    cout<<"Decoding...."<<endl;
    cout<< "SB type was decoded"<<endl;
    string imm_temp = instr.substr(0,1) + instr.substr(24,1) + instr.substr(1,6) + instr.substr(20,4) + "0"; 
    string operand1 = instr.substr(12,5);
    string operand2 = instr.substr(7,5);
    string funct3 = instr.substr(17,3);
    string opcode = instr.substr(25,7);
    int indicate;
    /*
    1 for beq 
    2 for bne
    */
   if(opcode=="1100011"&&funct3=="000")
    {indicate = 1;
    cout<<"It was beq"<<endl;
    }
   else if(opcode=="1100011"&&funct3=="001")
    {
        indicate = 2;
        cout<<"it was bne"<<endl;
    }
    reg1 = binary_decimal(operand1,false);
    reg2 = binary_decimal(operand2,false);
    immediate = binary_decimal(imm_temp,true);
   cout<<"Operand register 1 is "<<reg1<<" and the immediate value is "<<immediate<<endl;
    cout<<"The operand register 2 is "<<reg2<<endl;
    cout<<"Reading the registers"<<endl;
    cout<<"Initial values in x"<<reg1<< " is "<<registers[reg1]<<" value in x"<<reg2<<"register is "<<registers[reg2]<<endl;
    return indicate;
}

int decode_Utype(string instr)//for U types
{
    cout<<"Decoding....."<<endl;
    string imm_temp = instr.substr(0,20); //[31:12]
    string destination = instr.substr(20,5);
    string opcode= instr.substr(25,7);
    int indicator;
    /*
    1 for auipc
    2 for lui
    */
    if(opcode=="0010111")
    {
        indicator = 1;
        cout<<"it is auipc"<<endl;
    }
    else if(opcode=="0110111")
    {
        indicator = 2;
        cout<<"it is lui"<<endl;
    }

    imm_temp = imm_temp + "000000000000";
    dest_reg=binary_decimal(destination,false);
    immediate = binary_decimal(imm_temp,true);
    cout<<"Reading registers"<<endl;
    cout<<"destination register is x"<<dest_reg<<" and value in it is "<<registers[dest_reg]<<endl;
    return indicator;
}

int decode_UJtype(string instr)
{
    cout<<"Decoding"<<endl;
    string imm_1=instr.substr(0,1);
    string imm_2=instr.substr(1,10);
    string imm_3=instr.substr(11,1);
    string imm_4=instr.substr(12,8);
    string destination=instr.substr(20,5);
    string imm_temp = imm_1+imm_4+imm_3+imm_2+"0";
    string opcode = instr.substr(25,7);
    int indicator;
    /*
    1 fro jal
    */
    if(opcode=="1101111")
    {
        indicator = 1;
        cout<<"it was jal instruction, a UJ type"<<endl;
    }
    dest_reg = binary_decimal(destination,false);
    immediate = binary_decimal(imm_temp,true);
    cout<<"destination register is x"<<dest_reg<<" and value at it is "<<registers[dest_reg]<<endl;
    cout<<"Immediate value in use is "<<immediate<<endl;
    return indicator;
}

int decode_Stype(string instr)
{
    cout<<"Decoding..."<<endl;
    string imm_1 = instr.substr(0,7);
    string imm_2 = instr.substr(20,5);
    string imm_temp = imm_1+imm_2;
    string operand1 = instr.substr(12,5);
    string funct3 = instr.substr(17,3);
    string destination = instr.substr(7,5);
    int indicator;
    string opcode = instr.substr(25,7);
    cout<<"opcode is "<<opcode<<" and funct3 is "<<funct3<<endl;
    //implementing sw and sb
    /*
    1 for sb
    2 for sw
    */
    if(opcode=="0100011"&&funct3=="000")
     {
         cout<<"This is sb, a S-type instruction"<<endl;
         indicator = 1;
     }
    else if(opcode=="0100011"&&funct3=="010")
    {
        cout<<"This is sw, a S-type instruction"<<endl;
        indicator = 2; 
    }
    reg1 = binary_decimal(operand1,false);
    dest_reg = binary_decimal(destination,false);
    immediate = binary_decimal(imm_temp,true);
    cout<<"Operand register 1 is "<<reg1<<" and the immediate value is "<<immediate<<endl;
    cout<<"Reading the registers"<<endl;
    cout<<"Initial values in x"<<reg1<< " is "<<registers[reg1]<<" value in x"<<dest_reg<<"register is "<<registers[dest_reg]<<endl;
    return indicator;
}
//Decoding stage completed..
//Starting execution stage..
//function declaration
int execute_Rtype(int indicator); // for decoding r type instructions
int execute_SBtype(int indicator);// for sb types
int execute_Itype(int indicator);//for i types
int execute_Utype(int indicator);//for U types
int execute_Stype(int indicator);//for S types
int execute_UJtype(int indicator);//for UJ type 

//function definitons

int buffer_val=0; //for temporary buffer store, will write in write back stage 
int P_C; //for the program counter

int execute_Rtype(int indicator) // for decoding r type instructions
{
    /*
    1 for add
    2 for sub
    3 for xor 
    4 for mul
    5 for div
    6 for modulo
    7 for and
    */
   cout<<"executing R type"<<endl;
    if(indicator == 1)
        buffer_val = registers[reg1]+registers[reg2];
    else if(indicator == 2)
       buffer_val = registers[reg1]-registers[reg2];
    else if(indicator == 3)
        buffer_val = registers[reg1]^registers[reg2];
    else if(indicator == 4)
        buffer_val = registers[reg1]*registers[reg2];
    else if(indicator == 5)
        buffer_val = registers[reg1]/registers[reg2];
    else if(indicator == 6)
        buffer_val = registers[reg1]%registers[reg2];
    else if(indicator == 7)
        buffer_val = registers[reg1]&registers[reg2];
    cout<<"Value stored in buffer is"<<buffer_val<<endl;
    return indicator;
}

int execute_Itype(int indicator) //for i types
{
    /*
    1 for addi
    2 for jalr
    3 for ori 
    4 for lw
    */
    if(indicator == 1)
       buffer_val = registers[reg1]+immediate;
    else if(indicator == 2)
    {    buffer_val = P_C+4; //keep the current in temp useless
         P_C = immediate+registers[reg1]-4; // the adress where jump wants to go
    }
    else if(indicator == 3)
    {
        buffer_val = registers[reg1]|immediate;
    }
    else if(indicator == 4)
    {
        int real_add = immediate + registers[reg1];
        cout<<"The effective address is "<<real_add<<endl;
        string wrd = ""; //need to load 8 chars to get 32 bits ; stored in hexa in memory
        string p1,p2,p3,p4;
        if(data_memory.find(real_add) != data_memory.end()) 
         p4 = data_memory[real_add] ;
        else {
            cout<<"Nothing was stored there so putting a null"<<endl;
            p4 = "";
        }
        if(data_memory.find(real_add+1) != data_memory.end()) 
         p3 = data_memory[real_add+1] ;
        else {
            cout<<"Nothing was stored there so putting a null"<<endl;
            p3 = "";
        }
        if(data_memory.find(real_add+2) != data_memory.end()) 
         p2 = data_memory[real_add+2] ;
        else{
            cout<<"Nothing was stored there so putting a null"<<endl;
            p2 = "";
        }
        if(data_memory.find(real_add+3) != data_memory.end()) 
         p1 = data_memory[real_add+3];
        else{
            cout<<"Nothing was stored there so putting a null"<<endl;
            p1 = "";
        }
        wrd = p1+p2+p3+p4;
        buffer_val = hexa_decimal(wrd); // this is the value finally which will be loaded
    }
    cout<<"Value in buffer is "<<buffer_val<<endl;
    return 0;
}

int execute_SBtype(int indicator)// for sb types
{
    /*
    1 for beq 
    2 for bne*/
    if(indicator == 1)
    {
        if(registers[reg1] == registers[reg2])
        {P_C += immediate; cout<<"Yes the condition got satisfied"<<endl;}
        else 
        P_C += 4;
    }
    else {
        if(registers[reg1] != registers[reg2])
        {P_C += immediate; cout<<"Yes the condition got satisfied"<<endl;}
        else 
        P_C += 4;
    }
    //al other are similar
    cout<<"program counter is "<<P_C<<endl;
    return 0;
}

int execute_Utype(int indicator)
{
    /*
    1 for auipc
    2 for lui
    */
   if(indicator == 1)
    buffer_val = P_C+immediate;
   else
    buffer_val = immediate;
    cout<<"Executed"<<endl;
    cout<<"Value in buffer is "<<buffer_val<<endl;
    return 0;
}

int execute_Stype(int indicator)
{
    /*
    1 for sb
    2 for sw
    */
   //no matter what the sb or sw we need to go at EA
   int eff_add = immediate + registers[reg1];// basically EA
   cout<<"executed "<<endl;
   cout<<"The effective adress for s type is "<<eff_add<<endl;
   return eff_add;
}

int execute_UJtype(int indicator)
{
    buffer_val = P_C+4; //store older adress in temp
    P_C += immediate; // go to where it wants
    //uj types we have are basically jumps anyways
    return 0;
}

//Execution stage complete!!
//Now writing Memory acess stage

int Memory_access(int indicator, int real_add) //not required everytime
{
    /*
    1 for sb
    2 for sw
    */
   if(indicator == 2)
   {
       cout<<"Memory access ran"<<endl;
       string data = decimal_hexa(registers[dest_reg]); //it will be transferred
       cout<<"value in consideration is "<<registers[dest_reg]<<" and ans was "<<data<<endl; 
       data = data.erase(0,2);
       cout<<"The data from dest reg in hexa is "<<data<<endl;
       data_memory.insert(make_pair(real_add, data.substr(6,2)));
       data_memory.insert(make_pair(real_add+1, data.substr(4,2)));
       data_memory.insert(make_pair(real_add+2, data.substr(2,2)));
       data_memory.insert(make_pair(real_add+3, data.substr(0,2)));
   }

   else{
       string word = decimal_hexa(registers[dest_reg]);
       data_memory[real_add] = word.substr(7,1); 
   }

   return 0;
}

//final operation

int write_back()
{
    cout<<"Did write back"<<endl;
    registers[dest_reg] = buffer_val;
    cout<<"Destination register x"<<dest_reg<<" is "<<registers[dest_reg]<<endl;
    return 0;
}

int main()
{
    ifstream fin;
    string line;
    string counter = "", instru = "";
    int clocks = 0;
    fin.open("File Path here...");
    //let me first load it in instruction memory

    while(fin.eof() == false)
    {
        string adr;
        string data;
        getline(fin, line);
        stringstream iss(line);
        iss>>adr;
        iss>>data;
        if(adr == "text") break;
        adr = adr.erase(0,2);
        int real_add = hexa_decimal(adr);
        data = data.erase(0,2); 
        cout<<"data I read is "<<data<<endl;
        while(data.length()!=8)
        {
            data = "0"+data;
        }
        data_memory[real_add] = data.substr(6,2);
        //cout<<"I put "<<data.substr(7,2)<<" at "<<real_add<<" location in memory"<<endl; 
        data_memory[real_add+1] = data.substr(4,2);
       // cout<<"I put "<<data.substr(5,2)<<" at "<<real_add+1<<" location in memory"<<endl; 
        data_memory[real_add+2] = data.substr(2,2);
       // cout<<"I put "<<data.substr(3,2)<<" at "<<real_add+2<<" location in memory"<<endl; 
        data_memory[real_add+3] = data.substr(0,2); // 
        //cout<<"I put "<<data.substr(1,2)<<" at "<<real_add+3<<" location in memory"<<endl; 
    }

    string pc_adress;
    string instruction;

    while(fin.eof()==false)
    {
        getline(fin,line);  
        istringstream iss(line); 
        
        iss>>pc_adress;
        iss>>instruction;
      //  cout<<"Fetch working....."<<endl;
       // cout<<"The adress pc is "<<pc_adress<<endl;
       // cout<<"The instruction is "<<instruction<<endl;
        counter = pc_adress.erase(0,2); // has the pc in hex
        instru = instruction.substr(2,8);// has the inistruction in hex
        P_C = hexa_decimal(counter);
        instruction = hexa_binary(instru);
        cout<<"PC in decimal is "<<P_C<<endl;
        cout<<"instruction in hexa is "<<instru<<endl<<" and in bin is "<<instruction<<endl;
        //now we hae both pc and instruction in bin
        instr_memory.insert(make_pair(P_C,instruction));
    }
    for(auto x : instr_memory)
    {
        cout<<"The pc is "<<decimal_hexa(x.first)<<" and instr at it is "<<x.second<<endl;
    }
    int P_Cfinal = P_C;
    cout<<"PC final is "<<P_Cfinal<<endl;
    P_C = 0;
    while(P_C<=P_Cfinal){
        if(instr_memory.find(P_C) == instr_memory.end()) break;
        cout<<"Program counter is"<<P_C<<endl;
        instruction = instr_memory[P_C];
        string opcode = instruction.substr(25,7);
        if(opcode=="0110011")
        {
            int indicator = decode_Rtype(instruction);
            execute_Rtype(indicator);
            cout<<"NOP signal for memory access"<<endl;
            write_back();
            P_C += 4;
        }
        else if(opcode=="0000011"||opcode=="0010011"||opcode=="1100111")
        {
            int indicator=decode_Itype(instruction);
            execute_Itype(indicator);
            cout<<"NOP signal for memory access"<<endl;
            write_back();
            P_C += 4;
        }
        else if(opcode=="0100011")
        {
            int indicator = decode_Stype(instruction);
            int eff_add = execute_Stype(indicator);
            Memory_access(indicator, eff_add);
            cout<<"NOP for write back"<<endl;
            P_C += 4;
        }
        else if(opcode=="0010111"||opcode=="0110111"){
            int indicator = decode_Utype(instruction);
            execute_Utype(indicator);
            write_back();
            P_C += 4;
        }
        else if(opcode=="1101111")
        {
            int indicator = decode_UJtype(instruction);
            execute_UJtype(indicator);
            write_back();
        }
        else if(opcode=="1100011")
        {
            int indicator = decode_SBtype(instruction);
            execute_SBtype(indicator);
        }
        clocks++;
        cout<<"Clock (single cycle) = "<<clocks<<endl<<endl;
    }
    cout<<"All the registers are now:"<<endl;
    for(int i=0;i<32;i++)
    cout<<"register "<<i<<" has value "<<registers[i]<<endl;
    for(auto x : data_memory)
    {
        cout<<x.first<<" "<<x.second<<endl;
    }
    cout<<"Please check file"<<endl;
    ofstream fout;
    fout.open("D:\\SHubham_DHull\\CA\\output.txt");
    for(auto x: data_memory){
        fout<<"memory["<<decimal_hexa(x.first)<<"] = "<<x.second;
        fout<<endl;
    }
    fout<<"The contents of register are:"<<endl;
    for(int i=0;i<32;i++)
    {
        fout<<"x"<<i<<" "<<registers[i]<<endl;
    }
    return 0;
}