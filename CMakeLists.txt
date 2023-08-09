cmake_minimum_required(VERSION 3.23)
project(mystl)

set(CMAKE_CXX_STANDARD 11)

add_executable(mystl MyTinySTL/type_traits.h MyTinySTL/exceptdef.h MyTinySTL/iterator.h MyTinySTL/util.h MyTinySTL/algobase.h MyTinySTL/construct.h MyTinySTL/uninitialized.h MyTinySTL/allocator.h MyTinySTL/list.h FuncTest/func_list_test.h FuncTest/func_test.cpp Test/test.h Test/list_test.h Test/test.cpp MyTinySTL/functional.h Test/my_test.h MyTinySTL/vector.h FuncTest/func_vector_test.h Test/vector_test.h MyTinySTL/deque.h FuncTest/func_deque_test.h Test/deque_test.h MyTinySTL/stack.h Test/stack_test.h FuncTest/func_stack_tesst.h MyTinySTL/queue.h Test/queue_test.h MyTinySTL/heap_algo.h MyTinySTL/hashtable.h MyTinySTL/unordered_map.h MyTinySTL/unordered_set.h Test/unordered_set_test.h MyTinySTL/rb_tree.h MyTinySTL/map.h Test/map_test.h MyTinySTL/set.h Test/set_test.h)
