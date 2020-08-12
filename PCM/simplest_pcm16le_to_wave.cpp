
//关闭内存对齐
#pragma pack(1)
#include <string.h>
#include <stdio.h>
#include <cstdlib>


//定义RIFF区块
typedef struct WAVE_HEADER{
    char    ChunkID[4];       //内容为""RIFF
    unsigned long ChunkSize;   //整个文件的大小，但是不包含ChunkID 和 ChunkSize。 即 文件的大小 - 8
    char    Format[4];     //内容为"WAVE"
}WAVE_HEADER;

//定义fmt区块
typedef struct WAVE_FMT{
    char    Subchunk1ID[4];          //内容为"fmt "
    unsigned long  Subchunk1Size;     //内容为WAVE_FMT占的字节数，为16
    unsigned short AudioFormat; //如果为PCM，改值为 1
    unsigned short NumChannels;  //通道数，单通道=1，双通道=2
    unsigned long  SampleRate;//采用频率
    unsigned long  ByteRate;/* == SampleRate * NumChannels * BitsPerSample / 8 */
    unsigned short BlockAlign;//== NumChannels * BitsPerSample / 8
    unsigned short BitsPerSample;//每个采样点的bit数，8bits=8, 16bits=16
}WAVE_FMT;

typedef struct WAVE_DATA{
    char    Subchunk2ID[4];       //内容为"data"
    unsigned long Subchunk2Size;
}WAVE_DATA;

/**
 * Convert PCM16LE raw data to WAVE format
 * @param pcmpath      Input PCM file.
 * @param channels     PCM 文件的声道.
 * @param sample_rate  PCM 文件的采样率.
 * @param bits_per_sample  PCM 文件的采样位数.

 */
int simplest_pcm16le_to_wave(const char *pcmpath,int channels,int sample_rate ,short bits_per_sample)
{

    if(channels==0||sample_rate==0){
        channels = 2;
        sample_rate = 44100;
    }
    int bits = 16;

    WAVE_HEADER   pcmHEADER;
    WAVE_FMT   pcmFMT;
    WAVE_DATA   pcmDATA;

    unsigned   short   m_pcmData;
    FILE   *fp,*fpout;

    fp=fopen(pcmpath, "rb");  //打开pcm文件
    if(fp == NULL) {
        printf("open pcm file error\n");
        return -1;
    }
    fpout=fopen("output_nocturne.wav",   "wb+");
    if(fpout == NULL) {
        printf("create wav file error\n");
        return -1;
    }

    fseek(fp,0,SEEK_END);
    int pcm_data_size = ftell(fp);
    int header_size = sizeof(WAVE_HEADER) + sizeof(WAVE_FMT) + sizeof(WAVE_DATA);
    rewind(fp);

    // 对RIFF区块 赋值
    memcpy(pcmHEADER.ChunkID,"RIFF",strlen("RIFF"));
    pcmHEADER.ChunkSize = header_size + pcm_data_size - 8;
    memcpy(pcmHEADER.Format,"WAVE",strlen("WAVE"));

    // 对fmt区块 赋值
    memcpy(pcmFMT.Subchunk1ID,"fmt ",strlen("fmt "));
    pcmFMT.Subchunk1Size=16;
    pcmFMT.AudioFormat=1;
    pcmFMT.NumChannels=channels;
    pcmFMT.SampleRate=sample_rate;
    pcmFMT.ByteRate=channels;
    pcmFMT.ByteRate = sample_rate * channels * bits_per_sample / 8 ;
    pcmFMT.BlockAlign = channels*bits_per_sample/8;
    pcmFMT.BitsPerSample = bits_per_sample;

    // 对data区块 赋值
    memcpy(pcmDATA.Subchunk2ID,"data",strlen("data"));
    pcmDATA.Subchunk2Size = pcm_data_size;

    //写入头文件
    fwrite((unsigned char*)&pcmHEADER ,sizeof(WAVE_HEADER),1,fpout);
    fseek(fp,sizeof(WAVE_HEADER),SEEK_SET);

    fwrite((unsigned char*)&pcmFMT ,sizeof(WAVE_FMT),1,fpout);
    fseek(fp,sizeof(WAVE_HEADER) + sizeof(WAVE_FMT),SEEK_SET);

    fwrite((unsigned char*)&pcmDATA ,sizeof(WAVE_DATA),1,fpout);
    fseek(fp,sizeof(WAVE_HEADER) + sizeof(WAVE_FMT) + sizeof(WAVE_DATA) ,SEEK_SET);

    //写入pcm文件
    unsigned char *pcm_buffer= new unsigned char[pcm_data_size];
    fread(pcm_buffer,1,pcm_data_size,fp);
    fwrite(pcm_buffer ,pcm_data_size,1,fpout);

    fclose(fp);
    fclose(fpout);
    return 0;
}

int main(){
    simplest_pcm16le_to_wave("NocturneNo2inEflat_44.1k_s16le.pcm",2,44100 , 16);
}
