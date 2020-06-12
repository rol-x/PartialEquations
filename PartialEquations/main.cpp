// Karol Latos & Kamila Kwieciñska
#include <iostream>
#include <fstream>
#include <math.h>

constexpr double pi = 3.14159265358;

using namespace std;

//    IMPLEMENTATION NOTES:
// 1. Following the conventions of mathematical naming of
//    axes with respect to their dimensions, variable 'z'
//	  has been changed to 'x'.
// 2. Following the universal conventions for naming functions,
//    the name of function 'w' has been changed to 'y'.
// 3. Following the coding convention of nested 'for' loops,
//    the iterator of the w axis has been changed from 'k' to 'j'.
// 4. The initial condition function name has been changed
//    from w_0 to t_0 for better readability.
// 5. For the same reason, boundary conditions functions
//    are named x_0 and x_n, signifying setting the variable 'x' constant.
// 6. The name of the coefficient 'a^2' has been changed to 'A', since
//    the use of 'a' without the square is absent in the method.
// 7. The step in the t axis has been changed from 'h' to 'h_t'
//    for better readability.
// 8. For the same reason, the step in the x axis has been changed to 'h_x'.
// 9. For practical reasons, predetermined values are defined outside
//    main function, but due to unique naming no problems arise.

// Coefficient A.
double A = 0.05;

// Step in the t axis.
double h_t = 0.2;

// Step in the x axis.
double h_x = 0.1;

// Upper boundary for the t variable.
double t_max = 2;

// Upper boundary for the x variable.
double x_max = 1;

// Initial condition function (t = 0).
double t_0(double x_j)
{
	return sin(pi * x_j);
}

// Lower boundary condition function (x = 0).
double x_0(double t_i)
{
	return 0;
}

// Upper boundary condition function (x = j * h_x).
double x_n(double t_i)
{
	return 0;
}

// Free term function f.
double f(double t_i, double x_j)
{
	return 0;
}

// Function implementing the octave code visualizing the 3D space of points.
void convertToOctaveCode(double ** space, int x, int y)
{
	try
	{
		// Creating a file object and linking it to a text file on a disk.
		fstream dataFile;
		dataFile.open("plot.m", ios::out);

		// Opening of the text file failed - exception handling.
		if (!dataFile)
			throw "Error opening the output file!";

		// Introducing the authors.
		dataFile << "# Karol Latos & Kamila Kwieciñska" << endl;

		// Writing the t vector.
		dataFile << endl << "t = [";
		for (int i = 0; i < x; i++)
		{
			dataFile << i * h_t;
			if (i < x - 1)
				dataFile << " ";
			else
				dataFile << "];\n";
		}

		// Writing the x vector.
		dataFile << endl << "x = [";
		for (int j = 0; j < y; j++)
		{
			dataFile << j * h_x;
			if (j < y - 1)
				dataFile << "; ";
			else
				dataFile << "];\n";
		}

		// Writing the y matrix.
		dataFile << endl << "y = [";
		for (int j = 0; j < y; j++)
			for (int i = 0; i < x; i++)
			{
				dataFile << space[i][j];
				if (i < x - 1)
					dataFile << " ";
				else if (j < y - 1)
					dataFile << ";\n";
				else
					dataFile << "];\n";
			}

		// Calling the plotting function.
		dataFile << endl << "mesh(t, x, y)" << endl;

		// Labeling the axes and titling the graph.
		dataFile << "ylabel \"t-axis\";\nxlabel \"x-axis\";\nzlabel \"y-axis\";\ntitle(\"Surface of y(t, x)\");";

		// Success confirmation.
		cout << "Successfully written code to the output file!" << endl;
	}
	// Displaying the error message - exception handling.
	catch (const char* message)
	{
		std::cerr << message << endl;
	}
}

int main()
{
	// Dimensions of the net ('+ 1' accounts for including both ends of the inverval).
	int rows = t_max / h_t + 1;
	int columns = x_max / h_x + 1;

	// Array of solution function values on the considered discete space.
	double ** y = new double *[rows];
	for (int i = 0; i < rows; i++)
		y[i] = new double[columns];

	// Initial condition for t = 0.
	for (int j = 0; j < columns; j++)
		y[0][j] = t_0(j * h_x);

	// Boundary conditions.
	for (int i = 0; i < rows; i++)
	{
		y[i][0] = x_0(i * h_t);
		y[i][columns - 1] = x_n(i * h_t);
	}

	// Iterative solution of y for points in the net.
	// Since every iteration gives us the next state of y,
	// the loop should only go to the second to last step of t.
	for (int i = 0; i < rows - 1; i++)
		// The boundary conditions are already known.
		for (int j = 1; j < columns - 1; j++)
			y[i + 1][j] = y[i][j] + A * h_t * pow(h_x, -2) * (y[i][j - 1] - 2 * y[i][j] + y[i][j + 1]) + h_t * f(i * h_t, j * h_x);

	// The array of values of y is finally converted to viewable form.
	convertToOctaveCode(y, rows, columns);

	system("pause");
	return 0;
}