/** @brief Modified from /usr/src/sense-hat/examples/snake/
 *  @file led2472g.c
 *  @since 2024-02-16
 *  C code Demonstration of Astro Pi Sense Hat.
 *  Raspberry Pi Sense HAT add-on board serial, nav pad, and led2472g
 */

#include "led2472g.h"
#include "font.h"

/*compile with gcc led2472g.c, run with ./a.out
int main(void) 
{
    // Output 
    fprintf(stdout,"Unit: %LX\n",ShGetSerial());
    struct fb_t *fb;
    fb=ShInit(fb);

    //ShWipeScreen(0,fb); //same as below
    memset(fb, 0, 128); //ShWipeScreen() is unnecessary, remove in the future

    //memset(fb, MAGENTA, 128); //MAGENTA seems to be light blue, not the same as for a pixel
    //memset(fb, 0, 128);

    //ShLightPixel(4,4,MAGENTA,fb); //MAGENTA color is correct, same as below
    //fb->pixel[4][4] = MAGENTA; //ShLightPixel() is unnecessary, remove in the future

    usleep (300000);

    //Set a test pixel pattern
    fb->pixel[0][5]=BLUE;
    fb->pixel[1][5]=GREEN;
    fb->pixel[2][5]=RED;
    fb->pixel[3][5]=YELLOW;
    fb->pixel[4][5]=MAGENTA;
    fb->pixel[5][5]=CYAN;
    fb->pixel[6][5]=WHITE;
    usleep (300000); // to allow pixels to be set
    ShRotatePattern(90,fb);
    usleep (300000);

    //shtest
    ShViewPattern(logo,fb);
    usleep (300000);
    char str[33];
    sprintf(str,"T: %5.1fC  H: %5.1f%%  P: %6.1fmB",20.0,55.0,1013.0);
    ShViewMessage(str,100,WHITE,BLACK,fb,Tfont,font);

    //Simple drawing program reminiscent of an Etch A Sketch
    struct brush_t brush;
    brush.colourindex=0;
    brush.colours[0]=RED;
    brush.colours[1]=GREEN;
    brush.colours[2]=BLUE;
    brush.colours[3]=YELLOW;
    brush.colours[4]=MAGENTA;
    brush.colours[5]=CYAN;
    brush.colours[6]=WHITE;
    brush.colours[7]=BLACK;


    brush.apple.x = 4;
    brush.apple.y = 4;
    struct pollfd evpoll = {
        .events = POLLIN,
    };

    //printf("Quit by holding down Ctrl and typing a c\n");
    //while(1){
    //    while (poll(&evpoll, 1, 0) > 0){
    //        move_events(evpoll.fd, &brush);
    //    }
    //    fb->pixel[brush.apple.x][brush.apple.y]=brush.colours[brush.colourindex];
    //    usleep(30000);
    //}

    return EXIT_FAILURE;
}*/

/** Brief
 * @since 2024-02-16
 * @param 
 * @return 
 */
uint64_t ShGetSerial(void)
{
    static uint64_t serial = 0;
    FILE * fp;
    char buf[SYSINFOBUFSZ];
    char searchstring[] = SEARCHSTR;
    fp = fopen ("/proc/cpuinfo", "r");
    if (fp != NULL)
    {
        while (fgets(buf, sizeof(buf), fp) != NULL)
        {
            if (!strncasecmp(searchstring, buf, strlen(searchstring)))
            {
                sscanf(buf+strlen(searchstring), "%Lx", &serial);
            }
        }
        fclose(fp);
    }
    if(serial==0)
    {
        system("uname -a");
        system("ls --fu /usr/lib/codeblocks | grep -Po '\\.\\K[^ ]+'>stamp.txt");
        fp = fopen ("stamp.txt", "r");
        if (fp != NULL)
        {
            while (fgets(buf, sizeof(buf), fp) != NULL)
            {
                sscanf(buf, "%Lx", &serial);
            }
            fclose(fp);
        }
    }
    return serial;
}

/** Brief
 * @since 2024-02-16
 * @param 
 * @return 
 */
struct fb_t *ShInit(struct fb_t *fb)
{
    struct pollfd evpoll = {
		.events = POLLIN,
	};
    int fbfd = 0;

    //open up joystick for polling, reading
    evpoll.fd = open_evdev("Raspberry Pi Sense HAT Joystick");
    if (evpoll.fd < 0) {
        fprintf(stderr, "Event device not found.\n");
        exit(evpoll.fd);
    }
    else fprintf(stdout,"Joystick enabled.\n"); 

    //Open up framebuffer
    fbfd = open_fbdev("RPi-Sense FB");
    if (fbfd <= 0) {
        fprintf(stderr,"Error: cannot open framebuffer device.\n");
        exit(fbfd);  		
    }
    else fprintf(stdout,"Framebuffer now open.\n"); 

    //Map sense hat pixel display to the fb data structure	
    fb = mmap(0, 128, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (!fb) {
        fprintf(stderr,"Failed to mmap.\n");
        exit(EXIT_FAILURE); 
    }
    else fprintf(stdout,"Mapped pixels to memory\n"); 

    memset(fb, 0, 128);
    return fb;
}

/** Brief
 * @since 2024-02-16
 * @param 
 * @return 
 */
static int is_event_device(const struct dirent *dir)
{
    return strncmp(EVENT_DEV_NAME, dir->d_name,
            strlen(EVENT_DEV_NAME)-1) == 0;
}

/** Brief
 * @since 2024-02-16
 * @param 
 * @return 
 */
static int is_framebuffer_device(const struct dirent *dir)
{
    return strncmp(FB_DEV_NAME, dir->d_name,
            strlen(FB_DEV_NAME)-1) == 0;
}

/** Brief
 * @since 2024-02-16
 * @param 
 * @return 
 */
static int open_evdev(const char *dev_name)
{
    struct dirent **namelist;
    int i, ndev;
    int fd = -1;

    ndev = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, versionsort);
    if (ndev <= 0)
        return ndev;

    for (i = 0; i < ndev; i++)
    {
        char fname[64];
        char name[256];

        snprintf(fname, sizeof(fname),
            "%s/%s", DEV_INPUT_EVENT, namelist[i]->d_name);
        fprintf(stdout,"Opening in open_evdev:  %s\n", fname); 
        fd = open(fname, O_RDONLY);
        if (fd < 0)
            continue;
        ioctl(fd, EVIOCGNAME(sizeof(name)), name);
        if (strcmp(dev_name, name) == 0)
            break;
        close(fd);
    }

    for (i = 0; i < ndev; i++)
        free(namelist[i]);

    return fd;
}

/** Brief
 * @since 2024-02-16
 * @param 
 * @return 
 */
static int open_fbdev(const char *dev_name)
{
    struct dirent **namelist;
    int i, ndev;
    int fd = -1;
    struct fb_fix_screeninfo fix_info;

    ndev = scandir(DEV_FB, &namelist, is_framebuffer_device, versionsort);
    if (ndev <= 0)
        return ndev;

    for (i = 0; i < ndev; i++)
    {
        char fname[64];
        char name[256];

        snprintf(fname, sizeof(fname),
            "%s/%s", DEV_FB, namelist[i]->d_name);
        fprintf(stdout,"Opening in open_fbdev: %s\n", fname); 
        fd = open(fname, O_RDWR);
        if (fd < 0)
            continue;
        ioctl(fd, FBIOGET_FSCREENINFO, &fix_info);
        if (strcmp(dev_name, fix_info.id) == 0)
            break;
        close(fd);
        fd = -1;
    }
    for (i = 0; i < ndev; i++)
        free(namelist[i]);

    return fd;
}

/** Brief
 * @since 2024-02-16
 * @param 
 * @return 
 */
void move_dir(unsigned int code, struct brush_t *brush)
{
    brush->now = clock();
    if((double)(brush->now-brush->timeoflastcmd)>140){
        switch (code) {
            case KEY_UP:
                brush->apple.x--;
                break;
            case KEY_RIGHT:
                brush->apple.y++;
                break;
            case KEY_DOWN:
                brush->apple.x++;
                break;
            case KEY_LEFT:
                brush->apple.y--;
                break;
            case KEY_ENTER:
                if (brush->colourindex<7){
                    brush->colourindex++;
                }
                else{
                    brush->colourindex=0;
                }
                break;
        }
        brush->timeoflastcmd=brush->now;
    }
}

/** Brief
 * @since 2024-02-16
 * @param 
 * @return 
 */
void move_events(int evfd, struct brush_t *brush)
{
    struct input_event ev[64];
    int i, rd;

    rd = read(evfd, ev, sizeof(struct input_event) * 64);
    if (rd < (int) sizeof(struct input_event)) {
        fprintf(stderr, "expected %d bytes, got %d\n",
            (int) sizeof(struct input_event), rd);
        return;
    }
    for (i = 0; i < rd / sizeof(struct input_event); i++) {
        if (ev->type != EV_KEY)
            continue;
        if (ev->value != 1)
            continue;
        switch (ev->code) {
            default:
                move_dir(ev->code, brush);
        }
    }
}

/** @brief Clears the LED matrix
 *  @param uint16_t color 565 format
 */
void ShWipeScreen(uint16_t color, struct fb_t *fb)
{
    memset(fb, color, 128); //color of 0 is off
}

/** Brief
 * @since 2024-02-16
 * @param 
 * @return 
 */
void ShLightPixel(int row, int column, uint16_t color, struct fb_t *fb)
{
    if(row < 0)
	row = 0;
    if(column < 0)
	column = 0;

    fb->pixel[row%8][column%8] = color;
}

/** @brief Checks if a column on the LED matrix is clear
 */
bool ShEmptyColumn(int numcolumn,uint16_t image[8][8],uint16_t colorBackground)
{
    int i=0;

    for(i=0;i<8;i++)
    {
        if(image[i][numcolumn]!=colorBackground) { return false; }
    }
    return true;
}

/** Brief
 * @since 2024-02-16
 * @param 
 * @return 
 */
void ShImageContainment(int numcolumn,uint16_t image[][8][8], int taille)
{
    int i=0,j=0,k=0,l=0,isuivant,ksuivant;
    int nombredecolumns=taille*8; //8 columns par pattern

    for(l=numcolumn;l<nombredecolumns-1;l++)
    {
        i=l/8;
        k=l%8;
        isuivant=(l+1)/8;
        ksuivant=(l+1)%8;
        for(j=0;j<8;j++)
        {
            image[i][j][k]=image[isuivant][j][ksuivant];
        }
    }
}


/** @brief Displays a pattern on the LED matrix
 *  @param pattern 8*8 of uint16_t
 */
void ShViewPattern(uint16_t pattern[][8], struct fb_t *fb)
{
    for(int row=0; row <8 ; row++)
    {
        for(int column=0 ; column <8 ; column++)
        {
            fb->pixel[row][column] = pattern[row][column];
        }
    }

}

/** @brief Converts a character to an LED matrix pattern.
 */
void ShConvertCharacterToPattern(char c, uint16_t image[8][8], uint16_t colorText, uint16_t colorBackground)
{
    int i=0;
    int j,k;
    int tailleTableDeConvertion=sizeof(font)/sizeof(Tfont);

    // Recherche si le caractere existe dans la table de convertion (cf font.h)
    while(c!=font[i].caractere && i < tailleTableDeConvertion )
	i++;

    // Si le caractere est dans la table on le converti
    if(i < tailleTableDeConvertion)
    {
        for (j=0;j<8;j++)
        {
            for(k=0;k<8;k++)
            {
                if(font[i].binarypattern[j][k]) { image[j][k]=colorText; }
                else { image[j][k]=colorBackground; }
            }
        }
    }
    else // caractère inexistant on le remplace par un glyphe inconnu
    {
        ShConvertCharacterToPattern(255,image,colorText,colorBackground);
    }
}

/** @brief Displays a message on the LED matrix.
 */
void ShViewMessage(const char * message, int vitesseDefilement, uint16_t colorText, uint16_t colorBackground, struct fb_t *fb)
{
    int taille=strlen(message);
    uint16_t chaine[taille][8][8]; /* Le tableau de pattern (image/caractère) à afficher */
    int i=0,j=0,k=0,l=0,nombreDecolumnVide=0;
    int isuivant=0,ksuivant=0,nombreDecolumns=0;

    /* Convertion de tout le message en tableau de patterns
     * format caractère : 1 column vide + 5 columns réellement utilisées
     * + 2 columns vides */
    for( i=0,j=0; i<taille; i++,j++)
    {
        if(message[i]==195)  // les lettres accentuées sont codées sur deux octets  (195 167 pour ç)
        {
            i++;
            k++;
        }
  	// row suivante à décommenter pour obtenir le code des caractères UTF8
	// std::cout << "code : " << (int)message[i] << std::endl;
	ShConvertCharacterToPattern(message[i],chaine[j],colorText,colorBackground);
    }
	taille = taille - k;
	nombreDecolumns=(taille)*8-2;
	k = 0;
	// Parcours de toutes les columns de tous les patterns qui compose
	// la chaine de caractères à afficher pour supprimer les columns vides sauf celle
	// qui sépare les patterns (caractères). + gestion du caractère espace.
	for( l=0;l<nombreDecolumns;l++)
	{
		i=l/8;
		k=l%8;

		if(ShEmptyColumn(k,chaine[i],colorBackground)) // Une column Vide avant chaque caractère à ne pas supprimer

		{
			isuivant=(++l)/8;
			ksuivant=(l)%8;
			nombreDecolumnVide=1;

			// compter les columns vide après la première afin de les supprimer
			// si plus de 4 c'est le caractère espace que l'on doit garder
			while(ShEmptyColumn(ksuivant,chaine[isuivant],colorBackground) && nombreDecolumnVide++ < 6)
			{
				ShImageContainment(l,chaine,taille);
				nombreDecolumns--;
			}

		}
	}

	// Parcours de toutes les columns de tous les patterns qui composent
	// la chaine de caractères à afficher (sans les columns vides superflues).
	for( l=0; l<nombreDecolumns; l++)
	{
		// Decalage des columns vers la gauche sur l'image Numero 0 (celle qu'on affiche sur la matrice de LED
		for( i=0;i<taille;i++)
		{
			// Cas Normal, les columns sont sur le même pattern
			for(j=0;j<8;j++)
			{
				for(k=0;k<7;k++)
					chaine[i][j][k]=chaine[i][j][k+1];

			}
			// Cas où l'on doit changer de pattern
			for(j=0;j<8;j++)
			{
				chaine[i][j][7]=chaine[i+1][j][0];
            }
		}
		usleep(1000*vitesseDefilement);
		ShViewPattern(chaine[0], fb);
	}

}

/** @brief Rotates a pattern on the LED matrix
 *  @param int rotation angle 90, 180, 270, -90, -180, -270
 */
void ShRotatePattern(int angle, struct fb_t *fb)
{
    uint16_t tabAux[8][8];

    for(int row=0; row <8 ; row++)
    {
        for(int column=0 ; column <8 ; column++)
        {
            switch(angle)
            {
                case   90:
                case -270:
                    tabAux[7 - column][row] = fb->pixel[row][column];
                    break;
                case  180:
                case -180:
                    tabAux[7 - row][7 - column] = fb->pixel[row][column];
                    break;
                case  270:
                case  -90:
                    tabAux[column][7 - row] = fb->pixel[row][column];
                    break;
                default:
                    tabAux[row][column] = fb->pixel[row][column];
            }
        }
    }

    ShViewPattern(tabAux, fb);
}

