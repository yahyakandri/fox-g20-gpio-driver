#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>


#define LOW  0
#define HIGH 1
#define IN   1
#define OUT  0

struct S_GPIO_LINE {
    char id_line[4];
    int id_pin;
    int direction;
    int value;
    
    
};
typedef struct S_GPIO_LINE S_GPIO_LINE;

/* ********************************** */
/* LOAD_GPIO_LINE                     */
/* Initialisation of a GPIO line      */
/* ********************************** */
int load_gpio_line(S_GPIO_LINE *ps_line )
{
    FILE *p_gpio_line;

    /* Exporting GPIO line */
    if ((p_gpio_line = fopen("/sys/class/gpio/export", "w")) == NULL)
    {
        printf("Cannot open export file.\n");
        exit(1);
    }
  	fprintf(p_gpio_line,"%d",ps_line->id_pin);
    fclose(p_gpio_line);

    return 0;
}

/* ****************************************** */
/* SET_GPIO_DIRECTION                         */
/* Sets the direction (IN/OUT) of a GPIO line */
/* ****************************************** */
int set_gpio_direction(S_GPIO_LINE *ps_line, int i_direction)
{
    int fd;//file 
    char path[35];//chemin
    

    snprintf(path,35, "/sys/class/gpio/pio%s/direction",ps_line->id_line);
    printf("path:%s\n",path);
fd = open(path, O_WRONLY);
	if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing!\n");
		 exit(1);
	}
	 if( i_direction ) {
	if (-1 == write(fd, "in", 36)) {
		fprintf(stderr, "Failed to set direction!\n");
		 exit(1);
	}
    }
    else {
	if (-1 == write(fd, "out", 36)) {
		fprintf(stderr, "Failed to set direction!\n");
		 exit(1);
	}
}
    return 0;
}


/* ***************************** */
/* SET_GPIO_LINE                 */
/* Sets the value of a GPIO line */
/* ***************************** */
int set_gpio_line(S_GPIO_LINE *ps_line, int value)
{
    FILE *p_gpio_value;
    char gpio_value[35];
    char c_value[2];

    if( ps_line->direction == 0 ) {
        sprintf(gpio_value, "/sys/class/gpio/pio%s/value",ps_line->id_line);
        
        /* Setting value */
        if ((p_gpio_value = fopen(gpio_value, "w")) == NULL)
        {
            printf("Cannot open value file.\n");
            exit(1);
        }
        rewind(p_gpio_value);
        sprintf(c_value, "%d", value);
        ps_line->value = value;
        fwrite(&c_value, sizeof(char), 1, p_gpio_value);
        fclose(p_gpio_value);
    }
    else{
        printf("Wrong access.\n");
        exit(1);
    }
    return 0;
}

/* ****************************************** */
/* GET_GPIO_LINE                              */
/* Gets value of a GPIO line                  */
/* ****************************************** */
int get_gpio_line(S_GPIO_LINE *ps_line)
{
    FILE *p_gpio_value;
    char gpio_value[35];
    int value = 0;

    if( ps_line->direction == 1 ) {
        sprintf(gpio_value, "/sys/class/gpio/pio%s/value", ps_line->id_line);
		printf("***");
        /* Getting value */
        if ((p_gpio_value = fopen(gpio_value, "r")) == NULL)
        {
            printf("Cannot open value file.\n");
            exit(1);
        }

        value = fgetc (p_gpio_value) - 48;

        fclose(p_gpio_value);
    }
    else{
        printf("Wrong access.\n");
        exit(1);
    }

    return value;
}

/* ****************************************** */
/* exemple allumer une led                     */
/* ****************************************** */
int main() {

    S_GPIO_LINE s_line9;
    s_line9.id_pin=53;
   sprintf(s_line9.id_line, "B21");//voir sur la feuille
   
    int sw = 1;

    load_gpio_line(&s_line9);  
    set_gpio_direction(&s_line9, OUT);
    
    while(1)
    {
        set_gpio_line(&s_line9, sw);    
        sleep(3);
        set_gpio_line(&s_line9, 0);    
        sleep(1);
    }

    return 0;
}

