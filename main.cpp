#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include<bits/stdc++.h>
using namespace std;
mutex dataMutex;
vector<int> data1, data2;

void readData(const std::string& filename, std::vector<int>& data) {
    ifstream file(filename);
    if (file.is_open()) {
        int value;
        while (file >> value) {
            data.push_back(value);
        }
        cout<<"Data taken"<<endl;
        file.close();
    } else {
        cerr << "Error opening file: " << filename << std::endl;
    }
}

void processData(std::vector<int>& data) {
    for (int& value : data) {
        // modified the data by adding 10 to each value
        value += 10;
    }
    cout<<"Data Processed"<<endl;
}

void writeData(const std::string& filename, const std::vector<int>& data) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const int& value : data) {
            file << value << " ";
        }
        cout<<"File edited"<<endl;
        file.close();
    } else {
        cerr << "Error opening file for writing: " << filename << std::endl;
    }
}

void processDataThread(std::vector<int>& data, const std::string& filename) {
    
        vector<int> localData;
        {
            lock_guard<std::mutex> lock(dataMutex);
            if (!data.empty()) {
                localData = std::move(data);
                data.clear();
            } 
        }

        processData(localData);

        // wrote modified data back to the file
        {
            lock_guard<std::mutex> lock(dataMutex);
            data.insert(data.end(), localData.begin(), localData.end());
        }
        cout<<"Data thread processed"<<endl;
        
        // wrote back to the file outside the lock
        writeData(filename, localData);
  
}

int main() {

    readData("data1.txt", data1);
    readData("data2.txt", data2);

    thread thread1(processDataThread, std::ref(data1), "data1.txt");
    thread thread2(processDataThread, std::ref(data2), "data2.txt");

    thread1.join();
    thread2.join();

    return 0;
}
