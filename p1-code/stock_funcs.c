// stock_funcs.c: support functions for the stock_main program.

#include "stock.h"

// PROBLEM 1: Allocate a new stock struct and initialize its fields.
// Integer fields like 'count' and 'lo_index' should be initialied to
// -1.  Pointer fields like 'prices' should be initialized to
// NULL. The stock should be heap-allocated using malloc() and
// returned.  Since this is an allocation function, no use of 'free()'
// should appear.
stock_t *stock_new(){
  stock_t *stock = malloc(sizeof(stock_t));
  stock->count = -1;
  stock->data_file=NULL;           // name of the data file stock data was loaded >                    // length of prices array
  stock->prices=NULL;               // array of stock prices at different time poin>
  stock->lo_index=-1;                 // index of the lowest price
  stock->hi_index=-1;                 // index of the highest price
  stock->best_buy=-1;                 // index at which to buy to get best profit
  stock->best_sell=-1;     
  return stock;
}

// PROBLEM 1: Free a stock. Check the 'data_file' and 'prices' fields:
// if they are non-NULL, then free them. Then free the pointer to
// 'stock' itself.
void stock_free(stock_t *stock){
  // WRITE ME
  if((stock)->data_file != NULL){
    free((stock)->data_file);
  }
  if((stock)->prices != NULL){
    free((stock)->prices);
  }
  free(stock);
  return;
}

// PROBLEM 1: Prints data about a stock that is passed in via a
// pointer. Uses the syntax ptr->field to access fields of the struct
// pointed by 'stock'.  Output follows the general convention:
//
// ==STOCK DATA==
// data_file: data/stock-jagged.txt                      
// count: 15
// prices: [103.00, 250.00, 133.00, ...]
// lo_index:  8
// hi_index:  11
// best_buy:  8
// best_sell: 11
// profit:    232.00
//
// Each line prints a field of the stock_t struct. In all cases,
// floating point numbers are printed with 2 decimal digits of
// accuracy.
//
// NULLS FOR FIELDS
// The fields 'data_file' and 'prices' may be NULL in which case they
// will be printed specially as in
// 
// data_file: NULL
// prices: NULL
//
// This requires a manual if/else check for NULL values for these
// pointers.
// 
// PRICES FIELD
// When printing the 'prices' field, if the 'count' field is 0 to 3,
// print the entire array as in
//
// prices: []                        # count == 0
// prices: [70.00]                   # count == 1
// prices: [50.00, 90.00]            # count == 2
// prices: [59.00, 45.00, 103.00]    # count == 3
//
// Otherwise, print the first 3 elements with a ... at the end as in
//
// prices: [10.00, 20.00, 30.00, ...] # count > 3
//
// PROFIT
// There is no 'profit' field in the struct. Instead, calculate the
// profit as the difference between the price at the 'best_sell' index
// and 'best_buy' index.  If these indices are -1 indicating the best
// buy/sell time is not known or not viable, print a proit of 0.0
void stock_print(stock_t *stock){
  // WRITE ME
  printf("==STOCK DATA==\n");
  if((stock)->data_file == NULL){//checks null case
    printf("data_file: NULL\n");
  }
  else{
    printf("data_file: %s\n",stock->data_file);
  }
  printf("count: %d\n",stock->count);
  if((stock)->prices == NULL){
    printf("prices: NULL\n");;
  }
  else{//loop that goes through the prices array and prints them correctly 
    int counter = 0;
    
    printf("prices: [");
    
    if(stock->count<=3){
      while(counter<stock->count){
      if(counter==(stock->count-1)){
        printf("%.2lf",stock->prices[counter]);
	counter++;
      }
      else{
        printf("%.2lf, ",stock->prices[counter]);//only go to 2 decimal places
        counter++;
      }
      }
    printf("]\n");

    }
    else{
    while(counter<3){
      printf("%.2lf, ",stock->prices[counter]);
      counter++;
    }
    printf("...]\n");

  }
  }//printing the rest of the values
  printf("lo_index: %d\n",stock->lo_index);
  printf("hi_index: %d\n",stock->hi_index);
  printf("best_buy: %d\n",stock->best_buy);
  printf("best_sell: %d\n",stock->best_sell);
  if(stock->best_sell==-1 || stock->best_buy==-1){//skips the calculation for profit if either value is -1
  printf("profit:    0.00\n");
  }
  else{//calculating the profit
    double x = (stock->prices[stock->best_sell])-(stock->prices[stock->best_buy]);
    printf("profit:    %.2lf\n",x);
  }
   
  return;
}


// PROBLEM 1: Sets the index of 'lo_index' and 'hi_index' fields of
// the stock to be the positions in 'prices' of the lowest and highest
// values present in it. Uses a simple loop over the array 'prices'
// which is 'count' elements long to examine each for high/low. If
// 'count' is zero, makes no changes to 'lo_index' and 'hi_index'.
void stock_set_hilo(stock_t *stock){
  // WRITE ME
  int counter = 1;//counter for the maximum while loop
  int counterm = 1;//counter for the minimum while loop
  int index_max = 0;
  int max = stock->prices[0];
  int index_min = 0;
  int min = stock->prices[0];
  if(stock->count!=0){//loop finds the max value
    while(counter<stock->count){
    if(max<stock->prices[counter]){
      max = stock->prices[counter];
      index_max = counter;
    }
    counter++;
  }
  
  while(counterm<stock->count){//loop that finds the min value
    if(min>stock->prices[counterm]){
      min = stock->prices[counterm];
      index_min = counterm;
    }
    counterm++;
  }//setting the values
  stock->lo_index = index_min;
  stock->hi_index = index_max;

  }
  else{//if the prices array has a length of 0, sets vals to -1
    stock->lo_index = -1;
    stock->hi_index = -1;

  } 
  return;
}
  
// PROBLEM 2: Sets the 'best_buy' and 'best_sell' fields of 'stock'.
// This corresponds to the pair which produces the best profit. On
// determining the best buy/sell indices which produce a positive
// profit, sets these fields in 'stock' and returns 0. If there is no
// buy/sell point which would result in a positive profit, sets the
// 'best_buy' and 'best_sell' indices to -1 and returns -1. Always
// calculates the earliest buy/sell pair of indices that would get the
// best profit: if 5,8 and 5,9 and 7,10 all give the same, maximal
// profit, the best buy/sell indices are set to 5,7.
// 
// ALGORITHM NOTES
// One intuitive algorithm to compute this is to try every possible
// buy index (outer loop) and every possible sell index after it
// (inner loop) looking for the maximum profit produced in it. This is
// a O(N^2) algorithm with N=count.  Using this algorithm is a good
// start. Some MAKEUP CREDIT will be awarded for implementing a more
// efficient, O(N) algorithm here. See the specification for more details.
int stock_set_best(stock_t *stock){
  // WRITE ME
  int indexb = 0;//index of best buy
  int indexs = 0;//index of best sell
  int inner = 0;//inner loop counter
  int outer = 0;//outer loop counter
  int pro = 0;//profit
 
  for(outer = 0;outer < stock->count;outer = outer + 1){
    for(inner = 0; inner < stock->count; inner = inner + 1){
    if(outer > inner){
      if((stock->prices[outer]-stock->prices[inner])>pro){//if this combination of prices is greater than the profit, makes that combination the profit
        indexb = outer;
	indexs = inner;
	pro = stock->prices[outer]-stock->prices[inner];
      }
    }
    }
  
  }
  if(pro==0){
    stock->best_buy = -1;
    stock->best_sell = -1;
    return -1;
  }
  else{
    stock->best_buy = indexs;
    stock->best_sell = indexb;
    return 0;
  }
  return 0;
}

// PROBLEM 2: Opens file named 'filename' and counts how many times
// the '\n' newline character appears in it which corresponds to how
// many lines of text are in it.  Makes use of either fscanf() with
// the %c format to read single characters or alternative I/O
// functions like fgetc(). Closes the file before returning a count of
// how many lines are it it.  If for any reason the file cannot be
// opened, prints a message like
//
// Could not open file 'not-there.txt'
//
// and returns -1 to indicate failure.
int count_lines(char *filename){
  // WRITE ME
  FILE *fp = fopen(filename,"r");//open file
  int chars = 0;//character from the file
  int line_count = 0;//lines counter
  if(fp == NULL){//if the file is null 
    printf("Could not open file '%s'\n",filename);
    return -1;
  }
  
  while(!feof(fp)){//while not at the end of file
    chars = fgetc(fp);
    if(chars == '\n'){
      line_count++;
    }
  }
  fclose(fp);
  return (line_count);
  
}

// PROBLEM 2: Loads a stock from file 'filename' into 'stock' filling
// its 'prices' and 'count' fields in. Makes use of the count_lines()
// function to determine how many lines are in the file which will
// dictate 'count' and the length of 'prices'. Allocates space in the
// heap for the stock's 'prices' array, opens the 'filename' and
// iterates through reading prices into the array. The data format for
// prices files is
//
// time_03 133.00
// time_04 143.00
// time_05 168.00
// time_06 91.00
// 
// where each line has a time as as single string and a price which is
// floating point number. The times can be ignored which can be
// accomplished with a fscanf() call with format "%*s" to read a
// string but ignore/discard it.
// 
// Assigns the 'datafile' field to be a duplicated string of
// 'filename' for which 'strdup()' is extremely useful. This string
// must be free()'d later likely in 'stock_free()'
// 
// On successfully loading the stock, returns 0.
//
// If 'filename' cannot be opened, prints the message 
// 
// Unable to open stock file 'some-stock.txt', bailing out
//
// with 'filename' substituted in for the name of the stock and
// returns -1.
int stock_load(stock_t *stock, char *filename){
  int counter = count_lines(filename);//uses count_lines to get count
  stock->count = counter;  
  FILE *fp = fopen(filename,"r");
  if(fp == NULL){//if the file is null 
    printf("Unable to open stock file '%s', bailing out\n",filename);
    return -1;
  }
  double *nums = malloc(counter*sizeof(double));//allocating space in heap
  for(int i = 0;i<counter;i++){//loop to put the prices in the array
     fscanf(fp,"%*s %lf",&nums[i]);
  }
		  	  
  stock->data_file = strdup(filename);
  stock->prices = nums;
  fclose(fp);
  
  return 0;
}

// PROBLEM 2: Plots a graphical representation of stock
// information. First calculates and prints plot which is in the
// following format:
//
// ==PLOT DATA==
// start/stop:  0 15
// max_height:  14
// price range: 309.00
// plot step:   22.07
//            +--B=S----------+
//     300.93 |    H   *      |
//     278.86 | *  H   *      |
//     256.79 | *  H   *      |
//     234.71 | *  H   *      |
//     212.64 |**  H   *      |
//     190.57 |**  H * *      |
//     168.50 |**  H** *  *  *|
//     146.43 |**  H** *  ****|
//     124.36 |**  H****  ****|
//     102.29 |**  H****  ****|
//      80.21 |** *H***** ****|
//      58.14 |** *H***** ****|
//      36.07 |** *H***** ****|
//      14.00 |****H*****L****|
//            +^----^----^----+
//             0    5    10   
// 
// Here brief notes on the format with more detail in the project
// specification.
// - Parameters start, stop, and max_height are printed first along with
//   calculated information like the price_range (uses hi_index and
//   lo_index)
// - The main body of the plot is exactly max_height characters high. The
//   lowest line is the price at lo_index; the highest is hi_index minus
//   plot_step
// - The vertical axis prices can be printed with the format specifier
//   %10.2f to give the correct leading whitespace with 2 digits of
//   accuracy
// - If the hi and lo prices appear in the range, their bar is printed
//   with an H or an L while all other stocks are printed with a *
// - The top axis may include a B and an S at the positions of the
//   best_buy and best_sell index with a = between these to show the
//   period of ownership.
// - The bottom axis shows tic marks as ^ and below them index labels at
//   values divisible by 5. For the numeric index labels, it is easiest
//   to print spaces to a value divisible by 5 then use the format
//   specifier %-5d to print integers: this aligns left and prints
//   whitespace padding on the right to width 5. In a loop, one can
//   advance by +5 each time a label is printed.
void stock_plot(stock_t *stock, int max_height, int start, int stop){
  printf("==PLOT DATA==\n");
  printf("start/stop: %d %d\n",start,stop);
  printf("max_height: %d\n",max_height);
  printf("price range: %.2lf\n", ((stock->prices[stock->hi_index])-(stock->prices[stock->lo_index])));
  printf("plot step: %.2lf\n",(((stock->prices[stock->hi_index])-(stock->prices[stock->lo_index]))/max_height));//just printing the first part
  for(int d = start;d<stop;d=d+1){//first line starts with a +
   if(d==start&&d!=stock->best_buy){printf("           +");}
   else if(d==start&&d==stock->best_buy){printf("           +B");}
   
   if(d==stock->best_buy&&d!=start){printf("B");
    //  x = true;
   }
   if(d==stock->best_sell){printf("S");
   // x = false;
   }
   if(d>stock->best_buy && d<stock->best_sell ){printf("=");}
   else if(d<stock->best_buy||d>stock->best_sell){
    printf("-");}    
  }
    printf("+\n");  
  double tresh = 0.0;//threshold
  int out = 0;
  
  double counter = (((stock->prices[stock->hi_index])-(stock->prices[stock->lo_index])) - (((stock->prices[stock->hi_index])-(stock->prices[stock->lo_index]))/max_height))+stock->prices[stock->lo_index];
  //^number that gets printed out in front of plot, plot_range + lo price
  for(out = max_height-1; out>=0; out = out-1){//loop that creates the stock plot
    tresh = (out*(((stock->prices[stock->hi_index])-(stock->prices[stock->lo_index]))/max_height))+stock->prices[stock->lo_index];
    if(counter<0.0){printf("           0.00 |");}
    else{printf("%10.2f |",(counter));}
    for(int i = start; i<stop; i=i+1){
      if(i==stop-1){
       if(i==stock->hi_index){
        printf("H|");
      }
     else if(i==stock->lo_index){
        printf("L|");
      }
     else{
      if(stock->prices[i]>=tresh){
        printf("*|");
      }
      else{
        printf(" |");
      }
      }
      }
      else{
      if(i==stock->hi_index){
        printf("H");
      }
     else if(i==stock->lo_index&&out==0){
        printf("L");
      }
     else{
      if(stock->prices[i]>=tresh){
        printf("*");
      }
      else{
	printf(" ");
      }
     }
     }
    }
      printf("\n");
      counter = counter - (((stock->prices[stock->hi_index])-(stock->prices[stock->lo_index]))/max_height);
  } 
  printf("           +");
 for(int d = start;d<stop;d=d+1){//bottom line of plot
   if(d % 5==0){printf("^");}
   else{printf("-");}
 }
  printf("+\n");
  for(int d = start;d<stop;d=d+1){//print numbers at bottom 
   if(d==start){printf("            ");}
   if(d % 5==0){printf("%d",d);}
   else{printf(" ");}
 }
  
  return;

} 
