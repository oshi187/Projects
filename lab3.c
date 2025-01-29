/** Doxygen-Info
 * @file lab3.c
 * @author Oshini Jayaweera
 * @author Prof. Dr. Rainer Schoenen
 * @date SS2023 04/06/2024
 * @version 20230517
 * @brief HAW SS2023 SOL1 Lab3
 * Detailed description:
 *   Practise the following skills:
 *   - from mathematical formulas to algorithms
 *   - implement your own trigonometric function - that's how math works in a computer
 *   - math determine zeroes by BISECTION
 *   - math differentiation
 *   - using libraries, i.e., several C and H code files in a project
 *   - unit testing
 */

 /* all the important includes and settings ... */
#define _CRT_SECURE_NO_WARNINGS
#define PI 3.14159265358979323846
#include <stdio.h> /* printf(), scanf() */
#include <string.h>
#include <ctype.h> /* for tolower() */
#define _USE_MATH_DEFINES /* for M_PI */
#include <math.h>
#include <stdlib.h> /* for rand() */
#include <time.h>   /* for time() in srand() */
#include <stdint.h> /* for int16_t */
#include <limits.h> /* for SHRT_MAX etc */
#include <conio.h> /* for getch()  - direct console I/O, i.e., keyboard input */
// workaround here. We should use _getch() instead of getch() according to Microsoft.
#undef getch
#define getch _getch

// own local libraries:
#include "so1_library.h" /* our own library, taken from source code folder */
#define VERBOSE_USE_INDENTATION 1
// the following libraries (.h and .c files) must also be placed into the source code folder:
#include "verbose.h" /* Prof.Schoenen KLAB debugging library */
#include "prof_schoenen.h" /* our own external library for unit tests */
// the following libraries will appear later during the lecture:
#include "screenbuffer.h" /* our own external library for graph on console screen */
#include "haw_so1_complex.h" /* our own library for complex numbers */
#include "haw_so1_statistics.h" /* our own library for statistics */

// these are preprocessor statements, ignore them for now
// we just need a portable clrscr() function to clear the console screen
#ifdef _WIN32 /* Windows */
#  define clrscr() system("cls");
#else /* Linux */
#  define clrscr() printf("\e[1;1H\e[2J") /* ANSI escape sequences */
/* system("clear"); // also usable for Linux */
#endif

#ifndef MIN
#  define MIN(a, b) (((a)<(b)) ? (a):(b))
#endif
#ifndef MAX
#  define MAX(a, b) ((a>b) ? (a):(b))
#endif

#define print printf /* for the case that you mistyped printf() :-) */

#define COLUMNS 80
#define ROWS 40
#define MAX_DEVIATION 1e-6
#define DELTA_X 0.001
char blank_char = ' '; // used in plot

// this is a trick/hack to be able to set and change a real C function y=f(x) which is used inside the math functions here
typedef double(*myfunction)(double); // just the datatype 'function_of_one_double_argument'
myfunction fkt = NULL; // = &funktion; // global (pointer) variable to such a function.
myfunction fkt2 = NULL; // = &funktion; // global (pointer) variable to such a function.
// ^ this is for the second use of a generic funtion, outside, derivation

/* Forward Declarations */
int initialize();
double norm_angle(double x);
int value_table_norm_angle();
int test_norm_angle();
double sinus(double x, double deviation);
double our_sinus(double x);
void value_table_sinus();
int test_sinus();
double cosinus(double x, double deviation);
double our_cosinus(double x);
void value_table_cosinus();
int test_cosinus();
double zero_point(double lower, double upper);
double f(double x);
void value_table_f();
void value_table_fkt();
double derivation(double x, double h);
double our_derivation(double x);
void value_table_derivation();
int test_derivation();
double extreme_value(double lower, double upper);
/* from the lecture */
void do_plotting_frontend(); // prototype
void plot_function_by_key(char ch);
void plot_value(double value, double y_min, double y_max);

// if we forward-declare all functions, like shown above, we can put main() here to the beginning of the C file.

int main() {
	int rv = 0; // return value (0 == EXIT_SUCCESS)
	set_verbose_debuglevel(2); // instead of DEBUG=1, improved code
	rv |= initialize();
	if (DEBUG) printf("This is program " COLOR_GREEN "%s" COLOR_NORMAL "\n", __FILENAME__);
	//if (DEBUG) printf("%s()\n", __FUNCTION__);
	printf("This is code for SOL3 lab3.\n");
	clrscr();
	if (DEBUG) printf("Fresh screen of size %dx%d\n", COLUMNS, ROWS);

	const int TESTMODE = 1;		     // set to 1 if you want to activate the automatic testing
	const int SINUS_AVAILABLE = 1;   // set to 1 if you made the   sinus() function
	const int COSINUS_AVAILABLE = 0; // set to 1 if you made the cosinus() function

	if (DEBUG) printf("Normalize angles (to be within %.3lf...%.3lf):\n", -M_PI, +M_PI);
	if (DEBUG) printf("Exercise 1 (Angle) - (b)\n");
	if (TESTMODE) {
		rv |= test_norm_angle(); // UnitTest
	} else {
		value_table_norm_angle(); // simply print out table
	}

	if (DEBUG) printf("Exercise 1 (Angle) - (c)\n");
	if (SINUS_AVAILABLE) {
		if (DEBUG) printf("Value table for function sinus():\n");
		if (TESTMODE) {
			rv |= test_sinus(); // UnitTest
		}
		else {
			value_table_sinus(); // simply print out table
		}
		fkt = &f; // set the global function specifier
	}

	if (COSINUS_AVAILABLE) {
		if (DEBUG) printf("Value table for function cosinus():\n");
		if (TESTMODE) {
			rv |= test_cosinus(); // UnitTest
		}
		else {
			value_table_cosinus(); // simply print out table
		}
		fkt = &f; // set the global function specifier
	}

	/*if (DEBUG) printf("Exercise 1 (Angle) - (d)\n");
	if (1) { // try out this trick using function pointers
		if (DEBUG) printf("Value table for function sinus():  (using function pointers, advanced trick)\n");
		value_table_fkt();
		printf("\n");
	}*/

	if (DEBUG) printf("Exercise 2 (Bisection Method) - (a)\n");
	// this test only works correctly, if inside the function f() either sinus() or cosinus() is called, depending on what you implemented.
	double x_zero;
	if (SINUS_AVAILABLE) {
		if (DEBUG) printf("\nDetermine the zero point of our sinus function:\n");
		if (DEBUG) printf("f(x) properly set up? ");
		rv |= !print_ok_or_not(fabs(f(0.0) - 0.0) < MAX_DEVIATION && fabs(f(M_PI / 2) - 1.0) < MAX_DEVIATION); // 1=OK,0=FAIL
		x_zero = zero_point(-1.0, 2.0); // for sine
		printf("zero_point(sinus,[-1..2]) = %lf ", x_zero);
		rv |= !print_ok_or_not(fabs(x_zero - 0.0) < MAX_DEVIATION); // 1=OK,0=FAIL
		x_zero = zero_point(-2.0, 1.0); // for sine
		printf("zero_point(sinus,[-2..1]) = %lf ", x_zero);
		rv |= !print_ok_or_not(fabs(x_zero - 0.0) < MAX_DEVIATION); // 1=OK,0=FAIL
		x_zero = zero_point(2.0, 4.0); // for sine
		printf("zero_point(sinus,[2..4]) = %lf ", x_zero);
		rv |= !print_ok_or_not(fabs(x_zero - M_PI) < MAX_DEVIATION); // 1=OK,0=FAIL
		x_zero = zero_point(-4.0, -2.0); // for sine
		printf("zero_point(sinus,[-4..-2]) = %lf ", x_zero);
		rv |= !print_ok_or_not(fabs(x_zero - -M_PI) < MAX_DEVIATION); // 1=OK,0=FAIL
	}
	if (COSINUS_AVAILABLE) {
		if (DEBUG) printf("\nDetermine the zero point of our cosinus function:\n");
		if (DEBUG) printf("f(x) properly set up? ");
		rv |= !print_ok_or_not(fabs(f(0.0) - 1.0) < MAX_DEVIATION && fabs(f(M_PI / 2) - 0.0) < MAX_DEVIATION); // 1=OK,0=FAIL
		x_zero = zero_point(0.0, 2.0); // for cosine
		printf("zero_point(cosinus,[0..2]) = %lf ", x_zero);
		rv |= !print_ok_or_not(fabs(x_zero - M_PI / 2.0) < MAX_DEVIATION); // 1=OK,0=FAIL
		x_zero = zero_point(-2.0, 0.0); // for cosine
		printf("zero_point(cosinus,[-2..0]) = %lf ", x_zero);
		rv |= !print_ok_or_not(fabs(x_zero + M_PI / 2.0) < MAX_DEVIATION); // 1=OK,0=FAIL
		x_zero = zero_point(4.0, 5.0); // for cosine
		printf("zero_point(cosinus,[4..5]) = %lf ", x_zero);
		rv |= !print_ok_or_not(fabs(x_zero - M_PI - M_PI / 2.0) < MAX_DEVIATION); // 1=OK,0=FAIL
	}
	if (DEBUG) printf("\n");

	if (DEBUG) printf("\n");
	if (SINUS_AVAILABLE) {
		fkt2 = &our_sinus; // set the global function specifier 2
		if (TESTMODE) {
			rv |= test_derivation(); // UnitTest
		}
		else {
			value_table_derivation(); // simply print out table
		}
		if (DEBUG) printf("\n");
		double x_extremepoint = extreme_value(-1.0, 2.0); // for sine
		if (DEBUG) printf("\n");
		printf("extreme_value() = %lf ", x_extremepoint);
		rv |= !print_ok_or_not(fabs(x_extremepoint - M_PI / 2.0) < DELTA_X);
		if (DEBUG) printf("\n");
	}

	if (COSINUS_AVAILABLE) {
		fkt2 = &our_cosinus; // set the global function specifier 2
		if (TESTMODE) {
			rv |= test_derivation(); // UnitTest
		}
		else {
			value_table_derivation(); // simply print out table
		}
		if (DEBUG) printf("\n");
		double x_extremepoint = extreme_value(-1.0, 1.0); // for cosine
		if (DEBUG) printf("\n");
		printf("extreme_value() = %lf ", x_extremepoint);
		rv |= !print_ok_or_not(fabs(x_extremepoint - 0.0) < DELTA_X);
		if (DEBUG) printf("\n");
	}

	rv = unit_test_report_stdout("unit_tests_main() {all unit tests together}", rv == 0);
	exam_cheers(rv);

	do_plotting_frontend();

	if (haw_get_VisualStudioVersion() == 0) system("pause");
	if (DEBUG) printf("%s(): rv=%d\n", __FUNCTION__, rv);

	return rv;
	
} // main()

// prepare some settings before we do the core work
int initialize() {
	if (DEBUG) printf("%s()\n", __FUNCTION__);
	time_t t; srand((unsigned)time(&t)); // initialize random number generator. Always required at the start of the program.
	if (console_color_on_prof_schoenen() != EXIT_SUCCESS) return 9;
	return EXIT_SUCCESS;
} // initialize()

/************************************************/
// Lab3 1b)


double norm_angle(double x) {
	// Reduce the angle
	x = fmod(x + PI, 2 * PI);
	if (x < 0) {
		x += 2 * PI;
	}
	// Translate to the range [-PI, PI]
	return x - PI;
} // norm_angle()

// Optional Lab3 1b)
int value_table_norm_angle(void) {

	// your implementation here
	
	double x;
	for (x = -10; x < 10.1; x += 2.0) {
		printf("The angle %12.8lf (radiants) corresponds to %12.8lf\n", x, norm_angle(x));
	}
	return 0;

} // value_table_norm_angle()
/************************************************/

int test_norm_angle() {
    int total_ok = 1; // 1=OK, 0=NOT_OK. Changed during tests:
    int count_chars = 0;

	// test the values from the preparation
    for (double x = -10.0; x < +10.0 + 0.01; x += 2.0) {
        double y = norm_angle(x);
        //printf("x=%9.6lf, y=%9.6lf\n", x, y);
        count_chars += printf("The angle %12.8lf (radiants) corresponds to %12.8lf ", x, y);
        total_ok &= print_ok_or_not(y >= -M_PI && y <= +M_PI && fabs(cos(x) - cos(y)) < MAX_DEVIATION && fabs(sin(x) - sin(y)) < MAX_DEVIATION); // 1=OK,0=FAIL
    }
    // testing smaller values...
    for (double x = -20.0; x < -11.0; x += 2.0) {
        double y = norm_angle(x);
        //printf("x=%9.6lf, y=%9.6lf\n", x, y);
        count_chars += printf("The angle %12.8lf (radiants) corresponds to %12.8lf ", x, y);
        total_ok &= print_ok_or_not(y >= -M_PI && y <= +M_PI && fabs(cos(x) - cos(y)) < MAX_DEVIATION && fabs(sin(x) - sin(y)) < MAX_DEVIATION); // 1=OK,0=FAIL
    }
    // testing larger values...
    for (double x = 12.0; x < 20.0 + 0.01; x += 2.0) {
        double y = norm_angle(x);
        //printf("x=%9.6lf, y=%9.6lf\n", x, y);
        count_chars += printf("The angle %12.8lf (radiants) corresponds to %12.8lf ", x, y);
        total_ok &= print_ok_or_not(y >= -M_PI && y <= +M_PI && fabs(cos(x) - cos(y)) < MAX_DEVIATION && fabs(sin(x) - sin(y)) < MAX_DEVIATION); // 1=OK,0=FAIL
    }
    return unit_test_report_stdout(__FUNCTION__, total_ok); // 1=OK,0=FAIL
} // test_norm_angle()

/************************************************/
// Lab3 1c) (Groups 1 & 4)


double sinus(double x, double deviation) {

	// your implementation here
	x = norm_angle(x);
	double term = x;
	double result = term;
	int k = 1;
	while (fabs(term) > deviation) {
		term *= -x * x / (2 * k * (2 * k + 1));
		result += term;
		k++;
	}
	return result;

} // sinus()


// Lab3 1c) (Groups 2 & 3)
double cosinus(double x, double deviation) {

	// your implementation here


} // cosinus()

// Lab3 1d) generic f(x) can be used later as a generic function
double f(double x) {

	//your implementation here
	return sinus(x, 1e-6); // Example deviation

} // f()
/************************************************/


// function with just one argument (required for later)
double our_sinus(double x) {
	return sinus(x, MAX_DEVIATION);
} // our_sinus()

void value_table_sinus() {
	printf("%sf():\nx\t\ty\n", __FUNCTION__);
	for (double x = -M_PI; x < M_PI + 0.01; x += M_PI / 10.0) {
		double y = 0.0;
		y = our_sinus(x);
		printf("x=%9.6lf, y=%9.6lf\n", x, y);
	}
} // value_table_sinus()

int test_sinus() {
    int total_ok = 1; // 1=OK, 0=NOT_OK. Changed during tests:
    if (DEBUG) printf("%s():\nx\t\ty\n", __FUNCTION__);
    for (double x = -M_PI; x < M_PI + 0.01; x += M_PI / 10.0) {
        double y = 0.0;
        y = our_sinus(x);
        printf("x=%9.6lf, y=%9.6lf ", x, y);
        total_ok &= print_ok_or_not(fabs(y - sin(x)) < MAX_DEVIATION); // 1=OK,0=FAIL
    }
    // more extreme values (require norm_angle)
    for (double x = M_PI; x < 4.0*M_PI + 0.01; x += M_PI / 4.0) {
        double y = 0.0;
        y = our_sinus(x);
        printf("x=%9.6lf, y=%9.6lf ", x, y);
        total_ok &= print_ok_or_not(fabs(y - sin(x)) < MAX_DEVIATION); // 1=OK,0=FAIL
    }
    // more extreme values (require norm_angle)
    for (double x = -M_PI; x > -4.0*M_PI - 0.01; x -= M_PI / 4.0) {
        double y = 0.0;
        y = our_sinus(x);
        printf("x=%9.6lf, y=%9.6lf ", x, y);
        total_ok &= print_ok_or_not(fabs(y - sin(x)) < MAX_DEVIATION); // 1=OK,0=FAIL
    }
    return unit_test_report_stdout(__FUNCTION__, total_ok); // 1=OK,0=FAIL
} // test_sinus()



// function with just one argument (required for later)
double our_cosinus(double x) {
	return cosinus(x, MAX_DEVIATION);
}

void value_table_cosinus() {
	printf("%sf():\nx\t\ty\n", __FUNCTION__);
	for (double x = -M_PI; x < M_PI + 0.01; x += M_PI / 10.0) {
		double y = 0.0;
		y = our_cosinus(x);
		printf("x=%9.6lf, y=%9.6lf\n", x, y);
	}
} // value_table_cosinus()

int test_cosinus() {
    int total_ok = 1; // 1=OK, 0=NOT_OK. Changed during tests:
    if (DEBUG) printf("%s():\nx\t\ty\n", __FUNCTION__);
    for (double x = -M_PI; x < M_PI + 0.01; x += M_PI / 10.0) {
        double y = 0.0;
        y = our_cosinus(x);
        printf("x=%9.6lf, y=%9.6lf ", x, y);
        total_ok &= print_ok_or_not(fabs(y - cos(x)) < MAX_DEVIATION); // 1=OK,0=FAIL
    }
    // more extreme values (require norm_angle)
    for (double x = M_PI; x < 4.0*M_PI + 0.01; x += M_PI / 4.0) {
        double y = 0.0;
        y = our_cosinus(x);
        printf("x=%9.6lf, y=%9.6lf ", x, y);
        total_ok &= print_ok_or_not(fabs(y - cos(x)) < MAX_DEVIATION); // 1=OK,0=FAIL
    }
    // more extreme values (require norm_angle)
    for (double x = -M_PI; x > -4.0*M_PI - 0.01; x -= M_PI / 4.0) {
        double y = 0.0;
        y = our_cosinus(x);
        printf("x=%9.6lf, y=%9.6lf ", x, y);
        total_ok &= print_ok_or_not(fabs(y - cos(x)) < MAX_DEVIATION); // 1=OK,0=FAIL
    }
    return unit_test_report_stdout(__FUNCTION__, total_ok); // 1=OK,0=FAIL
} // test_cosinus()

// Lab3 1d)
// this uses the f() function, which cannot be changed.
void value_table_f() {
	printf("%sf():\nx\t\ty\n", __FUNCTION__);
	for (double x = -M_PI; x < M_PI + 0.01; x += M_PI / 10.0) {
		double y =0.0;
		y = f(x);
		printf("x=%9.6lf, y=%9.6lf\n",x,y);
	}
} // value_table_f()


void value_table_fkt() {
	printf("%sf():\nx\t\ty\n", __FUNCTION__);
	for (double x = -M_PI; x < M_PI + 0.01; x += M_PI / 10.0) {
		double y = 0.0;
		if (fkt != NULL)
			y = fkt(x); // the generic function which can be changed from the outside
		else
			y = f(x);
		printf("x=%9.6lf, y=%9.6lf\n", x, y);
	}
} // value_table_fkt()

/************************************************/
// Lab3 2a)
// // determine the zero of the function by BISECTION
// this uses the f() function, which cannot be changed.
double zero_point(double lower, double upper) {

	// your implementation here
	double middle;
	while (fabs(upper - lower) > 1e-8) {
		middle = (lower + upper) / 2.0;
		if (f(middle) == 0.0) {
			return middle;
		}
		else if (f(lower) * f(middle) < 0) {
			upper = middle;
		}
		else {
			lower = middle;
		}
	}
	return middle;


} // zero_point()

// Lab3 2b)
// calculate the derivative at position x using delta=h of the changeable function fkt2()
double derivation(double x, double h) {

	// your implementation here
	return (f(x + h) - f(x - h)) / (2 * h);

} // derivation()

// Lab3 2b)
// determine the x-position of the extreme value of the derivative of fkt(), which gives the maximum or minimum.
double extreme_value(double lower, double upper) {

	// your implementation here
	double middle, f_prime_lower, f_prime_upper, f_prime_middle;

	int iteration = 0;
	do {
		f_prime_lower = derivation(lower, 1e-8);
		f_prime_upper = derivation(upper, 1e-8);
		middle = (lower + upper) / 2.0;
		f_prime_middle = derivation(middle, 1e-8);



		if (f_prime_middle == 0.0) {
			break;
		}
		else if ((f_prime_lower > 0 && f_prime_middle < 0) || (f_prime_lower < 0 && f_prime_middle > 0)) {
			upper = middle;
		}
		else {
			lower = middle;
		}

		iteration++;
	} while (fabs(upper - lower) >= 1e-8);

	return middle;


} // extreme_value()
/************************************************/

// provide a function with exactly one (x) argument, required for plotting
double our_derivation(double x) {
	return derivation(x, MAX_DEVIATION);
} // our_derivation()

// Lab3 2c)
// print a value-table of the derivative
void value_table_derivation() {
	printf("%s():\nx\t\ty\n", __FUNCTION__);
	for (double x = -M_PI; x < M_PI + 0.01; x += M_PI / 10.0) {
		//double y = sinus(x, MAX_DEVIATION);
		double y = derivation(x, DELTA_X);
		printf("x=%9.6lf, y=%9.6lf\n", x, y);
	}
} // value_table_derivation()

int test_derivation() {
	printf("%s():\nx\t\ty\n", __FUNCTION__);
	int total_ok = 1; // 1=OK, 0=NOT_OK. Changed during tests:
	for (double x = -M_PI; x < M_PI + 0.01; x += M_PI / 10.0) {
		//double y = sinus(x, MAX_DEVIATION);
		double y = derivation(x, DELTA_X);
		printf("x=%9.6lf, y=%9.6lf ", x, y);
		total_ok &= print_ok_or_not((fkt2==&our_sinus)&&(fabs(y-cos(x))<DELTA_X)
			|| (fkt2==&our_cosinus)&&(fabs(y+sin(x))<DELTA_X)); // 1=OK,0=FAIL
	}
} // test_derivation()


// Lab3 3b)
// loop waiting and asking the user for a key-press
void do_plotting_frontend() {
	if (DEBUG) printf("### %s()\n", __FUNCTION__);
	char ch; // character from keyboard
	int key_pressed = ' '; // keyboard key value
	while (key_pressed != '.') { // loop
		print("Please select function to plot (keys a..z, .=quit)\n");
		key_pressed = _getch(); // get keyboard key
		ch = key_pressed; // char <- int conversion
		clrscr(); // clear screen
		if (DEBUG) print("Key pressed was '%c'=%04d=0x%04X\n", ch, key_pressed, key_pressed);
		plot_function_by_key(ch);
	}
	if (DEBUG) printf("### do_plotting_frontend() finished.\n");
} // do_plotting_frontend()

// a few constants for plotting
//#define PI 3.1415 // not precise enough
#define PI M_PI
#define XMIN -PI
#define XMAX +PI
#define XSTEP (2*PI/ROWS)
#define XEXTRA 0.01

// Lab3 3b)
// depending on the user's key-press, plot one of the functions based on that key
void plot_function_by_key(char ch) {
	if (DEBUG) printf("### %s('%c')\n", __FUNCTION__,ch);
	int index = 0;
	double x, y, y_min = 0.0, y_max = 0.0;
	double x_array[100], y_array[100];
	// loop over range of x values:
	for (x = XMIN; x <= XMAX + XEXTRA; x = x + XSTEP) { // #steps is ROWS
		switch (ch) { // depend on key pressed
		case 's': y = sin(x); break;
		case 'c': y = cos(x); break;
		case 't': y = tan(x); break;
		case 'l': y = x; break; // linear
		case 'q': y = x*x; break; // square
        case 'o': y = ((x < 0.0) ? -1 : 1)*sqrt(fabs(x)); break; // square
        case 'f': y = f(x); break; // our new sinus function
		case 'd': y = derivation(x, DELTA_X); break; // the first derivatice
		case 'g': y = fkt(x); break; // a generic function (global, can be set from outside)
		case 'r': y = rand()%100 - 50; break;
		default: y = 0.0;
		}
		//if (DEBUG) printf("x=%.3lf y=%.3lf\n", x, y);
		x_array[index] = x; // store into table
		y_array[index] = y; // store into table
		y_min = (y<y_min) ? y : y_min;
		y_max = (y>y_max) ? y : y_max;
		index++; // increase the index
	}
	int num_values = index; // so many elements in the table
	if (DEBUG) printf("num_values=%d, y_min=%.3f, y_max=%.3f\n", num_values, y_min, y_max);
	int index_of_x_zero = (0-XMIN)/XSTEP;
	for (index = 0; index < num_values; index++) { // #steps is ROWS
		x = x_array[index]; // read out values from table
		y = y_array[index];
		//if (DEBUG) printf("x=%.3lf y=%.3lf\n", x, y);
		printf("x=%6.3lf ", x);
		if (index == index_of_x_zero)
			blank_char = '-'; // global variable
		else
			blank_char = ' '; // global variable
		plot_value(y, y_min, y_max);
	}
	if (DEBUG) printf("### plot_function_by_key() finished.\n");
} // plot_function_by_key()

/*****************************************/
// Lab3 3b) You may change this function however you want. 
void plot_value(double value, double y_min, double y_max) {
	double range = y_max - y_min;
	int column = (value - y_min) / (range) * (COLUMNS - 10); // between 0 .. 80

	// Determine the position of the x-axis (where y = 0)
	int x_axis = (0 - y_min) / range * (COLUMNS - 10);

	int y_axis = (0 - y_min) / range * (COLUMNS - 10);


	for (int i = 0; i < COLUMNS - 10; i++) {
		if (i == y_axis) {
			printf("|");
		}
		else if (i == column) {
			printf("*");
		}
		else {
			printf("%c", blank_char);
		}
	}

	printf("*\n");
}
/*****************************************/