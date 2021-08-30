#include "Hashtable.h"

#include <cmath>
#include <iostream>
#include <ostream>
#include <random>

using namespace std;

Hashtable::Hashtable(bool debug, unsigned int probing) : d(debug), probeType(probing) {
    size = sizes[arrayIt];
    h = new pair<string, int>*[size]();
    if (!d) 
    {
        srand(time(NULL));
        for (int i = 0; i < 5; i++) {
            r[i] = rand() % size;
        }
    }
}

Hashtable::~Hashtable() {
    // loop thru old hashtable, add to new hashtable
    for (int i = 0; i < size; i++) {
        if (h[i] != nullptr) {
            delete h[i];  // deallocate old hashtable
        }
    }
    delete[] h;
}

void Hashtable::add(string k) {
    if ((double)(n) / size >= 0.5) {
        resize();
    }

    // get new hash
    int hK = hash(k);
    int hK2 = doubleHash(k);
    int i = 0;
    int index = getIndex(hK, k, i, hK2, hK);

    // is K already in hashtable?
    if (index >= 0) {
        h[index]->second++;
    } else  // not in hashtable
    {
        if (resized) {
            h[hK] = new pair<string, int>(k, oldNum);
        } else {
            h[hK] = new pair<string, int>(k, 1);
        }
        n += 1;
    }
}

int Hashtable::count(string k) {
    int hK = hash(k);
    int hK2 = doubleHash(k);
    int i = 0;
    int index = getIndex(hK, k, i, hK2, hK);  // find k
    if (index >= 0)
        return h[index]->second;
    else
        return 0;
}

int Hashtable::getIndex(int& hK, const string& k, int& i, int& hK2, const int temp) const {
    if (h[hK] == nullptr)
        return -1;  // empty
    else if (h[hK]->first == k)
        return hK;  // // found

    if (probeType == 0)  // linear
    {
        hK = (temp + (++i)) % size;
    } else if (probeType == 1)  // quadratic
    {
        hK = (temp + ((int)pow(++i, 2))) % size;
    } else  // double hashing
    {
        hK = (temp + (++i * hK2)) % size;
    }
    return getIndex(hK, k, i, hK2, temp);
}

string Hashtable::reverseString(string& k) const {
    string rString;

    for (size_t i = 0; i < k.length(); i++) {
        rString += k[k.length() - i - 1];
    }

    return rString;
}

int Hashtable::getW(const string& k, const int& i) const {
    int x = 0;
    for (int j = 0; j < 6; j++)  // calcaulte w values and store in x
    {
        if ((unsigned int)(j + (i * 6)) < k.length()) {
            x += (k[j + (i * 6)] - 'a') * pow(26, j);
        } else
            return x;
    }

    return x;
}

int Hashtable::hash(string k) const {
    long long hOfK = 0;
    int w[5] = {0};
    string rString = reverseString(k);

    // follows writeup algorithm
    for (int i = 0; i < (int)((rString.length() / 6.0) + 0.99); i++) {
        w[4 - i] = getW(rString, i);
    }

    for (int i = 0; i < 5; i++) {
        hOfK += ((long long)r[i] * w[i]);
    }

    return hOfK % size;
}

int Hashtable::doubleHash(string k) const {
    int p = primes[arrayIt];
    long long wSum = 0;
    string rString = reverseString(k);

    // follow write up algorithm
    for (int i = 0; i < (int)((rString.length() / 6.0) + 0.99); i++) {
        wSum += getW(rString, i);
    }

    return p - (wSum % p);
}

void Hashtable::resize() {
    // update member variables
    resized = true;
    n = 0;
    int oldSize = size;
    size = sizes[++arrayIt];

    if (!d) {
        for (int i = 0; i < 5; i++) {
            r[i] = rand() % size;
        }
    }

    // grab old hashtable
    pair<std::string, int>** buf = h;
    h = new pair<string, int>*[size]();

    // loop thru old hashtable, add to new hashtable
    for (int i = 0; i < oldSize; i++) {
        if (buf[i] != nullptr) {
            oldNum = buf[i]->second;
            this->add(buf[i]->first);
            delete buf[i];  // deallocate old hashtable
            oldNum = 0;
        }
    }
    resized = false;
    delete[] buf;
}

void Hashtable::reportAll(ostream& os) const {
    // outputs every key value pair in hashtable
    for (int i = 0; i < size; i++) {
        if (h[i] != nullptr) {
            os << h[i]->first << " " << h[i]->second << endl;
        }
    }
}