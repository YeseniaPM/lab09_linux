#include <stdio.h>
#include <sys/mman.h> 
#include <fcntl.h> 
#include <unistd.h>
#define _BSD_SOURCE

#define GPIO_DIP_OFFSET 2 //sw(3:0)
#define GPIO_LED_OFFSET 0 //led (3:0)
#define GPIO_RGB_OFFSET 1 //rgb led (6 bits led5 + led5)
#define DIPS_AND_LEDS_BASEADDR 0x41200000


// the below code uses a device called /dev/mem to get a pointer to a physical
// address.  We will use this pointer to read/write the custom peripheral

volatile unsigned int * get_a_pointer(unsigned int phys_addr)
{

	int mem_fd = open("/dev/mem", O_RDWR | O_SYNC); 
	void *map_base = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, phys_addr); 
	volatile unsigned int *radio_base = (volatile unsigned int *)map_base; 
	return (radio_base);
}

int main()
{

// first, get a pointer to the peripheral base address using /dev/mem and the function mmap

    volatile unsigned int *rgb_led= get_a_pointer(DIPS_AND_LEDS_BASEADDR);
    printf("\nTask 4: RGB + LED control w/ switches\n");
    unsigned int counter = 0;
    
while (1)
    {
  	    unsigned int sw_all = *(rgb_led + GPIO_DIP_OFFSET) & 0xF7; //read only sw(2:0)
            unsigned int rgb_code = sw_all & 0x7;
            usleep(500000); 
	  
	   *(rgb_led + GPIO_RGB_OFFSET) = rgb_code;		//set rgb leds 
	   *(rgb_led + GPIO_LED_OFFSET) = counter & 0xF; 	//set led(3:0)
	  
	   counter++;
	   usleep(500000); //500ms delay 
	    // for demonstration purposes, print the value of the DIPS
	    printf("Current Switches = %d\r\n",*(sw_all + GPIO_DIP_OFFSET));
    }
   return 0;
}

