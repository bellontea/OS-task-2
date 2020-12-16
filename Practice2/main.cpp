#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <ctime>
#include "sha256.h"
using namespace std;

/*
 * Найдите с помощью алгоритма брут форс пятибуквенные пароли, соответствующие
 * следующим хэшам SHA-256:
 * 1. 1115dd800feaacefdf481f1f9070374a2a81e27880f187396db67958b207cbad
 * 2. 3a7bd3e2360a3d29eea436fcfb7e44c735d117c42d1c1835420b6b9942dd4f1b
 * 3. 74e1bb62f8dabb8125a58852b63bdf6eaef667cb56ac7f7cdba6d7305c50a22f
 * Ваша программа должна перебрать все возможные пароли, состоящие только из пяти
 * строчных букв английского алфавита ASCII. Необходимо использовать параллельную
 * обработку. Вы можете вычислить хэши SHA-256, вызвав библиотеку (
 * java.security.MessageDigest) или используя пользовательскую реализацию. Напечатайте
 * каждый соответствующий пароль вместе с его хешем SHA-256.
 */

void findPassword(string* hash, string* result, char start, char end) {

    string input ;
    string output;

    input += start;
    input += "aaaa";

    if(end=='z')
        end = 0;

    do {
        output = sha256(input);

        for(int i = 0; i < 3; i++) {
            if(output == hash[i])
                result[i] = input;
        }

        for(int i = 4; i >= 0; i--) {
            if(input[i] == 'z') {
                input[i] = 'a';
            }
            else {
                input[i]++;
                break;
            }
        }
        //puts(input.c_str());
    }
    while (input[0] != end && input!="aaaaa");
}

string getLimits(int count) {
    string lim = "a";
    int step = 26/count;
    for(int i = 1; i < count - 1; i++) {
        lim += lim[i-1] + step;
    }
    lim += 'z';
    return lim;
}

int main() {
    ifstream file("Practice2.txt");

    if(!file) {
        cout << "Error" << endl;
        return 1;
    }

    string hash[3], result[3];

    for (int i = 0; i < 3; i++)
        file >> hash[i];

    file.close();

    int start1, end1, start2, end2;
    int selection;

    while (1) {

        cout << "Single-threaded (1) or multi-threaded mode (2)? Choose 1, 2 or 0 for exit" << endl;

        cin >> selection;

        switch (selection) {
            case 1:

                start1 = clock();

                findPassword(hash, result, 'a', 'z');

                end1 = clock();

                printf("Time: %f s\n", (end1 - start1)/1000.0);

                break;

            case 2:

                cout << "How much threads do you need?" << endl;

                int count;
                cin >> count;
                count--;
                string lim = getLimits(count+1);

                thread *th = new thread[count];

                start2 = clock();

                for(int i = 0; i < count; i++) {
                    th[i] = thread(findPassword, hash, result, lim[i], lim[i+1]);
                }

                for(int i = 0; i < count; i++) {
                    th[i].join();
                }

                end2 = clock();

                printf("Time: %f s\n", (end2 - start2)/1000.0);

                delete [] th;

                break;
        }

        if(selection == 0)
            break;

        else if(selection == 1 || selection == 2) {

            for(int i = 0; i < 3; i++)
                printf("%d password: %s\thash: %s\n", i+1, result[i].c_str(), hash[i].c_str());

        }
        cout << endl;
    }

    return 0;
}

