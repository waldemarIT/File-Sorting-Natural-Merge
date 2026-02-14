#ifndef NATURALMERGESORT_H
#define NATURALMERGESORT_H

#include "Record.h"
#include "BlockIO.h"
#include <string>

class NaturalMergeSort {
public:
    NaturalMergeSort(const std::string& file);
    void setShowPhases(bool show);
    void sort();
    int getPhaseCount() const;
    int getTotalReads() const;
    int getTotalWrites() const;
    static void displayFile(const std::string& filename, int maxRecords = 50);

private:
    void distribute();
    void merge();
    bool isSorted(const std::string& filename);

    std::string mainFile;
    std::string tape1File;
    std::string tape2File;
    int phaseCount;
    int totalReads;
    int totalWrites;
    bool showPhases;
};

#endif // NATURALMERGESORT_H
