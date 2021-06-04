//
// Created by Jokarc on 2021/5/25.
//

#ifndef USERANDPRODUCT_TEST_PRODUCT_H
#define USERANDPRODUCT_TEST_PRODUCT_H
#include "User.h"
class Product {
protected:
    bool isDiscount;
    int num;
    double price;
    double discount;
    string name;
    string productDescription;
    string owner;
    string Kind;

public:
    Product(string _name, string _Kind, double _price, int _num, double _dis, string _descrip, string _Mer) {
        name = _name;
        Kind = _Kind;
        price = _price;
        discount = _dis;
        num = _num;
        productDescription = _descrip;
        owner = _Mer;
    }
    virtual double getPrice(){return price;}
    bool IsDiscount() {return isDiscount;}
    string ShowProductDescription() {return productDescription;}
    string ShowOwner() {return owner;}
    string ShowName() {return name;}
    int ShowNum() {return num;}
    int productResNum() {return num;}
    void setNum(int _num) {
        num = _num;
    }
    void setPrice(double pri) {
        price = pri;
    }
    bool operator<(const Product& y) const {
        return price < y.price;
    }
};
class Book:public Product {
public:
    virtual double getPrice(){
        return price;
    }
};
class Clothes:public Product {
public:
    virtual double getPrice(){
        return price;
    }
};
class Food:public Product {
public:
    virtual double getPrice(){
        return price;
    }
};
#endif //USERANDPRODUCT_TEST_PRODUCT_H
