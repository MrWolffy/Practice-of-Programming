#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#pragma pack(1)
using namespace std;

typedef short WORD;
typedef int DWORD;
typedef int LONG;

// 文件头信息块
typedef struct tagBITMAPFILEHEADER
{
    WORD bfType; // 位图文件的类型，必须为“BM”
    DWORD bfSize; // 位图文件的大小，以字节为单位
    WORD bfReserved1; // 位图文件保留字，必须为0
    WORD bfReserved2; // 位图文件保留字，必须为0
    DWORD bfOffBits; // 位图数据的起始位置，以相对于位图文件头的偏移量表示，以字节为单位
} BITMAPFILEHEADER;

// 图像描述信息块
typedef struct tagBITMAPINFOHEADER{
    DWORD biSize; // 本结构所占用字节数
    LONG biWidth; // 位图的宽度，以像素为单位
    LONG biHeight; // 位图的高度，以像素为单位
    WORD biPlanes; // 目标设备的平面数不清，必须为1
    WORD biBitCount; // 每个像素所需的位数，必须是1(双色), 4(16色)，8(256色)或24(真彩色)之一
    DWORD biCompression; // 位图压缩类型，必须是 0(不压缩),1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
    DWORD biSizeImage; // 位图的大小，以字节为单位
    LONG biXPelsPerMeter; // 位图水平分辨率，每米像素数
    LONG biYPelsPerMeter; // 位图垂直分辨率，每米像素数
    DWORD biClrUsed;// 位图实际使用的颜色表中的颜色数
    DWORD biClrImportant;// 位图显示过程中重要的颜色数
} BITMAPINFOHEADER;


int main(int argc, const char **argv) {
    if (argc != 3) {
        cerr << "command error" << endl;
        return 0;
    }
    ifstream fin(argv[1], ios::in | ios::binary);
    ofstream fout(argv[2], ios::out | ios::binary);
    if (!fin || !fout) {
        cerr << "file open error" << endl;
        return 0;
    }
    BITMAPFILEHEADER header;
    BITMAPINFOHEADER info;
    fin.read((char *)&header.bfType, sizeof(WORD));
    fin.read((char *)&header.bfSize, sizeof(DWORD));
    fin.read((char *)&header.bfReserved1, sizeof(WORD));
    fin.read((char *)&header.bfReserved2, sizeof(WORD));
    fin.read((char *)&header.bfOffBits, sizeof(DWORD));
    fin.read((char *)&info.biSize, sizeof(DWORD));
    fin.read((char *)&info.biWidth, sizeof(LONG));
    fin.read((char *)&info.biHeight, sizeof(LONG));
    fin.read((char *)&info.biPlanes, sizeof(WORD));
    fin.read((char *)&info.biBitCount, sizeof(WORD));
    fin.read((char *)&info.biCompression, sizeof(DWORD));
    fin.read((char *)&info.biSizeImage, sizeof(DWORD));
    fin.read((char *)&info.biXPelsPerMeter, sizeof(LONG));
    fin.read((char *)&info.biYPelsPerMeter, sizeof(LONG));
    fin.read((char *)&info.biClrUsed, sizeof(DWORD));
    fin.read((char *)&info.biClrImportant, sizeof(DWORD));

    fout.write((char *)&header.bfType, sizeof(WORD));
    fout.write((char *)&header.bfSize, sizeof(DWORD));
    fout.write((char *)&header.bfReserved1, sizeof(WORD));
    fout.write((char *)&header.bfReserved2, sizeof(WORD));
    fout.write((char *)&header.bfOffBits, sizeof(DWORD));
    fout.write((char *)&info.biSize, sizeof(DWORD));
    fout.write((char *)&info.biHeight, sizeof(LONG));
    fout.write((char *)&info.biWidth, sizeof(LONG));
    fout.write((char *)&info.biPlanes, sizeof(WORD));
    fout.write((char *)&info.biBitCount, sizeof(WORD));
    fout.write((char *)&info.biCompression, sizeof(DWORD));
    fout.write((char *)&info.biSizeImage, sizeof(DWORD));
    fout.write((char *)&info.biXPelsPerMeter, sizeof(LONG));
    fout.write((char *)&info.biYPelsPerMeter, sizeof(LONG));
    fout.write((char *)&info.biClrUsed, sizeof(DWORD));
    fout.write((char *)&info.biClrImportant, sizeof(DWORD));

    char a[header.bfOffBits];
    fin.read(a, header.bfOffBits - 54);
    fout.write(a, header.bfOffBits - 54);

    int skip = 4 - ((info.biWidth * info.biBitCount) >> 3) & 3;
    char b[info.biHeight][info.biWidth][info.biBitCount/8+1];
    for (int i = 0; i < info.biHeight; ++i) {
        for (int j = 0; j < info.biWidth; ++j) {
            fin.read(b[i][j], info.biBitCount/8);
        }
        fin.seekg(skip, ios::cur);
    }
    for (int i = 0; i < info.biWidth; ++i) {
        for (int j = 0; j < info.biHeight; ++j) {
            fout.write(b[j][info.biWidth-i-1], info.biBitCount/8);
        }
        for (int j = 0; j < skip; ++j) {
            fout.write("0", sizeof(char));
        }
    }
    fin.close();
    fout.close();
    return 0;
}