#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LINE_MAX 100
#define DELIM "," // CSV delimiter

char *ifile, *ofile, *unmodified_line; //unmodified to be able to print original before it is altered by strtok
unsigned filter_age_max;
FILE *istream, *ostream;
//just initiating them early, will be assigned later

const char USAGE[] =
    "Filters CSV rows, keeping only those with provided maximum age\n "
    "%1$s max-age [input-file] [output-file]\n"

    "Example: \n"
    "%1$s max-age input.csv output.csv\n"
    "%1$s max-age input.csv (output to stdout)\n"
    "%1$s max-age           (input from stdin, output to stdout)\n";
// My compiler absolutely insists that Raw String Literals are a C++ feature and
// not a C feature, so I could not get it to work. I have changed it to a
// regular string.

void filter_stream(FILE *istream, FILE *ostream) {
  char line[LINE_MAX];
  char *fgets_return;
  char *name, *age_str;
  size_t line_no = 0;

  while (
      // Read a line from `istream` and assign the return value to
      // `fgets_return`
      (fgets_return = fgets(line, LINE_MAX, istream)) != nullptr //assigns fgets_return to a pointer to line unless EOF or some error
  ) {
    ++line_no;
    
    if (fgets_return && *fgets_return != '\n') {
      if (strlen(line) > 1) { // probably the extra strlen > 1 is for longer
                              // blank characters like \r\n ?
                              // Assign `name` and `age_str` using `strtok`
        unmodified_line = line;
        name = strtok(line, ",");
        age_str = strtok(nullptr, ","); //NULL because strtok remembers where it left off, and then "," cos counting on line ending after age or that whatever comes next is at least comma-separated.

        // Alternative to strtok:
        // sscanf(line, "%*[^,],%d", &age);

        if (!age_str) {
          // Error message remember to use error stream however it is you do that.
          fprintf(stderr, "Line %lu: Age missing\n", line_no); //Uses the third stream stderr (that is not stdin or stdout) - replaced perror because I had no defined error numbers for this one (and the next ones)
          continue;
        }
      }
    } else {
      // Error message
      fprintf(stderr, "Line %lu: Whole line missing\n", line_no);
      continue;
    }

    // Age processing
    unsigned age;
    auto recognized_count = sscanf(age_str, "%d", &age); //saves our string age_str as an integer to age instead. recognized_count is 1 if succesful
    if (recognized_count == 1) {
      if (age <= filter_age_max) {
        // Forward input line to `ostream`
        fprintf(ostream, "%s", unmodified_line); //as alternative to fprintf(ostream "%s, %d", line, age) as the requirements specifically say to print the unmodified line.
      }
    } else {
      // Error message
      fprintf(stderr, "Line %lu: Age value is not a number\n", line_no);
    }
  }
}

int main(int argc, char *argv[]) {
  switch (argc) {
  case 4:
    // max-age ifile ofile
    ofile = argv[3];
  case 3:
    // max-age ifile
    ifile = argv[2];
  case 2:
    // max-age
    if (!sscanf(argv[1], "%d", &filter_age_max)) {
      puts("First argument is not an age.\n");
      exit(EXIT_FAILURE);
    }
    break;
  default:
    printf(USAGE, argv[0]);
    return EXIT_SUCCESS;
  }

  if (ifile) {
    // Open `ifile` and assign it to `istream`
    istream = fopen(ifile, "r");

    // Exit program with an error message if file cannot be opened
    if (!istream) {
      perror("Input file could not be opened");
      exit(EXIT_FAILURE); //Get segmentation faults without this one
    }
  } else {
    // Set `istream` if no file provided
    istream = stdin; //sets it to just take the user input
  }

  if (ofile) {
    // Open `ofile` and assign it to `ostream`
    ostream = fopen(ofile, "w");

    // Exit program with an error message if file cannot be opened
    if (!ostream) {
      perror(
          "Output file could not be opened."); // Will this ever happen though?
                                               // Does it not just create one if
                                               // there is no one?
      exit(EXIT_FAILURE);
    }
  } else {
    // Set `ostream` if no file provided
    ostream = stdout; //just prints to console (I think that is the standard output?)
  }

  filter_stream(istream, ostream);
}