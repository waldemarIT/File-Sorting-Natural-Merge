# File-Sorting-Natural-Merge
C++ implementation of External Natural Merge Sort (2+1) with simulated block-based I/O and performance analysis


# External Natural Merge Sort (2+1) Implementation

A high-performance C++ implementation of the **External Natural Merge Sort** algorithm. This project demonstrates how to sort large datasets stored on external storage (disk) by simulating block-based I/O operations and managing data flow between multiple files (tapes).

## 📌 Project Overview
Unlike internal sorting (like `std::sort`), external sorting is used when the dataset size exceeds the available RAM. This project utilizes the **Natural Merge** method, which leverages existing ordered sequences (runs) in the data to minimize the number of passes over the disk.

### Key Technical Features:
- **Layered Architecture:** Clear separation between the **Block I/O Layer** (storage engine) and the **Sorting Logic**.
- **Block-Based I/O:** Simulates real database page access with a configurable blocking factor ($B = 10$).
- **Complexity Analysis:** Tracks and compares practical disk operations ($I/O$ reads/writes) against theoretical models.
- **Key Function:** Records are sorted based on a specific mathematical criterion $g(rec)$:
  $$g(rec) = 10 \cdot a \cdot (x^2 + 3c^3z^4 - 5y^7)$$

---

## 🛠 Project Structure
- `/src` — Refactored C++ source code (`Record.h`, `BlockIO.h`, `NaturalMergeSort.cpp`, etc.).
- `/docs` — Full technical report (PDF) containing detailed experiments and LaTeX documentation.
- `/data` — Sample test files for verification.

---

## 📊 Experimental Results
The following data was gathered during testing to analyze the efficiency of the algorithm across various dataset sizes ($N$):

| Records ($N$) | Blocks ($N_{bl}$) | Phases ($P_{pr}$) | Block Reads ($C_R$) | Block Writes ($C_W$) | Total $I/O$ |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 50 | 5 | 5 | 69 | 55 | 124 |
| 100 | 10 | 6 | 154 | 125 | 279 |
| 500 | 50 | 8 | 924 | 808 | 1,732 |
| 1,000 | 100 | 9 | 2,043 | 1,809 | 3,852 |
| 5,000 | 500 | 11 | 12,010 | 11,011 | 23,021 |

### Theoretical vs. Practical Findings
The experiment confirmed that the number of phases grows as $O(\log N)$. However, the actual $I/O$ cost is higher than the simplified theoretical model due to operational overhead (checking sorted status and distribution), accurately modeled as:
$$C_{IO} \approx 5 \cdot N_{blocks} \cdot P_{phases}$$

---

## 🚀 How to Run
1. **Clone the repository:**
   ```bash
   git clone [https://github.com/your-username/your-repo-name.git](https://github.com/your-username/your-repo-name.git)
