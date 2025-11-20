#include <stdint.h>     // fixed integer type should be uesed.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// -----------------------------------------------------------
// 3. 픽셀 구조체 (PIXEL, 3 bytes)
// -----------------------------------------------------------

// 픽셀 데이터는 BGR 순서로 저장됩니다.
typedef struct __attribute__((packed)) {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} PIXEL;




// BMP 파일의 해더 내용을 출력 하기 위한 함수
int read_header(*fp){
    
    int result;
    int num;

    BITMAPFILEHEADER header;
    num = fread(&header, sizeof(BITMAPFILEHEADER), 1, fp);

    if(num != 1){
        perror("FILE read failed\n");
        return -1;
    }

    
}


// BMP 파일을 읽어서 바디 내용을 16진수 값으로
// outfile에텍스트로 출력하기 위한
// 한 줄에 8바이트씩 출력하고
// 각 줄 앞에는 시작 바이트의 상대 주소르 16진수로 표현한다. 
int read_body(){
}




int main(int argc, **char argv[]){

    FILE *fp;
    const char *filename = argv[2];


    // 바이너리를 읽기 위한
    fp = fopen(filename, "rb");
    
    // argc
    if(argc < 3){
        perror("Command too short\n");
        return -1;
    } 
    

    // -h 명령어 시행시
    else if(strcmp(filename, '-h') == 0){

       // 파일 헤더 함수 호출
       // 만약에 호출 실패시
       if(read_header(fp) == 1){
           perror("-h read header failed\n");
           return -1;
       }
    
       if(read_body(&filename) == 1){
           perror("-h read body failed\n");
           return -1
       }

    }


    
       


    // -o 명령어 시행시
//    else if(strcmp(argv[1], '-o') == 0){
        
//    }


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

    return 0;
}
