/* sioloader.c by danhans42
simple sio loader - modded version of serial 1.3 by hitmen
 */


#define STACKP 0x801ffff0

#include <sys\types.h>
#include <libsn.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libsio.h>
#include <kernel.h>
#include <setjmp.h>

char read_sio (void);
void write_sio (char);
void init_sio (void);
void initt_sio (void);
u_long GetLongData(void);
void DispInit();
void sioload();

u_char header[2048];
static struct XF_HDR * head;
	int id;
	int i=0;
	u_long sync;
	u_long f_len;
	u_long x_addr;
	u_long count;
	u_long padRead;
	char * write_addr;
main()
{                             
	PadInit(0);
	DelSIO();
	StopCallback();
	DispInit();


	FntPrint("\n\n\n\n\n\n\n\n\n                     <R1> FOR 115k SIOLOADER\n\n                     <R2> FOR 345k SIOLOADER");
	FntFlush(-1);
	do
	{
		padRead = PadRead(0);
		if (padRead&PADR1) {init_sio();FntPrint("\n115k SIOLOADER");sioload(); }                                   
	} while (!(padRead&PADR2));
	while (PadRead(0)&PADR2);
	FntPrint("\n345k SIOLOADER");
	initt_sio();
	sioload();
}


char read_sio(void)
{
	char c;
	long sts;

	sts = _sio_control(0,1,0);
	_sio_control(1,1,sts|CR_RTS);		/* RTS:on */
	while(!(_sio_control(0,0,0)&SR_RXRDY));
	c = _sio_control(0,4,0)&0xff;
	sts = _sio_control(0,1,0);
	_sio_control(1,1,sts&(~CR_RTS));	/* RTS:off */
	return c;
}


void write_sio(char c)
{
	while((_sio_control(0,0,0)&(SR_TXU|SR_TXRDY))!=(SR_TXU|SR_TXRDY));
	_sio_control(1,4,c);
}

void init_sio(void)
{
	_sio_control(1,2,MR_SB_01|MR_CHLEN_8|0x02); /* 8bit, no-parity, 1 stop-bit */
	_sio_control(1,3,115200);
	_sio_control(1,1,CR_RXEN|CR_TXEN);	/* RTS:off DTR:off */
}

void initt_sio(void)
{
	_sio_control(1,2,MR_SB_01|MR_CHLEN_8|0x02); /* 8bit, no-parity, 1 stop-bit */
	_sio_control(1,3,345600);
	_sio_control(1,1,CR_RXEN|CR_TXEN);	/* RTS:off DTR:off */
}


u_long GetLongData(void)
{
    u_long dat;
    dat = (read_sio());    
    dat |= (read_sio()) <<8;
    dat |= (read_sio()) <<16;
    dat |= (read_sio()) << 24;
    return (dat); 
}

void sioload()
{
	PadStop();
	FntPrint("\n\n\n\n\n\n\n\n\n\n\n                        WAITING FOR CLIENT\n"); 
	FntFlush(-1);


	while (sync != 99){
		sync = read_sio();
	}

	FntPrint("\n\n\n\n\n\n\n\n\n\n\n\n                           Sync received\n");
	FntFlush(-1);

	for (i=0;i<2048;i++){
		header[i] = read_sio();
	}
    x_addr = GetLongData();
	write_addr =(char *) GetLongData();
	f_len = GetLongData();

	FntPrint("\n\n\n\n\n\n\n\n\n\n\n\n                          Receiving Data\n"); 
	FntFlush(-1);

	for (count=0;count<f_len;count++){
		*write_addr = read_sio();
		write_addr++;
	}
	FntPrint("\n\n\n\n\n\n\n\n\n\n\n\n                             EXECUTE!!!\n"); 
	FntFlush(-1);
	PadStop();
	ResetGraph(3);              
	StopCallback();

	head = (struct XF_HDR *)header;
	head->exec.s_addr = STACKP;
	head->exec.s_size = 0;
	EnterCriticalSection();
	Exec(&(head->exec),1,0);
}

void DispInit() {
	if (*(char *)0xbfc7ff52=='E')
		SetVideoMode(MODE_PAL);
	else
		SetVideoMode(MODE_NTSC);
	ResetGraph (0) ;
	GsInitGraph(512,240,0,0,0);
	GsDefDispBuff(0,0,0,240);
	GsInit3D();
	SetDispMask(1); 
	FntLoad (960,256);
	SetDumpFnt(id = FntOpen(0,0,512,240,1,1024));
}
