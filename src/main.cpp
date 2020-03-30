// КДЗ по дисциплине Алгоритмы и структуры данных, 2019-2020 уч.год
// Матевосян Армен Арсенович, группа БПИ-181, дата (07.04.2020)
// Среда разработки, CLion, MinGW 9.3.0 x64, Windows 10
//
// Cостав исходных файлов:
//
// src/main.cpp
// lib/bitbuf.hpp
// lib/timer.hpp
// lib/types.h
// lib/utils.h
// lib/archiver.hpp
// lib/huffman.hpp
// lib/lz77.hpp
// lib/lzw.hpp
//
// Реализованы следуюшие функции:
//
// сжатие и распаковка методом Хаффмана
// сжатие и распаковка методом LZ77
// сжатие и распаковка методом LZW
// проведен вычислительный эксперимент
// построены таблицы и графики, для измерения времени выполнения использовалось структура Timer
// оформлен отчет

#include <iostream>
#include "../lib/archiver.hpp"
#include "../lib/huffman.hpp"
#include "../lib/lz77.hpp"
#include "../lib/lzw.hpp"
#include "../lib/timer.hpp"

using namespace std;

/**
 * KiloByte size
 */
const unsigned int KB = 1024;

/**
 * Path variables
 */
const string DATA_PATH = "..\\DATA\\";
const string ORIGINAL_PATH = DATA_PATH + "original\\";
const string COMPRESSED_PATH = DATA_PATH + "compressed\\";
const string UNCOMPRESSED_PATH = DATA_PATH + "uncompressed\\";
const string RESULTS_PATH = DATA_PATH + "results\\";

/**
 * Gets and returns archivers
 * @return archivers
 */
vector<archiver *> getArchivers() {
    vector<archiver *> archivers;

    archivers.push_back(new huffman());
    archivers.push_back(new lz77<4 * KB, KB>());
    archivers.push_back(new lz77<8 * KB, 2 * KB>());
    archivers.push_back(new lz77<16 * KB, 4 * KB>());
    archivers.push_back(new lzw(16));

    return archivers;
}

void freeArchivers(const vector<archiver *> &archivers) {
    for (auto &el: archivers)
        delete el;
}

/**
 * Gets and returns endings for compression
 * @return endings for compression
 */
vector<string> getCompressedEndings() {
    vector<string> compressedEndings;

    compressedEndings.emplace_back("haff");
    compressedEndings.emplace_back("lz775");
    compressedEndings.emplace_back("lz7710");
    compressedEndings.emplace_back("lz7720");
    compressedEndings.emplace_back("lzw");

    return compressedEndings;
}

/**
 * Gets and returns endings for decompression
 * @return endings for decompression
 */
vector<string> getUncompressedEndings() {
    vector<string> uncompressedEndings;

    uncompressedEndings.emplace_back("unhaff");
    uncompressedEndings.emplace_back("unlz775");
    uncompressedEndings.emplace_back("unlz7710");
    uncompressedEndings.emplace_back("unlz7720");
    uncompressedEndings.emplace_back("unlzw");

    return uncompressedEndings;
}

/**
 * Gets and returns data filenames
 * @return data filenames
 */
vector<string> getDatFileNames() {
    vector<string> dataFileNames;

    dataFileNames.emplace_back("1.mp4");
    dataFileNames.emplace_back("2.docx");
    dataFileNames.emplace_back("3.exe");
    dataFileNames.emplace_back("4.pdf");
    dataFileNames.emplace_back("5.pptx");
    dataFileNames.emplace_back("6.txt");
    dataFileNames.emplace_back("7.bmp");
    dataFileNames.emplace_back("8.jpg");
    dataFileNames.emplace_back("9.bmp");
    dataFileNames.emplace_back("10.jpg");

    return dataFileNames;
}

/**
 * Computes file details(size, frequency table, entropy)
 * @param dataFileNames filenames
 */
void setupFileDetails(const vector<string> &dataFileNames) {
    huffman h;

    for (const auto &dataFileName : dataFileNames) {

        string originalFilePath = ORIGINAL_PATH + dataFileName;
        string detailsFilePath = originalFilePath + ".details.txt";
        string entropyFilePath = originalFilePath + ".entropy.txt";
        string sizeFilePath = originalFilePath + ".size.txt";

        size_t size;
        double entropy;
        vector<int> freqs;

        h.getDetails(originalFilePath, size, freqs, entropy);

        ofstream fsize(sizeFilePath, ios::out);
        fsize << size << endl;
        fsize.close();

        ofstream fentropy(entropyFilePath, ios::out);
        fentropy << entropy << endl;
        fentropy.close();

        ofstream fdetails(detailsFilePath, ios::out);

        for (const int &freq : freqs)
            fdetails << freq << endl;

        fdetails.close();
    }
}

/**
 * Starts the experiment
 * @param archivers archivers
 * @param compressedEndings endings for compression
 * @param uncompressedEndings endings for decompression
 * @param dataFileNames filenames
 * @param MEASURES_COUNT measures count
 * @param IGNORED_MEASURES_COUNT ignored measures count
 */
void startExperiment(const vector<archiver *> &archivers,
                     const vector<string> &compressedEndings,
                     const vector<string> &uncompressedEndings,
                     const vector<string> &dataFileNames,
                     const int MEASURES_COUNT,
                     const int IGNORED_MEASURES_COUNT) {

    const unsigned int ARCHIVERS_COUNT = archivers.size();
    const unsigned int FILES_COUNT = getDatFileNames().size();

    Timer timer;

    for (int i = 0; i < ARCHIVERS_COUNT; i++) {
        archiver *arch = archivers[i];

        cout << "Archiver: " << i + 1 << " was started" << endl;

        string timesFilePath = RESULTS_PATH + compressedEndings[i] + ".times.txt";
        string sizesFilePath = RESULTS_PATH + compressedEndings[i] + ".sizes.txt";
        string matchesFilePath = RESULTS_PATH + compressedEndings[i] + ".matches.txt";

        ofstream ftime(timesFilePath, ios::out);
        ofstream fsize(sizesFilePath, ios::out);
        ofstream fmatch(matchesFilePath, ios::out);

        for (int j = 0; j < FILES_COUNT; j++) {
            string originalFilePath = ORIGINAL_PATH + dataFileNames[j];
            string compressedFilePath = COMPRESSED_PATH + dataFileNames[j] + "." + compressedEndings[i];
            string uncompressedFilePath = UNCOMPRESSED_PATH + dataFileNames[j] + "." + uncompressedEndings[i];

            cout << "File: " << j + 1 << " was loaded" << endl;

            double compressTime = 0, decompressTime = 0;

            // ignoring results
            for (int k = 0; k < IGNORED_MEASURES_COUNT; k++) {
                arch->compress(originalFilePath, compressedFilePath);
                arch->decompress(compressedFilePath, uncompressedFilePath);
            }

            for (int k = 0; k < MEASURES_COUNT; k++) {
                timer.reset();
                arch->compress(originalFilePath, compressedFilePath);
                compressTime += timer.elapsed();

                timer.reset();
                arch->decompress(compressedFilePath, uncompressedFilePath);
                decompressTime += timer.elapsed();
            }

            compressTime /= MEASURES_COUNT;
            decompressTime /= MEASURES_COUNT;

            ftime << compressTime << " " << decompressTime << endl;
            fsize << arch->getSize(compressedFilePath) << endl;
            fmatch << (arch->compareFiles(arch->getContents(originalFilePath),
                                          arch->getContents(uncompressedFilePath)) ? "Matches" : "Differs") << endl;
        }

        ftime.close();
        fsize.close();
        fmatch.close();
    }
}

/**
 * Main entry point
 * @return exit code
 */
int main() {
    vector<archiver *> archivers = getArchivers();
    vector<string> compressedEndings = getCompressedEndings();
    vector<string> uncompressedEndings = getUncompressedEndings();
    vector<string> dataFileNames = getDatFileNames();

    setupFileDetails(dataFileNames);
    startExperiment(archivers, compressedEndings, uncompressedEndings, dataFileNames, 1, 0);

    freeArchivers(archivers);
    return 0;
}
