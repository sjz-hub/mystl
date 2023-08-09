//
// Created by 孙建钊 on 2023/7/7.
//

#ifndef MYSTL_HEAP_ALGO_H
#define MYSTL_HEAP_ALGO_H

#include "iterator.h"

namespace mystl{

    // 使用< 则为大顶堆

    /**********************************************************************
    push_heap
    *************************************************************************/
    //first表示容器的开始迭代器
    //holeIndex表示当前索引，也就是对其进行上溯的索引节点 需要填充的节点
    //topIndex表示的是堆顶，也即是排序到topindex这个子树为止
    //value 新插入节点的值，也就是当前索引对应的节点的值
    //默认是大顶堆
    //将新加入的节点上移再次形成大顶堆
    template <class RandomIter, class Distance, class T>
    void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value){

        Distance parent = (holeIndex - 1) / 2;//父亲节点索引
        while(holeIndex > topIndex &&  *(first + parent) < value){
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }

    template <class RandomIter, class Distance>
    void push_heap_d(RandomIter first, RandomIter last, Distance*){
        push_heap_aux(first, last - first - 1, static_cast<Distance>(0), *(last - 1));
    }

    template <class RandomIter>
    void push_heap(RandomIter first, RandomIter last){
        push_heap_d(first, last, mystl::distance_type(first));
    }





    template <class RandomIter, class Distance, class T, class Compared>
    void push_heap_aux(RandomIter first, Distance holeIndex, Distance topIndex, T value, Compared comp){
        Distance parent = (holeIndex - 1) / 2;
        while(holeIndex > topIndex && comp(*(first + parent), value)){
            *(first + holeIndex) = *(first + parent);
            holeIndex = parent;
            parent = (holeIndex - 1) / 2;
        }
        *(first + holeIndex) = value;
    }

    template <class RandomIter, class Compared, class Distance>
    void push_heap_d(RandomIter first, RandomIter last, Distance*, Compared comp){
        push_heap_aux(first, last - first - 1, static_cast<Distance>(0), *(last - 1), comp);
    }

    template <class RandomIter, class Compared>
    void push_heap(RandomIter first, RandomIter last, Compared comp){
        push_heap_d(first, last, mystl::distance_type(first), comp);
    }


    /**********************************************************************
    pop_heap
    *************************************************************************/
    //first 容器的开始迭代器
    //holeIndex 此时该索引为止的元素没了，使用孩子节点元素进行填补
    //len 表示容器大小-1，也就是最后一个元素所在的索引
    //value表示此时用这个元素进行填充，少了的那个节点
    template<class RandomIter, class T, class Distance>
    void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value){

        //先进行下溯，完成填补holeIndex的工作
        Distance child = holeIndex * 2 + 2;
        while(child < len){
            if(*(first + child) < *(first + child - 1))
                child -= 1;//左孩子更大
            *(first + holeIndex) = *(first + child);
            holeIndex = child;
            child = holeIndex * 2 + 2;
        }
        if(child == len){
            //说明下溯正好遍历到了最后一个节点，并且此时这最后一个节点还是右孩子 因为这个child是*2 + 2得到的，一定是右孩子
            *(first + holeIndex) = *(first + child - 1);
            holeIndex = child - 1;
        }
        push_heap_aux(first,holeIndex,static_cast<Distance>(0),value);
    }


    template <class RandomIter, class T, class Distance>
    void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value, Distance*){
        *result = *first;
        adjust_heap(first, static_cast<Distance>(0),last - first, value);
    }

    template <class RandomIter>
    void pop_heap(RandomIter first, RandomIter last){
        pop_heap_aux(first, last - 1, last - 1, *(last - 1), distance_type(first));
    }


    template <class RandomIter, class T, class Distance, class Compared>
    void adjust_heap(RandomIter first, Distance holeIndex, Distance len, T value, Compared comp){
        Distance topIndex = holeIndex;
        Distance child = holeIndex * 2 + 2;
        while(child < len){
            if(comp(*(first + child), *(first + child - 1))){
                child -= 1;
            }
            *(first + holeIndex) = *(first + child);
            holeIndex = child;
            child = holeIndex * 2 + 2;
        }

        if(child == len){
            child--;
            *(first + holeIndex) = *(first + child);
            holeIndex = child;
        }
        push_heap_aux(first,holeIndex,topIndex,value,comp);
    }

    template <class RandomIter, class T, class Distance, class Compared>
    void pop_heap_aux(RandomIter first, RandomIter last, RandomIter result, T value, Distance*, Compared comp){
        *result = *first;
        adjust_heap(first, static_cast<Distance>(0),last - first, value, comp);
    }

    template <class RandomIter, class Compared>
    void pop_heap(RandomIter first, RandomIter last, Compared comp){
        pop_heap_aux(first, last - 1, last - 1, *(last - 1), distance_type(first), comp);
    }



    /**********************************************************************
    sort_heap
    *************************************************************************/

    template <class RandomIter>
    void sort_heap(RandomIter first, RandomIter last){
        while(last - first > 1){
            pop_heap(first,last);
            --last;
        }
    }

    template <class RandomIter, class Compared>
    void sort_heap(RandomIter first, RandomIter last, Compared comp){
        while(last - first > 1){
            pop_heap(first,last, comp);
            --last;
        }
    }



    /**********************************************************************
    make_heap
    *************************************************************************/

    //虽然这样可以，但是为什么感觉没什么必要呢，为什么不直接对于遍历的holeIndex,只让其下溯到一定的位置，而不是直接到最后一层，而且也不需要上溯
    template <class RandomIter, class Distance>
    void make_heap_aux(RandomIter first, RandomIter last, Distance*){
        Distance len = last - first; //最后一个节点的索引号
        Distance holeIndex = (len - 2 ) / 2;
        while(holeIndex >= 0){
            adjust_heap(first,holeIndex,len,*(first + holeIndex));
            holeIndex--;
        }
    }

    template <class RandomIter>
    void make_heap(RandomIter first, RandomIter last){
        make_heap_aux(first, last, distance_type(first));
    }

    template <class RandomIter, class Distance, class Compared>
    void make_heap_aux(RandomIter first, RandomIter last, Distance*, Compared comp){
        Distance len = last - first; //最后一个节点的索引号
        Distance holeIndex = (len - 2 ) / 2;
        while(holeIndex >= 0){
            adjust_heap(first,holeIndex,len,*(first + holeIndex), comp);
            holeIndex--;
        }
    }

    template <class RandomIter, class Compared>
    void make_heap(RandomIter first, RandomIter last, Compared comp){
        make_heap_aux(first, last, distance_type(first), comp);
    }


















}



#endif //MYSTL_HEAP_ALGO_H
