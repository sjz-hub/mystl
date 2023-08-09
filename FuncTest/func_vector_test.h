//
// Created by 孙建钊 on 2023/6/22.
//

#ifndef MYSTL_FUNC_VECTOR_TEST_H
#define MYSTL_FUNC_VECTOR_TEST_H

#include <iostream>
#include "../MyTinySTL/vector.h"

using namespace std;

namespace mystl{
    namespace func_test{
        namespace func_test_vector{
            void func_test_vector(){

                vector<int> v1;
                for(int i = 0; i < 20; ++i){
                    v1.push_back(i);
                }

                for(auto it = v1.begin(); it != v1.end(); ++it){
                    cout<<*it<<" ";
                }
                cout<<endl;

                vector<int> v2(10);
                for(auto it = v2.begin(); it != v2.end(); ++it){
                    cout<<*it<<" ";
                }
                cout<<endl;


                vector<int> v3(10,42);
                for(auto it = v3.begin(); it != v3.end(); ++it){
                    cout<<*it<<" ";
                }
                cout<<endl;

                vector<int> v4(v3.begin(),v3.end());
                for(auto it = v4.begin(); it != v4.end(); ++it){
                    cout<<*it<<" ";
                }
                cout<<endl;

                vector<int> v5(v4);
                for(auto it = v5.begin(); it != v5.end(); ++it){
                    cout<<*it<<" ";
                }
                cout<<endl;

                vector<int> v6({1,2,3,4,5,6,7,8,9,10});
//                v6.clear();
                for(auto it = v6.begin(); it != v6.end(); ++it){
                    cout<<*it<<" ";
                }
                cout<<endl;

                vector<int> v7;
                v7 = v6;
                cout<<"v7:";
                for(auto it = v7.begin(); it != v7.end(); ++it){
                    cout<<*it<<" ";
                }
                cout<<endl;
                cout<<"v8:";
                vector<int> v8({1,2,3,4,5,6,7,8,9,10});
                vector<int>::iterator it = v8.begin();
                it++;
                it++;
                it++;
                cout<<*it<<endl;
                it = v8.erase(it);
                cout<<*it<<endl;

                vector<int> v9({1,2,3,4,5,6,7,8,9,10});
                it = v9.begin();
                cout<<*it<<endl;
                vector<int>::iterator it1 = v9.end();
                it++;
                it++;
                it1--;
                it1--;
                it1 = v9.erase(it,it1);
                cout<<*it1<<endl;

                vector<int> v10({1,2,3,4,5,6,7,8,9,10});
                it = v10.begin();
                it++;
                it++;
                it++;
                it = v10.insert(it,-1);
                cout<<*it<<endl;
                for(vector<int>::iterator it2 = v10.begin(); it2 != v10.end(); it2++){
                    cout<<*it2<<" ";
                }
                cout<<endl;

                vector<int> v11;
                for(int i = 0; i < 50; ++i){
                    v11.push_back(i);
                }
                v11.pop_back();
                v11.pop_back();
                v11.pop_back();
                v11.pop_back();
                for(vector<int>::iterator it2 = v11.begin(); it2 != v11.end(); it2++){
                    cout<<*it2<<" ";
                }
                cout<<endl;

                v11.resize(100,1);
                for(vector<int>::iterator it2 = v11.begin(); it2 != v11.end(); it2++){
                    cout<<*it2<<" ";
                }
                cout<<endl;

                v11.assign(10,5);

                for(vector<int>::iterator it2 = v11.begin(); it2 != v11.end(); it2++){
                    cout<<*it2<<" ";
                }
                cout<<endl;

                v11.assign(v7.begin(),v7.end());
                v11.assign({1,2,3,4,5,6,3,2,1,6,5,4});
                v11.insert(v11.begin()+2,v7.begin(),v7.end());
                cout<<v11.size()<<" "<<v11.capacity()<<endl;
                v11.shrink_to_fit();
                cout<<v11.size()<<" "<<v11.capacity()<<endl;
                for(vector<int>::iterator it2 = v11.begin(); it2 != v11.end(); it2++){
                    cout<<*it2<<" ";
                }
                cout<<endl;

                v11.emplace_back(2);
                cout<<v11.size()<<" "<<v11.capacity()<<endl;
                for(vector<int>::iterator it2 = v11.begin(); it2 != v11.end(); it2++){
                    cout<<*it2<<" ";
                }
                cout<<endl;

                vector<int> v12({1,2,3,4,5,6,7,8,9});
                v12.emplace_back(10);
                v12.insert(v12.end(),11);
                v12.push_back(12);
                v12.reverse();
                for(vector<int>::iterator it2 = v12.begin(); it2 != v12.end(); it2++){
                    cout<<*it2<<" ";
                }
                cout<<endl;








            }
        }
    }
}



#endif //MYSTL_FUNC_VECTOR_TEST_H
