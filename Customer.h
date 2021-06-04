//
// Created by Jokarc on 2021/5/25.
//

#ifndef USERANDPRODUCT_TEST_CUSTOMER_H
#define USERANDPRODUCT_TEST_CUSTOMER_H

#include "User.h"
#include "ShopCart.h"
#include <iostream>
using std::cout;
using std::endl;

class Customer : public User {
protected:
    ShopCart myShopCart;
public:
    Customer(){}
    ~Customer(){}
    Customer(string a, string b) {
        UserName = a;
        PassWord = b;
        UserType = 1;
    }
    void showBalance();
    int SettleMyOrder();
    void Shop();
    void ManageShopCart();
    virtual void getUserType();
};
/**********************************************************
函数：settleMyOrder
形参：
类型：int
作用：将当前购物车商品生成订单
返回：-1，结算订单失败(余额不足，无订单)。 1，成功
**********************************************************/
int Customer::SettleMyOrder() {
    if (!myShopCart.OrderExist()) {
        Operation.output(cSock, "no order");
        cout << "未生成订单" << endl;
        return -1;
    }
    char buf[100] = "";
    strcpy(buf, "need to pay:");
    Operation.StrCat(buf, to_string(myShopCart.HowMuch()));
    Operation.output(cSock, buf);
    strcpy(buf, "your balance:");
    Operation.StrCat(buf, to_string(accountBalance));
    Operation.output(cSock, buf);
    cout << "共需支付" << myShopCart.HowMuch() << endl;
    cout << "当前余额" << accountBalance << endl;
    if (accountBalance < myShopCart.HowMuch()) {
        strcpy(buf, "Balance not enough.");
        cout << "余额不足" << endl;
        return -1;
    }
    setBalance(accountBalance - myShopCart.HowMuch());
    strcpy(buf, "Pay OK, your balance:");
    Operation.StrCat(buf, to_string(accountBalance));
    Operation.output(cSock, buf);
    cout << "支付成功，当前余额" << accountBalance << endl;
    myShopCart.SettleOrder();
    return 1;
}
/**********************************************************
函数：ManageShopCart
形参：
类型：void
作用：管理购物车
返回：
**********************************************************/
void Customer::ManageShopCart() {
    while (1) {
        Operation.output(cSock, "*----------------------*\n"
                                "1:open ShopCart\n2:Clear ShopCart\n"
                                "3:Change ProductNum\n4:Add New Product\n-1:return");
        Operation.output(cSock, "*----------------------*");
        cout << "*----------------------*" << endl;
        cout << "1：查看购物车" << endl;
        cout << "2：清空购物车" << endl;
        cout << "3：修改商品加购数量" << endl;
        cout << "4：我要加购" << endl;
        cout << "-1：返回" << endl;
        cout << "*----------------------*" << endl;
        int op = Operation.checkOp(cSock);
        if (op == -1) return ;
        if (op == 1) myShopCart.Show();
        if (op == 2) myShopCart.Clear();
        if (op == 3) myShopCart.ManageShopCartProduct();
        if (op == 4) myShopCart.AddProduct();

    }
}
/**********************************************************
函数：Shop
形参：
类型：void
作用：打印购物页面，管理购物车，管理订单，进行加购等
返回：
**********************************************************/
void Customer::Shop() {
    while (1) {
        Operation.output(cSock, "*----------------------*\n"
                                "1:Manage ShopCart\n2:Generate Order\n"
                                "3:Show Order\n4:Settle Order\n5:cancel Order\n-1:return");
        Operation.output(cSock, "*----------------------*");
        cout << "*----------------------*" << endl;
        cout << "1：管理购物车" << endl;
        cout << "2：生成订单" << endl;
        cout << "3：查看订单" << endl;
        cout << "4：结算订单" << endl;
        cout << "5：取消订单" << endl;
        cout << "-1：返回" << endl;
        cout << "*----------------------*" << endl;
        int op = Operation.checkOp(cSock);
        if (op == -1) return ;
        if (op == 1) ManageShopCart();
        if (op == 2) myShopCart.GenerateOrder();
        if (op == 3) myShopCart.ShowOrder();
        if (op == 4) SettleMyOrder();
        if (op == 5) myShopCart.CancelOrder();
    }
}
void Customer::showBalance() {
    cout << "当前账户余额：" << this -> getAccountBalance() << endl;
}

void Customer::getUserType() {
    cout << "当前用户类型：消费者" << endl;
}

#endif //USERANDPRODUCT_TEST_CUSTOMER_H
