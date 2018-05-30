#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
int main(){
  FILE* fp = fopen("emoji.png", "rb");
  if(!fp){
    printf("Can not open file \n");
    return 1;
  }
  char header[1];
  int number = 1;
  fread(header,1,number,fp);
  int is_png = !png_sig_cmp(header,0, number);
  if(!is_png){
    printf("File is not a png \n");
    return 1;
  }
  png_voidp user_error_ptr, user_error_fn, user_warning_fn;
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
					       (png_voidp) user_error_ptr,
					       user_error_fn,
					       user_warning_fn);
  if(!png_ptr){
    printf("Error creating read struct \n");
    return 1;
  }
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr){
    png_destroy_read_struct(&png_ptr,
			    (png_infopp)NULL,
			    (png_infopp)NULL);

    printf("Error creating info ptr\n");
    return 1;
  }
  png_infop end_info = png_create_info_struct(png_ptr);
  if(!end_info){
    png_destroy_read_struct(&png_ptr, &info_ptr,
			    (png_infopp)NULL);
    printf("Error creating end info ptr \n");
    return 1;
    
  }
  png_init_io(png_ptr,fp);
  png_set_sig_bytes(png_ptr,number);

  int png_transforms = PNG_TRANSFORM_STRIP_ALPHA;
  png_read_png(png_ptr,info_ptr,png_transforms,NULL);

  int width = png_get_image_width(png_ptr,info_ptr);
  int height = png_get_image_height(png_ptr,info_ptr);
  png_bytep row_pointers[height];
  for(int y = 0; y < height; y++){
    row_pointers[y] = malloc( sizeof(png_byte)*width*3 );  
  }
  for(int y = 0; y < height; y++){
    row_pointers[y] = png_get_rows(png_ptr, info_ptr)[y];
  }


  //initscr();
  for(int y = 0; y < height; y++){
    printf("\n");
    for(int x = 0; x < width * 3; x += 3){
      int r,g,b;
      r =(int) row_pointers[y][x];
      g = (int) row_pointers[y][x + 1];
      b =(int) row_pointers[y][x + 2];
      //printf("\x1b]4;rgb:%2.2x/%2.2x/%2.2x\x1b\\",r,g,b);
      //printf("\x1b[48;5;rgb; %dm",r,g,b);
      printf("\x1b[38;2;%d;%d;%dm",r,g,b);
      printf("#");
      printf("\x1b[0m");
    }
  }
  //endwin();
 
  printf("Exiting without error\n");
  return 0;
}
