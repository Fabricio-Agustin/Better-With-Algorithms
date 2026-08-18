#include <stdio.h>

int binarySearch(int arr[], int size, int target) {
    int left = 0;
    int right = size - 1;

    while (left <= right) {
        int middle = left + (right - left) / 2;

        if (arr[middle] == target) {
            return middle;
        }

        if (arr[middle] < target) {
            left = middle + 1;
        }
        else {
            right = middle - 1;
        }
    }

    return -1;
}

int main() {
    int data[] = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};
    int n = sizeof(data) / sizeof(data[0]);
    int target;

    printf("Enter the number you want to search for: ");
    scanf("%d", &target);

    int result = binarySearch(data, n, target);

    if (result != -1) {
        printf("Element %d found at index: %d!\n", target, result);
    }
    else {
        printf("The element was not found in the array.\n");
    }

    return 0;
}
