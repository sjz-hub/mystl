//
// Created by 孙建钊 on 2023/6/30.
//

#ifndef MYSTL_FUNC_DEQUE_TEST_H
#define MYSTL_FUNC_DEQUE_TEST_H

#include <iostream>
#include "../MyTinySTL//deque.h"

namespace mystl{
    namespace func_test{
        namespace func_test_deque{

            void func_test_deque(){
                deque<int> d1;
                std::cout<<d1.empty()<<std::endl;
                std::cout<<d1.size()<<std::endl;
                deque<int> d2(10);
                for(auto it = d2.begin(); it != d2.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;
                std::cout<<d2.empty()<<std::endl;
                std::cout<<d2.size()<<std::endl;

                deque<int> d3(10,1);
                for(auto it = d3.begin(); it != d3.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;
                std::cout<<d3.size()<<std::endl;

                deque<int> d4(d3.begin()+1,d3.end()-1);
                for(auto it = d4.begin(); it != d4.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                deque<int> d5({1,2,3,4,5,6,7,8,9,10});
                for(auto it = d5.begin(); it != d5.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                deque<int> d6(d5);
                for(auto it = d6.begin(); it != d6.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;


                std::cout<<d1.size()<<std::endl;
                std::cout<<d2.size()<<std::endl;
                std::cout<<d3.size()<<std::endl;
                std::cout<<d4.size()<<std::endl;
                std::cout<<d5.size()<<std::endl;
                std::cout<<d6.size()<<std::endl;


                deque<int> d7{1,2,3,4,5,6,7,8,9,1,0,1,2,1,2,1,2,1,2};
                std::cout<<d7.front()<<" "<<d7.back()<<std::endl;
                d7.assign(100,1);
                for(auto it = d7.begin(); it != d7.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;



                deque<int> d8;
                for(int i = 0; i < 10000; ++i){
                    d8.push_front(i);
                }
//                std::cout<<"d8:";
//                for(auto it = d8.begin(); it != d8.end(); ++it){
//                    std::cout<<*it<<" ";
//                }
                std::cout<<std::endl;


                deque<int> d9;
                std::cout<<"error:";
                for(int i = 0; i < 10000; ++i){
                    d9.push_back(i);
                }

//                for(auto it = d9.begin(); it != d9.end(); ++it){
//                    std::cout<<*it<<" ";
//                }
//                std::cout<<std::endl;

                std::cout<<"d9:";
                int x = 0;
                for(int i = 0; i < 9995; ++i){
                    d9.pop_back();
//                    std::cout<<d9.front()<<" ";
                    x++;
                }
                std::cout<<x<<" "<<d9.size()<<std::endl;
                for(auto it = d9.begin(); it != d9.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;


                deque<int> d10;
                for(int i = 0; i <10; ++i){
                    d10.emplace_back(i);
                }
                for(auto it = d10.begin(); it != d10.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;


                deque<int> d11({1,2,3,4,5});
                auto it1 = d11.begin();
                ++it1;
                ++it1;
                ++it1;
                std::cout<<"d11:";
                for(int i = 0; i < 10; ++i){
                    std::cout<<*it1<<std::endl;
                    it1 = d11.insert(it1,i);
                }

                for(auto it = d11.begin(); it != d11.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;


                deque<int> d12({1,2,3,4,5,6,7,8,9,10});
                auto it12 = d12.begin();
                ++it12;//2
                ++it12;//3
                ++it12;//4
                ++it12;//5
                d12.insert(it12,5,12);
                for(auto it = d12.begin(); it != d12.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                deque<int> d13({1,2,3,4,5,6,7,8,9,10});
                auto it13 = d13.begin();
                ++it13;
                ++it13;
                ++it13;
                ++it13;//5

//                for(int i = 0; i < 4;++i){
//                    it13 = d13.erase(it13);
//                }
                for(auto it = d13.begin(); it != d13.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;


                deque<int> d14({1,2,3,4,5,6,7,8,9,10});
                auto it14_1 = d14.begin();
                it14_1++;
                it14_1++;
                it14_1++;//4
                auto it14_2 = d14.end();
                it14_2--;
                it14_2--;
                it14_2--;//8
//                d14.insert(it14_1,it13,d13.end());
//                d14.erase(it14_1,it14_2);
                for(auto it = d14.begin(); it != d14.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;

                deque<int> d15;
                d15 = {1,2,3,4,5,6,7,8,9,10};
                d15.resize(5,1);
//                d15.assign(3,1);
                d15.assign(it14_1,it14_2);
                d15.clear();
                for(auto it = d15.begin(); it != d15.end(); ++it){
                    std::cout<<*it<<" ";
                }
                std::cout<<std::endl;



            }



        }
    }
}


#endif //MYSTL_FUNC_DEQUE_TEST_H
