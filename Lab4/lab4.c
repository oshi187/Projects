/* Doxygen-Info
 * @file lab4.c
 * @author Oshini Jayaweera
 * @author Prof. Dr. Rainer Schoenen
 * @date SS2023 10/06/2024
 * @version 20230523
 * @brief HAW SS2023 SOL1 Lab4
 * Detailed description:
 *   Practise the following skills:
 *   - from mathematical formulas to algorithms
 *   - math integration
 *   - plotting
 *   - using libraries, i.e., several C and H code files in a project
 *   - unit testing
 */

 /* all the important includes and settings ... */
#define _CRT_SECURE_NO_WARNINGS
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
#define VERBOSE_USE_INDENTATION 1
// the following libraries (.h and .c files) must also be placed into the source code folder:
#include "verbose.h" /* Prof.Schoenen KLAB debugging library */
#include "haw_unittesting.h" /* our own external library for unit tests */
// the following libraries will appear later during the lecture:
#include "haw_screenbuffer.h" /* our own external library for graph on console screen */
#include "haw_complex.h" /* our own library for complex numbers */
#include "haw_statistics.h" /* our own library for statistics */

// these are preprocessor statements, ignore them for now
// we just need a portable clrscr() function to clear the console screen
#ifdef _WIN32 /* Windows */
#  define clrscr() system("cls");
#else /* Linux */
#  define clrscr() printf("\e[1;1H\e[2J") /* ANSI escape sequences */
/* system("clear"); // also usable for Linux */
#endif

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

// prepare some settings before we do the core work
int initialize() {
	if (DEBUG) printf("%s()\n", __FUNCTION__);
	time_t t; srand((unsigned)time(&t)); // initialize random number generator. Always required at the start of the program.
	if (console_color_on_prof_schoenen() != EXIT_SUCCESS) return 9;
    if (!screenbuffer_init(0, 0)) return 8;
    return EXIT_SUCCESS;
} // initialize()

// Lab3 1b)
double norm_angle(double x) {
	while (x > +M_PI) x -= 2.0*M_PI;
	while (x < -M_PI) x += 2.0*M_PI;
	return x;
} // norm_angle()

// generic f(x) can be used later as a generic function
double f(double x) {
    //return our_sinus(x);
    //return our_cosinus(x);
    return cos(x);
} // f()

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

// Lab3 2b)
// determine the zero of the function, using the changeable function generic_fkt() which is the first argument. Trick  ;-)
double zero_generic_arg(double generic_fkt(double), double unten, double oben) {
	if (DEBUG) printf("nullstelle_generisch(): u=%lf o=%lf\n", oben, unten);
	double mitte = 0;
	while (oben - unten > MAX_DEVIATION) {
		mitte = (oben + unten) / 2; // mean
		if (DEBUG) printf("u=%lf m=%lf o=%lf => fu=%lf fm=%lf fo=%lf\n",
			unten, mitte, oben, generic_fkt(unten), generic_fkt(mitte), generic_fkt(oben));
		if (generic_fkt(mitte) > 0 && generic_fkt(unten) < 0) oben = mitte;
		else if (generic_fkt(mitte) > 0 && generic_fkt(oben) < 0) unten = mitte;
		else if (generic_fkt(mitte) < 0 && generic_fkt(unten) > 0) oben = mitte;
		else if (generic_fkt(mitte) < 0 && generic_fkt(oben) > 0) unten = mitte;
		else if (generic_fkt(mitte) == 0.0) return mitte;
		else { printf("Error: o=%lf m=%lf u=%lf\n", oben, mitte, unten); return NAN; }
	}
	return mitte;
} // zero_generic_arg()

// Lab3 2c)
// calculate the derivative at position x using delta=h of the changeable function fkt2()
double derivation(double x, double h) {
	if (fkt2 == NULL) return 0.0; // if undefined global generic function
	return (fkt2(x+h)- fkt2(x-h))/(2.0*h);
} // derivation()

// provide a function with exactly one (x) argument, required for plotting
double our_derivation(double x) {
	return derivation(x, MAX_DEVIATION);
} // our_derivation()

// Lab3 2c)
// print a value-table of the derivative
void value_table_derivation() {
	printf("%s():\nx\t\ty\n", __FUNCTION__);
	for (double x = -M_PI; x < M_PI + 0.01; x += M_PI / 10.0) {
		//double y = cos(x);
		double y = derivation(x, DELTA_X);
		printf("x=%9.6lf, y=%9.6lf\n", x, y);
	}
} // value_table_derivation()

int test_derivation() {
	printf("%s():\nx\t\ty\n", __FUNCTION__);
	int total_ok = 1; // 1=OK, 0=NOT_OK. Changed during tests:
    fkt2 = cos; // function pointer trick
	for (double x = -M_PI; x < M_PI + 0.01; x += M_PI / 10.0) {
		//double y = cos(x, MAX_DEVIATION);
		double y = derivation(x, DELTA_X);
		printf("x=%9.6lf, y=%9.6lf ", x, y);
		total_ok &= print_ok_or_not((fkt2==&cos)&&(fabs(y+sin(x))<DELTA_X)
			|| (fkt2==&sin)&&(fabs(y+cos(x))<DELTA_X)); // 1=OK,0=FAIL
	}
    return total_ok?EXIT_SUCCESS:EXIT_FAILURE;
} // test_derivation()

// generic f(x) can be used later as a generic function
double f4(double x) {
    return cos(2.0*M_PI*x);
} // f4()

/********************************************************/
/* Lab 4 Exercise 1a */
double area(double left_x, double right_x) {

    // your implementation here
    double mid_x = (left_x + right_x) / 2.0;
    return (right_x - left_x) * f4(mid_x);

} // area()

/* Lab 4 Exercise 1b */
double integral(double left_x, double right_x, int steps) {

    // your implementation here
    double step_size = (right_x - left_x) / steps;
    double total_area = 0.0;
    for (int i = 0; i < steps; i++) {
        double start = left_x + i * step_size;
        double end = start + step_size;
        total_area += area(start, end);
    }
    return total_area;

} // integral()

/* Lab 4 Exercise 1c */
#define INT_STEPS 1000
double integral_1arg(double x) {

    // your implementation here
    return integral(0, x, INT_STEPS);

} // integral_1arg()

/* Lab 4 Exercise 1d */
double derivation_fix_h(double x) {

    // your implementation here
    double h = DELTA_X;
    return (integral_1arg(x + h) - integral_1arg(x - h)) / (2.0 * h);
} // derivation_fix_h()
/********************************************************/

// print a value-table of the derivative
void value_table_integration() {
    printf("%s():\nx\t\ty\n", __FUNCTION__);
    for (double x = -M_PI; x < M_PI + 0.01; x += M_PI / 10.0) {
        //double y = cos(x);
        double y = integral_1arg(x);
        printf("x=%9.6lf, y=%9.6lf\n", x, y);
    }
} // value_table_derivation()

int test_integration() {
    printf("%s():\n", __FUNCTION__);
    int total_ok = 1; // 1=OK, 0=NOT_OK. Changed during tests:
    fkt2 = cos; // function pointer trick
    double result = 0.0;
    result = area(-0.25, 0.25);
    printf("area(-0.25,0.25)=%9.6lf ", result);
    total_ok &= print_ok_or_not(fabs(result - 0.5) < DELTA_X); // 1=OK,0=FAIL
    //integral(double left_x, double right_x, int steps);
    result = integral(-0.25, 0.25, 100);
    printf("integral(-0.25,0.25,100)=%9.6lf ", result);
    total_ok &= print_ok_or_not(fabs(result - 0.318310) < DELTA_X); // 1=OK,0=FAIL
    printf("integral(f4(x) dx):\n");
    printf("x\t\ty\n");
    for (double x = -1.0; x < 1.0 + 0.01; x += 2.0 / 20.0) {
        //double y = cos(x, MAX_DEVIATION);
        double y = integral_1arg(x);
        printf("x=%9.6lf, y=%9.6lf ", x, y);
        total_ok &= print_ok_or_not(fabs(y - sin(2.0*M_PI*x)/2.0/M_PI) < DELTA_X); // 1=OK,0=FAIL
    }
    printf("diff(integral(f4(x) dx)/dx):\n");
    printf("x\t\ty\n");
    for (double x = -1.0; x < 1.0 + 0.01; x += 2.0 / 20.0) {
        double y = derivation_fix_h(x);
        printf("x=%9.6lf, y=%9.6lf ", x, y);
        total_ok &= print_ok_or_not(fabs(y - cos(2.0*M_PI*x)) < DELTA_X); // 1=OK,0=FAIL
    }
    return total_ok ? EXIT_SUCCESS : EXIT_FAILURE;
} // test_integration()


// a few constants for plotting
//#define PI 3.1415 // not precise enough
#define PI M_PI
#define XMIN -PI
#define XMAX +PI
#define XSTEP (2*PI/ROWS)
#define XEXTRA 0.01

// put a * on the console screen, column ~ y-axis proportional to value within range y_min and y_max
void plot_value(double value, double y_min, double y_max) {
    double range = y_max - y_min;
    int available_columns = (COLUMNS - 10);
    int y_value_column = (value - y_min) / range * available_columns; // between 0 .. 80
    int x_axis_column = -y_min / range * available_columns;
    for (int i = 0; i < available_columns; i++) {
        if (i == y_value_column)
            printf("*");
        else if (i == x_axis_column)
            printf("|");
        else
            printf("%c", blank_char);
    }
    printf("\n");
} // plot_value()

// depending on the user's key-press, plot one of the functions based on that key
void plot_function_by_key(char ch) {
    if (DEBUG) printf("### %s('%c')\n", __FUNCTION__, ch);
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
        case 'q': y = x * x; break; // square
        case 'o': y = ((x < 0.0) ? -1 : 1) * sqrt(fabs(x)); break; // square
        case 'f': y = f(x); break; // our new sinus function
        case 'd': y = derivation(x, DELTA_X); break; // the first derivative
        case 'i': y = integral_1arg(x / 2.0 / M_PI /* scaling */); break; /* integration */
        //case 'g': y = fkt(x); break; // a generic function (global, can be set from outside)
        case 'g': y = gauss(x); break; // gauss PDF function
        case 'r': y = rand() % 100 - 50; break;
        default: y = 0.0;
        }
        //if (DEBUG) printf("x=%.3lf y=%.3lf\n", x, y);
        x_array[index] = x; // store into table
        y_array[index] = y; // store into table
        y_min = (y < y_min) ? y : y_min;
        y_max = (y > y_max) ? y : y_max;
        index++; // increase the index
    }
    int num_values = index; // so many elements in the table
    if (DEBUG) printf("num_values=%d, y_min=%.3f, y_max=%.3f\n", num_values, y_min, y_max);
    int index_of_x_zero = (0 - XMIN) / XSTEP;
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

// loop waiting and asking the user for a key-press
void do_plotting_frontend() {
    if (DEBUG) printf("### %s()\n", __FUNCTION__);
    char ch; // character from keyboard
    int key_pressed = ' '; // keyboard key value
    while (key_pressed != '.') { // loop
        printf("Please select function to plot (keys a..z, .=quit)\n");
        key_pressed = _getch(); // get keyboard key
        ch = key_pressed; // char <- int conversion
        clrscr(); // clear screen
        if (DEBUG) printf("Key pressed was '%c'=%04d=0x%04X\n", ch, key_pressed, key_pressed);
        plot_function_by_key(ch);
    }
    if (DEBUG) printf("### do_plotting_frontend() finished.\n");
} // do_plotting_frontend()


/*******************************************************/
/* Lab4 Ex2 a) */
int dice() {

    // your implementation here
    return rand() % 6 + 1;
}

/* Lab4 Ex2 b) */
#define MAX_THROWS 10
int* dice_statistics(int throws, int experiments) {
    static int histogram_array[6 * MAX_THROWS + 1] = { 0 }; // initialized to all zeros
    for (int i = 0; i <= 6 * MAX_THROWS; i++) histogram_array[i] = 0; // clear histogram
    printf("dice_statistics(%d,%d)\n", throws, experiments);
    if (throws > MAX_THROWS) { printf("ERROR: too many throws\n"); return; }
    for (int experiment = 0; experiment < experiments; experiment++) {
        int number = 0;
        // throw the dice "throws" times and add up their value in "number":

        // your implementation here
        for (int t = 0; t < throws; t++) {
            number += dice();
        }


        histogram_array[number]++;
    }
    /*******************************************************/


    // output histogram as a table
    double y_min = 0.0, y_max = 0.0;
    for (int number = throws; number <= 6 * throws; number++) {
        //printf("histogram_array[%d] = %d\n", number, histogram_array[number]);
        double probability = (double)histogram_array[number] / experiments;
        if (probability > y_max) y_max = probability;
        printf("probability[%d] = %lf\n", number, probability);
    }
    y_max *= 1.10; // +10%
    // plot_value(double value, double y_min, double y_max)
    for (int number = throws; number <= 6 * throws; number++) {
        double probability = (double)histogram_array[number] / experiments;
        plot_value(probability, y_min, y_max);
    }
    return histogram_array;
} // dice_statistics()


// test dice() function
int test_dice() {
    printf("%s()\n", __FUNCTION__);
    int total_ok = 1; // 1=OK, 0=NOT_OK. Changed during tests:
    int experiments = 10000;
    double average = 0.0;
    for (int experiment = 0; experiment < experiments; experiment++) {
        int value = dice();
        average += value;
        total_ok &= (value >= 1 && value <= 6);
    }
    average /= (double)experiments;
    printf("average=%9.6lf ", average);
    total_ok &= print_ok_or_not(fabs(average - 3.5) < 0.1); // 1=OK,0=FAIL
    return total_ok ? EXIT_SUCCESS : EXIT_FAILURE;
} // test_dice()

// test dice_statistics() function
int test_dice_statistics() {
    printf("%s()\n", __FUNCTION__);
    int total_ok = 1; // 1=OK, 0=NOT_OK. Changed during tests:
    int experiments = 1000000;
    for (int throws = 1; throws <= 4; throws++) {
        int* histogram_array = dice_statistics(throws, experiments);
        // ^ did it work correctly?
        double p_sum = 0.0;
        double avg = 0.0;
        for (int number = throws; number <= 6 * throws; number++) {
            double probability = (double)histogram_array[number] / experiments;
            p_sum += probability;
            avg += probability * number;
        }
        printf("throws=%d: p_sum=%9.6lf ", throws, p_sum);
        total_ok &= print_ok_or_not(fabs(p_sum - 1.0) < DELTA_X); // 1=OK,0=FAIL
        printf("throws=%d: avg=%9.6lf ", throws, avg);
        total_ok &= print_ok_or_not(fabs(avg - 3.5 * throws) < 0.1); // 1=OK,0=FAIL
    }
    print_ok_or_not(total_ok); // 1=OK,0=FAIL
    return total_ok ? EXIT_SUCCESS : EXIT_FAILURE;
} // test_dice_statistics()



// main() of Lab4
int main() {
    int rv = 0; // return value (0 == EXIT_SUCCESS)
    set_verbose_debuglevel(2); // instead of DEBUG=1, improved code
    rv |= initialize();
    if (DEBUG) printf("This is program " COLOR_GREEN "%s" COLOR_NORMAL "\n", __FILENAME__);
    //if (DEBUG) printf("%s()\n", __FUNCTION__);
    printf("This is code for SOL1 lab4.\n");
    clrscr();
    if (DEBUG) printf("Fresh screen of size %dx%d\n", COLUMNS, ROWS);

    const int TESTMODE = 1;

    if (DEBUG) printf("\n");

    if (1) {
        fkt2 = &cos; // set the global function specifier 2
        if (TESTMODE) {
            rv |= test_integration(); // UnitTest
        } else {
            value_table_integration(); // simply print out table
        }
        // plotting:
        //if (!screenbuffer_init(0, 0)) return 8;
        screenbuffer_set_xy_boundaries(-1.0, +1.0, -1.0, +1.0);
        //screenbuffer_xy_axes();
        screenbuffer_draw_function_with_marker(integral_1arg, '+');
        //screenbuffer_to_console();
        // plotting:
        //if (!screenbuffer_init(0, 0)) return 8;
        screenbuffer_set_xy_boundaries(-1.0, +1.0, -1.0, +1.0);
        //screenbuffer_xy_axes();
        screenbuffer_draw_function_with_marker(derivation_fix_h, '*');
        screenbuffer_to_console();
        if (DEBUG) printf("\n");
    }
    if (1) {
        rv |= test_dice();
        rv |= test_dice_statistics();
    }
    rv = unit_test_report_stdout("unit_tests_main() {all unit tests together}", rv == 0);
    exam_cheers(rv);

    do_plotting_frontend();

    if (haw_get_VisualStudioVersion() == 0) system("pause");
    if (DEBUG) printf("%s(): rv=%d\n", __FUNCTION__, rv);
    return rv;
    double first_rectangle_area = area(-0.25, 0.25);
    // Check if you get a green OK in the output
    // Print the result
    if (first_rectangle_area == M_PI) {
        // Output OK
        // Your test passed
    }
    else {
        // Output not OK
        // Your test failed
    }

    return 0;
    double x = 0.0; // Test at x = 0
    double derivative_at_x = derivation_fix_h(x);

    printf("The derivative of f4 at x = %f is approximately %f\n", x, derivative_at_x);

    return 0;
   

} // main()
