# 최종 실행 파일의 이름을 정의합니다.
TARGET = hw2BMP

# 컴파일에 사용될 소스 파일을 정의합니다. (.c 파일)
SRCS = main.c # 필요한 .c 파일들을 모두 나열하세요.

# 소스 파일에서 .o (오브젝트 파일) 목록을 자동으로 생성합니다.
OBJS = $(SRCS:.c=.o)

# 사용할 컴파일러 (GCC 권장)
CC = gcc

# 컴파일러 옵션 (경고를 모두 켜고, 최적화 레벨을 줍니다.)
CFLAGS = -Wall -Wextra -std=c99 -g 

# 링크 시 사용할 라이브러리 (필요하다면 추가하세요. 예: -lm)
LDFLAGS = 

.PHONY: all clean

# 1. 'all' 타겟: 최종 목표인 hw2BMP 실행 파일을 생성합니다.
all: $(TARGET)

# 2. 실행 파일 생성 타겟: 오브젝트 파일들을 링크하여 실행 파일을 만듭니다.
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# 3. 오브젝트 파일 생성 규칙: 모든 .c 파일을 .o 파일로 컴파일합니다.
# $< : 현재 타겟의 의존 파일 목록 중 첫 번째 파일 (여기서는 .c 파일)
# $@ : 현재 타겟의 이름 (여기서는 .o 파일)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 4. 'clean' 타겟: 생성된 오브젝트 파일과 실행 파일을 삭제합니다.
clean:
	rm -f $(OBJS) $(TARGET)
