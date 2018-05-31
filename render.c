#include <png.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
  if(argc < 2){
    printf("Syntax:termpng filename.png -pattern\n");
    return 1;
  }
  FILE* fp = fopen(argv[1], "rb");
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
  char* pattern = "█";
  for(int i = 0; i < argc; i++){
    if(argv[i][0] == '-'){
      pattern = argv[i] + 1;
    }
  }
  for(int y = 0; y < height; y++){
    printf("\n");
    for(int x = 0; x < width * 3; x += 3){
      int r,g,b;
      r =(int) row_pointers[y][x];
      g = (int) row_pointers[y][x + 1];
      b =(int) row_pointers[y][x + 2];
      printf("\x1b[38;2;%d;%d;%dm",r,g,b);
      printf("%s",pattern);
      printf("\x1b[0m");
    }
  }
  
  printf("\n");
  fclose(fp);
  for(int y = 0; y < height; y++){
    free(row_pointers[y]);
  }
  
  return 0;
}
