#include <iostream>
#include <map>
#include "allocator.h"
#include "myList.h"

using namespace std;
#define TEST 1

void test1()
{
    //empty list =test
    auto list=MyList<int>();

    if (!list.isEmpty()) cout<<"test1::error 1"<<endl;
    if (list.size()!=0) cout<<"test1::error 2"<<endl;
    if (list.begin().isValid()) cout<<"test1::error 3"<<endl;
    cout<<"test1 end"<<endl;
}


void test2()
{
    //push =test
    auto list=MyList<int>();

    list.push_back(5);

    if (list.isEmpty()) cout<<"test2::error 1"<<endl;
    if (list.size()!=1) cout<<"test2::error 2"<<endl;
    if (!list.begin().isValid()) cout<<"test2::error 3"<<endl;
    if (*(list.begin())!=5) cout<<"test2::error 4"<<endl;

    auto it0=list.begin();
    if (list.end()!=it0++) cout<<"test2::error 4.1"<<endl;

    list.push_front(8);

    if (list.size()!=2) cout<<"test2::error 5";
    if (!list.begin().isValid()) cout<<"test2::error 6"<<endl;
    if (*(list.begin())!=8) cout<<"test2::error 7"<<endl;

    auto it=list.begin();
    if (!((it+1).isValid())) cout<<"test2::error 8"<<endl;
    if (*(it+1)!=5) cout<<"test2::error 9"<<endl;
    if (*(it++)!=5) cout<<"test2::error 10"<<endl;

    cout<<"test2 end"<<endl;
}

void test3()
{
    //pop =test
    MyList<int> list;

    list.push_back(5);
    list.push_front(8);
    list.push_back(1);
    list.push_front(2);

    if (list.size()!=4) cout<<"test3::error 1"<<endl;
    if (list.front()!=2) cout<<"test3::error 2"<<endl;
    if (list.back()!=1) cout<<"test3::error 3"<<endl;

    list.pop_front();
    if (list.size()!=3) cout<<"test3::error 4"<<endl;
    if (list.front()!=8) cout<<"test3::error 5"<<endl;
    if (*list.begin()!=8) cout<<"test3::error 6"<<endl;

    list.push_front(6);
    if (list.size()!=4) cout<<"test3::error 7"<<endl;
    if (list.front()!=6) cout<<"test3::error 8"<<endl;
    if (*list.begin()!=6) cout<<"test3::error 9"<<endl;
    if (*(list.begin()+1)!=8) cout<<"test3::error 10"<<endl;

    list.pop_back();
    if (list.size()!=3) cout<<"test3::error 11"<<endl;
    if (list.back()!=5) cout<<"test3::error 12"<<endl;
    if (*(list.end()--)!=5) cout<<"test3::error 13"<<endl;

    list.push_back(7);
    if (list.size()!=4) cout<<"test3::error 14"<<endl;
    if (list.back()!=7) cout<<"test3::error 15"<<endl;
    if (*(list.end()-2)!=5) cout<<"test3::error 16"<<endl;

    cout<<"test3 end"<<endl;
}

void test4()
{
    //insert, find, at =test
    MyList<int> list;

    list.push_back(5);
    list.push_front(8);
    list.push_back(1);
    list.push_front(2);

    auto it=list.end()-2;
    auto res=list.insert(32,it);
    if (!res.first) cout<<"test4::error 1";
    if ((*res.second)!=32) cout<<"test4::error 2";


    auto it2=list.find(64);
    if (it2.isValid()) cout<<"test4::error 3  " <<endl;

    it2=list.find(32);
    if (it2!=res.second) cout<<"test4::error 4 "<<endl;
    if (!it2.isValid()) cout<<"test4::error 4.1 "<<endl;
    if (*it2!=32) cout<<"test4::error 4.2 "<<endl;
    if (list.at(1)!=8) cout<<"test4::error 5 "<<endl;
    if (list.at(3)!=5) cout<<"test4::error 6"<<endl;

    cout<<"test4 end"<<endl;
}


void test5()
{
    //comparision copy clear =test
    MyList<int> list;
    MyList<int> list2;

    list.push_back(5);
    list.push_front(8);
    list.push_back(1);
    list.push_front(2);

    list2.push_back(5);
    list2.push_front(8);
    list2.push_back(1);
    list2.push_front(2);

    if (list!=list2) cout<<"test5::error 1"<<endl;
    if (!(list==list2)) cout<<"test5::error 2"<<endl;
    list.pop_back();
    if (list==list2) cout<<"test5::error 3"<<endl;

    list.clear();

    if (list.size()!=0) cout<<"test5::error 4"<<endl;

    list.push_back(5);
    list.push_front(8);
    list.push_back(1);
    list.push_front(2);
    if (list!=list2) cout<<"test5::error 5"<<endl;

    list.clear();
    if (list.begin().isValid()) cout<<"test5::error 6"<<endl;

    list=list2;

    if (list.front()!=2) cout<<"test5::error 7"<<endl;
    if (list.back()!=1) cout<<"test5::error 8"<<endl;
    if (list.size()!=4) cout<<"test5::error 9"<<endl;
    if (list!=list2) cout<<"test5::error 10"<<endl;

    cout<<"test5 end"<<endl;
}

void test6()
{
    //erase  for  =test
    MyList<int> list;


    list.push_back(5);
    list.push_front(8);
    list.push_back(1);
    list.push_front(2);

    auto it=list.begin()+1;
    if (*it!=8) cout<<"test6::error 1"<<endl;
    list.erase(it);

    if (list.size()!=3) cout<<"test6::error 2"<<endl;
    it=list.begin()+1;
    if (*it!=5) cout<<"test6::error 3"<<endl;

    int res[3]={0,0,0};
    int idx=0;
    for (auto x: list) {res[idx]=x; idx++;}

    if (res[0]!=2) cout<<"test6::error 4"<<endl;
    if (res[1]!=5) cout<<"test6::error 5"<<endl;
    if (res[2]!=1) cout<<"test6::error 6"<<endl;

    cout<<"test6 end"<<endl;
}


void test7()
{
    //operator+  initializer_list =test
    MyList<int> list;
    MyList<int> list2;

    list.push_back(5);
    list.push_back(4);
    list.push_back(3);



    list2.push_back(2);
    list2.push_back(1);


    auto list3=list+list2;

    if (list3.size()!=5) cout<<"test7::error 1"<<endl;
    if (list3.front()!=list.front()) cout<<"test7::error 2"<<endl;
    if (list3.back()!=list2.back()) cout<<"test7::error 3"<<endl;

    MyList<int> list4{1,2,3};
    if (list4.size()!=3) cout<<"test7::error 4"<<endl;
    if (list4.front()!=1) cout<<"test7::error 5"<<endl;
    if (list4.back()!=3) cout<<"test7::error 6"<<endl;


    cout<<"test7 end"<<endl;
}

int main()
{
    if (TEST)
    {
        test1();
        test2();
        test3();
        test4();
        test5();
        test6();
        test7();
        cout<<"All tests are finished"<<endl;
    }



      std::map<int,int> map_standart;
    std::map<int,int,std::less<int>,My_allocator<std::pair<const int, int>>> map_allocator;

    int n=1;
    for (int i=0; i<10; i++)
    {
        if (i>0) n*=i;
        map_standart[i]=(i==0)? 0 : n;
        map_allocator[i]=(i==0)? 0 : n;
    }

    for (auto it=map_standart.begin(); it!=map_standart.end(); it++)
        cout<<(*it).first<<" "<<(*it).second<<endl;
    cout<<endl;

    for (auto it=map_allocator.begin(); it!=map_allocator.end(); it++)
        cout<<(*it).first<<" "<<(*it).second<<endl;
    cout<<endl;


    MyList<int, std::allocator<int>> list{0,1,2,3,4,5,6,7,8,9};

    for (auto x: list)
        cout<<" "<<x;
    cout<<endl;

    MyList<int, My_allocator<int>> list2{0,1,2,3,4,5,6,7,8,9};

    for (auto x: list2)
        cout<<" "<<x;
    cout<<endl;



    return 0;
}
