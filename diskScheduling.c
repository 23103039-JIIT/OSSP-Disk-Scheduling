#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_REQUESTS 100   // maximum number of requests
#define MAX_SEQUENCE 200   // maximum for head movement sequence

void printSequence(int seq[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d", seq[i]);
        if (i != size - 1)
            printf(" -> ");
    }
    printf("\n");
}

int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

void fcfs_DiskScheduling(int requests[], int n, int head) {
    int seek_time = 0;
    int sequence[MAX_SEQUENCE];
    int seek_seq[MAX_SEQUENCE];
    int seq_index = 0, seek_index = 0;

    sequence[seq_index++] = head;
    for (int i = 0; i < n; i++) {
        int dist = abs(requests[i] - head);
        seek_seq[seek_index++] = dist;
        seek_time += dist;
        head = requests[i];
        sequence[seq_index++] = head;
    }

    printf("\n=== FCFS Disk Scheduling ===\n");
    printf("Head sequence: ");
    printSequence(sequence, seq_index);
    printf("Seek sequence (movements): ");
    printSequence(seek_seq, seek_index);
    printf("Total head movements: %d\n", seek_time);
    printf("Total seek time: %d ms\n", seek_time);
}

void sstf_DiskScheduling(int requests[], int n, int head) {
    int seek_time = 0;
    int visited[MAX_REQUESTS] = {0};
    int sequence[MAX_SEQUENCE];
    int seek_seq[MAX_SEQUENCE];
    int seq_index = 0, seek_index = 0;

    sequence[seq_index++] = head;

    for (int i = 0; i < n; i++) {
        int min_distance = INT_MAX;
        int index = -1;

        for (int j = 0; j < n; j++) {
            if (!visited[j]) {
                int distance = abs(requests[j] - head);
                if (distance < min_distance) {
                    min_distance = distance;
                    index = j;
                }
            }
        }

        visited[index] = 1;
        seek_time += min_distance;
        seek_seq[seek_index++] = min_distance;
        head = requests[index];
        sequence[seq_index++] = head;
    }

    printf("\n=== Shortest Seek Time First Disk Scheduling ===\n");
    printf("Head sequence: ");
    printSequence(sequence, seq_index);
    printf("Seek sequence (movements): ");
    printSequence(seek_seq, seek_index);
    printf("Total head movements: %d\n", seek_time);
    printf("Total seek time: %d ms\n", seek_time);
}

void scan_DiskScheduling(int requests[], int n, int head, int disk_size, char direction) {
    int seek_time = 0;
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int sequence[MAX_SEQUENCE], seek_sequence[MAX_SEQUENCE];
    int lcount = 0, rcount = 0, seq_index = 0, seek_index = 0;

    for (int i = 0; i < n; i++) {
        if (requests[i] < head)
            left[lcount++] = requests[i];
        else
            right[rcount++] = requests[i];
    }

    qsort(left, lcount, sizeof(int), compare);
    qsort(right, rcount, sizeof(int), compare);

    sequence[seq_index++] = head;

    if (direction == 'R') {
        for (int i = 0; i < rcount; i++) {
            int dist = abs(right[i] - head);
            seek_time += dist;
            seek_sequence[seek_index++] = dist;
            head = right[i];
            sequence[seq_index++] = head;
        }

        if (lcount > 0) {
            int dist_to_end = abs((disk_size - 1) - head);
            seek_time += dist_to_end;
            seek_sequence[seek_index++] = dist_to_end;
            head = disk_size - 1;
            sequence[seq_index++] = head;

            for (int i = lcount - 1; i >= 0; i--) {
                int dist = abs(left[i] - head);
                seek_time += dist;
                seek_sequence[seek_index++] = dist;
                head = left[i];
                sequence[seq_index++] = head;
            }
        }
    } else {
        for (int i = lcount - 1; i >= 0; i--) {
            int dist = abs(left[i] - head);
            seek_time += dist;
            seek_sequence[seek_index++] = dist;
            head = left[i];
            sequence[seq_index++] = head;
        }

        if (rcount > 0) {
            int dist_to_start = abs(head - 0);
            seek_time += dist_to_start;
            seek_sequence[seek_index++] = dist_to_start;
            head = 0;
            sequence[seq_index++] = head;

            for (int i = 0; i < rcount; i++) {
                int dist = abs(right[i] - head);
                seek_time += dist;
                seek_sequence[seek_index++] = dist;
                head = right[i];
                sequence[seq_index++] = head;
            }
        }
    }

    printf("\n=== SCAN Disk Scheduling ===\n");
    printf("Head sequence: ");
    printSequence(sequence, seq_index);
    printf("Seek sequence: ");
    for (int i = 0; i < seek_index; i++) printf("%d ", seek_sequence[i]);
    printf("\nTotal head movements: %d\nTotal seek time: %d\n", seek_time, seek_time);
}

void cscan_DiskScheduling(int requests[], int n, int head, int disk_size, char direction) {
    int seek_time = 0;
    int left[MAX_REQUESTS], right[MAX_REQUESTS];
    int sequence[MAX_SEQUENCE], seek_sequence[MAX_SEQUENCE];
    int lcount = 0, rcount = 0, seq_index = 0, seek_index = 0;

    for (int i = 0; i < n; i++) {
        if (requests[i] < head)
            left[lcount++] = requests[i];
        else
            right[rcount++] = requests[i];
    }

    qsort(left, lcount, sizeof(int), compare);
    qsort(right, rcount, sizeof(int), compare);

    sequence[seq_index++] = head;

    if (direction == 'R') {
        for (int i = 0; i < rcount; i++) {
            int dist = abs(right[i] - head);
            seek_time += dist;
            seek_sequence[seek_index++] = dist;
            head = right[i];
            sequence[seq_index++] = head;
        }

        if (lcount > 0) {
            int dist_to_end = abs((disk_size - 1) - head);
            seek_time += dist_to_end;
            seek_sequence[seek_index++] = dist_to_end;

            seek_time += (disk_size - 1);
            seek_sequence[seek_index++] = (disk_size - 1);

            head = 0;
            sequence[seq_index++] = head;

            for (int i = 0; i < lcount; i++) {
                int dist = abs(left[i] - head);
                seek_time += dist;
                seek_sequence[seek_index++] = dist;
                head = left[i];
                sequence[seq_index++] = head;
            }
        }
    } else {
        for (int i = lcount - 1; i >= 0; i--) {
            int dist = abs(left[i] - head);
            seek_time += dist;
            seek_sequence[seek_index++] = dist;
            head = left[i];
            sequence[seq_index++] = head;
        }

        if (rcount > 0) {
            seek_time += head;
            seek_sequence[seek_index++] = head;
            seek_time += (disk_size - 1);
            seek_sequence[seek_index++] = (disk_size - 1);

            head = disk_size - 1;
            sequence[seq_index++] = head;

            for (int i = rcount - 1; i >= 0; i--) {
                int dist = abs(right[i] - head);
                seek_time += dist;
                seek_sequence[seek_index++] = dist;
                head = right[i];
                sequence[seq_index++] = head;
            }
        }
    }

    printf("\n=== C-SCAN Disk Scheduling ===\n");
    printf("Head sequence: ");
    printSequence(sequence, seq_index);
    printf("Seek sequence: ");
    for (int i = 0; i < seek_index; i++) printf("%d ", seek_sequence[i]);
    printf("\nTotal head movements: %d\nTotal seek time: %d\n", seek_time, seek_time);
}

int main() {
    int head = 50;
    int requests[] = {176, 79, 34, 60, 92, 11, 41, 114};
    int n = sizeof(requests) / sizeof(requests[0]);
    int disk_size = 200;

    fcfs_DiskScheduling(requests, n, head);
    sstf_DiskScheduling(requests, n, head);
    scan_DiskScheduling(requests, n, head, disk_size, 'R');
    cscan_DiskScheduling(requests, n, head, disk_size, 'R');

    return 0;
}
