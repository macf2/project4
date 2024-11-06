#include "Cat.hpp"
#include "sorting-algorithms/bubbleSort.h"
#include "sorting-algorithms/heapSort.h"
#include "sorting-algorithms/insertionSort.h"
#include "sorting-algorithms/mergeSort.h"
#include "sorting-algorithms/quickSort.h"
#include "sorting-algorithms/radixSort.h"
#include "sorting-algorithms/selectionSort.h"
#include "stability-test/contactsList.h"
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;

void stabilityTest();

/* stuff from project 1 */
std::vector<Cat> project1() {
        std::ifstream cat_file("cats_dataset.csv");
        if (!cat_file.is_open())
                abort();
        std::stringstream cat_data;
        cat_data << cat_file.rdbuf();

        char *pdata = (char *)cat_data.str().data();

        std::vector<Cat> cats;

        while (*pdata++ != '\n')
                ;

        while (*pdata != '\0') {
                uint32_t i = atoi(pdata);

                while (*pdata++ != ',')
                        ;

                char *pbreed = pdata;

                while (*pdata++ != ',')
                        ;
                pdata[-1] = '\0';

                uint32_t age = atoi(pdata);

                while (*pdata++ != ',')
                        ;

                uint32_t weight = atoi(pdata);

                while (*pdata++ != ',')
                        ;

                uint32_t idx_color = pdata - pbreed;

                while (*pdata++ != ',')
                        ;
                pdata[-1] = '\0';

                uint32_t idx_gender = pdata - pbreed;

                while (*pdata++ != '\n')
                        ;
                pdata[-1] = '\0';

                cats.emplace_back(
                    Cat(i, pbreed, age, weight, idx_color, idx_gender));
        }

        return cats;
}

int main() {
        vector<Cat> cats = project1();

        // bubble reads and allocs
        ofstream br("graphs/bubble/reads.csv");
        br << "number of cats, reads\n"; 
        ofstream ba("graphs/bubble/allocs.csv");
        ba << "number of cats, allocs\n"; 

        // quick stable reads and allocs
        ofstream qsr("graphs/quick_stable/reads.csv");
        qsr << "number of cats, reads\n"; 
        ofstream qsa("graphs/quick_stable/allocs.csv");
        qsa << "number of cats, allocs\n"; 

        // quick unstable reads and allocs
        ofstream qur("graphs/quick_unstable/reads.csv");
        qur << "number of cats, reads\n"; 
        ofstream qua("graphs/quick_unstable/allocs.csv");
        qua << "number of cats, allocs\n"; 

        // heap reads and allocs
        ofstream hr("graphs/heap/reads.csv");
        hr << "number of cats, reads\n"; 
        ofstream ha("graphs/heap/allocs.csv");
        ha << "number of cats, allocs\n"; 

        uint32_t sz = 1000;
        do {
                cats.resize(sz);

                // number of bubble reads, allocs
                unsigned long nbr = 0, nba = 0;
                bubbleSort<Cat>(cats, nbr, nba);
                br << sz << ", " << nbr << '\n';
                ba << sz << ", " << nba << '\n';

                // number of quick stable reads, allocs
                unsigned long nqsr = 0, nqsa = 0;
                quickSortStable<Cat>(cats, nqsr, nqsa);
                qsr << sz << ", " << nqsr << '\n';
                qsa << sz << ", " << nqsa << '\n';

                // number of quick unstable reads, allocs
                unsigned long nqur = 0, nqua = 0;
                quickSortUnstable<Cat>(cats, nqur, nqua);
                qur << sz << ", " << nqur << '\n';
                qua << sz << ", " << nqua << '\n';

                // number of heap reads, allocs
                unsigned long nhr = 0, nha = 0;
                heapSort<Cat>(cats, nhr, nha);
                hr << sz << ", " << nhr << '\n';
                ha << sz << ", " << nha << '\n';

        } while (sz -= 100);

        br.close();
        ba.close();
        qsr.close();
        qsa.close();
        qur.close();
        qua.close();
        hr.close();
        ha.close();

        stabilityTest();
        return 0;
}

void stabilityTest() {
        ContactsList people;
        people.readContactsFromFile("stability-test/names.csv");
        cout << "People (Default ordering)" << endl;
        people.printList();

        cout << endl << "Bubble Sort" << endl;
        people.sortAndPrint(bubbleSort<contact>);

        cout << endl << "quick sort stable" << endl;
        people.sortAndPrint(quickSortStable<contact>);

        cout << endl << "Heap Sort" << endl;
        people.sortAndPrint(heapSort<contact>);

        cout << endl << "quick sort unstable" << endl;
        people.sortAndPrint(quickSortUnstable<contact>);
}