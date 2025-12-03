#include <stdint.h>     // fixed integer type should be uesed.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#pragma pack(push, 1)

/*
  Kim Kyung Jin 21900066
*/


// this is for the BMP file Header
// 데이터를 처리 하기 위한 데이터 구조체가 필요하다. 
// 3개의 구조체를 정의해야 한다. 
// 파일 해더이다. BMP 파일에 대하여서 정보를 주는 것
typedef struct __attribute__((packed)) {
    uint16_t bfType;      // 2 bytes: 파일 식별자 (0x4D42, 'BM')
    uint32_t bfSize;      // 4 bytes: 파일 전체 크기
    uint16_t bfReserved1; // 2 bytes: 예약 영역 (0)
    uint16_t bfReserved2; // 2 bytes: 예약 영역 (0)
    uint32_t bfOffBits;   // 4 bytes: 픽셀 데이터 시작 오프셋
} BITMAPFILEHEADER;


//BMP 파일이 담고 있는 정보들을 제공하기 위한
typedef struct __attribute__((packed)) {
    uint32_t biSize;          // 4 bytes: 현재 구조체 크기 (40)
    int32_t  biWidth;         // 4 bytes: 이미지 가로 크기 (픽셀)
    int32_t  biHeight;        // 4 bytes: 이미지 세로 크기 (픽셀)
    uint16_t biPlanes;        // 2 bytes: 평면 수 (항상 1)
    uint16_t biBitCount;      // 2 bytes: 픽셀당 비트 수 (예: 24)
    uint32_t biCompression;   // 4 bytes: 압축 방식 (0=압축 안 함)
    uint32_t biSizeImage;     // 4 bytes: 픽셀 데이터 크기
    int32_t  biXPelsPerMeter; // 4 bytes
    int32_t  biYPelsPerMeter; // 4 bytes
    uint32_t biClrUsed;       // 4 bytes
    uint32_t biClrImportant;  // 4 bytes
} BITMAPINFOHEADER;

#pragma pack(pop)

// -----------------------------------------------------------
// 3. 픽셀 구조체 (PIXEL, 3 bytes)
// -----------------------------------------------------------

// 픽셀 데이터는 BGR 순서로 저장된다. 
// 그레이스케일로 변환한다는 것은 픽셀의 RGB 값을 단일로 바꾼다는 것이다. 
typedef struct __attribute__((packed)) {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} PIXEL;

// BMP 파일의 해더 내용을 출력 하기 위한 함수
int read_header(FILE *fp, BITMAPFILEHEADER *file_header, BITMAPINFOHEADER *file_info_header){
    
    printf("=======================================\n");
    printf("               BMP FILE HEAD\n");
    printf("=======================================\n");

    int num;

    // BITMAPFILEHEADER header;
    num = fread(file_header, sizeof(BITMAPFILEHEADER), 1, fp);

    if(num != 1){
        perror("FILE read failed\n");
        return 1;
    }

    printf("This is the BMP header\n");
    printf("%hu\n", file_header->bfType);
    printf("%u\n", file_header->bfSize);
    printf("%hu\n", file_header->bfReserved1);
    printf("%hu\n", file_header->bfReserved2);
    printf("%u\n", file_header->bfOffBits);

    num = fread(file_info_header, sizeof(BITMAPINFOHEADER), 1, fp);

    if(num != 1){
        perror("FILE read failed\n");
        return 1;
    }
    printf("%u\n", file_info_header->biSize);
    printf("%u\n", file_info_header->biWidth);
    printf("%u\n", file_info_header->biHeight);
    printf("%u\n", file_info_header->biPlanes);
    printf("%hu\n", file_info_header->biBitCount);
    printf("%hu\n", file_info_header->biCompression);
    printf("%u\n", file_info_header->biSizeImage);
    printf("%u\n", file_info_header->biXPelsPerMeter);
    printf("%u\n", file_info_header->biYPelsPerMeter);
    printf("%u\n", file_info_header->biClrUsed);
    printf("%u\n", file_info_header->biClrImportant);

    return 0; 
}

int read_body(FILE *fp, BITMAPFILEHEADER *file_header){

    printf("=======================================\n");
    printf("               BMP FILE BODY\n");
    printf("=======================================\n");
    unsigned char buff[8];
    int distance;
    int count = 0;
    uint8_t read_data;

    // bfOffBits은 파일해더 전체크기와 동일한 의미를 갖는다
    // 따라서 그 만큼 컨너뛰면 다음부터는 파일의 바디가 나온다. 

    uint32_t size = file_header->bfOffBits;
    distance = sizeof(size);

    // 바디를 출력하기 위해서
    // 파일 포인터를 바디 부분의 시작으로 옮긴다. 
    if((fseek(fp, distance, SEEK_SET)) == 1){ 
        perror("Failed to movele pointer\n");
        fclose(fp);
        return 1;
    }


    while(1){
    
        // 한번 읽어 오는 크기가 8바이트
        read_data = fread(buff, 8 , 1, fp);

        if(read_data == 0){
            break;
        }

        //16진수 형태로 주소 출력 필요
        
        printf("%08x ", count);
        count = count + 8;

        // 16진수로 출력해야한다. 
        for(int i = 0; i < 8; i++){ 
            printf("%02x ", buff[i]);
        }
        
        printf("\n");
    }

    return 0;
}


// output BMP file을 만들어야 한다. 
// 거기에 전체 픽셀값을 변형시켜서
// 그레이스케일로 넣어야 한다. 
// 일단은 그레이스케일로 넣어야 한다.
int gray_scale(FILE *fp, BITMAPFILEHEADER *file_header, BITMAPINFOHEADER *file_info_header, const char *filename){

    // 명령어 실행을 알아 볼 수 있도록
    printf("=======================================\n");
    printf("               GRAY SCALE\n");
    printf("=======================================\n");

    FILE *gray_fp;
    int num = 0;

    // 메모리에 넣을 데이터 형식이 필요하다. 
    // 픽셀에 관한 데이터는 정해져 있지 않다.
    // 따라서 동적 메모리 할당이 중요하다. 
    size_t bit_size = file_info_header->biSizeImage;
    unsigned char pix_buff[3];
    unsigned char zero_byte = 0; // 패딩용 바이트
    int pixels_per_row = file_info_header->biWidth; // 한 줄의 픽셀 수

    int distance;
    const int bytes_per_pixel = 3;

    long row_size = file_info_header->biWidth * bytes_per_pixel;

    int padding = (4 - (row_size % 4)) % 4;

    // 원본 파일의 해더와 관련된 것들을 모두 같은 형식으로 붙여 쓰기한다. 
    // 일단 파일을 하나 만들어야 한다. 
    const char *gray_filename = filename;

    // 새로운 파일을 하나 만들어야 한다. 
    // 바이너리 데이터를 써야한다. 
    gray_fp = fopen(gray_filename, "wb");
    
    // 오류처리
    if(gray_fp == NULL){
        perror("File open failed\n");
        return 1;
    }
    
    // BITMAPFILEHEADER에서 정해진 구조체 방식으로 새로운 파일에 넣는다
    // fread를 사용하여서 넣는다.
    // fwrite를 통하여서 써 넣는다. 
    num = num + fread(file_header, sizeof(BITMAPFILEHEADER), 1, fp);
    num = num + fwrite(file_header, sizeof(BITMAPFILEHEADER), 1, gray_fp);
    
    num = num + fread(file_info_header, sizeof(BITMAPINFOHEADER), 1, fp);
    num = num + fwrite(file_info_header, sizeof(BITMAPINFOHEADER), 1, gray_fp);

    // 에러처리
    if(num != 4){
        perror("FILE read and write FAILED\n");
        return 1;
    }

    // 다 복사했으면 다음에는 픽셀 데이터들을 읽어 와야한다. 
    // fseek를 통해서 픽셀 데이터가 시작되는 포인터로 방향을 바꾼다. 
    // 반복문을 통하여서 데이터를 붙어 넣어야 한다. 
    // 전체 픽셀 수를 기반으로 하는 반복문을 작성한다. 
    // 그리고 그부분부터 데이터를 계속해서 붙여 넣어야 한다. 
  
    uint32_t size = file_header->bfOffBits;

    // 사이즈만큼 파일 포인터를 이동시킨다. 
    distance = size;
    fseek(fp, distance, SEEK_SET);
    fseek(gray_fp, distance, SEEK_SET);

    // 바디 데이터의 수를 알아야한다. 
    long pixnum = file_info_header->biWidth * file_info_header->biHeight;

    // 픽셀데이터를 읽고 변환하고 쓰는 과정을
    // 반복해 나간다. 
    // 스트리밍 방법으로할 것이기 때문에 동적 할당은 안해도 된다. 
    for(long i = 0; i < pixnum; i++){

        // fread는 저절로 파일 포인터의 위치를 바꾸어 준다.
        if((fread(pix_buff, 3, 1, fp)) != 1){
                perror("Read data failed\n");
                fclose(fp);
                fclose(gray_fp);
                return 1;
        }

        // 그레이 스케일로 바꾸어야 한다. 
        unsigned char B = pix_buff[0];
        unsigned char G = pix_buff[1];
        unsigned char R = pix_buff[2];

        unsigned char Y = (unsigned char)(0.299 * R + 0.587 * G + 0.114 * B);

        pix_buff[0] = Y; // Blue
        pix_buff[1] = Y; // Green
        pix_buff[2] = Y; // Red


        if ((i + 1) % pixels_per_row == 0) {

                for (int p = 0; p < padding; p++) {
                    fwrite(&zero_byte, sizeof(unsigned char), 1, gray_fp);
               }
        }

        fwrite(pix_buff, 3, 1, gray_fp);
    }
    
    printf("GRAY SCALE COMPLETED!\n");
    fclose(gray_fp);
    return 0;
}

// BMP 파일을 읽어서 바디 내용을 16진수 값으로
// outfile에텍스트로 출력하기 위한
// 한 줄에 8바이트씩 출력하고
// 각 줄 앞에는 시작 바이트의 상대 주소르 16진수로 표현한다. 
int main(int argc, char *argv[]){

    // 파일 포인터
    FILE *fp;

    // 명령어에서 받는 파일 이름
    const char *filename =(const char *)argv[2];

        // 바이너리도 읽을 수 있도록
    fp = fopen(filename, "rb");

    // 커맨드가 너무 짧다면
    if(argc < 3){
        perror("Command too shordt\n");
        return 1;
    }

    // 파일 잘 못읽어오면
    if(fp == NULL){
        perror("Failed to read file\n");
        return 1;   
    }

    // 해더파일데이터를 저장하기 위한 구조체 선언하기
    BITMAPFILEHEADER file_header; 
    BITMAPINFOHEADER file_info_header;

    // -h 명령어
    // 올바르게 파일 이름을 입력했다면 0을 반환한다. 
    if(strcmp((const char *)argv[1], "-h") == 0){
       read_header(fp, &file_header, &file_info_header); 
    }
   
    // -o 명령어 시행시
    else if(strcmp((const char *)argv[1], "-o") == 0){
       read_body(fp, &file_header);
    }
    
    else if(strcmp(argv[1], "-g") == 0){
        gray_scale(fp, &file_header, &file_info_header, argv[3]);
    }
   
    // -d 명령어 시행시
//    else if(strcmp(argv[1], '-d') == 0){

//    }

//    else {

//        perror("Wrong Cammand\n);
//        return -1
//    }


    fclose(fp);
    return 0;
}
