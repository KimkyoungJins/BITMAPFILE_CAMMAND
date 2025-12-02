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

// 픽셀 데이터는 BGR 순서로 저장됩니다.
typedef struct __attribute__((packed)) {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} PIXEL;

//int read_body(){
  
//    return 0;
//}


// BMP 파일의 해더 내용을 출력 하기 위한 함수
int read_header(FILE *fp, BITMAPFILEHEADER *file_header, BITMAPINFOHEADER *file_info_header){

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
    int count2 = 0;
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

/*
        if(count2 == 5){
            printf("\n");
            count2 = 0;
        }

        */

        // 16진수로 출력해야한다. 
        for(int i = 0; i < 8; i++){ 
            printf("%02x ", buff[i]);
        }
        
        printf("\n");
    }

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


    // -e 명령어 시행시
//    else if(strcmp(argv[1], '-e') == 0){

//    }
    
    // -d 명령어 시행시
//    else if(strcmp(argv[1], '-d') == 0){

//    }

//    else {

//        perror("Wrong Cammand\n");
//        return -1
//    }


    fclose(fp);
    return 0;
}
