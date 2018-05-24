/*-----------------------------------------------------------------------------
 *				HTBLA-Leonding / Class: <your class name here>
 *-----------------------------------------------------------------------------
 * Exercise Number: #exercise_number#
 * File:			lottery.c
 * Author(s):		Alexander Kapsammer
 * Due Date:		#due#
 *-----------------------------------------------------------------------------
 * Description:
 * Implementation of a library for analyzing lottery tips.
 *-----------------------------------------------------------------------------
*/
#include <stdio.h>
#include <stdlib.h>
#include "lottery.h"
#define MAX_TIP_LEN 17
#define UUID_LEN 40
#define MAX_LINE_LEN (UUID_LEN + 1 + MAX_TIP_LEN + 1)

static char* file_name;
static char seperator;
static bool is_drawn = false;
static int* drawing;

bool init_lottery(char* csv_file, char csv_separator){
  FILE* fd = fopen(csv_file, "r");
  seperator = csv_separator;
  is_drawn = false;
  file_name = csv_file;
  if (fd == 0)return false;
  fclose(fd);
  return true;
}
bool get_tip(int tip_number, int tip[TIP_SIZE]){
  FILE* fd = fopen(file_name, "r");
  if (tip_number < 0 || fd == 0) {
    fclose(fd);
    return false;
  }
  char input[MAX_LINE_LEN];
  int count = 0;
  char* file_array = fgets(input, MAX_LINE_LEN, fd);
  while (file_array != 0 && count < tip_number) {
    file_array = fgets(input, MAX_LINE_LEN, fd);
    count++;
    if (file_array != input) {
      fclose(fd);
      return false;
    }
  }
  int arr_pos = 0;
  while (input[arr_pos] != seperator && arr_pos < UUID_LEN) {
    arr_pos++;
  }
  int insert_pos = 0;
  int tens = 0;
  while (insert_pos != 6) {
    arr_pos++;
    if (input[arr_pos] <= '9' && input[arr_pos] >= '0') {
      tens = tens * 10 + (input[arr_pos] - '0');
    }
    else{
      tip[insert_pos] = tens;
      tens = 0;
      insert_pos++;
    }
  }
  fclose(fd);
  return true;
}
bool set_drawing(int drawing_numbers[TIP_SIZE]){
  is_drawn = true;
  drawing = drawing_numbers;
  return false;
}
int get_tip_result(int tip_number){
  int tip[TIP_SIZE];
  if(!is_drawn)return -1;
  if(!get_tip(tip_number, tip))return -2;
  int hits = 0;
  for (int i = 0; i < TIP_SIZE; i++) {
    for (int j = 0; j < TIP_SIZE; j++) {
      if (tip[i] == drawing[j])hits++;
    }
  }
  return hits;
}
int	get_right_tips_count(int right_digits_count){
  if (is_drawn && right_digits_count >= 0 && right_digits_count <= TIP_SIZE) {
    int tip = 0;
    int count = 0;
    int correct_tips = 0;
    do {
      correct_tips = get_tip_result(tip);
      if (correct_tips == right_digits_count) {
        count++;
      }
      tip++;
    } while(correct_tips != -2 && correct_tips != -1);
    return count;
  }
  return -1;
}
