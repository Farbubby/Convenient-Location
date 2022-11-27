#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "leak_detector_c.h"

// Point struct representing (x,y).
typedef struct point {

  int x;
  int y;

} point;

// Global file pointer and current location variables.
FILE *inptr, *outptr;
point initLocation;

// Reads the coordinates (x,y) into a struct and returns that struct.
point ReadData() {

  point points;
  fscanf(inptr, "%d", &(points.x));
  fscanf(inptr, "%d", &(points.y));

  return points;

}

// Checks whether coordinates of ptrPt1 is closer or farther than another point ptrPt2.
// It also contains tie breakers if the distance is the same.
int compareTo(point *ptrPt1, point *ptrPt2) {

  // Uses the distance formula to calculate the distance.
  double d1 = sqrt(pow(ptrPt1->x - initLocation.x, 2) + pow(ptrPt1->y - initLocation.y, 2));
  double d2 = sqrt(pow(ptrPt2->x - initLocation.x, 2) + pow(ptrPt2->y - initLocation.y, 2));

  // Returns 1 if distance from ptrPt1 is larger.
  if (d1 > d2) {

    return 1;

  }

  // Returns if distance from ptrPt2 is smaller.
  else if (d1 < d2) {

    return -1;

  }

  // A tie breaker for points with the same distances.
  else {

    // A point is considered larger if that x-coordinate is larger.
    if (ptrPt1->x > ptrPt2->x) {

      return 1;

    }

    // A point is considered smaller if that x-coordinate is smaller.
    else if (ptrPt1->x < ptrPt2->x) {

      return -1;

    }

    // A tie breaker for points with the same x-coordinates.
    else {

      // A point is considered larger if that y-coordinate is larger.
      if (ptrPt1->y > ptrPt2->y) {

        return 1;

      }

      // A point is considered smaller if that y-coordinate is smaller
      else if (ptrPt1->y < ptrPt2->y) {

        return -1;

      }

      // Returns 0 if the coordinates of the points are exactly the same.
      else {

        return 0;

      }

    }

  }

}

// Does an insertion sort of an "imaginary array"
void insertionSort(point *points, int l, int r) {

  int i, j;

  // Starts at the point struct after the beginning.
  for (i = l+1; i < r+1; i++) {
  
    point head = points[i];

    // Starts at the previous point struct and does a comparison for whether a shift is needed or not.
    for (j = i-1; j >= l; j--) {

      if (compareTo(&points[j], &head) == 1) {

        points[j+1] = points[j];

      }

      else {

        break;

      }

    }

    // Adds the displaced point struct in the correct position.
    points[j+1] = head;

  }

}

// Merges two different arrays in sorting order.
void merge(point *points, int l, int mid, int r) {

  // Calculates the sizes of the left and right side arrays.
  int s1 = (mid-l)+1;
  int s2 = (r-mid);
  int i, j, k;

  // Left array represeting the left side of the array after the split by mergeSort.
  // Right array representing the right side of the array after split. 
  // Allocates memory for a left and right array of pointers of size s1 and s2 respectively.
  point *left = (point*)malloc(s1 * sizeof(point));
  point *right = (point*)malloc(s2 * sizeof(point));

  // Adds all of the left side values of the array into the left array.
  for (i = 0; i < s1; i++) {

    left[i] = points[l+i];

  }

  // Adds all of the right side values of the array into the right array.
  for (j = 0; j < s2; j++) {

    right[j] = points[mid+1+j];

  }

  // Resets the values.
  i = 0, j = 0, k = l;

  // Uses comparison to add each element from the left and right array in sorting order into the main point array.
  while (i < s1 && j < s2) {

    if (compareTo(&left[i], &right[j]) == 1) {

      points[k] = right[j];
      j++;
      k++;

    }

    else if (compareTo(&left[i], &right[j]) == -1) {

      points[k] = left[i];
      i++;
      k++;

    }

    else {

      points[k] = right[i];
      i++;
      k++;

    }

  }

  // After while loop exits, it adds the rest of the leftover sorted points in the main point array.
  while (i < s1) {

    points[k] = left[i];
    i++;
    k++;

  }

  while (j < s2) {

    points[k] = right[j];
    j++;
    k++;

  }

  // Frees the memory of the dynamically allocated arrays.
  free(left);
  free(right);

}

// It uses recursion to repeated split the main points array into halves and create "imaginary arrays" until a threshold size is reached.
void mergeSort(point *points, int l, int r, int threshold) {

  if (l < r && (r-l+1) >= threshold) {

    // Calculates the midpoint to split the main array in half.
    int mid = (l+r)/2;

    mergeSort(points, l, mid, threshold);
    mergeSort(points, mid+1, r, threshold);
    merge(points, l, mid, r);

  }

  // Uses insertion sort as a shortcut when the threshold is reached.
  else {

    insertionSort(points, l, r);

  }
  
}

// A wrapper sort function.
void sortPoints(point *points, int size, int threshold) {

  mergeSort(points, 0, size, threshold);

}

// It takes a target value from the query and checks if that value is in the sorted main points array.
int binarySearch(point *points, point *query, int l, int r) {

  if (l <= r) {

    // Calculates midpoint of the array for efficiency.
    int mid = (l+r)/2;

    if (compareTo(&points[mid], query) == -1) {

      return binarySearch(points, query, mid+1, r);

    }

    else if (compareTo(&points[mid], query) == 1) {

      return binarySearch(points, query, l, mid-1);

    }

    // If the value is found, all of the functions in the call stack will return that value.
    else {

      return mid;

    }

  }

  // If the if statement above is fall, then the value wasn't found and returns -1;
  return -1;

}

// Prints all of the sorted points from the main points array of size numPoints into the console and the out.txt file.
void printPoints(point *points, int numPoints) {

  for (int i = 0; i < numPoints; i++) {

    printf("%d %d\n", points[i].x, points[i].y);
    fprintf(outptr, "%d %d\n", points[i].x, points[i].y);

  }

}

// Prints all of the queries and prints out whether the value wasnt found or what rank the value was found in.
void printQueries(point *points, point *query, int numQueries, int numPoints) {

  int index;

  for (int i = 0; i < numQueries; i++) {

    // Uses binary search to find the target index or prints a different message if the function returns -1;
    index = binarySearch(points, &query[i], 0, numPoints-1);

    if (index != -1) {

      printf("%d %d found at rank %d\n", points[index].x, points[index].y, index+1);
      fprintf(outptr, "%d %d found at rank %d\n", points[index].x, points[index].y, index+1);

    }

    else {

      printf("%d %d not found\n", query[i].x, query[i].y);
      fprintf(outptr, "%d %d not found\n", query[i].x, query[i].y);

    }

  }

}

int main(void) {

  atexit(report_mem_leak);

  // Variables needed for the reading into the in.txt file.
  // An array that hold all of the points and an array that holds all of the queries needed to be solved.
  int numPoints, threshold, numQueries;
  point *points, *query;

  // Opens the in.txt and the out.txt file.
  inptr = fopen("in.txt", "r");
  outptr = fopen("out.txt", "w");

  // Establishes the initial location (Your location).
  initLocation = ReadData();

  // Reads the numbers into the respective variables.
  fscanf(inptr, "%d", &numPoints);
  fscanf(inptr, "%d", &numQueries);
  fscanf(inptr, "%d", &threshold);

  // Dynamically allocated the points and query array of size numPoints and numQueries respectively.
  points = (point*)malloc(numPoints * sizeof(point));
  query = (point*)malloc(numQueries * sizeof(point));

  // Reads each point in the in.txt file and stores in each index of the points array.
  for (int i = 0; i < numPoints; i++) {

    points[i] = ReadData();  

  }

  // Reads each point in the in.txt file and stores in each index of the points array.
  for (int i = 0; i < numQueries; i++) {

    query[i] = ReadData();

  }

  // Calls the sorting and printing functions.
  sortPoints(points, numPoints-1, threshold);
  printPoints(points, numPoints);
  printQueries(points, query, numQueries, numPoints);

  // Frees the allocated memory of the points and query array.
  free(points);
  free(query);
  
  return 0;
  
}