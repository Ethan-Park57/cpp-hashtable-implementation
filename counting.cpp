#include "Hashtable.h"
#include "avlbst.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string process(string s)  // get s into usable form
{
    string buf = "";
    for (unsigned int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            buf += s[i];
        }
    }
    s = "";
    for (unsigned int i = 0; i < buf.length(); i++) {
        s += tolower(buf[i]);
    }
    return s;
}

int main(int argc, char** argv) {
    clock_t start;
    double duration = 0;

    if (argc < 6) {
        cout << "Bad number command-line arguments" << endl;
        return -1;
    }

    ifstream ifile(argv[1]);
    if (ifile.fail()) {
        cout << "No input file opened" << endl;
        return -1;
    }

    ofstream ofile(argv[2]);
    int x = atoi(argv[3]);  // probe type
    int d = atoi(argv[4]);  // debug mode?
    int r = atoi(argv[5]);  // repeat num
    vector<string> words;
    stringstream ss;
    string line = "";
    string buf = "";

    // store all input words in words vector
    while (getline(ifile, line)) {
        if (line.length() == 0) {
            continue;
        }
        ss << line;
        while (ss >> skipws >> buf >> skipws) {
            buf = process(buf);
            if (buf == "") {
                continue;
            }
            words.push_back(buf);
        }
        ss.clear();
    }

    // DONE PROCESSING

    start = clock();
    for (int i = 0; i < r; i++) {
        // reinstatiate every iterations
        Hashtable myHT(d, x);
        AVLTree<string, int> a;

        if (x != 3) {
            for (unsigned int j = 0; j < words.size(); j++) {
                myHT.add(words[j]);
            }
        } else {
            for (unsigned int j = 0; j < words.size(); j++) {
                AVLTree<string, int>::iterator it = a.find(words[j]);
                if (it != a.end())
                    a.insert(make_pair(words[j], it->second + 1));
                else
                    a.insert(make_pair(words[j], 1));
            }
        }
        // output results for human readability
        if (i == r - 1) {
            duration = (clock() - start) / (double)CLOCKS_PER_SEC;
            if (x != 3) {
                ofile << "Hashtable with ";
                if (x == 0)
                    ofile << "linear probing" << endl;
                else if (x == 1)
                    ofile << "quadratic probing" << endl;
                else if (x == 2)
                    ofile << "double hashing" << endl;
            } else {
                ofile << "AVLTree" << endl;
            }
            ofile << words.size() << " words" << endl;
            ofile << "ALL TIMES ARE IN SECONDS" << endl;
            ofile << "Time for r iterations: " << duration << endl;
            ofile << "Per iteration (average): " << duration / r << endl;
            ofile << "Per operation: " << (duration / r) / words.size() << endl << endl;

            if (x != 3)
                myHT.reportAll(ofile);
            else {
                ofile << "AVLTree" << endl;
                for (BinarySearchTree<string, int>::iterator it = a.begin(); it != a.end(); ++it) 
                {
                    ofile << it->first << " " << it->second << endl;
                }
            }
            ofile.close();
        }
    }

    return 1;
}