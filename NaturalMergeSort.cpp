#include "NaturalMergeSort.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

NaturalMergeSort::NaturalMergeSort(const string& file) : mainFile(file), tape1File("tape1.dat"), tape2File("tape2.dat"), phaseCount(0), totalReads(0), totalWrites(0), showPhases(false) {}

void NaturalMergeSort::setShowPhases(bool show) { showPhases = show; }

bool NaturalMergeSort::isSorted(const string& filename) {
    BlockIO io(totalReads, totalWrites);
    if (!io.openForRead(filename)) return true;

    Record prev, curr;
    if (!io.readRecord(prev)) {
        io.close();
        return true;
    }

    while (io.readRecord(curr)) {
        if (g(curr) < g(prev)) {
            io.close();
            return false;
        }
        prev = curr;
    }
    io.close();
    return true;
}

void NaturalMergeSort::distribute() {
    BlockIO input(totalReads, totalWrites);
    BlockIO output1(totalReads, totalWrites);
    BlockIO output2(totalReads, totalWrites);

    input.openForRead(mainFile);
    output1.openForWrite(tape1File);
    output2.openForWrite(tape2File);

    Record prev, curr;
    bool toTape1 = true;
    bool firstRecord = true;

    while (input.readRecord(curr)) {
        if (!firstRecord && g(curr) < g(prev)) {
            toTape1 = !toTape1;
        }

        if (toTape1) output1.writeRecord(curr);
        else output2.writeRecord(curr);

        prev = curr;
        firstRecord = false;
    }

    input.close();
    output1.close();
    output2.close();
}

void NaturalMergeSort::merge() {
    BlockIO input1(totalReads, totalWrites);
    BlockIO input2(totalReads, totalWrites);
    BlockIO output(totalReads, totalWrites);

    input1.openForRead(tape1File);
    input2.openForRead(tape2File);
    output.openForWrite(mainFile);

    Record rec1, rec2;
    bool has1 = input1.readRecord(rec1);
    bool has2 = input2.readRecord(rec2);

    while (has1 || has2) {
        bool inRun1 = has1;
        bool inRun2 = has2;

        while (inRun1 || inRun2) {
            if (!inRun1) {
                output.writeRecord(rec2);
                Record temp = rec2;
                has2 = input2.readRecord(rec2);
                if (!has2 || g(rec2) < g(temp)) inRun2 = false;
            }
            else if (!inRun2) {
                output.writeRecord(rec1);
                Record temp = rec1;
                has1 = input1.readRecord(rec1);
                if (!has1 || g(rec1) < g(temp)) inRun1 = false;
            }
            else if (g(rec1) <= g(rec2)) {
                output.writeRecord(rec1);
                Record temp = rec1;
                has1 = input1.readRecord(rec1);
                if (!has1 || g(rec1) < g(temp)) inRun1 = false;
            }
            else {
                output.writeRecord(rec2);
                Record temp = rec2;
                has2 = input2.readRecord(rec2);
                if (!has2 || g(rec2) < g(temp)) inRun2 = false;
            }
        }
    }

    input1.close();
    input2.close();
    output.close();
}

void NaturalMergeSort::sort() {
    phaseCount = 0;
    totalReads = 0;
    totalWrites = 0;

    while (!isSorted(mainFile)) {
        phaseCount++;

        if (showPhases) {
            cout << "\nPhase " << phaseCount << endl;
            cout << "Distribution to tapes..." << endl;
        }

        distribute();

        if (showPhases) {
            cout << "Tape 1:" << endl;
            displayFile(tape1File);
            cout << "Tape 2:" << endl;
            displayFile(tape2File);
            cout << "Merging..." << endl;
        }

        merge();

        if (showPhases) {
            cout << "Result after phase " << phaseCount << ":" << endl;
            displayFile(mainFile);
        }
    }
}

int NaturalMergeSort::getPhaseCount() const { return phaseCount; }
int NaturalMergeSort::getTotalReads() const { return totalReads; }
int NaturalMergeSort::getTotalWrites() const { return totalWrites; }

void NaturalMergeSort::displayFile(const string& filename, int maxRecords) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Cannot open file: " << filename << endl;
        return;
    }

    Record rec;
    int count = 0;
    while (file.read(reinterpret_cast<char*>(&rec), sizeof(Record)) && count < maxRecords) {
        cout << "(" << rec.a << "," << rec.x << "," << rec.c << "," << rec.z << "," << rec.y << ") g=" << g(rec) << endl;
        count++;
    }

    if (file.peek() != EOF) cout << "... (showing first " << maxRecords << " records)" << endl;
    file.close();
}
