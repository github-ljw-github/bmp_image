#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#ifndef u8
#define u8 unsigned char
#endif
#ifndef u16
#define u16 unsigned short
#endif
#ifndef u32
#define u32 unsigned int
#endif
#define BMP_COLOR_R 0xFF0000
#define BMP_COLOR_G 0x00FF00
#define BMP_COLOR_B 0x0000FF
/*can be used only after bmp_init()*/
#define SET_BMP_SIZE(bmp, width, height) do {\
(bmp)->info.bi_width=(width);\
(bmp)->info.bi_height=(height);\
(bmp)->info.bi_size_image=(bmp)->info.bi_width*(bmp)->info.bi_height*(bmp)->info.bi_bit_count/8;\
(bmp)->head.bf_size = (bmp)->info.bi_size_image+(bmp)->head.bf_offset;\
}while(0)
struct bmp_header {
  u16 bf_type;
  u32 bf_size;
  u32 bf_reserved;
  u32 bf_offset;
};
struct bmp_color_table {
  /*nothing*/
};
struct bmp_bi_info {
  u32 bi_size;
  u32 bi_width;
  u32 bi_height;
  u16 bi_planes;
  u16 bi_bit_count;
  u32 bi_compression;
  u32 bi_size_image;
  u32 bi_x_pels_per_meter;
  u32 bi_y_pels_per_meter;
  u32 bi_clr_used;
  u32 bi_clr_important;
};
struct bmp {
  struct bmp_header head;
  struct bmp_color_table table;
  struct bmp_bi_info info;
  u8 *rgb;
};
int bmp_read_info(FILE *filp, struct bmp *image)
{
  struct bmp_header *h = NULL;
  struct bmp_bi_info *i = NULL;
  if((image || filp) == 0)
    return (-1);
  h = &image->head;
  i = &image->info;
  rewind(filp);
  if(fread(&h->bf_type,sizeof(h->bf_type),1,filp) != 1)
    return (-1);
  if(fread(&h->bf_size,sizeof(h->bf_size),1,filp) != 1)
    return (-1);
  if(fread(&h->bf_reserved,sizeof(h->bf_reserved),1,filp) != 1)
    return (-1);
  if(fread(&h->bf_offset,sizeof(h->bf_offset),1,filp) != 1)
    return (-1);
  if(fread(&i->bi_size,sizeof(i->bi_size),1,filp) != 1)
    return (-1);
  if(fread(&i->bi_width,sizeof(i->bi_width),1,filp) != 1)
    return (-1);
  if(fread(&i->bi_height,sizeof(i->bi_height),1,filp) != 1)
    return (-1);
  if(fread(&i->bi_planes,sizeof(i->bi_planes),1,filp) != 1)
    return (-1);
  if(fread(&i->bi_bit_count,sizeof(i->bi_bit_count),1,filp) != 1)
    return (-1);
  if(fread(&i->bi_compression,sizeof(i->bi_compression),1,filp) != 1)
    return (-1);
  if(fread(&i->bi_size_image,sizeof(i->bi_size_image),1,filp) != 1)
    return (-1);
  if(fread(&i->bi_x_pels_per_meter,sizeof(i->bi_x_pels_per_meter),1,filp) != 1)
    return (-1);
  if(fread(&i->bi_y_pels_per_meter,sizeof(i->bi_y_pels_per_meter),1,filp) != 1)
    return (-1);
  if(fread(&i->bi_clr_used,sizeof(i->bi_clr_used),1,filp) != 1)
    return (-1);
  if(fread(&i->bi_clr_important,sizeof(i->bi_clr_important),1,filp) != 1)
    return (-1);

  return 0;
}
void bmp_init(struct bmp *image)
{
  struct bmp_header *h = NULL;
  struct bmp_bi_info *i = NULL;
  if((image) == 0)
    return;
  h = &image->head;
  i = &image->info;
  h->bf_type = 19778;
  h->bf_reserved = 0;
  h->bf_offset = 54;
  i->bi_size = 40;
  i->bi_width = 1920;
  i->bi_height = 1080;
  i->bi_planes = 1;
  i->bi_bit_count = 24;
  i->bi_compression = 0;
  i->bi_size_image = i->bi_width*i->bi_height*i->bi_bit_count/8;
  i->bi_x_pels_per_meter = 0;
  i->bi_y_pels_per_meter = 0;
  i->bi_clr_used = 0;
  i->bi_clr_important = 0;
  h->bf_size = i->bi_size_image+h->bf_offset;
  image->rgb = 0;
  return;
}
void bmp_deinit(struct bmp *image)
{
  if(image == 0)
	return;
  if(image->rgb)
	free(image->rgb);
}
int bmp_make_image(FILE *filp, struct bmp *image, u32 color)
{
  struct bmp_header *h = NULL;
  struct bmp_bi_info *i = NULL;
  if((image || filp) == 0)
    return (-1);
  h = &image->head;
  i = &image->info;
  rewind(filp);
  if(fwrite(&h->bf_type,sizeof(h->bf_type),1,filp) != 1)
    return (-1);
  if(fwrite(&h->bf_size,sizeof(h->bf_size),1,filp) != 1)
    return (-1);
  if(fwrite(&h->bf_reserved,sizeof(h->bf_reserved),1,filp) != 1)
    return (-1);
  if(fwrite(&h->bf_offset,sizeof(h->bf_offset),1,filp) != 1)
    return (-1);
  if(fwrite(&i->bi_size,sizeof(i->bi_size),1,filp) != 1)
    return (-1);
  if(fwrite(&i->bi_width,sizeof(i->bi_width),1,filp) != 1)
    return (-1);
  if(fwrite(&i->bi_height,sizeof(i->bi_height),1,filp) != 1)
    return (-1);
  if(fwrite(&i->bi_planes,sizeof(i->bi_planes),1,filp) != 1)
    return (-1);
  if(fwrite(&i->bi_bit_count,sizeof(i->bi_bit_count),1,filp) != 1)
    return (-1);
  if(fwrite(&i->bi_compression,sizeof(i->bi_compression),1,filp) != 1)
    return (-1);
  if(fwrite(&i->bi_size_image,sizeof(i->bi_size_image),1,filp) != 1)
    return (-1);
  if(fwrite(&i->bi_x_pels_per_meter,sizeof(i->bi_x_pels_per_meter),1,filp) != 1)
    return (-1);
  if(fwrite(&i->bi_y_pels_per_meter,sizeof(i->bi_y_pels_per_meter),1,filp) != 1)
    return (-1);
  if(fwrite(&i->bi_clr_used,sizeof(i->bi_clr_used),1,filp) != 1)
    return (-1);
  if(fwrite(&i->bi_clr_important,sizeof(i->bi_clr_important),1,filp) != 1)
    return (-1);  
  if(image->rgb) {
    if(fwrite(image->rgb,i->bi_size_image,1,filp) != 1)
      return (-1); 	
  }
  else {
	int bpp = i->bi_bit_count/8;
	int loop_time = i->bi_size_image / bpp;
    int i;
	for(i=0; i<loop_time; i++) {
      if(fwrite(&color, bpp, 1, filp) != 1)
		return (-1);
	}
  }
  return 0;
}
int bmp_read_rgb(FILE *filp, struct bmp *image)
{
  struct bmp_header *h = NULL;
  struct bmp_bi_info *i = NULL;
  if((filp || image) == 0)
	return (-1); 
  if(image->rgb == 0)
	return (-1);
  h = &image->head;
  i = &image->info;
  if(fseek(filp, (long int)h->bf_offset, SEEK_SET) == -1)
    return (-1);
  if(fread(image->rgb,i->bi_size_image,1,filp) != 1)
    return (-1);
  return 0;
}
void bmp_show_info(struct bmp *image)
{
  struct bmp_header *h = NULL;
  struct bmp_bi_info *i = NULL;
  if(image == 0)
	return;
  h = &image->head;
  i = &image->info;
  printf("\n");
  printf("bf_type = %u\n",h->bf_type);
  printf("bf_size = %u\n",h->bf_size);
  printf("bf_reserved = %u\n",h->bf_reserved);
  printf("bf_offset = %u\n",h->bf_offset);
  printf("bi_size = %u\n",i->bi_size);
  printf("bi_width = %u\n",i->bi_width);
  printf("bi_height = %u\n",i->bi_height);
  printf("bi_planes = %u\n",i->bi_planes);
  printf("bi_bit_count = %u\n",i->bi_bit_count);
  printf("bi_compression = %u\n",i->bi_compression);
  printf("bi_size_image = %u\n",i->bi_size_image);
  printf("bi_x_pels_per_meter = %u\n",i->bi_x_pels_per_meter);
  printf("bi_y_pels_per_meter = %u\n",i->bi_y_pels_per_meter);
  printf("bi_clr_used = %u\n",i->bi_clr_used);
  printf("bi_clr_important = %u\n",i->bi_clr_important);
}
int bmp_main(int argc, char *argv[])
{
  FILE *filp = NULL;
  struct bmp image;
  u32 color[] = {BMP_COLOR_R, BMP_COLOR_G, BMP_COLOR_B};
  char filename[][512] = {"bmp_red", "bmp_green", "bmp_blue"};
  int i = 0;
  
  bmp_init(&image);
  for(i=0; i<3; i++) {
    filp = fopen(filename[i],"w+");
    if(filp == NULL) {
	  perror("fopen");
	  exit(-1);
    }
    if(bmp_make_image(filp, &image, color[i]) != 0) {
	  printf("bmp_make_image error.\n");
    }
    fclose(filp);
  }

  filp = fopen(filename[0],"r");
  if(filp == NULL) {
	  perror("fopen");
	  exit(-1);
  }
  bmp_read_info(filp, &image);
  bmp_show_info(&image);
  image.rgb = calloc(1, image.info.bi_size_image);
  bmp_read_rgb(filp, &image);
  fclose(filp);
  filp = fopen("bmp_copy.bmp","w+");
  if(filp == NULL) {
	  perror("fopen");
	  exit(-1);
  }  
  bmp_make_image(filp, &image, 0);
  fclose(filp);
  
  SET_BMP_SIZE(&image, 1920, 720);
  filp = fopen("bmp_copy2.bmp","w+");
  if(filp == NULL) {
	  perror("fopen");
	  exit(-1);
  }  
  bmp_make_image(filp, &image, 0);
  fclose(filp);
  bmp_deinit(&image);  
  
  return 0;
}