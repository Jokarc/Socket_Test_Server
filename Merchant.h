//
// Created by Jokarc on 2021/5/25.
//

#ifndef USERANDPRODUCT_TEST_MERCHANT_H
#define USERANDPRODUCT_TEST_MERCHANT_H
#include <fstream>
#include "User.h"
#include "Product.h"
using std::ifstream;
class Merchant : public User {
private:

public:
    Merchant(){}
    ~Merchant(){}
    Merchant(string a, string b) {
        UserName = a;
        PassWord = b;
        UserType = 1;
    }
    void ShowProduct();
    void ManageProduct();
    void ProductDiscount();
    void ChangeProductPrice();
    void ChangeProductInfo(char *FileName, string name, string info, int offset);
    void ShowThisKindProduct(string kind);
    void ShowThisNameProduct(string name);
    int AddNewProduct();
    int AddProductNum();
    int FindProduct(char *, string);
    virtual void getUserType();
};

void Merchant::getUserType() {
    cout << "当前用户类型：商家" << endl;
}
/**********************************************************
函数：FindProduct
形参：
类型：void
作用：寻找商品，//名称，种类，价格，数量，打折力度，描述
返回：
**********************************************************/
int Merchant::FindProduct(char *FileName, string ProductName) {
    ifstream FileIn(FileName, ios::in);
    if (!FileIn) {
        cout << "can't find " << FileName << " when FindProduct." << endl;
        return -1;
    }
    string temp, temp2, temp3, temp4, temp5, temp6;
    getline(FileIn, temp);
    while (getline(FileIn, temp)) {
        getline(FileIn, temp2);
        getline(FileIn, temp3);
        getline(FileIn, temp4);
        getline(FileIn, temp5);
        getline(FileIn, temp6);
        if (ProductName == temp){
            return -1;//商品存在
        }
    }
    FileIn.close();
    return 1;//商品不存在
}
/**********************************************************
函数：ChangeProductInfo
形参：
类型：void
作用：修改商品，//名称，种类，价格，数量，打折力度，描述
返回：
**********************************************************/
void Merchant::ChangeProductInfo(char *FileName, string name, string info, int offset) {
    ifstream FileIn(FileName, ios::in);
    if (!FileIn) {
        cout << "can't find " << FileName << " when ChangeProductInfo." << endl;
        return ;
    }
    int line = 2;
    int flag = 0;
    string temp, temp2, temp3, temp4, temp5, temp6;
    getline(FileIn, temp);
    while (getline(FileIn, temp)) {
        getline(FileIn, temp2);
        getline(FileIn, temp3);
        getline(FileIn, temp4);
        getline(FileIn, temp5);
        getline(FileIn, temp6);
        if (name == temp){
            flag = 1;
            Operation.ModifyLineData(FileName, line + offset, info);
        }
        line += 6;
    }
    if (!flag) {
        Operation.output(cSock, "Find ERROR.");
        cout << "Find ERROR" << endl;
    }
    FileIn.close();
    return ;
}
/**********************************************************
函数：AddProduct
形参：
类型：void
作用：添加商品
返回：
**********************************************************/
int Merchant::AddNewProduct() {
    char FileName[50] = "../Data/ProductData/";
    Operation.StrCat(FileName, getUserName());
    strcat(FileName, "_Products.txt");
    ifstream FileIn(FileName, ios::in);
    ofstream FileOut(FileName, ios::app);
    if (!FileIn) {
        cout << "can't find " << FileName << " when AddNewProduct." << endl;
        return -1;
    }
    string s;
    getline(FileIn, s);
    if (s.length() < 1 || !isdigit(s[0])) {
//        cout << "kkk" << endl;
        FileOut << 0 << endl;
    }
    double tempPrice;
    int tempNum;
    string tempDescription;
    string ProductName;
    Operation.output(cSock, "Enter ProductName:");
    cout << "请输入商品名称" << endl;
    getline(cin, ProductName);
    if (FindProduct(FileName,ProductName) == -1) {
        Operation.output(cSock, "Product Exists.");
        cout << "您的该商品已存在，请勿重复添加" << endl;
        return 0;
    }
    string temp;
    Operation.output(cSock, "Enter Price of Product:");
    cout << "请输入商品价格，浮点数，支持三位精度" << endl;
    getline(cin, temp);
    if (!Operation.checkDouble(temp)) {
        Operation.output(cSock, "ERROR:");
        cout << "请按要求输入" << endl;
        return 0;
    }
    tempPrice = Operation.Converse(temp);
    Operation.output(cSock, "Enter Num of Product:");
    cout << "请输入商品数量，非负整数" << endl;
    getline(cin, temp);
    if (!Operation.checkInt(temp)) {
        Operation.output(cSock, "ERROR:");
        cout << "请按要求输入" << endl;
        return 0;
    }
    tempNum = Operation.Converse(temp);
    Operation.output(cSock, "Enter Description of Product:");
    cout << "请输入商品描述" << endl;
    char recvBuf[500] = {};
    int ret = recv(cSock, recvBuf, 500, 0);
    temp = recvBuf;
    //getline(cin, temp);
    Operation.output(cSock, "Enter Kind\n"
                            "1:Book\n2:Clothes\n3:Food");
    cout << "请选择商品种类" << endl;
    cout << "1：书本" << endl;
    cout << "2：衣服" << endl;
    cout << "3：食品" << endl;
    int op = Operation.checkOp(cSock);
    if (op == -1) {
        Operation.output(cSock, "ERROR");
        cout << "输入不合法" << endl;
        return 0;
    }
    tempDescription = temp;
    FileOut << ProductName << endl;
    if (op == 1) FileOut << "Book" << endl;
    if (op == 2) FileOut << "Clothes" << endl;
    if (op == 3) FileOut << "Food" << endl;
    FileOut << tempPrice << endl;
    FileOut << tempNum << endl;
    FileOut << 1 << endl;
    FileOut << tempDescription << endl;
    FileIn.close();
    ifstream FileIn2(FileName, ios::in);
    getline(FileIn2, s);
    Operation.ModifyLineData(FileName, 1, to_string(int(Operation.Converse(s) + 1)));
    FileIn2.close();
    FileOut.close();
    return 0;
}
/**********************************************************
函数：AddProductNum
形参：
类型：int
作用：添加商品库存
返回：返回0，商品不存在，返回-1，无文件
**********************************************************/
int Merchant::AddProductNum() {
    Operation.output(cSock, "Enter Name of Product:");
    cout << "正在调整商品库存，请输入要调整的商品名称" << endl;
    string s; //getline(cin, s);
    char recvBuf[500] = {};
    int ret = recv(cSock, recvBuf, 500, 0);
    s = recvBuf;
    char FileName[50] = "../Data/ProductData/";
    Operation.StrCat(FileName, getUserName());
    strcat(FileName, "_Products.txt");
    if (FindProduct(FileName, s) == 1) {
        Operation.output(cSock, "Product do not exist.");
        cout << "商品不存在，请返回选择添加新品" << endl;
        return 0;
    }
    ifstream FileIn(FileName, ios::in);
    if (!FileIn) {
        cout << "can't find " << FileName << " when AddProductNum." << endl;
        return -1;
    }
    int line = -1; bool flag = false;
    string temp, temp2, temp3, temp4, temp5, temp6;
    getline(FileIn, temp);
    while (getline(FileIn, temp)) {
        getline(FileIn, temp2);
        getline(FileIn, temp3);
        getline(FileIn, temp4);
        getline(FileIn, temp5);
        getline(FileIn, temp6);
        line += 6;
        if (s == temp){
            flag = true;
            break;
        }
    }
    if (!flag) {
        Operation.output(cSock, "Find ERROR.");
        cout << "Find Error." << endl;
    } else {
        char buf[100] = "";
        strcpy(buf, "num now:");
        Operation.StrCat(buf, temp4);
        strcat(buf, "Enter a Num, pos to add, neg to dec:");
        Operation.output(cSock, buf);
        int num = Operation.Converse(temp4);
        cout << "当前数量为：" << num << endl;
        cout << "请输入调整数量，添加为正整数，减少为负整数" << endl;
        string s;
        getline(cin, s);
        int x = Operation.checkInt(s);
        if (!x) {
            Operation.output(cSock, "ERROR.");
            cout << "输入不合法" << endl;
            return -1;
        }
        int op = (int)Operation.Converse(s) + num;
        if (op >= 0) {
            Operation.ModifyLineData(FileName, line, to_string(op));
            char buf[100] = "Success, real_time_num:";
            Operation.StrCat(buf, to_string(op));
            Operation.output(cSock, buf);
            cout << "库存调整成功，该商品实时库存为：" << op << endl;
        }
        else {
            Operation.output(cSock, "cannot be neg.");
            cout << "ERROR, 库存不能为负数" << endl;
        }
    }
    FileIn.close();
    return 1;
}
/**********************************************************
函数：ShowThisKindProduct
形参：string kind
类型：void
作用：展示kind类商品
返回：
**********************************************************/
void Merchant::ShowThisKindProduct(string kind) {
    char FileName[50] = "../Data/ProductData/";
    Operation.StrCat(FileName, getUserName());
    strcat(FileName, "_Products.txt");
    ifstream FileIn(FileName, ios::in);
    if (!FileIn) {
        cout << "can't find " << FileName << " when FindProduct." << endl;
        return ;
    }
    int line = -4, num = 0;
    string temp, temp2, temp3, temp4, temp5, temp6;
    getline(FileIn, temp);
    while (getline(FileIn, temp)) {
        getline(FileIn, temp2);
        getline(FileIn, temp3);
        getline(FileIn, temp4);
        getline(FileIn, temp5);
        getline(FileIn, temp6);
        line += 6;
        if (kind == temp2){
            num++;
            double k = Operation.Converse(temp5);
            double newPrice = k * Operation.Converse(temp3);
            double koff = 1-k;
            char buf[500] = "";
            strcpy(buf, "\n*------------------------*");
            strcat(buf, "\nName       :"); Operation.StrCat(buf, temp);
            strcat(buf, "\nPrice      :"); Operation.StrCat(buf, temp3);
            strcat(buf, "\nInventory  :"); Operation.StrCat(buf, temp4);
            strcat(buf, "\ndiscount   :-"); Operation.StrCat(buf, to_string(koff * 100));
            strcat(buf, "%, Real_Price:"); Operation.StrCat(buf, to_string(newPrice));
            strcat(buf, "\nDescription:"); Operation.StrCat(buf, temp6);
            strcat(buf, "\nMerchant   :"); Operation.StrCat(buf, getUserName());
            Operation.output(cSock, buf);

            cout << "商品名称：" << temp << endl;
            cout << "商品定价：" << temp3 << endl;
            cout << "商品库存：" << temp4 << endl;
//            double k = Operation.Converse(temp5);
//            double newPrice = k * Operation.Converse(temp3);
//            double koff = 1-k;
            cout << "折扣力度：减" << koff * 100 << "%，实时价格：" << newPrice << endl;
            cout << "商品描述：" << temp6 << endl;
            cout << "*------------------------*" << endl;
        }
    }
    char buf[100] = "You have ";
    Operation.StrCat(buf, to_string(num));
    Operation.StrCat(buf, kind);
    strcat(buf, " Products.");
    cout << "您共有" << kind << "类商品" << num << "种" << endl;
    FileIn.close();
}
/**********************************************************
函数：ShowThisNameProduct
形参：string Name
类型：void
作用：展示名称Name商品
返回：
**********************************************************/
void Merchant::ShowThisNameProduct(string name) {
    char FileName[50] = "../Data/ProductData/";
    Operation.StrCat(FileName, getUserName());
    strcat(FileName, "_Products.txt");
    ifstream FileIn(FileName, ios::in);
    if (!FileIn) {
        cout << "can't find " << FileName << " when FindProduct." << endl;
        return ;
    }

    int line = -4, num = 0;
    string temp, temp2, temp3, temp4, temp5, temp6;
    getline(FileIn, temp);
    while (getline(FileIn, temp)) {
        getline(FileIn, temp2);
        getline(FileIn, temp3);
        getline(FileIn, temp4);
        getline(FileIn, temp5);
        getline(FileIn, temp6);
        line += 6;
        if (name == temp){
            num++;
            double k = Operation.Converse(temp5);
            double newPrice = k * Operation.Converse(temp3);
            double koff = 1-k;
            char buf[500] = "";
            strcpy(buf, "\n*------------------------*");
            strcat(buf, "\nName       :"); Operation.StrCat(buf, temp);
            strcat(buf, "\nPrice      :"); Operation.StrCat(buf, temp3);
            strcat(buf, "\nInventory  :"); Operation.StrCat(buf, temp4);
            strcat(buf, "\ndiscount   :-"); Operation.StrCat(buf, to_string(koff * 100));
            strcat(buf, "%, Real_Price:"); Operation.StrCat(buf, to_string(newPrice));
            strcat(buf, "\nDescription:"); Operation.StrCat(buf, temp6);
            strcat(buf, "\nMerchant   :"); Operation.StrCat(buf, getUserName());
            Operation.output(cSock, buf);
            cout << "商品名称：" << temp << endl;
            cout << "商品定价：" << temp3 << endl;
            cout << "商品库存：" << temp4 << endl;
//            double k = Operation.Converse(temp5);
//            double newPrice = k * Operation.Converse(temp3);
//            double koff = 1-k;
            cout << "折扣力度：减" << koff * 100 << "%，实时价格：" << newPrice << endl;
            cout << "商品描述：" << temp6 << endl;
            cout << "*------------------------*" << endl;
            return ;
        }
    }
    Operation.output(cSock, "You have no this Product.");
    cout << "您未曾添加名为 " << name << " 的商品" << endl;
    FileIn.close();
}
/**********************************************************
函数：ShowProduct
形参：
类型：void
作用：展示商品
返回：
**********************************************************/
void Merchant::ShowProduct() {
    Operation.output(cSock, "*----------------------*\n"
                            "1:Book\n2:Clothes\n3:Food\n4:Name\n"
                            "*----------------------*");
    cout << "*----------------------*" << endl;
    cout << "1：查看书本类商品" << endl;
    cout << "2：查看衣服类商品" << endl;
    cout << "3：查看食物类商品" << endl;
    cout << "4：以名称查看商品" << endl;
    cout << "*----------------------*" << endl;
    int op = Operation.checkOp(cSock);
    if (op == -1) {
        Operation.output(cSock, "ERROR");
        cout << "输入不合法" << endl;
        return ;
    }
    string book = "Book";
    string clothes = "Clothes";
    string food = "Food";
    if (op == 1) ShowThisKindProduct(book);
    if (op == 2) ShowThisKindProduct(clothes);
    if (op == 3) ShowThisKindProduct(food);
    if (op == 4) {
        string productname; //getchar();
        Operation.output(cSock, "Enter Product Name:");
        cout << "输入要查看的商品名称：" << endl;
//        getline(cin, productname);
        char recvBuf[128] = {};
        int ret = recv(cSock, recvBuf, 500, 0);
        productname = recvBuf;
        ShowThisNameProduct(productname);
    }
}
/**********************************************************
函数：ProductDiscount
形参：
类型：void
作用：商品打折
返回：
**********************************************************/
void Merchant::ProductDiscount() {
    Operation.output(cSock, "Enter the kind (Book, Clothes, Food)");
    cout << "正在对已有商品进行打折，请输入要打折的商品种类(Book, Clothes, Food)" << endl;
    string s;
    //getline(cin, s);
    char recvBuf[500] = {};
    int ret = recv(cSock, recvBuf, 500, 0);
    s = recvBuf;
    char FileName[50] = "../Data/ProductData/";
    Operation.StrCat(FileName, getUserName());
    strcat(FileName, "_Products.txt");
//    if (FindProduct(FileName, s) == 1) {
//        cout << "商品不存在，请返回选择添加新品" << endl;
//        return ;
//    }
    ifstream FileIn(FileName, ios::in);
    if (!FileIn) {
        cout << "can't find " << FileName << " when AddProductNum." << endl;
        return ;
    }
    Operation.output(cSock, "Enter discount(such as 0.9):");
    cout << "请输入打折力度，请输入小于1的浮点数，如九折输入0.9即可：" << endl;
    string discount;
    recvBuf[500] = {};
    ret = recv(cSock, recvBuf, 500, 0);
    discount = recvBuf;
    //getline(cin, discount);
    int x = Operation.checkDouble(discount);
    if (!x) {
        Operation.output(cSock, "ERROR.");
        cout << "输入不合法" << endl;
        return ;
    }
    double op = Operation.Converse(discount);
    if (op > 1) {
        Operation.output(cSock, "Enter double < 1.");
        cout << "请输入小于1的浮点数" << endl;
        return ;
    }

    int line = -1; bool flag = false;
    string temp, temp2, temp3, temp4, temp5, temp6;
    getline(FileIn, temp);
    while (getline(FileIn, temp)) {
        getline(FileIn, temp2);
        getline(FileIn, temp3);
        getline(FileIn, temp4);
        getline(FileIn, temp5);
        getline(FileIn, temp6);
        line += 6;
        if (s == temp2){
            flag = true;
            Operation.ModifyLineData(FileName, line+1, to_string(op));
        }
    }

    if (!flag) {
        Operation.output(cSock, "no this kind Product, discount ERROR.");
        cout << "没有该品类商品，discount ERROR." << endl;
    }
    FileIn.close();
    return ;
}
/**********************************************************
函数：ChangeProductPrice
形参：
类型：void
作用：修改商品定价，非现价
返回：
**********************************************************/
void Merchant::ChangeProductPrice() {
    Operation.output(cSock, "Changing price, Enter Name:");
    cout << "正在修改商品定价，请输入要修改的商品名称" << endl;
//    getchar();
    string s;
    //getline(cin, s);
    char recvBuf[500] = {};
    int ret = recv(cSock, recvBuf, 500, 0);
    s = recvBuf;
    cout << s << endl;
    char FileName[50] = "../Data/ProductData/";
    Operation.StrCat(FileName, getUserName());
    strcat(FileName, "_Products.txt");
    if (FindProduct(FileName, s) == 1) {
        Operation.output(cSock, "Product do not exist.");
        cout << "商品不存在，请返回选择添加新品" << endl;
        return ;
    }
    ifstream FileIn(FileName, ios::in);
    if (!FileIn) {
        cout << "can't find " << FileName << " when ChangeProductPrice." << endl;
        return ;
    }
    int line = -2; bool flag = false;
    string temp, temp2, temp3, temp4, temp5, temp6;
    getline(FileIn, temp);
    while (getline(FileIn, temp)) {
        getline(FileIn, temp2);
        getline(FileIn, temp3);
        getline(FileIn, temp4);
        getline(FileIn, temp5);
        getline(FileIn, temp6);
        line += 6;
        if (s == temp){
            flag = true;
            break;
        }
    }
    if (!flag) {
        Operation.output(cSock, "Find ERROR.");
        cout << "Find Error." << endl;
    } else {
        double price = Operation.Converse(temp3);
        char buf[100] = "price now:";
        Operation.StrCat(buf, temp3);
        strcat(buf, "Enter new price:");
        Operation.output(cSock, buf);
        cout << "当前定价为：" << price << endl;
        cout << "请输入新定价" << endl;
        string s;
//        getline(cin, s);
        char recvBuf[500] = {};
        int ret = recv(cSock, recvBuf, 500, 0);
        s = recvBuf;
        int x = Operation.checkDouble(s);
        if (!x) {
            Operation.output(cSock, "ERROR.");
            cout << "输入不合法" << endl;
            return ;
        }
        double np = Operation.Converse(s);
        if (np >= 0) {
            Operation.ModifyLineData(FileName, line, to_string(np));
            char buf[100] = "new price:";
            Operation.StrCat(buf, to_string(np));
            Operation.output(cSock, buf);
            cout << "定价调整成功，该商品新定价为：" << np << endl;
        }
        else {
            Operation.output(cSock, "cannot be neg.");
            cout << "ERROR, 定价不能为负数" << endl;
        }
    }
    FileIn.close();
    return ;
}
/**********************************************************
函数：ManageProduct
形参：
类型：void
作用：打印商品管理界面，支持添加商品，添加库存，进行打折活动
返回：
**********************************************************/
void Merchant::ManageProduct() {
    Operation.output(cSock, "*----------------------*\n"
                            "1:AddNew\n2:ChangeNum\n3:Discount\n4:Show\n"
                            "5:ChangePrice\n"
                            "*----------------------*");
    cout << "*----------------------*" << endl;
    cout << "1：添加新品" << endl;
    cout << "2：调整库存" << endl;
    cout << "3：商品打折" << endl;
    cout << "4：查看商品" << endl;
    cout << "5：修改定价" << endl;
    cout << "*----------------------*" << endl;
    int op = Operation.checkOp(cSock);
    if (op == -1) return ;
    if (op == 1) if (AddNewProduct() == -1) AddNewProduct();
    if (op == 2) AddProductNum();
    if (op == 3) ProductDiscount();
    if (op == 4) ShowProduct();
    if (op == 5) ChangeProductPrice();

}
#endif //USERANDPRODUCT_TEST_MERCHANT_H
