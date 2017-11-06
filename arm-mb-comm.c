#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>

#define UART_BASE_ADDR 0x42C00000
#define GPIO_LED_ADDR 0xE000A000
#define IN 0
#define XUartLite_Out32 Xil_Out32
#define XUartLite_WriteReg(BaseAddress, RegOffset, Data) \
	XUartLite_Out32((BaseAddress) + (RegOffset),  (uint32_t)(Data))
//#define XUartLite_GetStatusReg(BaseAddress) \
//	XUartLite_
//may actually need to implement this, requires defining a bunch of read functions...

#define XUL_RX_FIFO_OFFSET	0
#define XUL_TX_FIFO_OFFSET	4
#define XUL_STATUS_REG_OFFSET	8
#define XUL_CONTROL_REG_OFFSET	12
#define XUL_CR_ENABLE_INTR	0x10
#define XUL_SR_TX_FIFO_FULL	0X08

#define XST_SUCCESS 0L
#define XST_FAILURE 1L
#define INLINE __inline

#define BUFF_SIZE 100

typedef uintptr_t UINTPTR;

typedef struct {
	uint8_t *NextBytePtr;
	unsigned int RequestedBytes;
	unsigned int RemainingBytes;
} XUartLite_Buffer;

typedef struct {
	UINTPTR RegBaseAddress;
	uint32_t IsReady;
	XUartLite_Buffer SendBuffer;
	XUartLite_Buffer ReceiveBuffer;
	//may need some handler stuff here.  see xuartlite.h
} XUartLite;

static INLINE void Xil_Out32(UINTPTR Addr, uint32_t Value) {
	volatile uint32_t *LocalAddr = (volatile uint32_t *)Addr;
	*LocalAddr = Value;
}

int XUartLite_CfgInitialize(XUartLite *InstancePtr) {
	UINTPTR base_addr = UART_BASE_ADDR;
	InstancePtr->SendBuffer.NextBytePtr = NULL;
	InstancePtr->SendBuffer.RemainingBytes = 0;
	InstancePtr->SendBuffer.RequestedBytes = 0;

	InstancePtr->ReceiveBuffer.NextBytePtr = NULL;
	InstancePtr->ReceiveBuffer.RemainingBytes = 0;
	InstancePtr->ReceiveBuffer.RequestedBytes = 0;

	InstancePtr->RegBaseAddress = base_addr;
	XUartLite_WriteReg(InstancePtr->RegBaseAddress, XUL_CONTROL_REG_OFFSET, 0);
	return XST_SUCCESS;
}

unsigned int XUartLite_SendBuffer(XUartLite *InstancePtr) {
	unsigned int SentCount = 0;
	uint8_t StatusRegister;
	uint8_t IntrEnableStatus;
	//there is some register stuff that goes on here...need read reg though to implement it
	
	//There are normall some check to make sure TX buff isn't full, but requires read reg
	while(SentCount < InstancePtr->SendBuffer.RemainingBytes) {
		XUartLite_WriteReg(InstancePtr->RegBaseAddress,
				XUL_TX_FIFO_OFFSET,
				InstancePtr->SendBuffer.NextBytePtr[SentCount]);
		SentCount++;
		//reg check status...need reg read
	}
	InstancePtr->SendBuffer.NextBytePtr += SentCount;
	InstancePtr->SendBuffer.RemainingBytes -= SentCount;

	//update some stats here
	//also they restore interrupt enable registers...which requires them to be disbaled, which never did
	
	return SentCount;
}

unsigned int XUartLite_Send(XUartLite *InstancePtr, uint8_t *DataBufferPtr, unsigned int NumBytes) {
	unsigned int BytesSent;
	//asserts done here...
	
	//k, in order to uncomment this, we need to add read reg	
	//XUartLite_WriteReg(InstancePtr->RegBaseAddress, XUL_CONTROL_REG_OFFSET, 0);

	InstancePtr->SendBuffer.RequestedBytes = NumBytes;
	InstancePtr->SendBuffer.RemainingBytes = NumBytes;
	InstancePtr->SendBuffer.NextBytePtr = DataBufferPtr;
	
	//may actuall have to implement read reg...
	//XUartLite_WriteReg(InstancePtr->RegBaseAddress,XUL...

	BytesSent = XUartLite_SendBuffer(InstancePtr);
	return BytesSent;
}

XUartLite UartLite;

int main(int argc, char **argv) {
	unsigned uart_addr = strtoul("0x42C00000", NULL, 0);
	unsigned gpio_addr = strtoul("0xE000A000", NULL, 0);
	unsigned rx_fifo_trig_addr = strtoul("0x42C00008", NULL, 0);
	unsigned rx_fifo_trig_reg;
	int fd;
	int value; 
	int direction = IN;
	unsigned page_addr, page_offset;
	void *ptr;
	unsigned page_size=sysconf(_SC_PAGESIZE);
	//unsigned char in_buf[BUFF_SIZE];
    //unsigned char out_buf[BUFF_SIZE];

    fd = open("/dev/mem", O_RDWR);
	if(fd<1) {
		perror(argv[0]);
		return -1;
	}
	
	page_addr = (uart_addr & (~(page_size-1)));
	page_offset = uart_addr - page_addr;
	ptr = mmap(NULL, page_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, page_addr);
	while(1) {
		unsigned char out_buf[BUFF_SIZE];
		rx_fifo_trig_reg = *((unsigned *)(ptr + page_offset + 8));
		while(!(rx_fifo_trig_reg == 0x05 || rx_fifo_trig_reg == 0x07)) {
			rx_fifo_trig_reg = *((unsigned *)(ptr + page_offset + 8));
			usleep(100000);
		}
		int i = 0;
		while(rx_fifo_trig_reg != 0x04) {
			value = *((int *)(ptr + page_offset));
			if(value >= 0x20 && value <= 0x7E) {
				out_buf[i] = (unsigned char)value;
				i++;
			}
			rx_fifo_trig_reg = *((unsigned *)(ptr + page_offset + 8));
			usleep(1000);
		}
		printf("Rx from MicroBlaze: %s\n", out_buf);
		*((unsigned *)(ptr + page_offset + 0x44)) = 0x3F;
		memset(out_buf,0,strlen(out_buf));
	}

	/*
	while (1) {
		unsigned char in_buf[BUFF_SIZE];
		unsigned char out_buf[BUFF_SIZE];	
        //memset(in_buf,0,strlen(in_buf));
        //memset(out_buf,0,strlen(out_buf));
        //need input from user
	    printf("Enter a string: \n");
	    fgets(in_buf, BUFF_SIZE-1,stdin);
	    //buffer[strlen(buffer)-1] = 0;
	    printf("You entered the string: %s", in_buf);

        rx_fifo_trig_reg = *((unsigned *)(ptr + page_offset + 8));
        while(rx_fifo_trig_reg == 0x07) {
            value = *((int *)(ptr+page_offset));
            printf("value: %d\n", value);
            usleep(10000);
            rx_fifo_trig_reg = *((unsigned *)(ptr + page_offset + 8));
            printf("rx_fif_trig_reg: %d\n", rx_fifo_trig_reg);
            usleep(10000);
        }
        int i=0;
        while(rx_fifo_trig_reg != 0x04) {
            printf("rx_fif_trig_reg: %d\n", rx_fifo_trig_reg);
            value = *((int *)(ptr+page_offset));
            //printf("rx count: %d\n",i);
            //printf("read val(int): %d && read val(char): %c\n", value, value);
            if(value >= 0x20 && value <= 0x7E) {
                out_buf[i] = (unsigned char)value;
                i++;
            }
            rx_fifo_trig_reg = *((unsigned *)(ptr + page_offset + 8));
            usleep(1000);
        }

        printf("Received from MicroBlaze: %s\n", out_buf);
        
        //send to microblaze
        *((unsigned *)(ptr + page_offset)) == 0x18; //sets transmit enable
        *((unsigned *)(ptr + page_offset + 8)) = 0x0400; //sets TTRIG (transmit fifo trigget interrupt)
		*((unsigned *)(ptr + page_offset + 0x44)) = 0x3F;        
        usleep(10000);
        unsigned x = *((unsigned *)(ptr + page_offset + 8));
        rx_fifo_trig_reg = *((unsigned *)(ptr + page_offset + 8));
        printf("enable transmit reg val: %d\n", x);
        printf("set ttrig reg val: %d\n", rx_fifo_trig_reg);
        
        //printf("rx_fif_trig_reg: %d\n", rx_fifo_trig_reg);
        //printf("here0\n");
  */    
  	/*
        int i;
        for(i=0;i<strlen(buffer)-1;i++) {
            *((unsigned *)(ptr + page_offset)) = buffer[i];
            printf("send count: %d\n", i);
            usleep(1000);
        }
        printf("here\n");
        usleep(1000000);
        
        for(i=0;i<BUFF_SIZE;i++) {
            value = *((int *)(ptr+page_offset));
            printf("rx count: %d\n",i);
	    printf("read val(int): %d && read val(char): %c\n", value, value);
		buffer[i] = (unsigned char)value;
		usleep(1000);
	    }
	    printf("here1\n");

	    printf("ARM received the following string back from microblaze: \n%s",buffer);
    */
	//}
	munmap(ptr, page_size);
    

	return 0;
}
