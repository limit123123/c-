#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<graphics.h>
#pragma comment(lib,"winmm.lib")//引入音乐库文件
#include<mmsystem.h>
//图片宽度
#define SIZE 50
#define ROW 10//行
#define COL 10//列
#define NUM 10//雷的个数
int count;//点开的数量
IMAGE img[12];//12张图片
int map[ROW + 2][COL + 2];//需要辅助区域，用来计算9宫格中雷的数量



void gameinit() {
    //随机数的种子
    srand((unsigned int)time(NULL));

    for (int i = 0; i < ROW+2; i++)
    {
        for (int j = 0; j < COL+2; j++)
        {
            /* code */
            map[i][j] = 0;
        }
    }
    //布雷 -1表示雷 NUM表示雷数量
    int n = 0;
    while (n < NUM)
    {
        int r = rand() % ROW;
        int c = rand() % COL;
        if (map[r][c] == 0)//如果前一个位置不是雷，那么使他为雷。
        {
            map[r][c] = -1;
            n++;
        }
    }//while 
    //根据雷的分布，填充不为雷的区域的数据
    for (int i = 1; i <= ROW + 1; i++)
    {
        for (int j = 1; j < COL + 1; j++)
        {
            if (map[i][j] != -1)
            {
                for (int m = i - 1; m <= i + 1; m++)
                {
                    for (int n = j - 1; n <= j + 1; n++)
                    {
                        if (map[m][n] == -1)
                        {
                            map[i][j]++;
                            n++;
                            /* code */
                        }
                    }

                }
            }
        }
    }
    //先进行加密
    for (int i = 1;i < ROW + 1;i++) {
        for (int j = 1;j < COL + 1;j++) {
            map[i][j] += 20;
        }
    }


}

void gamedraw() {
    //打印游戏区，而不是辅助区域
    for (int i = 1; i <= ROW; i++)
    {
        for (int j = 1; j <= COL; j++)
        {
            /* code */
            printf("%3d", map[i][j]);
            if(map[i][j] == -1){
                putimage((i - 1) * SIZE, (j - 1) * SIZE, &img[9]);//雷
            }
            else if (map[i][j] >= 0 && map[i][j] <= 8) {
                putimage((i - 1) * SIZE, (j - 1) * SIZE, &img[map[i][j]]);//数字
            }
            else if (map[i][j] >= 19 && map[i][j] <= 28) {
                putimage((i - 1) * SIZE, (j - 1) * SIZE, &img[10]);//空白图片
            }
            else if (map[i][j] >30) {
                putimage((i - 1) * SIZE, (j - 1) * SIZE, &img[11]);//标记
            }
        }
        printf("\n");
    }
}
void openzero(int r,int c) {//打开0函数
    //先打开这个0
    map[r][c]-=20;
    count++;
    //游戏区
    for (int m = r - 1;m<= r + 1;m++) {
        for (int n = c - 1;n<= c + 1;n++) {
            if (m >= 1 && m <= ROW && n >= 1 && n <= COL) {
                if (map[m][n] >= 19 && map[m][n] <= 28) {
                    if (map[m][n] != 20) {
                        map[m][n]-=20;
                        count++;
                    }//if
                    else {
                        openzero(m, n);
                    }
                }//if 
            }
        }
    }
}

int  Playgame() {
    //定义一个鼠标消息
    MOUSEMSG msg = { 0 };
    int r, c;
    while (1) {
        msg = GetMouseMsg();
        switch (msg.uMsg)
        {
        case WM_LBUTTONDOWN://鼠标左键 打开扫雷的空白图片
            r = msg.x / SIZE + 1;
            c = msg.y / SIZE + 1;
            if (map[r][c] >= 19 && map[r][c] <= 28) {
                //如果是0，遍历这个九宫格，全部打开
                if (map[r][c] == 20) {
                    openzero(r, c);//调用该函数
                }
                else {
                    map[r][c] -= 20;
                    count++;
                }
             
            }
            return map[r][c];
            break;
        case WM_RBUTTONDOWN://鼠标右键 :标记空白图片 取消标记图片
            r = msg.x / SIZE + 1;
            c = msg.y / SIZE + 1;
            if (map[r][c] >= 19 && map[r][c] <= 28) {
                map[r][c] += 50;
            }
            else  if (map[r][c] > 30) {//取消标记
                map[r][c] -= 50;
            }
            return map[r][c];
            break;
        }
    }
}
int main() {
   HWND hwnd=initgraph(SIZE * ROW, SIZE* COL, SHOWCONSOLE);
  //  mciSendString(L"open music.mp3 alias music",0,0,0);
   // mciSendString(L"play music", 0, 0, 0);
    mciSendString(TEXT("open music.mp3 alias s1"), NULL, 0, NULL);
    mciSendString(TEXT("play s1"), NULL, 0, NULL);
 
    
    loadimage(&img[0], L"0.jpg", SIZE, SIZE);
    loadimage(&img[1], L"1.jpg", SIZE, SIZE);
    loadimage(&img[2], L"2.jpg", SIZE, SIZE);
    loadimage(&img[3], L"3.jpg", SIZE, SIZE);
    loadimage(&img[4], L"4.jpg", SIZE, SIZE);
    loadimage(&img[5], L"5.jpg", SIZE, SIZE);
    loadimage(&img[6], L"6.jpg", SIZE, SIZE);
    loadimage(&img[7], L"7.jpg", SIZE, SIZE);
    loadimage(&img[8], L"8.jpg", SIZE, SIZE);
    loadimage(&img[9], L"9.jpg", SIZE, SIZE);//雷
    loadimage(&img[10], L"10.jpg", SIZE, SIZE);//空白
    loadimage(&img[11], L"11.jpg", SIZE, SIZE);//标记
    //之前取值范围为-1到8，加密（加20），得到19到28 为空白的对应
    gameinit();


    while (1) {
        gamedraw();
        //输的时候
        if (Playgame() == -1) {
            gamedraw();
            MessageBox(hwnd, L"you are lost", L"", MB_OK);  
            break;
        }
      //赢
        if (ROW*COL-NUM==count) {
            gamedraw();
            MessageBox(hwnd, L"you are win!", L"", MB_OK);
            break;
        }

    }
    

    return 0;


}
