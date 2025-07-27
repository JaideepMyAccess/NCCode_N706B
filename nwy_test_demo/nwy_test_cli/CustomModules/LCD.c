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
    delay_us(200000); 
    nwy_test_cli_echo("\r\nPrint 2 from LCD.c\r\n");

    while(1){
        char *sptr = NULL;
        sptr = nwy_test_cli_input_gets("\r\n Click Enter \r\n");

        lcd_main_test();
    }

}

void lcd_main_test()
{
    lcd_init();
    lcd_clear();
    lcd_test_pattern();
    delay_us(1000000); 
    DispNewImage(image_data_Image);
}

// Delay Function
void delay_us(uint32_t us)
{
    volatile uint32_t count;
    volatile uint32_t dummy = 0;
    for (count = 0; count < us * 10; count++) // Rough delay loop
    {
        // dummy++;
    }
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
    delay_us(200); 
    nwy_gpio_value_set(DISPLAY_RESET_PIN, (nwy_value_e)1);
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
    //  delay_us(20);
    spi_bitbang_send(cmd);
}

// Send Data to Display
void lcd_send_data(uint8_t data)
{
    nwy_gpio_value_set(DISPLAY_DC_PIN, (nwy_value_e)1);  // Data Mode
    //  delay_us(20);
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

        // Clock Pulse
        // delay_us(20);
        nwy_gpio_value_set(DISPLAY_CLK_PIN, (nwy_value_e)1);
        delay_us(2);
        nwy_gpio_value_set(DISPLAY_CLK_PIN, (nwy_value_e)0);
        //  delay_us(20);
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
        delay_us(2);
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
            lcd_send_data_image(Adata[index]); // Send pixel byte
            // lcd_send_data_image2(Adata[index]); // Send pixel byte
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