#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

#include "lcd.h"
#include "touch.h"
#define LCD_TEAM_NAME_X 40
#define LCD_TEAM_NAME_Y 20
#define LCD_ON_X 40
#define LCD_ON_Y 70
#define LCD_REC_X 40
#define LCD_REC_Y 80
#define LCD_REC_LEN 40
#define LCD_BUT_X 70
#define LCD_BUT_Y 120
#define LCD_COUNT_X 40
#define LCD_COUNT_Y 240

void Delay(void);

int btn[4] = {0,0,0,0};
int pwd[4] = {1,2,3,4};
extern int select;
int count = 0;
uint16_t cur_x,cur_y,pixel_x,pixel_y;

void Delay(void) {
    int i;
    for (i = 0; i < 200000; i++);
}

void resetPassword() {
  int i;
  for (i = 0; i < 4; i++) {
    btn[i] = 0;
  }
}

void chkPassword() {
  int i;
  select = 2;
  for (i = 0; i < 4; i++) {
    if(btn[i] !=  pwd[i]) {
      select = 0;
      break;
    }
  }
}

void touch_pad() {
    LCD_DrawRectangle(LCD_REC_X, LCD_REC_Y, LCD_REC_X + LCD_REC_LEN, LCD_REC_Y + LCD_REC_LEN);//1
    LCD_ShowString(LCD_REC_X + 16, LCD_REC_Y + 11, "1", BLACK, WHITE);
    LCD_DrawRectangle(LCD_REC_X + 60, LCD_REC_Y, LCD_REC_X + 60 + LCD_REC_LEN, LCD_REC_Y + LCD_REC_LEN);//2
    LCD_ShowString(LCD_REC_X + 76, LCD_REC_Y + 11, "2", BLACK, WHITE);
    LCD_DrawRectangle(LCD_REC_X + 120, LCD_REC_Y, LCD_REC_X + 120 + LCD_REC_LEN, LCD_REC_Y + LCD_REC_LEN);//3
    LCD_ShowString(LCD_REC_X + 136, LCD_REC_Y + 11, "3", BLACK, WHITE);
    
    LCD_DrawRectangle(LCD_REC_X, LCD_REC_Y + 60, LCD_REC_X + LCD_REC_LEN, LCD_REC_Y + 60 + LCD_REC_LEN);//4
    LCD_ShowString(LCD_REC_X + 16, LCD_REC_Y + 71, "4", BLACK, WHITE);
    LCD_DrawRectangle(LCD_REC_X + 60, LCD_REC_Y + 60, LCD_REC_X + 60 + LCD_REC_LEN, LCD_REC_Y + 60 + LCD_REC_LEN);//5
    LCD_ShowString(LCD_REC_X + 76, LCD_REC_Y + 71, "5", BLACK, WHITE);
    LCD_DrawRectangle(LCD_REC_X + 120, LCD_REC_Y + 60, LCD_REC_X + 120 + LCD_REC_LEN, LCD_REC_Y + 60 + LCD_REC_LEN);//6
    LCD_ShowString(LCD_REC_X + 136, LCD_REC_Y + 71, "6", BLACK, WHITE);
    
    LCD_DrawRectangle(LCD_REC_X, LCD_REC_Y + 120, LCD_REC_X + LCD_REC_LEN, LCD_REC_Y + 120 + LCD_REC_LEN);//7
    LCD_ShowString(LCD_REC_X + 16, LCD_REC_Y + 131, "7", BLACK, WHITE);
    LCD_DrawRectangle(LCD_REC_X + 60, LCD_REC_Y + 120, LCD_REC_X + 60 + LCD_REC_LEN, LCD_REC_Y + 120 + LCD_REC_LEN);//8
    LCD_ShowString(LCD_REC_X + 76, LCD_REC_Y + 131, "8", BLACK, WHITE);
    LCD_DrawRectangle(LCD_REC_X + 120, LCD_REC_Y + 120, LCD_REC_X + 120 + LCD_REC_LEN, LCD_REC_Y + 120 + LCD_REC_LEN);//9
    LCD_ShowString(LCD_REC_X + 136, LCD_REC_Y + 131, "9", BLACK, WHITE);
    
    LCD_DrawRectangle(LCD_REC_X-10, LCD_REC_Y + 180, LCD_REC_X + LCD_REC_LEN+10, LCD_REC_Y + 180 + LCD_REC_LEN); //enter btn
    LCD_ShowString(LCD_REC_X, LCD_REC_Y + 191, "ENTER", BLUE, WHITE);
    
    LCD_DrawRectangle(LCD_REC_X + 110, LCD_REC_Y + 180, LCD_REC_X + 130 + LCD_REC_LEN, LCD_REC_Y + 180 + LCD_REC_LEN); //exit btn
    LCD_ShowString(LCD_REC_X + 125, LCD_REC_Y + 191, "EXIT", RED, WHITE);
    

    while (1) {      
        Touch_GetXY(&cur_x, &cur_y, 1);
        Convert_Pos(cur_x, cur_y, &pixel_x, &pixel_y);
        
        
        if(pixel_x > LCD_REC_X && pixel_x < (LCD_REC_X + LCD_REC_LEN)){ //1
            if(pixel_y > LCD_REC_Y && pixel_y < LCD_REC_Y+LCD_REC_LEN){
                LCD_Fill(LCD_REC_X, LCD_REC_Y, LCD_REC_X + LCD_REC_LEN, LCD_REC_Y + LCD_REC_LEN, RED);
                LCD_ShowString(LCD_REC_X + 16, LCD_REC_Y + 11, "1", WHITE, RED);
                Delay();
                LCD_Fill(LCD_REC_X, LCD_REC_Y, LCD_REC_X + LCD_REC_LEN, LCD_REC_Y + LCD_REC_LEN, WHITE);
                LCD_DrawRectangle(LCD_REC_X, LCD_REC_Y, LCD_REC_X + LCD_REC_LEN, LCD_REC_Y + LCD_REC_LEN);
                LCD_ShowString(LCD_REC_X + 16, LCD_REC_Y + 11, "1", BLACK, WHITE);
                count = count % 4;
                btn[count] = 1;
                count++;
            }
        }

        if(pixel_x > (LCD_REC_X + 60) && pixel_x < (LCD_REC_X + 60 + LCD_REC_LEN)){ //2
            if(pixel_y > LCD_REC_Y && pixel_y < LCD_REC_Y+LCD_REC_LEN){
                LCD_Fill(LCD_REC_X + 60, LCD_REC_Y, LCD_REC_X + 60 + LCD_REC_LEN, LCD_REC_Y + LCD_REC_LEN, RED);
                LCD_ShowString(LCD_REC_X + 76, LCD_REC_Y + 11, "2", WHITE, RED);
                Delay();
                LCD_Fill(LCD_REC_X+60, LCD_REC_Y, LCD_REC_X + 60 + LCD_REC_LEN, LCD_REC_Y + LCD_REC_LEN, WHITE);
                LCD_DrawRectangle(LCD_REC_X+60, LCD_REC_Y, LCD_REC_X + 60 +LCD_REC_LEN, LCD_REC_Y + LCD_REC_LEN);
                LCD_ShowString(LCD_REC_X + 76, LCD_REC_Y + 11, "2", BLACK, WHITE);
                count = count % 4;
                btn[count] = 2;
                count++;
            }
        }

        if(pixel_x > (LCD_REC_X + 120) && pixel_x < (LCD_REC_X + 120 + LCD_REC_LEN)){ //3
            if(pixel_y > LCD_REC_Y && pixel_y < LCD_REC_Y+LCD_REC_LEN){
                LCD_Fill(LCD_REC_X + 120, LCD_REC_Y, LCD_REC_X + 120 + LCD_REC_LEN, LCD_REC_Y + LCD_REC_LEN, RED);
                LCD_ShowString(LCD_REC_X + 136, LCD_REC_Y + 11, "2", WHITE, RED);
                Delay();
                LCD_Fill(LCD_REC_X+120, LCD_REC_Y, LCD_REC_X + 120 + LCD_REC_LEN, LCD_REC_Y + LCD_REC_LEN, WHITE);
                LCD_DrawRectangle(LCD_REC_X+120, LCD_REC_Y, LCD_REC_X + 120 +LCD_REC_LEN, LCD_REC_Y + LCD_REC_LEN);
                LCD_ShowString(LCD_REC_X + 136, LCD_REC_Y + 11, "3", BLACK, WHITE);
                count = count % 4;
                btn[count] = 3;
                count++;
            }
        }

        if(pixel_x > LCD_REC_X && pixel_x < LCD_REC_X+LCD_REC_LEN){ //4
            if(pixel_y > (LCD_REC_Y + 60) && pixel_y < (LCD_REC_Y + 60 + LCD_REC_LEN)){
                LCD_Fill(LCD_REC_X, LCD_REC_Y+60, LCD_REC_X + LCD_REC_LEN, LCD_REC_Y + 60 + LCD_REC_LEN, RED);
                LCD_ShowString(LCD_REC_X + 16, LCD_REC_Y + 71, "4", WHITE, RED);
                Delay();
                LCD_Fill(LCD_REC_X, LCD_REC_Y + 60, LCD_REC_X + LCD_REC_LEN, LCD_REC_Y + 60 + LCD_REC_LEN, WHITE);
                LCD_DrawRectangle(LCD_REC_X, LCD_REC_Y + 60, LCD_REC_X + LCD_REC_LEN, LCD_REC_Y + 60 + LCD_REC_LEN);
                LCD_ShowString(LCD_REC_X + 16, LCD_REC_Y + 71, "4", BLACK, WHITE);
                count = count % 4;
                btn[count] = 4;
                count++;
            }
        }

        if(pixel_x > (LCD_REC_X  + 60) && pixel_x < (LCD_REC_X + 60 + LCD_REC_LEN)){ //5
            if(pixel_y > (LCD_REC_Y + 60) && pixel_y < (LCD_REC_Y + 60 + LCD_REC_LEN)){
                LCD_Fill(LCD_REC_X + 60, LCD_REC_Y+60, LCD_REC_X + 60 + LCD_REC_LEN, LCD_REC_Y + 60 + LCD_REC_LEN, RED);
                LCD_ShowString(LCD_REC_X + 76, LCD_REC_Y + 71, "5", WHITE, RED);
                Delay();
                LCD_Fill(LCD_REC_X + 60, LCD_REC_Y + 60, LCD_REC_X + 60 + LCD_REC_LEN, LCD_REC_Y + 60 + LCD_REC_LEN, WHITE);
                LCD_DrawRectangle(LCD_REC_X + 60, LCD_REC_Y + 60, LCD_REC_X + 60 + LCD_REC_LEN, LCD_REC_Y + 60 + LCD_REC_LEN);
                LCD_ShowString(LCD_REC_X + 76, LCD_REC_Y + 71, "5", BLACK, WHITE);
                count = count % 4;
                btn[count] = 5;
                count++;
            }
        }

        if(pixel_x > (LCD_REC_X + 120) && pixel_x < (LCD_REC_X + 120 + LCD_REC_LEN)){ //6
            if(pixel_y > (LCD_REC_Y + 60) && pixel_y < (LCD_REC_Y + 60 + LCD_REC_LEN)){
                LCD_Fill(LCD_REC_X + 120, LCD_REC_Y + 60, LCD_REC_X + 120 + LCD_REC_LEN, LCD_REC_Y + 60 + LCD_REC_LEN, RED);
                LCD_ShowString(LCD_REC_X + 136, LCD_REC_Y + 71, "6", WHITE, RED);
                Delay();
                LCD_Fill(LCD_REC_X + 120, LCD_REC_Y + 60, LCD_REC_X + 120 + LCD_REC_LEN, LCD_REC_Y + 60 + LCD_REC_LEN, WHITE);
                LCD_DrawRectangle(LCD_REC_X + 120, LCD_REC_Y + 60, LCD_REC_X + 120 + LCD_REC_LEN, LCD_REC_Y + 60 + LCD_REC_LEN);
                LCD_ShowString(LCD_REC_X + 136, LCD_REC_Y + 71, "6", BLACK, WHITE);
                count = count % 4;
                btn[count] = 6;
                count++;
            }
        }

        if(pixel_x > LCD_REC_X && pixel_x < LCD_REC_X+LCD_REC_LEN){ //7
            if(pixel_y > (LCD_REC_Y + 120) && pixel_y < (LCD_REC_Y + 120 + LCD_REC_LEN)){
                LCD_Fill(LCD_REC_X, LCD_REC_Y + 120, LCD_REC_X + LCD_REC_LEN, LCD_REC_Y + 120 + LCD_REC_LEN, RED);
                LCD_ShowString(LCD_REC_X + 16, LCD_REC_Y + 131, "7", WHITE, RED);
                Delay();
                LCD_Fill(LCD_REC_X, LCD_REC_Y + 120, LCD_REC_X + LCD_REC_LEN, LCD_REC_Y + 120 + LCD_REC_LEN, WHITE);
                LCD_DrawRectangle(LCD_REC_X, LCD_REC_Y + 120, LCD_REC_X + LCD_REC_LEN, LCD_REC_Y + 120 + LCD_REC_LEN);
                LCD_ShowString(LCD_REC_X + 16, LCD_REC_Y + 131, "7", BLACK, WHITE);
                count = count % 4;
                btn[count] = 7;
                count++;
            }
        }

        if(pixel_x > (LCD_REC_X + 60) && pixel_x < (LCD_REC_X + 60 + LCD_REC_LEN)){ //8
            if(pixel_y > (LCD_REC_Y + 120) && pixel_y < (LCD_REC_Y + 120 + LCD_REC_LEN)){
                LCD_Fill(LCD_REC_X + 60, LCD_REC_Y + 120, LCD_REC_X + 60 + LCD_REC_LEN, LCD_REC_Y + 120 + LCD_REC_LEN, RED);
                LCD_ShowString(LCD_REC_X + 76, LCD_REC_Y + 131, "8", WHITE, RED);
                Delay();
                LCD_Fill(LCD_REC_X + 60, LCD_REC_Y + 120, LCD_REC_X + 60 + LCD_REC_LEN, LCD_REC_Y + 120 + LCD_REC_LEN, WHITE);
                LCD_DrawRectangle(LCD_REC_X + 60, LCD_REC_Y + 120, LCD_REC_X + 60 + LCD_REC_LEN, LCD_REC_Y + 120 + LCD_REC_LEN);
                LCD_ShowString(LCD_REC_X + 76, LCD_REC_Y + 131, "8", BLACK, WHITE);
                count = count % 4;
                btn[count] = 8;
                count++;
            }
        }
        if(pixel_x > (LCD_REC_X + 120) && pixel_x < (LCD_REC_X + 120 + LCD_REC_LEN)){ //9
            if(pixel_y > (LCD_REC_Y + 120) && pixel_y < (LCD_REC_Y + 120 + LCD_REC_LEN)){
                LCD_Fill(LCD_REC_X + 120, LCD_REC_Y + 120, LCD_REC_X + 120 + LCD_REC_LEN, LCD_REC_Y + 120 + LCD_REC_LEN, RED);
                LCD_ShowString(LCD_REC_X + 136, LCD_REC_Y + 131, "9", WHITE, RED);
                Delay();
                LCD_Fill(LCD_REC_X + 120, LCD_REC_Y + 120, LCD_REC_X + 120 + LCD_REC_LEN, LCD_REC_Y + 120 + LCD_REC_LEN, WHITE);
                LCD_DrawRectangle(LCD_REC_X + 120, LCD_REC_Y + 120, LCD_REC_X + 120 + LCD_REC_LEN, LCD_REC_Y + 120 + LCD_REC_LEN);
                LCD_ShowString(LCD_REC_X + 136, LCD_REC_Y + 131, "9", BLACK, WHITE);
                count = count % 4;
                btn[count] = 9;
                count++;
            }
        }
        if(pixel_x > (LCD_REC_X) && pixel_x < (LCD_REC_X + LCD_REC_LEN)){ // Enter
            if(pixel_y > (LCD_REC_Y + 180) && pixel_y < (LCD_REC_Y + 180 + LCD_REC_LEN)){
                LCD_Fill(LCD_REC_X-10, LCD_REC_Y + 180, LCD_REC_X + LCD_REC_LEN+10, LCD_REC_Y + 180 + LCD_REC_LEN, BLUE); //enter btn
                LCD_ShowString(LCD_REC_X, LCD_REC_Y + 191, "ENTER", WHITE, BLUE);
                Delay();
                LCD_Fill(LCD_REC_X-10, LCD_REC_Y + 180, LCD_REC_X + LCD_REC_LEN+10, LCD_REC_Y + 180 + LCD_REC_LEN, WHITE);
                LCD_DrawRectangle(LCD_REC_X-10, LCD_REC_Y + 180, LCD_REC_X + LCD_REC_LEN+10, LCD_REC_Y + 180 + LCD_REC_LEN); //enter btn
                LCD_ShowString(LCD_REC_X, LCD_REC_Y + 191, "ENTER", BLUE, WHITE);
                chkPassword();
                resetPassword();
                break;
            }
        }
        
        if(pixel_x > (LCD_REC_X + 120) && pixel_x < (LCD_REC_X + 120 + LCD_REC_LEN)){ // Exit
            if(pixel_y > (LCD_REC_Y + 180) && pixel_y < (LCD_REC_Y + 180 + LCD_REC_LEN)){
                LCD_Fill(LCD_REC_X + 110, LCD_REC_Y + 180, LCD_REC_X + 130 + LCD_REC_LEN, LCD_REC_Y + 180 + LCD_REC_LEN, RED); //exit btn
                LCD_ShowString(LCD_REC_X + 125, LCD_REC_Y + 191, "EXIT", WHITE, RED);
                Delay();
                LCD_Fill(LCD_REC_X + 110, LCD_REC_Y + 180, LCD_REC_X + 130 + LCD_REC_LEN, LCD_REC_Y + 180 + LCD_REC_LEN, WHITE);
                LCD_DrawRectangle(LCD_REC_X + 110, LCD_REC_Y + 180, LCD_REC_X + 130 + LCD_REC_LEN, LCD_REC_Y + 180 + LCD_REC_LEN); //exit btn
                LCD_ShowString(LCD_REC_X + 125, LCD_REC_Y + 191, "EXIT", RED, WHITE);
                select = 0;
                resetPassword();
                break;
            }
        }
        for (int i = 0; i < 4; i++){    // 입력숫자 출력
            LCD_ShowNum(LCD_TEAM_NAME_X + 60 + 10*i, LCD_TEAM_NAME_Y+30 , btn[i], 1, RED, WHITE);
        }    
       Delay();
    }
}
