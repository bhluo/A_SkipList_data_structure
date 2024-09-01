#include <iostream>
#include <chrono>
#include <cstdlib>
#include <pthread.h>
#include <random>
#include <ctime>
#include <cstdint>  // For intptr_t and uintptr_t
#include "../skiplist.h"

#define NUM_THREADS 1
#define TEST_COUNT 100000
SkipList<int, std::string> skipList(18);

pthread_mutex_t skipListMutex = PTHREAD_MUTEX_INITIALIZER;

void *insertElement(void* threadid) {
    intptr_t tid = reinterpret_cast<intptr_t>(threadid);
    std::cout << tid << std::endl;

    int tmp = TEST_COUNT / NUM_THREADS;
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 rng(rd()); // Seed the generator
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, TEST_COUNT - 1);

    for (int i = tid * tmp, count = 0; count < tmp; i++) {
        count++;
        int value = dist(rng);

        pthread_mutex_lock(&skipListMutex);
        skipList.insert_element(value, "a");
        pthread_mutex_unlock(&skipListMutex);
    }

    return NULL;
}

void *getElement(void* threadid) {
    intptr_t tid = reinterpret_cast<intptr_t>(threadid);
    std::cout << tid << std::endl;

    int tmp = TEST_COUNT / NUM_THREADS;
    std::random_device rd;  // Obtain a random number from hardware
    std::mt19937 rng(rd()); // Seed the generator
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, TEST_COUNT - 1);

    for (int i = tid * tmp, count = 0; count < tmp; i++) {
        count++;
        int value = dist(rng);

        pthread_mutex_lock(&skipListMutex);
        skipList.search_element(value);
        pthread_mutex_unlock(&skipListMutex);
    }

    return NULL;
}

int main() {
    srand(static_cast<unsigned>(time(NULL)));

    // pthread_t threads[NUM_THREADS];
    // int rc;
    // intptr_t i;

    // auto start = std::chrono::high_resolution_clock::now();

    // for (i = 0; i < NUM_THREADS; i++) {
    //     std::cout << "main() : creating thread, " << i << std::endl;
    //     rc = pthread_create(&threads[i], NULL, insertElement, reinterpret_cast<void*>(i));

    //     if (rc) {
    //         std::cout << "Error: unable to create thread, " << rc << std::endl;
    //         exit(-1);
    //     }
    // }

    // void *ret;
    // for (i = 0; i < NUM_THREADS; i++) {
    //     if (pthread_join(threads[i], &ret) != 0) {
    //         perror("pthread_join() error");
    //         exit(3);
    //     }
    // }
    // auto finish = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> elapsed = finish - start;
    // std::cout << "insert elapsed: " << elapsed.count() << std::endl;

    // Un-comment to test getElement
    // {
        pthread_t threads[NUM_THREADS];
        int rc;
        intptr_t i;
        auto start = std::chrono::high_resolution_clock::now();

        for (i = 0; i < NUM_THREADS; i++) {
            std::cout << "main() : creating thread, " << i << std::endl;
            rc = pthread_create(&threads[i], NULL, getElement, reinterpret_cast<void*>(i));

            if (rc) {
                std::cout << "Error: unable to create thread, " << rc << std::endl;
                exit(-1);
            }
        }

        void *ret;
        for (i = 0; i < NUM_THREADS; i++) {
            if (pthread_join(threads[i], &ret) != 0) {
                perror("pthread_join() error");
                exit(3);
            }
        }

        auto finish = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "get elapsed: " << elapsed.count() << std::endl;
    // }

    pthread_mutex_destroy(&skipListMutex);
    return 0;
}

// #include <iostream>
// #include <chrono>
// #include <cstdlib>
// #include <pthread.h>
// #include <time.h>
// #include "../skiplist.h"

// #define NUM_THREADS 1
// #define TEST_COUNT 100000
// SkipList<int, std::string> skipList(18);

// void *insertElement(void* threadid) {
//     long tid; 
//     tid = (long)threadid;
//     std::cout << tid << std::endl;  
//     int tmp = TEST_COUNT/NUM_THREADS; 
// 	for (int i=tid*tmp, count=0; count<tmp; i++) {
//         count++;
// 		skipList.insert_element(rand() % TEST_COUNT, "a"); 
// 	}
//     pthread_exit(NULL);
// }

// void *getElement(void* threadid) {
//     long tid; 
//     tid = (long)threadid;
//     std::cout << tid << std::endl;  
//     int tmp = TEST_COUNT/NUM_THREADS; 
// 	for (int i=tid*tmp, count=0; count<tmp; i++) {
//         count++;
// 		skipList.search_element(rand() % TEST_COUNT); 
// 	}
//     pthread_exit(NULL);
// }

// int main() {
//     srand (time(NULL));  
//     {

//         pthread_t threads[NUM_THREADS];
//         int rc;
//         int i;

//         auto start = std::chrono::high_resolution_clock::now();

//         for( i = 0; i < NUM_THREADS; i++ ) {
//             std::cout << "main() : creating thread, " << i << std::endl;
//             rc = pthread_create(&threads[i], NULL, insertElement, (void *)i);

//             if (rc) {
//                 std::cout << "Error:unable to create thread," << rc << std::endl;
//                 exit(-1);
//             }
//         }

//         void *ret;
//         for( i = 0; i < NUM_THREADS; i++ ) {
//             if (pthread_join(threads[i], &ret) !=0 )  {
//                 perror("pthread_create() error"); 
//                 exit(3);
//             }
//         }
//         auto finish = std::chrono::high_resolution_clock::now(); 
//         std::chrono::duration<double> elapsed = finish - start;
//         std::cout << "insert elapsed:" << elapsed.count() << std::endl;
//     }
//     // skipList.displayList();

//     // {
//     //     pthread_t threads[NUM_THREADS];
//     //     int rc;
//     //     int i;
//     //     auto start = std::chrono::high_resolution_clock::now();

//     //     for( i = 0; i < NUM_THREADS; i++ ) {
//     //         std::cout << "main() : creating thread, " << i << std::endl;
//     //         rc = pthread_create(&threads[i], NULL, getElement, (void *)i);

//     //         if (rc) {
//     //             std::cout << "Error:unable to create thread," << rc << std::endl;
//     //             exit(-1);
//     //         }
//     //     }

//     //     void *ret;
//     //     for( i = 0; i < NUM_THREADS; i++ ) {
//     //         if (pthread_join(threads[i], &ret) !=0 )  {
//     //             perror("pthread_create() error"); 
//     //             exit(3);
//     //         }
//     //     }

//     //     auto finish = std::chrono::high_resolution_clock::now(); 
//     //     std::chrono::duration<double> elapsed = finish - start;
//     //     std::cout << "get elapsed:" << elapsed.count() << std::endl;
//     // }

// 	pthread_exit(NULL);
//     return 0;

// }
