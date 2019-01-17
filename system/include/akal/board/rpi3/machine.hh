#ifndef BOUTGLAY_AKAL_BOARD_RPI3_MACHINE_HH
#define BOUTGLAY_AKAL_BOARD_RPI3_MACHINE_HH

// akal/kernel
#include "akal/kernel/machine.hh"
#include "akal/kernel/mmio.hh"
#include "akal/kernel/timer.hh"
#include "akal/kernel/scheduler.hh"
// akal/board/rpi3
#include "akal/board/rpi3/timer.hh"
#include "akal/board/rpi3/interrupt.hh"
#include "akal/board/rpi3/mailbox.hh"
#include "akal/board/rpi3/device/uart.hh"

extern volatile unsigned char _binary_system_resources_font_psf_start;

u32 abs(i32 v);

namespace akal {
    namespace rpi3 {

        u8 *lfb;
            u32 width, height, pitch;

        class RPiScreen: public ScreenDevice {
        public:

            RPiScreen() {
                // nothing happen here
            }

            ~RPiScreen() {
                // nothing happen here
            }

            void init(Machine& machine) {
                mbox[0] = 35*4;
                mbox[1] = MBOX_REQUEST;

                mbox[2] = 0x48003;  //set phy wh
                mbox[3] = 8;
                mbox[4] = 8;
                mbox[5] = 800;         //FrameBufferInfo.width
                mbox[6] = 480;          //FrameBufferInfo.height

                mbox[7] = 0x48004;  //set virt wh
                mbox[8] = 8;
                mbox[9] = 8;
                mbox[10] = 800;        //FrameBufferInfo.virtual_width
                mbox[11] = 480;         //FrameBufferInfo.virtual_height
                
                mbox[12] = 0x48009; //set virt offset
                mbox[13] = 8;
                mbox[14] = 8;
                mbox[15] = 0;           //FrameBufferInfo.x_offset
                mbox[16] = 0;           //FrameBufferInfo.y.offset
                
                mbox[17] = 0x48005; //set depth
                mbox[18] = 4;
                mbox[19] = 4;
                mbox[20] = 32;          //FrameBufferInfo.depth

                mbox[21] = 0x48006; //set pixel order
                mbox[22] = 4;
                mbox[23] = 4;
                mbox[24] = 0;           //RGB, not BGR preferably

                mbox[25] = 0x40001; //get framebuffer, gets alignment on request
                mbox[26] = 8;
                mbox[27] = 8;
                mbox[28] = 4096;        //FrameBufferInfo.pointer
                mbox[29] = 0;           //FrameBufferInfo.size

                mbox[30] = 0x40008; //get pitch
                mbox[31] = 4;
                mbox[32] = 4;
                mbox[33] = 0;           //FrameBufferInfo.pitch

                mbox[34] = MBOX_TAG_LAST;

                if(mbox_call(MBOX_CH_PROP) && mbox[20]==32 && mbox[28]!=0) {
                    mbox[28]&=0x3FFFFFFF;
                    width=mbox[5];
                    height=mbox[6];
                    pitch=mbox[33];
                    lfb=(u8*)((u64)mbox[28]);
                } else {
                    // error
                }

                // white
                int x,y;
                unsigned char *ptr=lfb;
                unsigned int pixel = 0xFFFFFF;

                for(y=0;y<height;y++) {
                    for(x=0;x<width;x++) {
                        *((unsigned int*)ptr)=*((unsigned int *)&pixel);
                        ptr+=4;
                    }
                    ptr+=pitch-width*4;
                }
            }

            void setPixel(Point p, Color c) {
                char pixel[4];
                pixel[0] = c.b;
                pixel[1] = c.g;
                pixel[2] = c.r;
                *((u32 *)(lfb + (p.y > 1 ? p.y - 1 : 0) * width * 4 + p.x * 4)) = *((u32 *) &pixel);
            }

            void plotLine(Point p0, Point p1, Color color) {
                int x0 = p0.x, y0 = p0.y, x1 = p1.x, y1 = p1.y;
                int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
                int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1; 
                int err = dx+dy, e2;
             
                for(;;){
                    this->setPixel(Point(x0,y0), color);
                    if(x0 == x1 && y0 == y1) {
                        break;
                    }
                    e2 = 2 * err;
                    if(e2 >= dy) {
                        err += dy;
                        x0 += sx;
                    }
                    if(e2 <= dx) {
                        err += dx;
                        y0 += sy;
                    }
                }
            }
        };

        typedef struct {
    unsigned int magic;
    unsigned int version;
    unsigned int headersize;
    unsigned int flags;
    unsigned int numglyph;
    unsigned int bytesperglyph;
    unsigned int height;
    unsigned int width;
    unsigned char glyphs;
} __attribute__((packed)) psf_t;

        class Console {
        private:
            
        public:
            void init() {
                
            }
            /**
 * Display a string
 */
void print(int x, int y, const char *s)
{
    // get our font
    psf_t *font = (psf_t*)&_binary_system_resources_font_psf_start;
    // draw next character if it's not zero
    while(*s) {
        // get the offset of the glyph. Need to adjust this to support unicode table
        unsigned char *glyph = (unsigned char*)&_binary_system_resources_font_psf_start +
         font->headersize + (*((unsigned char*)s)<font->numglyph?*s:0)*font->bytesperglyph;
        // calculate the offset on screen
        int offs = (y * font->height * pitch) + (x * (font->width+1) * 4);
        // variables
        int i,j, line,mask, bytesperline = (font->width+7)/8;
        // handle carrige return
        if(*s=='\r') {
            x=0;
        } else
        // new line
        if(*s=='\n') {
            x=0; y++;
        } else {
            // display a character
            for(j=0;j<font->height;j++){
                // display one row
                line=offs;
                mask=1<<(font->width-1);
                for(i=0;i<font->width;i++){
                    // if bit set, we use white color, otherwise black
                    *((unsigned int*)(lfb + line))=!(((int)*glyph) & mask) ?0x00FFFFFF:0;
                    mask>>=1;
                    line+=4;
                }
                // adjust to next line
                glyph+=bytesperline;
                offs+=pitch;
            }
            x++;
        }
        // next character
        s++;
    }
}
        };

        //*-- RaspberryPi3
        class RaspberryPi3: public Machine {
        public:
            InterruptController interruptController;
            Scheduler scheduler;
            MemoryManager memory;
            #ifdef AKAL_APPLICATION_TARGET_REQUIRE_UART
            Uart1Device uart1;
            Uart0Device uart0;
            #endif
            ARMTimer timer;
            #ifdef AKAL_APPLICATION_TARGET_REQUIRE_SCREEN
            RPiScreen screen;
            #endif
            #ifdef AKAL_APPLICATION_TARGET_REQUIRE_CONSOLE
            Console console;
            #endif

            void init() {
                #ifdef AKAL_APPLICATION_TARGET_REQUIRE_UART
                this->uart1.init(*this);
                this->uart0.init(*this);
                #endif
                #ifdef AKAL_APPLICATION_TARGET_REQUIRE_SCREEN
                this->screen.init(*this);
                #endif
                #ifdef AKAL_APPLICATION_TARGET_REQUIRE_CONSOLE
                this->console.init();
                #endif
            }
        };

    }
}


#endif // BOUTGLAY_AKAL_BOARD_RPI3_MACHINE_HH
