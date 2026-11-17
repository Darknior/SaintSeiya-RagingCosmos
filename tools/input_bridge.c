#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <execinfo.h>
#include <unistd.h>
static void crash(int sig){void *frames[40];int n=backtrace(frames,40);backtrace_symbols_fd(frames,n,2);_exit(139);}
static volatile sig_atomic_t capture_requested;
static void request_capture(int s){capture_requested=1;}
__attribute__((constructor)) static void setup(void){signal(SIGUSR1,request_capture);}
const unsigned char *SDL_GetKeyboardState(int *count){
 static const unsigned char*(*real)(int*);static unsigned char keys[512];
 if(!real)real=dlsym(RTLD_NEXT,"SDL_GetKeyboardState");int n=0;const unsigned char *raw=real(&n);
 for(int i=0;i<n && i<512;i++)keys[i]=raw[i];if(count)*count=n;
 const char *name=getenv("TEST_KEY_FILE");if(name){FILE *f=fopen(name,"r");int k;if(f){while(fscanf(f,"%d",&k)==1)if(k>=0&&k<512)keys[k]=1;fclose(f);}}
 return keys;
}
void SDL_RenderPresent(void *renderer){
 static void(*real)(void*);if(!real){real=dlsym(RTLD_NEXT,"SDL_RenderPresent");signal(SIGSEGV,crash);}
 if(capture_requested){capture_requested=0;const char *name=getenv("TEST_CAPTURE");int w=0,h=0;
 int(*size)(void*,int*,int*)=dlsym(RTLD_NEXT,"SDL_GetRendererOutputSize");int(*read)(void*,void*,unsigned int,void*,int)=dlsym(RTLD_NEXT,"SDL_RenderReadPixels");
 if(name&&!size(renderer,&w,&h)&&w>0&&h>0){uint32_t *b=malloc(w*h*4);if(b&&!read(renderer,NULL,0x16362004,b,w*4)){FILE*f=fopen(name,"wb");if(f){fprintf(f,"P6\n%d %d\n255\n",w,h);for(int i=0;i<w*h;i++){fputc((b[i]>>16)&255,f);fputc((b[i]>>8)&255,f);fputc(b[i]&255,f);}fclose(f);}}free(b);}}
 real(renderer);
}
