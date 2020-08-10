//
// Created by admin on 2020-08-05.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * Convert YUV420P file to gray picture
 * @param url     Location of Input YUV file.
 * @param w       Width of Input YUV file.
 * @param h       Height of Input YUV file.
 * @param num     Number of frames to process.
 */
int simplest_yuv420_gray(char *url, int w, int h,int num){
    FILE *fp=fopen(url,"rb+");
    FILE *fp1=fopen("output_gray.yuv","wb+");
    unsigned char *pic=(unsigned char *)malloc(w*h*3/2);

    for(int i=0;i<num;i++){
        fread(pic,1,w*h*3/2,fp);
        //Gray
        memset(pic+w*h,128,w*h/2);
        fwrite(pic,1,w*h*3/2,fp1);
    }

    free(pic);
    fclose(fp);
    fclose(fp1);
    return 0;
}



int main(int argc, char* argv[]) {

    //Test
    simplest_yuv420_gray("lena_256x256_yuv420p.yuv",256,256,1);
}