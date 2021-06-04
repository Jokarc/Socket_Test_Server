//
// Created by Jokarc on 2021/5/25.
//

#ifndef USERANDPRODUCT_TEST_BASICOP_H
#define USERANDPRODUCT_TEST_BASICOP_H
#include <iostream>
#include <fstream>
#include <string>
#include "windows.h"
using namespace std;

class BasicOp {
public:
    BasicOp(){}
    ~BasicOp(){}
    int checkOp(SOCKET cSock);
    bool checkPswd(string s);
    bool checkName(string s);
    bool checkInt(string s);
    bool checkDouble(string s);
    double Converse(string s);
    string intToStr(int num);
    string numToStr(double num);
    void StrCat(char *chs, string s);
    void output(SOCKET cSock, char *s);
    int ModifyLineData(char* fileName, int lineNum, string lineData);
};
void BasicOp::output(SOCKET cSock, char *s) {
//    cout << "nb--" << s << endl;
    send(cSock, s,strlen(s)+1,0);
}
/**********************************************************
函数：StrCat
形参：char *chs，string s
类型：void
作用：将string添加至chs后
返回：
**********************************************************/
void BasicOp::StrCat(char *chs, string s){
    char temp[20] = "";
    for (int i = 0; i < s.length(); i++) temp[i] = s[i];
//    cout << "string:" << s << endl;
//    cout << temp << endl;
    strcat(chs,temp);
}
/**********************************************************
函数：checkOp
形参：string s
类型：int
作用：判断输入Op的合法
返回：合法返回相应数字，不合法返回-1
**********************************************************/
int BasicOp::checkOp(SOCKET cSock) {
    string s;
    //getline(cin, s);
    char buf[128] = "";
    int ret = recv(cSock, buf, 128, 0);
    if (ret <= 0) {
        cout << "client exit...." << endl;
        return -1;
    }
    s = buf;
    cout << "op" << s  << "op"<< endl;
    if (s.length() > 1) return -1;
    if (!isdigit(s[0])) return -1;
    return s[0]-'0';
}
/**********************************************************
函数：checkPswd
形参：string s
类型：bool
作用：判断密码字符串的合法性
返回：密码合法返回true
     密码格式：长度至少6位，不能含空格，同时拥有字母和数字
**********************************************************/
bool BasicOp::checkPswd(string s) {
    if (s.length() < 6) return 0;
    int chNum(0), digNum(0);
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == ' ') return 0;
        if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z')) chNum++;
        if (isdigit(s[i])) digNum++;
    }
    return chNum && digNum;
}
/**********************************************************
函数：checkName
形参：string s
类型：bool
作用：判断用户名字符串的合法性
返回：用户名合法返回true
     用户名格式：长度至少4位，仅能由字母和数字以及下划线‘_’构成，且必须由字母开头
**********************************************************/
bool BasicOp::checkName(string s) {
    if (s.length() < 4) return 0;
    if (!((s[0] >= 'a' && s[0] <= 'z') || (s[0] >= 'A' && s[0] <= 'Z'))) return 0;
    bool flag = 1;
    for (int i = 1; i < s.length(); i++) {
        if (isdigit(s[i])) flag = 1;
        else if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || s[i] == '_') flag = 1;
        else {
            flag = 0;
            break;
        }
    }
    return flag;
}
/**********************************************************
函数：checkInt
形参：string s
类型：bool
作用：判断一个字符串是否表示为一个非负整数
返回：表示为合法返回true
**********************************************************/
bool BasicOp::checkInt(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (i == 0 && s[i] == '-') continue;
        if (!isdigit(s[i])) return 0;
    }
    return 1;
}
/**********************************************************
函数：checkDouble
形参：string s
类型：bool
作用：判断一个字符串是否表示为一个浮点数
返回：表示为合法浮点数返回true
     格式要求：仅由小数点和数字构成，为充值判断函数，不支持负数充值
             至多一个小数点'.'以及小数点不能为第一位或最后一位，及小数点前后都有数字
**********************************************************/
bool BasicOp::checkDouble(string s) {
    bool flag = 1;
    bool dotflag = 0;
    if (!isdigit(s[0])) return 0;
    for (int i = 0; i < s.length(); i++) {
        if (!isdigit(s[i]) && s[i] != '.') return 0;
        if (s[i] == '.') {
            if (!dotflag) dotflag = 1;
            else return 0;
            if (i == s.length() - 1) return 0;
        }
    }
    return flag;
}
/**********************************************************
函数：Converse
形参：string s
类型：double
作用：将字符串s转换为浮点数
返回：相应浮点数值
**********************************************************/
double BasicOp::Converse(string s) {
    double x = 0;
    int i = 0;
    double flag = 1;
    if (s[0] == '-') {
        flag = -1;
        i = 1;
    }
    //cout << x << endl;
    for ( ; i < s.length(); i++) {
        if (s[i] == '.') break;
        x = x * 10 + s[i] - 48;
    }
    double f = 0.1;
    i++;
    for (; i < s.length(); i++){
        x += f * (s[i] - 48);
        f *= 0.1;
    }
    return x * flag;
}
/**********************************************************
函数：numToStr
形参：double num
类型：string
作用：将double num转换为字符串形式
返回：str
**********************************************************/
string BasicOp::numToStr(double num) {
    std::stringstream stream{};
    int precision = 3;
    stream<<std::fixed<<std::setprecision(precision)<<num;
    return stream.str();
}
/**********************************************************
函数：ModifyLineData
形参：char *filename，int lineNum，string lineData
类型：int
作用：修改文件filename中第lineNum行的数据
返回：失败-1，成功1
**********************************************************/
int BasicOp::ModifyLineData(char* fileName, int lineNum, string lineData) {
    if (lineNum < 1) {
        cout << "linNum ERROR." << endl;
        return -1;
    }
    ifstream FileIn(fileName, ios::in);
    if (!FileIn) {
        cout << "can't find " << fileName  << " when ModifyLineData." << endl;
        return -1;
    }
    string strFileData = "";
    string tmpLineData;
    int line = 1;
    while(getline(FileIn, tmpLineData)) {
        if (line == lineNum) {
            strFileData += lineData;
            strFileData += "\n";
        }
        else {
            strFileData += tmpLineData;
            strFileData += "\n";
        }
        line++;
    }
    FileIn.close();
    //写入
    ofstream out;
    out.open(fileName);
    out.flush();
    out << strFileData;
    out.close();
    return 1;
}
string BasicOp::intToStr(int num) {
    return to_string(num);
}
#endif //USERANDPRODUCT_TEST_BASICOP_H
