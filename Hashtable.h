#include <cstdlib>
#include <ostream>
#include <string>

class Hashtable {
public:
    Hashtable(bool debug = false, unsigned int probing = 0);
    ~Hashtable();
    void add(std::string k);
    int count(std::string k);
    void reportAll(std::ostream& os) const;

private:
    int hash(std::string k) const;                       // h1(k)
    int doubleHash(std::string k) const;                 // h2(k)
    int getW(const std::string& k, const int& i) const;  // gets w1-w5 array
    bool insert(int& hK, const std::string& k);
    int getIndex(int& hK, const std::string& k, int& i, int& hK2, const int temp) const;
    void resize();
    std::string reverseString(std::string& k) const;

    bool d;                           // debug
    unsigned int probeType;           // probing
    std::pair<std::string, int>** h;  // Hashtable array
    int n = 0;                        // # items in hashtable, used for calculating loading factor
    int size;                         // # indices in hashtable
    int arrayIt = 0;
    bool resized = false;
    int oldNum = 0;

    int sizes[28]
            = {11,       23,       47,       97,        197,       397,       797,       1597,      3203,    6421,
               12853,    25717,    51437,    102877,    205759,    411527,    823117,    1646237,   3292489, 6584983,
               13169977, 26339969, 52679969, 105359969, 210719881, 421439783, 842879579, 1685759167};
    int primes[28] 
            = {  // for 2nd hash function
            7,        19,       43,       89,        193,       389,       787,       1583,      3191,    6397,
            12841,    25703,    51431,    102871,    205721,    411503,    823051,    1646221,   3292463, 6584957,
            13169963, 26339921, 52679927, 105359939, 210719881, 421439749, 842879563, 1685759113};
    int r[5] =  // for debug mode "random" numbers
            {983132572, 62337998, 552714139, 984953261, 261934300};
};
