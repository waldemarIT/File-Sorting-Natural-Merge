#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

#include "NaturalMergeSort.h"

using namespace std;

void generateRandomFile(const string& filename, int count) {
    ofstream file(filename, ios::binary);

    for (int i = 0; i < count; ++i) {
        Record rec;
        rec.a = rand() % 10 - 5;
        rec.x = rand() % 10;
        rec.c = rand() % 5;
        rec.z = rand() % 3;
        rec.y = rand() % 3;
        file.write(reinterpret_cast<const char*>(&rec), sizeof(Record));
    }

    file.close();
}

void inputRecordsManually(const string& filename) {
    ofstream file(filename, ios::binary);
    int count;

    cout << "How many records to enter? ";
    cin >> count;

    for (int i = 0; i < count; ++i) {
        Record rec;
        cout << "\nRecord " << (i + 1) << ":" << endl;
        cout << "a: "; cin >> rec.a;
        cout << "x: "; cin >> rec.x;
        cout << "c: "; cin >> rec.c;
        cout << "z: "; cin >> rec.z;
        cout << "y: "; cin >> rec.y;
        file.write(reinterpret_cast<const char*>(&rec), sizeof(Record));
    }

    file.close();
}

void loadFromTestFile(const string& outputFile, const string& testFile) {
    ifstream input(testFile);
    ofstream output(outputFile, ios::binary);

    Record rec;
    while (input >> rec.a >> rec.x >> rec.c >> rec.z >> rec.y) {
        output.write(reinterpret_cast<const char*>(&rec), sizeof(Record));
    }

    input.close();
    output.close();
}

void runExperiment() {
    cout << "\n EXPERIMENT " << endl;

    vector<int> sizes = { 50, 100, 500, 1000, 5000 };

    cout << left << setw(10) << "N"
        << setw(12) << "Phases"
        << setw(15) << "Block Reads"
        << setw(15) << "Block Writes" << endl;
    cout << string(52, '-') << endl;

    for (int n : sizes) {
        generateRandomFile("data.dat", n);
        NaturalMergeSort sorter("data.dat");
        sorter.sort();

        cout << left << setw(10) << n
            << setw(12) << sorter.getPhaseCount()
            << setw(15) << sorter.getTotalReads()
            << setw(15) << sorter.getTotalWrites() << endl;
    }

    cout << "\nTheoretical phase estimate: O(log n)" << endl;
    cout << "Theoretical operations estimate: O(n log n)" << endl;
}

int main() {
    srand((unsigned)time(nullptr));

    int choice;
    string filename = "data.dat";

    while (true) {
        cout << "\n MENU " << endl;
        cout << "1. Generate random data" << endl;
        cout << "2. Input data manually" << endl;
        cout << "3. Load from test file" << endl;
        cout << "4. Display file contents" << endl;
        cout << "5. Sort file" << endl;
        cout << "6. Run experiment" << endl;
        cout << "7. Demo load test.txt and sort (show phases)" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
        cin >> choice;

        if (choice == 0) break;

        switch (choice) {
        case 1: {
            int count;
            cout << "Number of records: ";
            cin >> count;
            generateRandomFile(filename, count);
            cout << "File generated!" << endl;
            break;
        }
        case 2:
            inputRecordsManually(filename);
            cout << "Data entered!" << endl;
            break;
        case 3: {
            string testFile;
            cout << "Test file name: ";
            cin >> testFile;
            loadFromTestFile(filename, testFile);
            cout << "Data loaded!" << endl;
            break;
        }
        case 4:
            cout << "\nFile contents:" << endl;
            NaturalMergeSort::displayFile(filename);
            break;
        case 5: {
            NaturalMergeSort sorter(filename);
            sorter.setShowPhases(true);
            cout << "\nSorting..." << endl;
            sorter.sort();
            cout << "\nFile sorted!" << endl;
            cout << "Number of phases: " << sorter.getPhaseCount() << endl;
            cout << "Block reads: " << sorter.getTotalReads() << endl;
            cout << "Block writes: " << sorter.getTotalWrites() << endl;
            cout << "\nSorted file:" << endl;
            NaturalMergeSort::displayFile(filename);
            break;
        }
        case 6:
            runExperiment();
            break;
        case 7: {
            // load test.txt into data.dat and run with phases shown
            const string testFile = "test.txt";
            loadFromTestFile(filename, testFile);
            cout << "Loaded " << testFile << " into " << filename << endl;
            NaturalMergeSort sorter(filename);
            sorter.setShowPhases(true);
            cout << "\nSorting..." << endl;
            sorter.sort();
            cout << "\nFile sorted!" << endl;
            cout << "Number of phases: " << sorter.getPhaseCount() << endl;
            cout << "Block reads: " << sorter.getTotalReads() << endl;
            cout << "Block writes: " << sorter.getTotalWrites() << endl;
            cout << "\nSorted file:" << endl;
            NaturalMergeSort::displayFile(filename);
            break;
        }
        default:
            cout << "Invalid choice!" << endl;
        }
    }

    return 0;
}