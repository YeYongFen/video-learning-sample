//
// Created by yee on 2020-08-08.
//

//关闭内存对齐
#pragma pack(1)

#include <string.h>
#include <stdio.h>
#include <cstdlib>

using namespace std;

typedef struct
{
    unsigned short   bfType;
    unsigned int     bfSize;
    unsigned short   bfReserved1;
    unsigned short   bfReserved2;
    unsigned int     bfOffBits;
}BitMapFileHeader;

typedef struct
{
    unsigned int     biSize;
    int              biWidth;
    int              biHeight;
    unsigned short   biPlanes;
    unsigned short   biBitCount;
    unsigned int     biCompression;
    unsigned int     biSizeImage;
    int              biXPelsPerMeter;
    int              biYPelsPerMeter;
    unsigned int     biClrUsed;
    unsigned int     biClrImportant;
} BitMapInfoHeader;



unsigned char * ConvertRGBToBMP(unsigned char *RGBBuffer, unsigned int ImageWidth, unsigned int ImageHeight)
{
    BitMapFileHeader bitMapFile = {0};
    BitMapFileHeader* BmpFileHeader = &bitMapFile;//填充BMP文件头信息

    int totalSize = sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader) + ImageWidth*ImageHeight * 3;  //文件大小

    //文件类型
    BmpFileHeader->bfType = 0x4D42;//'BM‘

    printf( "%d" ,sizeof(unsigned int));

    //文件大小
    BmpFileHeader->bfSize = totalSize;

    //位图数据的偏移（跳过BMP头，位图头，调色板(如果有)）
    //RGB图为24 ,没有调色板
    BmpFileHeader->bfOffBits = sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader);

    //保留位，必须设置0
    BmpFileHeader->bfReserved1 = 0;

    //保留位，必须设置0
    BmpFileHeader->bfReserved2 = 0;


    BitMapInfoHeader bitMapInfo = {0};
    BitMapInfoHeader* BmpInfoHeader = &bitMapInfo;

    //位图信息头大小
    BmpInfoHeader->biSize = sizeof(BitMapInfoHeader); // 40

    //图像宽度
    BmpInfoHeader->biWidth = ImageWidth;

    //图像高度
    //如果是负数，那么图像是正向的，也就是2D屏幕坐标系（y轴由上到下，位图数据也由左上角一行行存放)
    BmpInfoHeader->biHeight = -ImageHeight;

    //颜色平面数，总是为1
    BmpInfoHeader->biPlanes = 1;

    //每个像素的比特数
    BmpInfoHeader->biBitCount = 24;//RGB图像

    //0:BI_RGB 不压缩
    BmpInfoHeader->biCompression = 0;

    //图像数据字节大小
    BmpInfoHeader->biSizeImage = ImageWidth*ImageHeight * 3;

    //水平分辨率
    BmpInfoHeader->biXPelsPerMeter = 0;

    //垂直分辨率
    BmpInfoHeader->biYPelsPerMeter = 0;

    //实际使用的彩色表中的颜色索引数
    //如果为0采用所用调色板中的索引或者没有调色板。
    BmpInfoHeader->biClrUsed = 0;

    //对图像显示有重要影响的颜色索引的数目
    //如果是0，表示都重要
    BmpInfoHeader->biClrImportant = 0;

    unsigned char *BMPBuffer = new unsigned char[totalSize];
    memset(BMPBuffer,0,totalSize);

    unsigned char *p = BMPBuffer;
    memcpy(p, (unsigned char*)BmpFileHeader, sizeof(BitMapFileHeader));

    p = p + sizeof(BitMapFileHeader);
    memcpy(p, (unsigned char*)BmpInfoHeader, sizeof(BitMapInfoHeader));

    p = p + sizeof(BitMapInfoHeader);
    memcpy(p, RGBBuffer, ImageWidth*ImageHeight * 3);

    return BMPBuffer ;
}


/**
 * Convert RGB24 file to BMP file
 * @param rgb24path    Location of input RGB file.
 * @param width        Width of input RGB file.
 * @param height       Height of input RGB file.
 * @param url_out      Location of Output BMP file.
 */
int simplest_rgb24_to_bmp(const char *rgb24path,int width,int height) {

    FILE *fp=fopen(rgb24path,"rb+");
    FILE *out_fp=fopen("utput_lena.bmp","wb+");

    unsigned char *RGBBuffer= new unsigned char[width*height*3];


    fread(RGBBuffer,1,width*height*3,fp);


    //24位RGB按照BGR的顺序来存储每个像素的各颜色通道的值
    //所以要把 24RGB 数据中的 'R' 和 'B' 交换

    int k=0;
    for (int k = 0; k < width*height*3 ; k+=3) {
        char temp=RGBBuffer[k];
        RGBBuffer[k]=RGBBuffer[k+2];
        RGBBuffer[k+2]=temp;

    }

    unsigned char *BMPBuffer = ConvertRGBToBMP(RGBBuffer ,  width , height);

    int totalSize = sizeof(BitMapFileHeader) + sizeof(BitMapInfoHeader) + width*height * 3;  //文件大小

    fwrite(BMPBuffer,totalSize,1,out_fp);


    fclose(fp);
    fclose(out_fp);
    free(RGBBuffer);
    free(BMPBuffer);

    return  0;


}

int main(){
    simplest_rgb24_to_bmp("lena_256x256_rgb24.rgb",256,256);
}