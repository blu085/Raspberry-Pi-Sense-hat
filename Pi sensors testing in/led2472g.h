/** @brief Constants, structures, function prototypes for led2472g, nav pad, and serial
 *  @file led2472g.h
 *  @date 4 mars 2018
 *  @version 1.2
 *  @authors Philippe SIMIER Philippe CRUCHET Christophe GRILLO
 *  @details Classe SenseHat : Gestion de la carte SenseHat
 *  @version 1.3
 *  @date 30 July 2019
 *  @authors Jon Dellaria bug fixes, method translation to English and Temperature Measurement adjustments required for the Raspberry for a true temperature reading.
 *  @version 1.3a
 *  @date 01 October 2020
 *  @authors Paul Moggach removed french to compact files and make it more readable. Made changes to conform with Allman style.
 *  @since 2024-02-16
 *  @authors Kris Medri a return to c instead of cpp
 */
#ifndef LED2472G_H
#define LED2472G_H
#define _GNU_SOURCE //Must occur before library includes

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <time.h>
#include <poll.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <linux/input.h>
#include <linux/fb.h>

// Constants
#define SEARCHSTR "serial\t\t:"
#define SYSINFOBUFSZ 512

#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"
#define DEV_FB "/dev"
#define FB_DEV_NAME "fb"

#define COLOR_SENSEHAT uint16_t
#define PI 3.14159265
#define RED 	0xF800
#define BLUE    0x001F
#define GREEN   0x07E0
#define WHITE   0xFFFF
#define BLACK   0x0000
#define ORANGE  0xFC00
#define CYAN    0x87FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define HB 0x00E7
#define HY 0xC4A0
#define HW 0xFFFF
#define blue(value)  (((unsigned short) value & 0xF) << 1)  //mask out everything but the lower 3 bits 
#define green(value) (((unsigned short) value & 0x1F) << 6)  
#define red(value)   (((unsigned short) value & 0xF) << 12)
#define rgb(r,g,b)   (red(r) | green(g) | blue(b))

// Enumerated Types
enum direction_t{
    UP,
    RIGHT,
    DOWN,
    LEFT,
    NONE,
};

// Structures
struct segment_t {
    struct segment_t *next;
    int x;
    int y;
};

struct snake_t {
    struct segment_t head;
    struct segment_t *tail;
    enum direction_t heading;
};

struct apple_t {
    int x;
    int y;
};

struct fb_t {
    uint16_t pixel[8][8];
};

struct brush_t {
    unsigned short colourindex;
    unsigned short colours[8];
    clock_t now,timeoflastcmd;
    struct apple_t apple;
};

// Function Prototypes
/// @cond INTERNAL
uint64_t ShGetSerial(void);
struct fb_t *ShInit(struct fb_t *fb);
static int is_event_device(const struct dirent *dir);
static int is_framebuffer_device(const struct dirent *dir);
static int open_evdev(const char *dev_name);
static int open_fbdev(const char *dev_name);
void move_dir(unsigned int code, struct brush_t *brush);
void move_events(int evfd, struct brush_t *brush);
void ShWipeScreen(uint16_t color, struct fb_t *fb);
void ShLightPixel(int row, int column, uint16_t color, struct fb_t *fb);
bool ShEmptyColumn(int numcolumn,uint16_t image[8][8],uint16_t colorBackground);
void ShImageContainment(int numcolumn,uint16_t image[][8][8], int taille);
void ShViewPattern(uint16_t pattern[][8], struct fb_t *fb);
void ShConvertCharacterToPattern(char c, uint16_t image[8][8], uint16_t colorText, uint16_t colorBackground);
void ShViewMessage(const char * message, int vitesseDefilement, uint16_t colorText, uint16_t colorBackground, struct fb_t *fb);
void ShRotatePattern(int angle, struct fb_t *fb);

/// @endcond

#endif // LED2472G_H
