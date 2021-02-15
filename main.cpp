#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <bits/stdc++.h>

using namespace std;

string sign,sign1,sign2;
vector<pair<string,string>> v;
int noOfAdds = 0;

void CMPR_EXP(string n1, string n2, int &PC, int &cycles);
void ADD(string a, string b, int &exp, int &PC, int &cycles);
void NRMLS(string &r, int &exp, int &PC, int &cycles);
void RNDOFF(string &r, int &exp, int &PC, int &cycles);

string shrgt(string &s, int n){
    for(int i=0;i<n;i++){
        if(i==0) s.insert(0,"1");
        else s.insert(0,"0");
    }
    return s;
}


string badd(string a, string b)
{
    string result = "";
    int s = 0;
    int i = a.size() - 1, j = b.size() - 1;
    while (i >= 0 || j >= 0 || s == 1)
    {
        s= s+ ((i >= 0)? a[i] - '0': 0);
        s= s+ ((j >= 0)? b[j] - '0': 0);
        result = char(s % 2 + '0') + result;
        s=s/2;
        i--; j--;
    }
    return result;
}

string bsub(string a, string b)
{
    string result = "";
    string x= "";

    for(int i=0;i<b.length();i++){
        if(b.at(i)== '1') x.append("0");
        else x.append("1");
    }

    x= badd(x, "1");
    x= badd(a, x);
    return x.substr(1, x.length()-1);
}

void FETCH(vector<pair<string,string>> v, int PC) {
    cout<<"\nProgram Counter: "<<PC<<endl;
    cout<<"\nFETCH\n"<<endl;
    if(PC < ::noOfAdds) {
        int cycles=0;
        pair<string,string> inst = ::v.at(PC);
        CMPR_EXP(inst.first, inst.second, PC, cycles);
    }
}

void CMPR_EXP(string n1, string n2, int &PC, int &cycles){
    cycles++;
    string exp1 = n1.substr (1,8);
    string exp2 = n2.substr (1,8);
    cout<<"exp1: "<<exp1<<" exp2: "<<exp2<<endl;         ///////
    int ex1 = stoi(exp1, nullptr, 2);
    int ex2 = stoi(exp2, nullptr, 2);
    cout<<"exp1: "<<ex1<<" exp2: "<<ex2<<endl;          ////////
    ::sign1 = n1.substr(0,1);
    ::sign2 = n2.substr(0,1);
    cout<<"sign1: "<<::sign1<< " sign2: "<<sign2<<endl;    //////////
    string frctn1 = n1.substr(9,23);
    string frctn2 = n2.substr(9,23);
    cout<<"frac1: "<<frctn1<<" frac2: "<<frctn2<<endl;    ////////
    string sign="";

    if(exp1.compare("11111111")==0 && exp2.compare("11111111")==0){
        if(::sign1.compare("0")==0 && ::sign2.compare("0")==0) {cout<<"\ninf + inf = inf\nNumber of cycles: 1\n"<<endl;FETCH(::v, PC+1);}
        else if(::sign1.compare("1")==0 && ::sign2.compare("1")==0) {cout<<"\n-inf + -inf = -inf\nNumber of cycles: 1\n"<<endl;FETCH(::v, PC+1);}
        else if(::sign1.compare("0")==0 && ::sign2.compare("1")==0) {cout<<"\ninf + -inf = NaN\nNumber of cycles: 1\n"<<endl;FETCH(::v, PC+1);}
        else if(::sign1.compare("1")==0 && ::sign2.compare("0")==0) {cout<<"\n-inf + inf = NaN\nNumber of cycles: 1\n"<<endl;FETCH(::v, PC+1);}
    }
    else if(exp1.compare("11111111")==0){
        if(::sign1.compare("0")==0) {cout<<"\ninf + finite = inf\nNumber of cycles: 1\n"<<endl;FETCH(::v, PC+1);}
        else {cout<<"\n-inf + finite = -inf\nNumber of cycles: 1\n"<<endl;FETCH(::v, PC+1);}
    }
    else if(exp2.compare("11111111")==0){
        if(::sign2.compare("0")==0) {cout<<"\ninf + finite = inf\nNumber of cycles: 1\n"<<endl;FETCH(::v, PC+1);}
        else {cout<<"\n-inf + finite = -inf\nNumber of cycles: 1\n"<<endl;FETCH(::v, PC+1);}
    }
    else if(exp1.compare("00000000")==0 && exp2.compare("00000000")==0){
        cout<<"\nZERO + ZERO = ZERO\nNumber of cycles: 1\n"<<endl;FETCH(::v, PC+1);
    }
    else if(exp2.compare("00000000")==0){
        cout<<"result: "<<n1<<endl;FETCH(::v, PC+1);
    }
    else if(exp1.compare("00000000")==0){
        cout<<"result: "<<n2<<endl;FETCH(::v, PC+1);
    }
    else if(ex1 > ex2){
        ::sign = ::sign1;
        frctn1.insert(0,"1");
        for(int k=0;k< ex1-ex2;k++) frctn1.append("0");
        frctn2 = shrgt(frctn2, (ex1-ex2));
        ADD(frctn1, frctn2, ex1, PC, cycles);
    }
    else if(ex2>ex1){
        ::sign = ::sign2;
        frctn2.insert(0,"1");
        for(int k=0;k< ex2-ex1;k++) frctn2.append("0");
        frctn1 = shrgt(frctn1, (ex2-ex1));
        ADD(frctn2, frctn1, ex2, PC, cycles);
    }
    else {
        if(sign1.compare(sign2)==0){
            ::sign=::sign1;
            frctn1.insert(0,"1");
            frctn2.insert(0,"1");
            ADD(frctn1, frctn2, ex1, PC, cycles);
        }
        else{
            int f1= stoi(frctn1.substr(0,23), nullptr, 2);
            int f2= stoi(frctn2.substr(0,23), nullptr, 2);
            if(f1>f2) { ::sign=::sign1; ADD(frctn1, frctn2, ex1, PC, cycles);}
            else { ::sign=::sign2; ADD(frctn2, frctn1, ex1, PC, cycles);}
        }
    }

}

void ADD(string a, string b, int &exp, int &PC, int &cycles){
    cycles++;
    string result = "";
    string sign = "";

    if(::sign1.compare(::sign2)==0) {sign= ::sign1; result = badd(a,b);}
    else result = bsub(a, b);
    cout<<"significand add: "<<result<<endl;         /////

    NRMLS(result, exp, PC, cycles);

}

void NRMLS(string &r, int &exp, int &PC, int &cycles){
    cycles++;
    if(r.length()==25) exp++;
    else{
        for(int i=0;i<r.length();i++){
            if(r[i]=='0') exp--;
            else break;
        }
    }
    cout<<"exp: "<<exp<<endl;                    //
    if(0<exp<255) RNDOFF(r, exp, PC, cycles);
    else {
        if(exp<=0) {cout<<"\nUNDERFLOW\nNo of cycles taken: "<<cycles<<endl; FETCH(::v, PC+1);}
        else {cout<<"\nOVERFLOW\nNo of cycles taken: "<<cycles<<endl; FETCH(::v, PC+1);}
    }

}

void RNDOFF(string &r, int &exp, int &PC, int &cycles){
    cycles++;
    bool check = false;
    if(r.length()==25) {
        if(r[23]=='1' && r[24]=='1') {r=r.substr(0,24);check=true;r= badd(r,"1");}
        else r=r.substr(0,24);
    }
    else if(r.length()>25) {
        if(r[23]=='0' && r[24]=='1'){
            int z=0;
            for(int k=25;k<r.length();k++){
                if(r[k]=='1') {z=k;break;}
            }
            if(z!=0) {r=r.substr(0,24);check=true;r= badd(r,"1");}
            else r=r.substr(0,24);
        }
        else if(r[23]=='1' && r[24]=='1') {r=r.substr(0,24);check=true;r= badd(r,"1");}
        else r=r.substr(0,24);
    }
    if(check==true){
        if(r.length()>24)  NRMLS(r, exp, PC, cycles);
    }
    string fraction= "";
    string result= "";
    if(r.compare("000000000000000000000000")==0){
        result = "00000000000000000000000000000000";
        cout<<"\n"<<result<<"\nNo of cycles taken: "<<cycles<<endl;
        FETCH(::v, PC+1);
    }
    else{
        fraction= r.substr(1, 23);
        cout<<"Fraction of result: "<<fraction<<endl;  //
        string ex = bitset<8>(exp).to_string();

        result= ::sign.append(ex.append(fraction));
        cout<<"\n"<<result<<"\nNo of cycles taken: "<<cycles<<endl;
        FETCH(::v, PC+1);
    }
}

int loadfile(vector<pair<string,string>> &inst){
    ifstream myfile ("t0");
    int inum=0;
    string line;
    int i=0;
    while ( getline (myfile,line)) {
        if(line == "") return inum;
        inum++;
        istringstream ss(line);
        int j=0;
        string f,s="";
        do {
            j++;
            string word;
            ss >> word;
            if(j==1) f=word;
            if(j==2) s=word;
            cout << word <<" ";
        } while (ss);
        cout<<endl;
        inst.push_back(make_pair(f,s));
        i++;
    }
    return inum;
}

int main()
{
    int PC=0;
    ::noOfAdds = loadfile(::v);
    cout<<(::v.size())<<endl;

    if(::noOfAdds == 0) {cout<<"No additions are present"<<endl; return 0;}
    else {
        FETCH(::v, PC);
        return 0;
    }
}
