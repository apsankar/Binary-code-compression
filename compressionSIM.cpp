/* On my honor, I have neither given nor received unauthorized aid on this assignment. */

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map> 
#include <vector>
#include <algorithm>
#include <bitset>
#include <string>
#include <sstream>
#define BINARY_SIZE 31

using namespace std;

void readInstructions(string inputfile, unordered_map <string,
               pair <int,int> > &freqMap, vector <string> &input) {
  int i=0;
  ifstream infile(inputfile);
  string line;
  while (getline(infile, line)) {
    if (!freqMap.count(line)) {
      freqMap[line].first = i;
      freqMap[line].second = 1;
    }
    else freqMap[line].second++;
    input.push_back(line);
    i++;
  }
}

inline bool sortByFrequency (const pair<string,pair<int,int>> &lhs, 
              const pair<string,pair<int,int>> &rhs) {
  if (lhs.second.second == rhs.second.second) return lhs.second.first < rhs.second.first; 
  else return (lhs.second.second > rhs.second.second);
}

void freqSort(unordered_map <string, pair <int,int> > &freqMap,
               vector <pair<string,pair<int,int> > > &sortVector) {
  for (auto it = freqMap.begin(); it != freqMap.end(); ++it)
    sortVector.push_back(make_pair(it->first, make_pair(it->second.first,it->second.second)));
  sort(sortVector.begin(), sortVector.end(), sortByFrequency);
}

void buildDict(vector <pair<string,pair<int,int> > > &sortVector,
 unordered_map <string, string> &dict, map <string, string> &dictIndex ) {
  int i=0;
  for ( auto it = sortVector.begin(); it != sortVector.begin() + 8; ++it, ++i) {
     bitset<3> x(i);
     string binary = x.to_string();
     dict[it->first] = binary;
     dictIndex[binary] = it->first;
  }
}

int checkOneMismatch(string ip, string &str, unordered_map <string, string> &dict) {
  bitset<32> inputBit(ip);
  for (auto it = dict.begin(); it != dict.end(); ++it) {
    bitset<32> dictEntry(it->first);
    dictEntry^= inputBit;
    if (dictEntry.count() == 1) {
      int test = 0;
      while (test < dictEntry.size() && !dictEntry.test(test))
        ++test;
      bitset<5> mismatchpos(BINARY_SIZE - test);
      str.append("010");
      str.append(mismatchpos.to_string());
      str.append(it->second);
      return 1;
    }
  }
  return 0;
}

int checkTwoConsecutive(string ip, string &str, unordered_map <string, string> &dict) {
  bitset<32> inputBit(ip);
  for (auto it = dict.begin(); it != dict.end(); ++it) {
    bitset<32> dictEntry(it->first);
    dictEntry^= inputBit;
    if (dictEntry.count() == 2) {
      int test_1 = 0, test_2 = 0;
      while (test_1 < dictEntry.size() && !dictEntry.test(test_1)) {
        ++test_1;
        ++test_2;
      }
      ++test_2;
      while (test_2 < dictEntry.size() && !dictEntry.test(test_2))
        ++test_2;
      if (test_2 - test_1 == 1) {
        bitset<5> mismatchpos(BINARY_SIZE - test_2);
        str.append("011");
        str.append(mismatchpos.to_string());
        str.append(it->second);
        return 1;
      }
    } 
  }
  return 0;
}

int checkTwoAnywhere(string ip, string &str, unordered_map <string, string> &dict) {
  bitset<32> inputBit(ip);
  for (auto it = dict.begin(); it != dict.end(); ++it) {
    bitset<32> dictEntry(it->first);
    dictEntry^= inputBit;
    if (dictEntry.count() == 2) {
      int test_1 = 0, test_2 = 0;
      while (test_1 < dictEntry.size() && !dictEntry.test(test_1)) {
        ++test_1;
        ++test_2;
      }
      ++test_2;
      while (test_2 < dictEntry.size() && !dictEntry.test(test_2))
        ++test_2; 
      if (test_2 - test_1 != 1) {
        bitset<5> mismatchpos_1(BINARY_SIZE - test_2);
        bitset<5> mismatchpos_2(BINARY_SIZE - test_1);
        str.append("100");
        str.append(mismatchpos_1.to_string());
        str.append(mismatchpos_2.to_string());
        str.append(it->second);
        return 1;
      }
    } 
  }
  return 0;
}

int checkBitMask(string ip, string &str, unordered_map <string, string> &dict) {
  bitset<32> inputBit(ip);
  bitset<4> mask;
  int p=0,q=0;
  for (auto it = dict.begin(); it != dict.end(); ++it) {
    bitset<32> dictEntry(it->first);
    dictEntry^=inputBit;
    if (dictEntry.count() <= 4) {
      int test = 0, test_1 = 0;
      while (test < dictEntry.size() && !dictEntry.test(test))
        ++test;
      if (test > 28) {
        test_1 = 32;
        if (test == 29) q++;
        for (p = test; p < test_1; ++p, ++q) {
          if (dictEntry.test(p)) mask.set(q);
        }
        str.append("00100000");
        str.append(mask.to_string());
        str.append(it->second);
        return 1;
      }
      test_1 = test + 4;
      while (test_1 < dictEntry.size() && !dictEntry.test(test_1))
        ++test_1;
      if (test_1 != 32) continue;
      else { 
        if (test != 0 && !dictEntry.test(test + 3)) {
          for (p = test - 1; p < test + 3; ++p, ++q) {
            if (dictEntry.test(p)) mask.set(q);
          }
          bitset<5> location(BINARY_SIZE - (test + 2));
          str.append("001");
          str.append(location.to_string());
          str.append(mask.to_string());
          str.append(it->second);
          return 1;
        }
        for ( p = test; p < test + 4; ++p, ++q ) {
        if (dictEntry.test(p)) mask.set(q);
        }
        bitset<5> location(BINARY_SIZE - (test + 3));
        str.append("001");
        str.append(location.to_string());
        str.append(mask.to_string());
        str.append(it->second);
        return 1;
      }
    }
  }
  return 0;
}

void writeCompressed(vector <string> &input, 
    unordered_map <string, string> &dict, map <string, string> &dictIndex ) {
ofstream outfile("cout.txt");
int runLength = 0;
string str;
auto prev = input.end();
for (auto it = input.begin(); it != input.end(); prev = it, ++it) {
  if (prev != input.end()) { //runlength check
  if (*it == *prev) {
    runLength++;
    continue;
    }
  else if (runLength > 0) {
    int num = runLength / 4;
    for (int i=0; i<num; i++)
      str.append("00011");
    runLength = runLength % 4;
    if (runLength != 0) {
    bitset<2> rle(runLength - 1);
    runLength = 0;
    str.append("000");
    str.append(rle.to_string());
      }
    }
  }  
  if (dict.count(*it)) {
    str.append("101");
    str.append(dict[*it]);
    continue;     
    }
  if (checkOneMismatch(*it, str, dict)) continue;
  if (checkTwoConsecutive(*it, str, dict)) continue;
  if (checkBitMask(*it, str, dict)) continue;
  if (checkTwoAnywhere(*it, str, dict)) continue;
  str.append("111");
  str.append(*it);
  }
  if (runLength > 0) {
    bitset<2> rle(runLength - 1);
    str.append("000");
    str.append(rle.to_string());
  } 
  unsigned int x = 0;
  for (auto iter = str.begin(); iter != str.end(); ++iter) {
    outfile<<*iter;
    x++;
    if (x % 32 == 0) {
      x = 0;
      outfile<<endl; 
    }
  }
  int t;
  for (t=0; x != 0 && t < 32 - x; t++)
    outfile<<"1";
  if (t != 0) outfile<<endl;
  outfile<<"xxxx"<<endl;
  for (auto iter1 = dictIndex.begin(); iter1 != dictIndex.end(); ++iter1) 
    outfile<<iter1->second<<endl;
}

void readCompressed(string &str,string inputfile, map <string,string> &dict) {
  ifstream infile(inputfile);
  stringstream buffer;
  buffer << infile.rdbuf();
  str = buffer.str();
  string line;
  bool flag = 0;
  infile.clear();
  infile.seekg(0, std::ios::beg); 
  int i = 0;
  while (getline(infile,line)) {
    if (line == "xxxx")
      flag = 1;
    else if (flag == 1) {
      bitset<3> index(i);
      dict[index.to_string()] = line;
      ++i;
    }
  }
}

void decompress(string &str, string &buffer, 
      map <string, string> &dict, ofstream &outfile, int &i, bool &flag) {
  string str2,str3,str4,str5,str6,str7,str8;
  str2 = str.substr(i,3);
  i+= 3;
  if (str2 == "000") { //run-length
    str3 = str.substr(i,2);
    bitset<2> str3_b(str3);
    for (int j = 0; j <=str3_b.to_ulong();j++) {
      outfile<<buffer<<endl;
    }
    //buffer.clear();
    i+=2;
  }
  else if (str2 == "001") { //bitmask-based
    str3 = str.substr(i,5); //location
    bitset<5> str3_b(str3);
    i+=5;
    str4 = str.substr(i,4); //bitmask
    bitset<4> str4_b(str4);
    i+=4;
    str5 = str.substr(i,3);
    i+=3;
    str6 = dict[str5];
    str7 = str6.substr(str3_b.to_ulong(),4); //contains substr to be bitmasked
    bitset<4> str7_b(str7);
    str4_b ^= str7_b; //str4_b contains masked substr.
    str6.replace(str3_b.to_ulong(),4,str4_b.to_string());
    outfile<<str6<<endl;
    buffer = str6;
  }
  else if (str2 == "010") { //1-bit mismatch
    str3 = str.substr(i,5);
    bitset<5> str3_b(str3); //mismatch location
    i+=5;
    str4 = str.substr(i,3);
    i+=3;
    str6 = dict[str4];
    if (str6[str3_b.to_ulong()] == '1')    
      str6.replace(str3_b.to_ulong(),1,"0");
    else
      str6.replace(str3_b.to_ulong(),1,"1");
    outfile<<str6<<endl;
    buffer = str6;
  }
  else if (str2 == "011") { //2-bit consecutive mismatch
    str3 = str.substr(i,5);
    bitset<5> str3_b(str3);  //mismatch position  
    i+=5;
    str4 = str.substr(i,3); 
    i+=3;
    str6 = dict[str4];
    str5 = str6.substr(str3_b.to_ulong(),2);
    bitset<2> str5_b(str5); 
    str5_b.flip();
    str6.replace(str3_b.to_ulong(),2,str5_b.to_string());
    outfile<<str6<<endl;
    buffer = str6;
  }
  else if (str2 == "100") { //2-bit mismatch anywhere
    str3 = str.substr(i,5);
    bitset<5> str3_b(str3); //1st mismatch position
    i+=5;
    str4 = str.substr(i,5);
    bitset<5> str4_b(str4); //2nd mismatch position
    i+=5;
    str5 = str.substr(i,3);
    i+=3;
    str6 = dict[str5]; 
    str7 = str6.substr(str3_b.to_ulong(),1);
    bitset<1> str7_b(str7);
    str7_b.flip();
    str8 = str6.substr(str4_b.to_ulong(),1);
    bitset<1> str8_b(str8);
    str8_b.flip();
    str6.replace(str3_b.to_ulong(),1,str7_b.to_string());
    str6.replace(str4_b.to_ulong(),1,str8_b.to_string());
    outfile<<str6<<endl;
    buffer = str6;
  }
  else if (str2 == "101") { //direct match
    str3 = str.substr(i,3);
    i+=3;
    str4 = dict[str3];
    outfile<<str4<<endl;
    buffer = str4;
  }
  else if (str2 == "111") {
    bool exists = str.substr(i,32).find("x") != string::npos;
    if (exists) {
    flag = 1;
    return;
    }
    outfile<<str.substr(i,32)<<endl;
    buffer = str.substr(i,32);
    i+=32;
    }
  else {
    cout<<"Something's wrong with the input formatting. Exiting..."<<endl;
    exit(1);
  }
}

int main(int argc, char* argv[]) {
  if (argc !=2) {
    cout<<"Usage: ./SIM 1(for compression) or 2(for decompression)"<<endl;
    exit(1);
  }
  string arg1(argv[1]);
  if (arg1 != "1" && arg1 != "2") {
    cout<<"Usage: ./SIM 1(for compression) or 2(for decompression)"<<endl;
    exit(1);
  }
  if (arg1 == "1") {
  vector <string> input; 
  unordered_map <string, pair <int,int> > freqMap;
  unordered_map <string, string> dict;
  map <string, string> dictIndex;
  vector <pair<string,pair<int,int> > > sortVector;  
  readInstructions("original.txt", freqMap, input);
  freqSort(freqMap, sortVector);
  buildDict(sortVector, dict, dictIndex);
  writeCompressed(input, dict, dictIndex); 
  }
  else {
  //code for decompression
  string str;
  map <string, string> dict;
  string buffer;
  ofstream outfile("dout.txt");
  readCompressed(str, "compressed.txt", dict);
  str.erase(remove(str.begin(), str.end(), '\n'),str.end());
  int i = 0;
  bool flag = 0;
  while (flag != 1)
    decompress(str, buffer, dict, outfile, i, flag);
  }
}
