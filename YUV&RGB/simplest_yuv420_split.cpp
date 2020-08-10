//
// Created by admin on 2020-08-01.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/**
 * Split Y, U, V planes in YUV420P file.
 * @param url  Location of Input YUV file.
 * @param w    Width of Input YUV file.
 * @param h    Height of Input YUV file.
 * @param num  Number of frames to process.
 *
 */
int simplest_yuv420_split(char *url, int w, int h,int num){
    FILE *fp=fopen(url,"rb+");
    FILE *fp1=fopen("output_420_y.y","wb+");
    FILE *fp2=fopen("output_420_u.y","wb+");
    FILE *fp3=fopen("output_420_v.y","wb+");
    unsigned char *pic=(unsigned char *)malloc(w*h*3/2);

    for(int i=0;i<num;i++){
        //如果一视频帧的分辨率是 w*h ，那么一帧YUV420P像素数据的大小是w*h*3/2 Byte
        fread(pic,1,w*h*3/2,fp);

        //Y
        // 前w*h Byte存储Y
        fwrite(pic,1,w*h,fp1);

        //U
        //w*h*1/4 Byte存储U
        fwrite(pic+w*h,1,w*h/4,fp2);

        //V
        //w*h*1/4 Byte存储V
        fwrite(pic+w*h*5/4,1,w*h/4,fp3);
    }

    free(pic);
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    return 0;
}


int main(int argc, char* argv[]) {

    //Test
    simplest_yuv420_split("lena_256x256_yuv420p.yuv",256,256,1);
}