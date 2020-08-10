
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * Halve Y value of YUV420P file
 * @param url     Location of Input YUV file.
 * @param w       Width of Input YUV file.
 * @param h       Height of Input YUV file.
 * @param num     Number of frames to process.
 */
int simplest_yuv420_halfy(char *url, int w, int h,int num){
    FILE *fp=fopen(url,"rb+");
    FILE *fp1=fopen("output_half.yuv","wb+");

    unsigned char *pic=(unsigned char *)malloc(w*h*3/2);

    for(int i=0;i<num;i++){
        fread(pic,1,w*h*3/2,fp);
        //Half
        for(int j=0;j<w*h;j++){
            unsigned char temp=pic[j]/2;
            //printf("%d,\n",temp);
            pic[j]=temp;
        }
        fwrite(pic,1,w*h*3/2,fp1);
    }

    free(pic);
    fclose(fp);
    fclose(fp1);

    return 0;
}


int main(int argc, char* argv[]) {
    //Test
    simplest_yuv420_halfy("lena_256x256_yuv420p.yuv",256,256,1);

}