// #include "nwy_test_cli_utils.h"
// #include "nwy_pahomqtt_api.h"
// #include "nwy_osi_api.h"
#include "nwy_gpio_api.h"
#include "nwy_test_cli_utils.h"
#include "nwy_test_cli_adpt.h"
#include "Image1.c"

#include <stdint.h>
#include <string.h>

#define DISPLAY_CLK_PIN        57   // GPIO 57 - KEYOUT1 - Clock
#define DISPLAY_DATA_PIN       01   // GPIO 01 - KEYIN4 - Data
// #define DISPLAY_DC_PIN         87   // GPIO 87 - KEYIN1 - Data/Command
#define DISPLAY_DC_PIN         0   // GPIO 0 - KEYOUT5 - Data/Command A0
#define DISPLAY_RESET_PIN      84   // GPIO 84 - KEYOUT4 - Reset

#define COL1      1
#define PAGE1     0xB0
#define PAGE2     0xB1
#define PAGE3     0xB2
#define PAGE4     0xB3
#define PAGE5     0xB4
#define PAGE6     0xB5
#define PAGE7     0xB6
#define PAGE8     0xB7

uint8_t COL;
uint8_t PAGE;

void nwy_test_lcd()
{

    nwy_test_cli_echo("\r\nPrint from LCD.c\r\n");
    nwy_thread_sleep(1000);
    nwy_test_cli_echo("\r\nPrint 2 from LCD.c\r\n");
    lcd_init();
    while(1){
        char *sptr = NULL;
        sptr = nwy_test_cli_input_gets("\r\n Click Enter \r\n");

        lcd_main_test();
        nwy_thread_sleep(1000);
        lcd_clear();
        // nwy_thread_sleep(1000);
        Display(0, PAGE1, 0, "|       ANUSHA      |");
        Display(0, PAGE2, 0, "|            |");
        Display(0, PAGE3, 0, "|      LIMITED      |");
    }

}



void lcd_main_test()
{
    lcd_clear();
    lcd_test_pattern();
    nwy_thread_sleep(1000);
    DispNewImage(image_data_Image);
}

// Delay Function
void delay_us(uint32_t us)
{
    // volatile uint32_t count;
    // for (count = 0; count < us * 1; count++) // Rough delay loop
    // {
    //     // dummy++;
    //     //  __asm__("NOP");

    // }
}


// Set GPIO Directions
void init_display_gpio(void) 
{
    uint8_t dir = 1;
    nwy_gpio_direction_set(DISPLAY_CLK_PIN, (nwy_dir_mode_e)dir);
    nwy_gpio_direction_set(DISPLAY_DATA_PIN, (nwy_dir_mode_e)dir);
    nwy_gpio_direction_set(DISPLAY_DC_PIN, (nwy_dir_mode_e)dir);
    nwy_gpio_direction_set(DISPLAY_RESET_PIN, (nwy_dir_mode_e)dir);

    nwy_gpio_value_set(DISPLAY_CLK_PIN, (nwy_value_e)0);
    nwy_gpio_value_set(DISPLAY_DATA_PIN, (nwy_value_e)0);
    nwy_gpio_value_set(DISPLAY_DC_PIN, (nwy_value_e)0);
    nwy_gpio_value_set(DISPLAY_RESET_PIN, (nwy_value_e)1);

    nwy_gpio_value_set(DISPLAY_RESET_PIN, (nwy_value_e)0);
    nwy_thread_sleep(500);
    nwy_gpio_value_set(DISPLAY_RESET_PIN, (nwy_value_e)1);
    nwy_thread_sleep(50);
}

// Clear LCD
void lcd_clear(void)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        lcd_send_command(0xB0 + i); // Set Page
        lcd_send_command(0x10);     // Set Column Address (MSB)
        lcd_send_command(0x00);     // Set Column Address (LSB)
        
        for (uint8_t j = 0; j < 128; j++)
        {
            lcd_send_data(0x00);
        }
    }
}

// Initialize LCD
void lcd_init(void)
{
    init_display_gpio();

    lcd_send_command(0xAF); // DISPLAY ON
    lcd_send_command(0xA0); // ADC Select
    lcd_send_command(0xA2); // Set Bias = 1/9
    lcd_send_command(0xCF); // COM Scan Direction
    lcd_send_command(0x2C); // Power Control 1 (Boost On)
    lcd_send_command(0x2E); // Power Control 2 (Regulator On)
    lcd_send_command(0x2F); // Power Control 3 (Follow On)
    lcd_send_command(0x25); // Regulator Resistor Select
    lcd_send_command(0x81); // Set Reference Voltage Mode
    lcd_send_command(0x17); // Set Reference Voltage Register
    lcd_send_command(0xA4); // Normal Display
    lcd_send_command(0xA6); // Normal Display
    lcd_send_command(0xB0); // 1st Page
    lcd_send_command(0x40); // Initial Start Line
    lcd_send_command(0x10); // Set Column Addr (H)
    lcd_send_command(0x00); // Set Column Addr (L)
}


// Send Command to Display
void lcd_send_command(uint8_t cmd)
{
    nwy_gpio_value_set(DISPLAY_DC_PIN, (nwy_value_e)0);  // Command Mode
    spi_bitbang_send(cmd);
}

// Send Data to Display
void lcd_send_data(uint8_t data)
{
    nwy_gpio_value_set(DISPLAY_DC_PIN, (nwy_value_e)1);  // Data Mode
    spi_bitbang_send(data);
}

// Send Bit-Banged SPI Data
void spi_bitbang_send(uint8_t data)
{
    uint8_t i, mask = 0x80;
    for (i = 0; i < 8; i++) 
    {
        // Set Data Line
        if (data & mask)
            nwy_gpio_value_set(DISPLAY_DATA_PIN, (nwy_value_e)1);
        else
            nwy_gpio_value_set(DISPLAY_DATA_PIN, (nwy_value_e)0);

        nwy_gpio_value_set(DISPLAY_CLK_PIN, (nwy_value_e)1);
        // delay_us(2);
        // __asm__("NOP");
        nwy_gpio_value_set(DISPLAY_CLK_PIN, (nwy_value_e)0);
        mask >>= 1;
    }
}

// Send Bit-Banged SPI Data
void spi_bitbang_send_image(uint8_t data)
{
    uint8_t i, mask = 0x01;
    for (i = 0; i < 8; i++) 
    {
        // Set Data Line
        if (data & mask)
            nwy_gpio_value_set(DISPLAY_DATA_PIN, (nwy_value_e)1);
        else
            nwy_gpio_value_set(DISPLAY_DATA_PIN, (nwy_value_e)0);

        // Clock Pulse
        nwy_gpio_value_set(DISPLAY_CLK_PIN, (nwy_value_e)1);
        // delay_us(2);
        __asm__("NOP");
        nwy_gpio_value_set(DISPLAY_CLK_PIN, (nwy_value_e)0);
        
        mask <<= 1;
    }
}

// Test Pattern
void lcd_test_pattern(void)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        lcd_send_command(0xB0 + i); // Set Page
        lcd_send_command(0x10);     // Set Column Address (MSB)
        lcd_send_command(0x00);     // Set Column Address (LSB)

        for (uint8_t j = 0; j < 128; j++)
        {
            lcd_send_data(0xAA); // Alternating pattern
        }
    }
}


void DispNewImage(uint8_t *Adata) // Left to Right & Top to Bottom
{
    int row, col, index = 0;

    for(row = 0; row < 8; row++) // Pages from top (0) to bottom (7)
    {
        lcd_send_command(0xB0 | row);  // Set page address
        lcd_send_command(0x10);        // Set column address upper nibble
        lcd_send_command(0x00);        // Set column address lower nibble

        for(col = 0; col < 128; col++) // Left to right (0 to 127)
        {    
            lcd_send_data_image2(Adata[index]); // Send pixel byte
            // lcd_send_data_image2(Adata[index]); // Send pixel byte
            index++; // Move to the next byte in image data
        }
    }
}


void DispNewImage2(uint8_t *Adata) // Left to Right & Top to Bottom
{
    int row, col, index = 0;

    for(row = 4; row < 8; row++) // Pages from top (0) to bottom (7)
    {
        lcd_send_command(0xB0 | row);  // Set page address
        lcd_send_command(0x10);        // Set column address upper nibble
        lcd_send_command(0x00);        // Set column address lower nibble
        
        for(col = 0; col < 128; col++) // Left to right (0 to 127)
        {    
            // lcd_send_data_image(Adata[index]); // Send pixel byte
            lcd_send_data_image2(Adata[index]); // Send pixel byte
            index++; // Move to the next byte in image data
			// vTaskDelay(pdMS_TO_TICKS(10)); 
        }
    }
}

// void DispNewImage2(uint8_t *Adata) // Left to Right & Top to Bottom
// {
//     int row, col, index = 0;

//     for(row = 4; row < 8; row++) // Pages from top (0) to bottom (7)
//     {
//         lcd_send_command(0xB0 | row);  // Set page address
//         lcd_send_command(0x10);        // Set column address upper nibble
//         lcd_send_command(0x00);        // Set column address lower nibble
        
//         nwy_gpio_value_set(DISPLAY_DC_PIN, (nwy_value_e)1); 
//         for(col = 0; col < 128; col++) // Left to right (0 to 127)
//         {    
//             // lcd_send_data_image(Adata[index]); // Send pixel byte
//             // lcd_send_data_image2(Adata[index]); // Send pixel byte
//             uint8_t inverted = ~Adata[index];
//             uint8_t i, mask = 0x01;
//             for (i = 0; i < 8; i++) 
//             {
//                 // Set Data Line
//                 if (inverted & mask)
//                     nwy_gpio_value_set(DISPLAY_DATA_PIN, (nwy_value_e)1);
//                 else
//                     nwy_gpio_value_set(DISPLAY_DATA_PIN, (nwy_value_e)0);

//                 // Clock Pulse
//                 nwy_gpio_value_set(DISPLAY_CLK_PIN, (nwy_value_e)1);
//                 // delay_us(2);
//                 __asm__("NOP");
//                 nwy_gpio_value_set(DISPLAY_CLK_PIN, (nwy_value_e)0);
                
//                 mask <<= 1;
//             }

//             index++; // Move to the next byte in image data
// 			// vTaskDelay(pdMS_TO_TICKS(10)); 
//         }
//     }
// }

void DispBarImage(uint8_t *Adata) // Left to Right & Top to Bottom
{
    int row, col, index, page = 0;
        LCD_SET_PAGE(page,COL);
        lcd_send_command(0xB0 | row);  // Set page address
        lcd_send_command(0x10);        // Set column address upper nibble
        lcd_send_command(0x00);        // Set column address lower nibble
    
        LCD_SET_PAGE(page,112);
        for(col = 112; col < 128; col++) // Left to right (0 to 127)
        {    
            // lcd_send_data_image(Adata[index]); // Send pixel byte
            lcd_send_data_image2(Adata[index]); // Send pixel byte
            index++; // Move to the next byte in image data
			// vTaskDelay(pdMS_TO_TICKS(10)); 
        }

}

void DispQRImage(uint8_t *Adata) // Left to Right & Top to Bottom
{
    int row, col, index = 0;

    for(row = 0; row < 8; row++) // Pages from top (0) to bottom (7)
    {
        lcd_send_command(0xB0 | row);  // Set page address
        lcd_send_command(0x10);        // Set column address upper nibble
        lcd_send_command(0x00);        // Set column address lower nibble

        for(col = 64; col < 128; col++) // Left to right (0 to 127)
        {    
            // lcd_send_data_image(Adata[index]); // Send pixel byte
            lcd_send_data_image2(Adata[index]); // Send pixel byte
            index++; // Move to the next byte in image data
			// vTaskDelay(pdMS_TO_TICKS(10)); 
        }
    }
}

void lcd_send_data_image(uint8_t data)
{
    nwy_gpio_value_set(DISPLAY_DC_PIN, (nwy_value_e)1);  // Data Mode
    spi_bitbang_send_image(data);
}

void lcd_send_data_image2(uint8_t data)
{
    nwy_gpio_value_set(DISPLAY_DC_PIN, (nwy_value_e)1);  // Data Mode
        uint8_t inverted = ~data;  // Flip all bits (logic level inversion)
    spi_bitbang_send_image(inverted);

}

uint8_t alpha[95][6] ={
{0x0,0x0,0x0,0x0,0x0,0x0},		//
{0x0,0x0,0x4F,0x0,0x0,0x0},		// !
{0x0,0x7,0x0,0x7,0x0,0x0},		// "
{0x14,0x7F,0x14,0x7F,0x14,0x0},	//#
{0x24,0x2A,0x7F,0x2A,0x12,0x0},	//$
{0x23,0x13,0x8,0x64,0x62,0x0},	//%
{0x36,0x49,0x55,0x22,0x50,0x0},	//&
{0x0,0x5,0x3,0x0,0x0,0x0},		//'
{0x0,0x1C,0x22,0x41,0x0,0x0},	//(
{0x0,0x41,0x22,0x1C,0x0,0x0},	//)
{0x14,0x8,0x3E,0x8,0x14,0x0},	//*
{0x8,0x8,0x3E,0x8,0x8,0x0},		//+
{0x0,0x50,0x30,0x0,0x0,0x0},	//,
{0x8,0x8,0x8,0x8,0x8,0x0},		//-
{0x0,0x60,0x60,0x0,0x0,0x0},	//.
{0x20,0x10,0x8,0x4,0x2,0x0},	// /
{0x3E,0x51,0x49,0x45,0x3E,0x0},	//0
{0x0,0x42,0x7F,0x40,0x0,0x0},	//1
{0x42,0x61,0x51,0x49,0x46,0x0},	//2
{0x21,0x41,0x45,0x4B,0x31,0x0},	//3
{0x18,0x14,0x12,0x7F,0x10,0x0},	//4
{0x27,0x45,0x45,0x45,0x39,0x0},	//5
{0x3C,0x4A,0x49,0x49,0x30,0x0},	//6
{0x1,0x71,0x9,0x5,0x3,0x0},		//7
{0x36,0x49,0x49,0x49,0x36,0x0},	//8
{0x6,0x49,0x49,0x29,0x1E,0x0},	//9
{0x0,0x36,0x36,0x0,0x0,0x0},	//:
{0x0,0x56,0x36,0x0,0x0,0x0},	//;
{0x8,0x14,0x22,0x41,0x0,0x0},	//<
{0x14,0x14,0x14,0x14,0x14,0x0},	//=
{0x0,0x41,0x22,0x14,0x8,0x0},	//>
{0x2,0x1,0x51,0x9,0x6,0x0},		//?
{0x32,0x49,0x79,0x41,0x3E,0x0},	//@
{0x7E,0x11,0x11,0x11,0x7E,0x0},	//A
{0x7F,0x49,0x49,0x49,0x36,0x0},	//B
{0x3E,0x41,0x41,0x41,0x22,0x0},	//C
{0x7F,0x41,0x41,0x22,0x1C,0x0},	//D
{0x7F,0x49,0x49,0x49,0x41,0x0},	//E
{0x7F,0x9,0x9,0x9,0x1,0x0},		//F
{0x3E,0x41,0x49,0x49,0x7A,0x0},	//G
{0x7F,0x8,0x8,0x8,0x7F,0x0},	//H
{0x0,0x41,0x7F,0x41,0x0,0x0},	//I
{0x20,0x40,0x41,0x3F,0x1,0x0},	//J
{0x7F,0x8,0x14,0x22,0x41,0x0},	//K
{0x7F,0x40,0x40,0x40,0x40,0x0},	//L
{0x7F,0x2,0xC,0x2,0x7F,0x0},	//M
{0x7F,0x4,0x8,0x10,0x7F,0x0},	//N
{0x3E,0x41,0x41,0x41,0x3E,0x0},	//O
{0x7F,0x9,0x9,0x9,0x6,0x0},		//P
{0x3E,0x41,0x51,0x21,0x5E,0x0},	//Q
{0x7F,0x9,0x19,0x29,0x46,0x00},//R
{0x46,0x49,0x49,0x49,0x31,0x0},	//S
//{0x0A,0x2A,0x6A,0x9A,0x0E,0x0A},	//rupee
{0x1,0x1,0x7F,0x1,0x1,0x0},		//T
{0x3F,0x40,0x40,0x40,0x3F,0x0},	//U
{0x1F,0x20,0x40,0x20,0x1F,0x0},	//V
{0x3F,0x40,0x38,0x40,0x3F,0x0},	//W
{0x63,0x14,0x8,0x14,0x63,0x0},	//X
{0x7,0x8,0x70,0x8,0x7,0x0},		//Y
{0x61,0x51,0x49,0x45,0x43,0x0},	//Z
{0x0,0x7F,0x41,0x41,0x0,0x0},	//[
{0x15,0x16,0x7C,0x16,0x15,0x0},	//
{0x0,0x41,0x41,0x7F,0x0,0x0},	//]
{0x4,0x2,0x1,0x2,0x4,0x0},		//^
{0x40,0x40,0x40,0x40,0x40,0x0},	//_
{0x0,0x1,0x2,0x4,0x0,0x0},		//
{0x20,0x54,0x54,0x54,0x78,0x0},	//a
{0x7F,0x48,0x44,0x44,0x38,0x0},	//b
{0x38,0x44,0x44,0x44,0x20,0x0},	//c
{0x38,0x44,0x44,0x48,0x7F,0x0},	//d
{0x38,0x54,0x54,0x54,0x18,0x0},	//e
{0x8,0x7E,0x9,0x1,0x2,0x0},		//f
{0xC,0x52,0x52,0x52,0x3E,0x0},	//g
{0x7F,0x8,0x4,0x4,0x78,0x0},	//h
{0x0,0x44,0x7D,0x40,0x0,0x0},	//i
{0x20,0x40,0x44,0x3D,0x0,0x0},	//j
{0x7F,0x10,0x28,0x44,0x0,0x0},	//k
{0x0,0x41,0x7F,0x40,0x0,0x0},	//l
{0x7C,0x4,0x18,0x4,0x78,0x0},	//m
{0x7C,0x8,0x4,0x4,0x78,0x0},	//n
{0x30,0x48,0x48,0x48,0x30,0x0},	//o
{0x7C,0x14,0x14,0x14,0x8,0x0},	//p
{0x8,0x14,0x14,0x18,0x7C,0x0},	//q
{0x7C,0x8,0x4,0x4,0x8,0x0},		//r
{0x48,0x54,0x54,0x54,0x20,0x0},	//s
{0x4,0x3F,0x44,0x40,0x20,0x0},	//t
{0x3C,0x40,0x40,0x20,0x7C,0x0},	//u
{0x1C,0x20,0x40,0x20,0x1C,0x0},	//v
{0x3C,0x40,0x30,0x40,0x3C,0x0},	//w
{0x44,0x28,0x10,0x28,0x44,0x0},	//x
{0xC,0x50,0x50,0x50,0x3C,0x0},	//y
{0x44,0x64,0x54,0x4C,0x44,0x0},	//z
{0x0,0x8,0x36,0x41,0x0,0x0},	//{
{0x0,0x0,0x7F,0x0,0x0,0x0},		//|
{0x0,0x41,0x36,0x8,0x0,0x0}		//}

};

void  Display(char Font, uint16_t Page_No, char Col_No, const char *MESSAGE)
{
//	taskENTER_CRITICAL();
//	LCD_INIT(true);

	PAGE = Page_No;
	COL = Col_No;
	LCD_SET_PAGE(PAGE,COL);	

	switch(Font)
	{
		while(1)
		{
		  case 0: 
		       while(*MESSAGE != '\0')
			{		
				LCD_PRINT_FONT1(*MESSAGE);
				MESSAGE++;
			}
			break;
		  case 1: 		  
		  
		       while(*MESSAGE != '\0')
			{		
				// LCD_PRINT_2nd_FONT(*MESSAGE);
				MESSAGE++;
			}
			break;
			
		 default :
			break;
		}
	}
//	taskEXIT_CRITICAL();
}

//set the dispaly data fount size
void LCD_PRINT_FONT1(uint8_t data)
{
	uint8_t i,j,index;

	static uint8_t PAGE_var	=	PAGE1;
	static uint8_t COL_var	=	COL1;
	
	index	= data - 32;
	
	LCD_PAGE_COL_CHECK_FONT1();
	
	for(i = 0; i < 6; i++)
    lcd_send_data(alpha[index][i]);

	COL += 1;	

}

// set the display data fount size in column
void LCD_PAGE_COL_CHECK_FONT1(void)
{
	
	if (COL >= 127)
	{
		PAGE += 1;
		if(PAGE		>=	PAGE8 + 1)
        lcd_clear();

		COL 	=	COL1;

		LCD_SET_PAGE(PAGE,COL);		// initial page,col
	}
}

// set the display line or page to show the data
void LCD_SET_PAGE(uint8_t page,uint8_t col)
{
	uint8_t msb,lsb;
	msb		=	(((col & 0xF0) >> 4)| 0x10);
	lsb		=	(col & 0x0F) ;
	lcd_send_command(page);
	lcd_send_command(msb);
	lcd_send_command(lsb);
}