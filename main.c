#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>
#include <psp2/net/net.h>
#include <psp2/net/netctl.h>
#include <psp2/sysmodule.h>
#include <psp2/io/fcntl.h>
#include <vita2d.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// --- CONFIGURATION ---
// Change this to your PC's local IP address
const char *GLEEP_PC_IP = "192.168.1.100"; 
const int GLEEP_PORT = 8080;
// ---------------------

vita2d_pgf *font;
vita2d_texture *background, *screen_bg;

// Simple HTTP GET function using basic sockets
void send_command(const char *command) {
    int sock = sceNetSocket("gleep_http", SCE_NET_AF_INET, SCE_NET_SOCK_STREAM, 0);
    if (sock < 0) return;

    SceNetSockaddrIn addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = SCE_NET_AF_INET;
    addr.sin_port = sceNetHtons(GLEEP_PORT);
    sceNetInetPton(SCE_NET_AF_INET, GLEEP_PC_IP, &addr.sin_addr);

    if (sceNetConnect(sock, (SceNetSockaddr*)&addr, sizeof(addr)) >= 0) {
        char request[256];
        snprintf(request, sizeof(request), "GET /%s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", command, GLEEP_PC_IP);
        sceNetSend(sock, request, strlen(request), 0);
    }
    sceNetSocketClose(sock);
}

int main() {
    // Initialize graphics
    vita2d_init();
    vita2d_set_clear_color(RGBA8(0, 0, 0, 255));
    font = vita2d_load_default_pgf();

    // Initialize Networking
    sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
    SceNetInitParam netInitParam;
    int size = 128 * 1024;
    netInitParam.memory = malloc(size);
    netInitParam.size = size;
    netInitParam.flags = 0;
    sceNetInit(&netInitParam);
    sceNetCtlInit();

    SceCtrlData pad;
    memset(&pad, 0, sizeof(pad));

    int running = 1;
    while (running) {
        vita2d_start_drawing();
        vita2d_clear_screen();

        // --- DRAW UI ---
        // Draw Background
        vita2d_draw_rectangle(0, 0, 960, 544, RGBA8(0, 0, 0, 255));

        // Draw Remote Body
        vita2d_draw_rectangle(280, 20, 400, 500, RGBA8(17, 17, 17, 255));
        vita2d_draw_rectangle(280, 20, 400, 500, RGBA8(255, 0, 0, 255)); // Red Border
        vita2d_draw_rectangle(285, 25, 390, 490, RGBA8(0, 0, 0, 255)); // Inner Black

        // Title
        vita2d_pgf_draw_text(font, 410, 60, RGBA8(255, 0, 0, 255), 1.5f, "GLEEP");

        // LCD Screen
        vita2d_draw_rectangle(330, 90, 300, 80, RGBA8(0, 50, 0, 255));
        vita2d_pgf_draw_text(font, 400, 140, RGBA8(0, 255, 0, 255), 1.5f, "GLEEP TV");

        // Buttons
        // CH+
        vita2d_draw_rectangle(330, 200, 300, 50, RGBA8(50, 50, 50, 255));
        vita2d_pgf_draw_text(font, 450, 235, RGBA8(255, 255, 255, 255), 1.2f, "CH +");

        // CH-
        vita2d_draw_rectangle(330, 270, 300, 50, RGBA8(50, 50, 50, 255));
        vita2d_pgf_draw_text(font, 450, 305, RGBA8(255, 255, 255, 255), 1.2f, "CH -");

        // PAUSE
        vita2d_draw_rectangle(330, 340, 300, 50, RGBA8(50, 50, 50, 255));
        vita2d_pgf_draw_text(font, 430, 375, RGBA8(255, 255, 255, 255), 1.2f, "PAUSE");

        // VHS
        vita2d_draw_rectangle(330, 410, 300, 50, RGBA8(50, 50, 50, 255));
        vita2d_pgf_draw_text(font, 420, 445, RGBA8(255, 255, 255, 255), 1.2f, "VHS TOGGLE");

        vita2d_end_drawing();
        vita2d_swap_buffers();

        // --- INPUT HANDLING ---
        sceCtrlPeekBufferPositive(0, &pad, 1);
        
        if (pad.buttons & SCE_CTRL_START) running = 0;
        
        // Simulate button presses
        if (pad.buttons & SCE_CTRL_UP) {
            send_command("ch_up");
            sceKernelDelayThread(200000); // Debounce
        }
        if (pad.buttons & SCE_CTRL_DOWN) {
            send_command("ch_down");
            sceKernelDelayThread(200000);
        }
        if (pad.buttons & SCE_CTRL_CROSS) {
            send_command("pause");
            sceKernelDelayThread(200000);
        }
        if (pad.buttons & SCE_CTRL_SQUARE) {
            send_command("vhs_toggle");
            sceKernelDelayThread(200000);
        }

        sceKernelDelayThread(10000);
    }

    // Cleanup
    vita2d_fini();
    vita2d_free_pgf(font);
    free(netInitParam.memory);
    sceNetCtlTerm();
    sceNetTerm();
    sceSysmoduleUnloadModule(SCE_SYSMODULE_NET);
    sceKernelExitProcess(0);
    return 0;
}