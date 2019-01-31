
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BMP_IMAGE_LOCATION_OFFSET 0x0A
#define BMP_WIDTH_OFFSET 0x12
#define BMP_HEIGHT_OFFSET 0x16
#define BMP_BPP_OFFSET 0x1C
#define BMP_IMAGE_SIZE_OFFSET 0x22

struct
{
    int width;
    int height;
    int bpp;
    ssize_t buflen;
    uint8_t* buf; // contains the raw pixel data (with padding, if any)
} typedef bitmap;

void buf_to_bitmap(uint8_t* buf, ssize_t length, bitmap* bmp)
{
    uint32_t* imageOffset = (uint32_t*)(buf+BMP_IMAGE_LOCATION_OFFSET);
    uint32_t* width = (uint32_t*)(buf+BMP_WIDTH_OFFSET);
    uint32_t* height = (uint32_t*)(buf+BMP_HEIGHT_OFFSET);
    uint16_t* bpp = (uint16_t*)(buf+BMP_BPP_OFFSET);
    bmp->width = *width;
    bmp->height = *height;
    bmp->bpp = *bpp;
    bmp->buf = (buf + *imageOffset);
    bmp->buflen = (length - *imageOffset);
}

void buf_to_mono(bitmap* bmp, uint8_t* buf_mono)
{
    int j = 0;
    // will always be an integer, as bmp's are padded so they always land on an address of multiple of 4
    int bytesPerLine = bmp->buflen / bmp->height;
    printf("buf_to_mono: \n");
    for(int i = 0; i < bmp->buflen; ++i)
    {
        if((iter - bmp->buf) % bmp->width == 0)
        {
            i += (bytesPerLine - bmp->width);
        }
        printf("%02X", bmp->buf[i]);
        buf_mono[j++] = (bmp->buf[i] >> 7) & 0x01;
        buf_mono[j++] = (bmp->buf[i] >> 6) & 0x01;
        buf_mono[j++] = (bmp->buf[i] >> 5) & 0x01;
        buf_mono[j++] = (bmp->buf[i] >> 4) & 0x01;
        buf_mono[j++] = (bmp->buf[i] >> 3) & 0x01;
        buf_mono[j++] = (bmp->buf[i] >> 2) & 0x01;
        buf_mono[j++] = (bmp->buf[i] >> 1) & 0x01;
        buf_mono[j++] = (bmp->buf[i] >> 0) & 0x01;
    }
    printf("\n");
}

int main(void)
{
    char filename[] = "/home/bhudd/workspace/hackerRank/src/bmpToRaw/bmpTest_mono.bmp";
    int bmp_fd = open(filename, O_RDONLY);

    if(!bmp_fd)
    {
        printf("Failed to open file: %s\n", strerror(errno));
        return 1;
    }

    // get file size
    struct stat st;
    stat(filename, &st);
    ssize_t size = st.st_size;

    uint8_t* buf = (uint8_t*)malloc(size);
    bitmap* bmp = (bitmap*)malloc(sizeof(bitmap));


    if(read(bmp_fd, buf, size) == size)
    {
        buf_to_bitmap(buf, size, bmp);
        printf("yay! We did a thing!\n");
        printf("width: %d height: %d bpp: %d size: %d\n", bmp->width, bmp->height, bmp->bpp, bmp->buflen);
        uint8_t* buf_mono = malloc(bmp->width * bmp->height);

        printf("ACTUAL BUFFER: \n");
        for(int i = 0; i < bmp->buflen; ++i)
        {
            if(i % bmp->width == 0)
            {
                printf("\n");
            }
            printf("%02X", buf[i]);
        }
        printf("\n");

        buf_to_mono(bmp, buf_mono);

        printf("MONO BUFFER: \n");
        for(int i = 0; i < (bmp->width * bmp->height); ++i)
        {
            if(i % bmp->width == 0)
            {
                printf("\n");
            }
            printf("%d", buf_mono[i]);
        }
        printf("\n");
    }
    else
    {
        printf("READ FAILED!\n");
        printf("error: %s\n", strerror(errno));
    }
}