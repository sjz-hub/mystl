//
// Created by 孙建钊 on 2023/6/16.
//

#ifndef MYSTL_FUNC_LIST_TEST_H
#define MYSTL_FUNC_LIST_TEST_H


#include <iostream>
#include "../MyTinySTL/list.h"

namespace mystl{
    namespace func_test{
        namespace func_test_list{

            bool isOdd(int x){
                if(x%2==0) return true;
                else return false;
            }

            void func_test_list(){
                list<int> l1;
                list<int> l2(10);
                for(auto it = l1.begin(); it != l1.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;
                for(auto it = l2.begin(); it != l2.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                for(int i = 0; i < 10; ++i){
                    l1.push_front(i);
                }

                for(auto it = l1.begin(); it != l1.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;


                list<int> l3;
                for(int i = 0; i < 10; ++i){
                    l3.push_back(i);
                }
                for(auto it = l3.begin(); it != l3.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;


                list<int>l4;
                for(int i = 0; i < 10; ++i){
                    l4.push_back(i);
                }

                l4.pop_front();
                l4.pop_front();
                l4.pop_back();
//                l4.pop_back();
                std::cout<<"l4:";
                for(auto it = l4.begin(); it != l4.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;
                std::cout<<"l4:size:"<<l4.size()<<std::endl;


//                for(auto it:l2){
//                    std::cout<<it<<std::endl;
//                }


                list<int>l5;
                for(int i = 0; i < 10; ++i){
                    l5.push_back(i);
                }
                std::cout<<std::endl;
                auto it1 = l5.begin();
                ++it1;
                ++it1;

                auto it2 = l5.end();
                --it2;
                --it2;
                it1 = l5.insert(it2,10);
                l5.insert(it1,-1);
//                l5.erase(it1,it2);
                for(auto it = l5.begin(); it != l5.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                std::cout<<"max_size:"<<l5.max_size()<<std::endl;

                std::cout<<l5.front()<<std::endl;
                int &x = l5.front();
                x = 100;
                std::cout<<l5.front()<<std::endl;


                std::cout<<l5.back()<<std::endl;
                int &y = l5.back();
                y = 100;
                std::cout<<l5.back()<<std::endl;

                for(auto it = l5.begin(); it != l5.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;
                l5.fill_insert(it2,5,42);


                for(auto it = l5.begin(); it != l5.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                l5.copy_insert(l5.begin(),5,l1.begin());

                for(auto it = l1.begin(); it != l1.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;
                std::cout<<"l5:";
                for(auto it = l5.begin(); it != l5.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                list<int> l6;
                l6.copy_assign(l5.begin(),l5.end());
                for(auto it = l6.begin(); it != l6.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

//                l5.push_back(10);
//                l5.push_back(10);
//                l5.push_back(10);

                l5.pop_front();
                l5.pop_front();
                l5.pop_front();
                std::cout<<"l6:";
                for(auto it = l6.begin(); it != l6.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;
                l6.fill_assign(2,2);
                for(auto it = l6.begin(); it != l6.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                list<int> l7;
                l7.copy_init(l6.begin(),l6.end());
                for(auto it = l7.begin(); it != l7.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                l7.clear();
                std::cout<<l7.size()<<std::endl;
//                l7.copy_init(l6.begin(),l6.end());
                for(auto it = l7.begin(); it != l7.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;


                list<int> l8(l6.begin(),l6.end());

                std::cout<<"l6:";
                for(auto it = l6.begin(); it != l6.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;
                for(auto it = l8.begin(); it != l8.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;


                list<int> l9({1,2,3,4,5,6,7,8,9});
                for(auto it = l9.begin(); it != l9.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                list<int> l10 = l9;
                std::cout<<"l10:size:"<<l10.size()<<std::endl;
                l10 = l9;
                std::cout<<"l10:size:"<<l10.size()<<std::endl;
                l10 = {0,1,2,3,4,5,4,3,2,1,0};
                std::cout<<"l10:size:"<<l10.size()<<std::endl;
                l10.assign(10,11);
                std::cout<<"l10:size:"<<l10.size()<<std::endl;
                l10.assign(l9.begin(),l9.end());
                std::cout<<"l10:size:"<<l10.size()<<std::endl;
                l10.assign({1,2,3,4,5,4,3,2,1});
                std::cout<<"l10:size:"<<l10.size()<<std::endl;
                l10.resize(9,0);
                std::cout<<"l10:size:"<<l10.size()<<std::endl;
                l10.resize(10,0);
                for(auto it = l10.begin(); it != l10.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;
                l10.insert(l10.end(),l9.begin(),l9.end());
                for(auto it = l10.begin(); it != l10.end(); ++it){
                    std::cout<<*it<<" ";
                }

                std::cout<<std::endl;
                list<int> l11;
                l11.emplace(l11.end(),1);
                for(auto it = l11.begin(); it != l11.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                class A{
                    int a,b;
                    char c;
                public:
                    A(int a, int b, char c):a(a),b(b),c(c){
                        std::cout<<"A(int a, int b, char c)"<<std::endl;
                    }
                    A() = default;
                    void print(){
                        std::cout<<a<<" "<<b<<" "<<c<<std::endl;
                    }
                };

//                list<A> l12;
//                l12.emplace(l12.end(),1,2,'a');
//                l12.emplace_front(2,3,'d');
//                l12.emplace_back(3,4,'s');
//                std::cout<<std::endl;
//                for(auto it = l12.begin(); it != l12.end(); ++it){
//                    (*it).print();
//                }
                std::cout<<std::endl;

                list<int> l13{1,2,3,4,5};
                list<int> l14{6,7,8,9,10};
                auto it = l13.begin();
                ++it;
                ++it;
                ++it;

                l14.splice(++l14.begin(),l13,it,l13.end());
                std::cout<<std::endl;
                for(auto it = l13.begin(); it != l13.end(); ++it){
                    std::cout<<*it<<" ";
                }

                std::cout<<std::endl;

                for(auto it = l14.begin(); it != l14.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                l14.remove_if(isOdd);


                for(auto it = l14.begin(); it != l14.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                l14.remove(5);

                for(auto it = l14.begin(); it != l14.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                list<int> l15{1,2,1,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,6};
                l15.unique();
                for(auto it = l15.begin(); it != l15.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;


                list<int>l16{1,3,5,7,9};
                list<int>l17{2,4,6,8,10};

                for(auto it = l16.begin(); it != l16.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                for(auto it = l17.begin(); it != l17.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                l16.merge(l17);
                for(auto it = l16.begin(); it != l16.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;
                std::cout<<"l16:size:"<<l16.size()<<std::endl;
//                int x1 = 10;
//                x1++;
//                std::cout<<"l16:";
//                for(auto it = l16.begin(); it != l16.end(); ++it){
//                    std::cout<<*it<<" ";
//                }
//                std::cout<<std::endl;

//                list<int> l18{3,4,2,1,2};
                list<int> l18{5,6,4,1,2,3,7,8,5,1,2,36,4,2,4,6,6,2,3,1,4,7,8,5,12,4,5,5,5,5};
                for(auto it = l18.begin(); it != l18.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;
                l18.sort();
                for(auto it = l18.begin(); it != l18.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                std::cout<<"wqe";
                std::cout<<std::endl;

                for(auto it:l18){
                    std::cout<<it<<" ";
                }
                std::cout<<std::endl;


            }



        }
    }
}


#endif //MYSTL_FUNC_LIST_TEST_H
