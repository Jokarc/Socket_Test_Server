//
// Created by Jokarc on 2021/5/25.
//

#ifndef USERANDPRODUCT_TEST_USER_H
#define USERANDPRODUCT_TEST_USER_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <iomanip>
#include "BasicOp.h"
using namespace std;
extern SOCKET cSock;
class User {
protected:
    string UserName;
    string PassWord;
    int UserType;
    double accountBalance;
    BasicOp Operation;
public:
    virtual void getUserType() = 0;
    double getAccountBalance();
    double findBalance();
    /*设置余额*/
    void setBalance(double x);
    void setInfo(string a, string b){
        UserName = a;
        PassWord = b;
    }
    //退出登录
    int UserLogOut(int &Logged);
    /*用户注册*/
    int UserRegister(string usrname, string password, string usrtype);
    /*用户充值提现*/
    int UserRecharge(int kind);
    /*改密码*/
    int UserChangePswd();
    string getUserName() {return UserName;}
};
/**********************************************************
函数：getAccountBalance
形参：
类型：double
作用：
返回：当前类成员accountBalance值
**********************************************************/
double User::getAccountBalance() {
    return this -> accountBalance;
}
/**********************************************************
函数：setAccountBalance
形参：
类型：double
作用：
返回：设置当前对象余额数值
**********************************************************/
void User::setBalance(double x) {
    accountBalance = x;
    char filename[50] = "../Data/UserData/UserBalance.txt";
    ifstream FileIn(filename, ios::in);
    string temp;
    string temp2;
    int line = 0;
    while (getline(FileIn, temp)) {
        getline(FileIn, temp2);
        line += 2;
        if (temp == this -> UserName) {
            Operation.ModifyLineData(filename, line, to_string(x));
        }
    }
}
/**********************************************************
函数：findBalance
形参：
类型：double
作用：在文件中寻找当前类的Balance，转换为double
返回：Balance值
**********************************************************/
double User::findBalance() {
    ifstream FileIn("../Data/UserData/UserBalance.txt", ios::in);
    string temp;
    string temp2;
    while (getline(FileIn, temp)) {
        getline(FileIn, temp2);
        if (temp == this -> UserName) {
            return Operation.Converse(temp2);
        }
    }
    return 0;
}
/**********************************************************
函数：UserLogOut
形参：int &Logged
类型：int
作用：退出登录，置Logged为0
返回：1
**********************************************************/
int User::UserLogOut(int &Logged) {
    Logged = 0;
    Operation.output(cSock, "LogOut Success.");
    cout << "LogOut Success." << endl;
    return 1;
}
/**********************************************************
函数：UserRegister
形参：string usrname，string password，string usrtype
类型：int
作用：注册账号
返回：成功1，失败0
**********************************************************/
int User::UserRegister(string usrname, string password, string usrtype) {

    if (!Operation.checkName(usrname)) {
        Operation.output(cSock, "UserName ERROR");
        cout << "UserName ERROR" << endl;
        return 0;
    }
    if (!Operation.checkPswd(password)) {
        Operation.output(cSock, "PassWord ERROR");
        cout << "PassWord ERROR" << endl;
        return 0;
    }
    ifstream FileIn("../Data/UserData/UserInfo.txt", ios::in);
    ofstream FileOut("../Data/UserData/UserInfo.txt", ios::app);
    ofstream BalanceOut("../Data/UserData/UserBalance.txt", ios::app);
    if (!FileIn) {
        Operation.output(cSock, "can't find file when UserRegister.");
        cout << "can't find UserInfo.txt when UserRegister." << endl;
        return -1;
    }
    bool UserNameExist = false;
    string temp;
    while (getline(FileIn, temp)) {
        if (temp == usrname) {
            UserNameExist = true;
            break;
        }
        getline(FileIn, temp);
        getline(FileIn, temp);
    }

    FileIn.close();
    if (UserNameExist){
        Operation.output(cSock, "UserName Exists.");
        cout << "UserName Exists." << endl;
        return 0;
    }
    //如果类型为商家，为其创建商品信息文件
    if (usrtype[0] == 'M') {
//        cout << "product" << endl;
        char productFileName[80]="../Data/ProductData/";
        char UserName[20];
        for (int i = 0; i < usrname.length(); i++) UserName[i] = usrname[i];
        strcat(productFileName,UserName);
        strcat(productFileName,"_Products.txt");
        FILE *fp = NULL;
        //fp = fopen(productFileName,"a");
        fp = fopen(productFileName,"w");
        if (!fp) fp = fopen(productFileName,"w");
        //fprintf(fp, "num of products：0");
        fclose(fp);
    }
    FileOut << usrname << endl;
    FileOut << password << endl;
    FileOut << usrtype << endl;
    BalanceOut << usrname << endl;
    BalanceOut << "0" << endl;
    FileOut.close();
    Operation.output(cSock, "Registration is successful, please keep your username and password.");
    cout << "Registration is successful, please keep your username and password." << endl;
    return 1;
}

/**********************************************************
函数：UserRecharge
形参：double num
类型：int
作用：为当前类用户充值num金额
返回：失败-1，成功1
**********************************************************/
int User::UserRecharge(int kind) {
    if (kind == 1) {
        Operation.output(cSock, "ReCharge\nEnter Positive Double:");
//        cout << "充值金额为正浮点数，最多支持三位精度" << endl;
//        cout << "请输入充值金额：" << endl;
    } else if (kind == -1) {
        Operation.output(cSock, "Cash_Out\nEnter Positive Double:");
//        cout << "提现金额为正浮点数，最多支持三位精度" << endl;
//        cout << "请输入提现金额：" << endl;
    }
    double val, num; string s;
    char recvBuf[500] = {};
    int ret = recv(cSock, recvBuf, 500, 0);
    s = recvBuf;
    if (!Operation.checkDouble(s)) {
        Operation.output(cSock, "ERROR");
        cout << "输入不合法" << endl;
        return -1;
    }
    //cout << s << endl;
    val = Operation.Converse(s);
    if (kind == 1) {
        char temp[30] = "";
        strcpy(temp, "ReCharge:");
        Operation.StrCat(temp, to_string(val));
        Operation.output(cSock, temp);
        cout << "您要充值的金额为：" << val << endl;
    }
    if (kind == -1) {
        char temp[30] = "";
        strcpy(temp, "Cash_Out:");
        Operation.StrCat(temp, to_string(val));
        Operation.output(cSock, temp);
        cout << "您要提现的金额为：" << val << endl;
    }
    num =  getAccountBalance() + val * kind;
    if (num < 0) {
        Operation.output(cSock, "ERROR, Balance cannot be Neg.");
        cout << "ERROR, 余额不能为负数" << endl;
        return -1;
    }
    ifstream FileIn("../Data/UserData/UserBalance.txt", ios::in);
    if (!FileIn) {
        Operation.output(cSock, "can't find file when Recharge");
        cout << "can't find UserBalance.txt when UserRecharge." << endl;
        return -1;
    }
    int line = 0;
    string temp, temp2;
    while (getline(FileIn, temp)) {
        getline(FileIn, temp2);
        line += 2;
        if (this -> UserName == temp) {
            this -> accountBalance = num;
            char filename[100] = "../Data/UserData/UserBalance.txt";
            Operation.ModifyLineData(filename, line, Operation.numToStr(num));
        }
    }
    char buf[50] = "";
    string ba = to_string(getAccountBalance());
    strcpy(buf, "Success, Your Balance: ");
    Operation.StrCat(buf, ba);
    Operation.output(cSock, buf);
    cout << "成功，当前余额为："<<getAccountBalance() << endl;
    return 1;
}
/**********************************************************
函数：UserChangePswd
形参：
类型：int
作用：修改密码
返回：失败-1，成功1，取消0
**********************************************************/
int User::UserChangePswd() {
    ifstream FileIn("../Data/UserData/UserInfo.txt", ios::in);
    if (!FileIn) {
        Operation.output(cSock, "cannot find file when ChangePswd.");
        cout << "can't find UserInfo.txt when UserChangePswd." << endl;
        return -1;
    }
    int line = 2;
    string temp, temp2, temp3;
    while (getline(FileIn, temp)) {
        getline(FileIn, temp2);
        getline(FileIn, temp3);

        if (this -> UserName == temp) {
            while (1) {
                Operation.output(cSock, "*----------------------*\n"
                                        "1:Enter old Password\n"
                                        "2:return\n"
                                        "*----------------------*");
                cout << "*----------------------*" << endl;
                cout << "1：输入原密码" << endl;
                cout << "2：不改了" << endl;
                cout << "*----------------------*" << endl;
                int op = Operation.checkOp(cSock);
                if (op == -1) {
                    Operation.output(cSock, "ERROR, Enter number");
                    cout << "输入不合法，请输入数字" << endl;
                    continue;
                }
                if (op == 2) break;
                Operation.output(cSock, "old PassWord:");
                string s; //getchar();
                char recvBuf[500] = {};
                int ret = recv(cSock, recvBuf, 500, 0);
                s = recvBuf;
                cout << s;
                if (s != temp2) {
                    Operation.output(cSock, "PassWord ERROR");
                    cout << "原密码错误" << endl;
                } else {
                    while (1) {
                        Operation.output(cSock, "*----------------------*\n"
                                                "1:Enter new Password\n"
                                                "2:return\n"
                                                "*----------------------*");
                        cout << "*----------------------*" << endl;
                        cout << "1：输入新密码" << endl;
                        cout << "2：不改了" << endl;
                        cout << "*----------------------*" << endl;
                        int op2 = Operation.checkOp(cSock);
                        if (op2 == -1) {
                            Operation.output(cSock, "ERROR, Enter number");
                            cout << "输入不合法，请输入数字" << endl;
                            continue;
                        }
                        if (op2 == 2) break;
                        Operation.output(cSock, "new PassWord:");
                        string nPswd; //getchar();
                        char recvBuf[500] = {};
                        int ret = recv(cSock, recvBuf, 500, 0);
                        nPswd = recvBuf;
                        if (Operation.checkPswd(nPswd)) {
                            char filename[100] = "../Data/UserData/UserInfo.txt";
                            Operation.ModifyLineData(filename, line, nPswd);
                            Operation.output(cSock, "Pswd Changed.");
                            cout << "密码已修改" << endl;
                            return 1;
                        } else {
                            Operation.output(cSock, "Pswd ERROR.");
                            cout << "密码格式不合法，请重试" << endl;
                        }
                    }
                }
            }
        }
        line += 3;
    }
    return 0;
}
#endif //USERANDPRODUCT_TEST_USER_H
