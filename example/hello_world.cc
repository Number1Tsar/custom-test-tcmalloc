#include <iostream>
#include <cstddef>
#include <vector>
#include <thread>
#include <unordered_set>
#include "tcmalloc/malloc_extension.h"

void printStats()
{
    while(1)
    {
        std::cout<<tcmalloc::MallocExtension::GetStats()<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

int main() 
{
    int size = 10000;
    auto randomGen = [](){return 8192 + (std::rand() % ( 262144 - 8192 + 1 ) );};
    auto randomSizeToRemove = [size](){return size/3 + (std::rand() % (size/2 - size/3 +1));};
    auto randomIndex = [size](){return 0 + (std::rand()% (size));};
    std::vector<size_t> objectSizes(size);
    std::vector<char*> objects(size);
    for(int i=0;i<size;i++) objectSizes[i] = randomGen();

    //Make first allocations
    std::unordered_set<uint64_t> unique;
    for(int i=0;i<size;i++)
    {
        objects[i] = (char*)malloc(objectSizes[i]);

    // The following commented portion correctly show the number of HugePages used in the program.
    //   auto pageAllign =  reinterpret_cast<uintptr_t>(objects[i]) >> 13;
    //   auto hugePageAllign = (pageAllign >> (21 - 13));
    //   uint64_t hNumber = reinterpret_cast<uint64_t>(hugePageAllign);
    //   uint64_t pNumber = reinterpret_cast<uint64_t>(pageAllign);
    //   unique.insert(pNumber);
    //   std::cout<<hNumber<<std::endl;
    }
    std::cout<<unique.size()<<std::endl;
    std::cout<<"Initial state"<<std::endl;
    std::cout<<tcmalloc::MallocExtension::GetStats()<<std::endl;


    while(1)
    {
        //delete random objects 
        int randomSize = randomSizeToRemove();
        std::vector<int> indexToFree;
        std::unordered_set<int> visited;
        for(int i=0;i<randomSize;)
        {
            int index = randomIndex();
            if(visited.find(index) != visited.end()) continue;
            indexToFree.emplace_back(index);
            free(objects[index]);
            visited.insert(index);
            i++;
        }

        std::cout<<"After random Removal"<<std::endl;
        std::cout<<tcmalloc::MallocExtension::GetStats()<<std::endl;

        //repopulate deleted indexes
        for(int i=0;i<randomSize;i++)
        {
            objects[indexToFree[i]] = (char*)malloc(objectSizes[randomIndex()]);
        }

        //std::cout<<"After repopulation"<<std::endl;
        //std::cout<<tcmalloc::MallocExtension::GetStats()<<std::endl;
    }

    std::cout<<"Done"<<std::endl;
    return 0;
}


































    //std::cout << "Standard Alignment: " << alignof(std::max_align_t) << '\n';

//     // std::cout<<"Begin\n";
//     // double *ptr = (double*) malloc(sizeof(double));
//     // std::cout << "Double Alignment: " << alignof(*ptr) << '\n';
//     // std::cout<<"First\n";

//     // char *ptr2 = (char*) malloc(1);
//     // std::cout << "Char Alignment: " << alignof(*ptr2) << '\n';
//     // std::cout<<"Second\n";

//     // void *ptr3;
//     // std::cout << "Sizeof void*: " << sizeof(ptr3) << '\n';

//     // std::cout<<"Large allocation\n";
//     // const size_t SIZE =  1024<<20;
//     // int *ptr4 = (int*) malloc(SIZE);
//     // std::cout<<"Int Alignment: " <<alignof(*ptr4)<<'\n';
//     // free(ptr4);
// int times = 0;
// std::thread t1(printStats);
// while(times <= 10000000){
//     //std::cout<<"Large object allocation: "<< std::endl;
//     //size_t largeSize = 2097154;//300<<10;
//     //char* lptr = (char*) malloc(largeSize);
//     //char* lptr2 = (char*) malloc(largeSize);
//     //std::cout<<"Freeing large object"<<std::endl<<std::endl;
//     // free(lptr);
//     // free(lptr2);

//     //std::cout<<"New \n";
//     //Sample* s = new Sample();
//     //std::cout<<s->getA();

//     for(int i=0;i<1000;i++)
//     {
//     //std::cout<<"Small object allocation: "<< std::endl;
//     size_t size1 = 100<<10;
//     size_t size2 = 200<<10;
//     size_t size3 = 220<<10;
//     char* ptr = (char*) malloc(size1);
//     char* ptr2 = (char*) malloc(size2);
//     char* ptr3 = (char*) malloc(size3);
//     //printf("memory location %p\n", ptr);
//     //printf("memory location %p\n", ptr2);
//     //printf("memory location %p\n", ptr3);
//     //std::cout<<ptr<<" "<<ptr2<<" "<<ptr3<<std::endl;
//     //std::cout<<"Freeing small object"<<std::endl<<std::endl;
//     // free(ptr);
//     // free(ptr2);
//     // free(ptr3);
//     }

//     times++;
// }
// std::cout<<"Done\n";
// t1.join();
