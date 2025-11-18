#include <filesystem>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <string>
#include <string_view>
#include <chrono>
#include <vector>
#include <fstream>
#include <iostream>
#include <functional>
#include <unordered_set>

using namespace std;
namespace ch = std::chrono;
namespace fs = std::filesystem;
// global variable
const auto current_path { fs::current_path() };

// create for O(1) a hash table lookup scan
/*
    Broken
*/
unordered_set<string> Strings_From_RawData(vector<byte>& raw_data){
    const string valid_chars{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"};
    unordered_set<string> set;
    string curr = "";
    for(byte b : raw_data){
        if(valid_chars.find(static_cast<char>(b)) != string::npos)
            curr += static_cast<char>(b);
        else {
            if(!curr.empty())
                set.insert(curr);
            curr.clear();
        }
    }
    return set;
}

// slower but needed for boyer scan
const string LongString_From_RawData(vector<byte>& raw_data){
    const string valid_chars{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"};
    string ans = "";
    for(byte b : raw_data){
        if(valid_chars.find(static_cast<char>(b)) != string::npos)
            ans += static_cast<char>(b);
    }
    return ans;
}

/*
    C++17 in detail page 197, Loading file into a string
    And:
    https://stackoverflow.com/questions/51352863/what-is-the-idiomatic-c17-standard-approach-to-reading-binary-files
*/
vector<byte> GetFileContents(const fs::path& path){
    ifstream ifs{path, ios::in|ios::binary};
    if(!ifs)
        throw runtime_error("Cannot open" + path.string());

    const auto fsz = fs::file_size(path);
    if(fsz > numeric_limits<size_t>::max())
        throw runtime_error("File too large to fit size_t! " + path.string());
    
    vector<byte> buff(fsz);
    ifs.read((char*)buff.data(), buff.size());

    if(!ifs)
        throw runtime_error("Couldnt read data " + path.string());
    
    return buff;
}

string BytesToSignature(vector<byte>& bytes){
    string signature = "";
    for(auto b : bytes){
        signature += static_cast<char>(b);
    }
    return signature;
}

int Set_SignatureScan(unordered_set<string>& set, vector<string>& signatures){
    int hits = 0;
    for(auto signature : signatures){
        if(auto s = set.find(signature); s != set.end())
            hits++;
    }
    return hits;
}

/*
    Returns number of hits
*/
int BoyerString_SignatureScan(const string& data, vector<string>& signatures)
{
    int hits = 0;
    for(auto needle : signatures){
        const auto it = search(
            data.begin(), data.end(),
            boyer_moore_searcher(needle.begin(), needle.end())
        );
        if(it != data.end())
            hits++;
    }
    return hits;
}

bool Boyer_FindString(const string& data, const string& needle)
{
    const auto it = search(
        data.begin(), data.end(),
        boyer_moore_searcher(needle.begin(), needle.end())
    );
    return it != data.end();
}

int main(int argc, char** argv){
    if(argc < 2){
        cout << "Usage ./app.exe <target.exe>";
        return -1;
    }
    fs::path file_path { argv[1] };

    vector<string> signatures{
        "virus", "malware", "loadLibraryA", "CreateRemoteThread"
    };
    
    // heap
    vector<byte> raw_file_contents = GetFileContents(file_path);
    cout << "Raw data sz: " << raw_file_contents.size() << "\n";

    
    // TEST 1
    // calc start time
    auto start_time = ch::high_resolution_clock::now(); 
    {
        // algorithm
        string long_strings { LongString_From_RawData(raw_file_contents) };
        int res = BoyerString_SignatureScan(long_strings, signatures);

        // deduce flag
        if(res > 0)
            cout << "Hits : " << res << "\n";
        else
            cout << "No flags found\n";
    }
    // calc run time
    auto stop_time = ch::high_resolution_clock::now();
    auto duration =
        ch::duration_cast<ch::microseconds>(stop_time - start_time).count();
    cerr << "TEST 1:\n\nLong string boyer strings lookup\nRun time: " << duration << " micro s\n\n\n";

    // TEST 2
    
    auto start_time2 = ch::high_resolution_clock::now();
    {
        unordered_set set_data = Strings_From_RawData(raw_file_contents);
        int hits = Set_SignatureScan(set_data, signatures);

        if(hits > 0)
            cout << "Hits: " << hits << "\n";
        else
            cout << "No signatures found\n";
    }
    // calc run time
    auto stop_time2 = ch::high_resolution_clock::now();
    auto duration2 =
        ch::duration_cast<ch::microseconds>(stop_time2 - start_time2).count();
    cerr << "TEST 2:\n\nHashMap strings lookup\nRun time: " << duration2 << " micro s\n\n\n";
}
