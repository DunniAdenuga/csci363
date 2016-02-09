#include "cookies.h"

/*-----------------------------------------------------------------------
 * init_cookies: read the cookie file into a string
 *-----------------------------------------------------------------------
 */
void 
init_cookies(char * fname) {

  // get file size so we can allocate proper amount of memory
  int size = get_file_size(fname);
  if (size <= 0) {
    perror("cookie file name error");
    exit(1);
  }

  // allocate memory for the cookie string and its working buffer
  cookies = malloc(size + 1);
  cookie_buf = malloc(size + 1);

  // read the file content into the cookie string
  int n = read_file(fname, cookies, size);
  if (n <= 0) {
    perror("cookie file error");
    exit(1);
  }
  cookies[n] = 0; // terminate the cookie string

  // count cookie pieces
  cookie_count = count_cookies();
}

/*
 *---------------------------------------------------
 * count_cookies: count the number of cookies in the string.
 *---------------------------------------------------
 */
int
count_cookies() {

  /*
   * Cookies are stored as a 1-D string, each piece of cookie
   * is separated by a "%" on its own line, except the one at 
   * the very end that doesn't have the "%" sign.
   */
  int count = 0;
  int len = strlen(cookies);
  int i;
  for (i = 0; i < len - 1; i ++)
    if (cookies[i] == '%' && cookies[i+1] == '\n')
      count ++;
  count ++;
  return count;
}

/*
 *---------------------------------------------------
 * get_cookie: return the ith cookie
 *---------------------------------------------------
 */
char *
get_cookie(int i) {

  if (i < 0)
    return get_i_th_cookie(0);
  i = i % cookie_count;  // so i is within the range
  return get_i_th_cookie(i);
}

/*
 *---------------------------------------------------
 * get_i_th_cookie: return the ith cookie
 *---------------------------------------------------
 */
char *
get_i_th_cookie(int i) {

  int k;
  int s = 0;   // start
  int e = s;   // end

  for (k = 0; k <= i; k ++) {
    while ((cookies[e] != 0) && 
	   !(cookies[e] == '%' && cookies[e+1] == '\n'))
      e ++;
    if (cookies[e] != 0 && k < i) {
      s = e;  // move to next cookie
      e ++;
    }
  }

  int len;
  if (cookies[s] == '%')    // cookie separators, skip over
    s += 2;
  len = e - s;
  strncpy(cookie_buf, &(cookies[s]), len);
  cookie_buf[len] = 0;

  return cookie_buf;
}

/*
 *---------------------------------------------------
 * get_file_size: return the size of a given file.
 *---------------------------------------------------
 */
int 
get_file_size(char * fname) {
  
  // default file size
  int file_size   = 0;
  int file_status = 0; 
  // structure to contain the result of the fstat call (info on this file)
  struct stat file_info;

  // try to access file
  file_status = stat(fname, &file_info);
  if (file_status != 0) { // stat() call failed
    file_size = 0;
  } else {                // extract file size
    file_size = file_info.st_size;
  }

  return file_size;
}

/*-----------------------------------------------------------------------
 * read_file: read content of given file name, pass back the buff
 *-----------------------------------------------------------------------
 */
int
read_file(char * fname, char * buff, int buff_size) {

  FILE * f = fopen(fname, "r");
  
  int size_read = fread(buff, 1, buff_size, f);
  if (size_read > 0) {
    buff[size_read] = 0;
  }
  fclose(f);

  return size_read;
}

void freeCookie(void){
  free(cookies);
  free(cookie_buf);
}
