#include <bits/stdc++.h>
using namespace std;

void printSequence(const vector<int>& seq) {
    for (int i = 0; i < seq.size(); ++i) {
        cout << seq[i];
        if (i != seq.size() - 1) cout << " -> ";
    }
    cout << endl;
}

void fcfs_DiskScheduling(vector<int> &requests, int head) {
    int seek_time = 0;
    vector<int> sequence;
    vector<int> seek_seq;
    int n = requests.size();

    sequence.push_back(head);
    for (int i = 0; i < n; i++) {
        seek_seq.push_back(abs(requests[i] - head));
        seek_time += abs(requests[i] - head);
        head = requests[i];
        sequence.push_back(head);
    }

    cout << "\n=== FCFS Disk Scheduling ===\n";
    cout << "Head sequence: ";
    printSequence(sequence);
    cout << "Seek sequence (movements): ";
    printSequence(seek_seq);
    cout << "Total head movements: " << seek_time << endl;
    cout << "Total seek time: " << seek_time << " ms\n";
}

void sstf_DiskScheduling(vector<int> &requests, int head) {
    int seek_time = 0;
    int n = requests.size();
    vector<bool> visited(n, false);
    vector<int> sequence;
    vector<int> seek_seq;

    sequence.push_back(head);

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

        visited[index] = true;
        seek_time += min_distance;
        seek_seq.push_back(min_distance);
        head = requests[index];
        sequence.push_back(head);
    }

    cout << "\n=== Shortest Seek Time First Disk Scheduling ===\n";
    cout << "Head sequence: ";
    printSequence(sequence);
    cout << "Seek sequence (movements): ";
    printSequence(seek_seq);
    cout << "Total head movements: " << seek_time << endl;
    cout << "Total seek time: " << seek_time << " ms\n";
}

void scan_DiskScheduling(vector<int> &requests, int head, int disk_size, char direction) {
    int seek_time = 0;
    int n = requests.size();
    vector<int> left, right;
    vector<int> sequence;
    vector<int> seek_sequence;

    for (int i = 0; i < n; i++) {
        if (requests[i] < head)
            left.push_back(requests[i]);
        else
            right.push_back(requests[i]);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    sequence.push_back(head);

        if (direction == 'R') {
        // Move right first
        for (int i = 0; i < right.size(); i++) {
            int dist = abs(right[i] - head);
            seek_time += dist;
            seek_sequence.push_back(dist);
            head = right[i];
            sequence.push_back(head);
        }
        // Then reverse direction if there are requests on the left
        if (!left.empty()) {
            int dist = abs(head - left.back());
            seek_time += dist;
            seek_sequence.push_back(dist);
            head = left.back();
            sequence.push_back(head);

            for (int i = left.size() - 2; i >= 0; i--) {
                dist = abs(left[i] - head);
                seek_time += dist;
                seek_sequence.push_back(dist);
                head = left[i];
                sequence.push_back(head);
            }
        }
    } else {
        // Move left first
        for (int i = left.size() - 1; i >= 0; i--) {
            int dist = abs(left[i] - head);
            seek_time += dist;
            seek_sequence.push_back(dist);
            head = left[i];
            sequence.push_back(head);
        }
        // Then reverse direction if there are requests on the right
        if (!right.empty()) {
            int dist = abs(head - right.front());
            seek_time += dist;
            seek_sequence.push_back(dist);
            head = right.front();
            sequence.push_back(head);

            for (int i = 1; i < right.size(); i++) {
                dist = abs(right[i] - head);
                seek_time += dist;
                seek_sequence.push_back(dist);
                head = right[i];
                sequence.push_back(head);
            }
        }
    }

    cout << "\nSCAN Disk Scheduling:\n";
    cout << "Head sequence: ";
    printSequence(sequence);
    cout << "Seek sequence: ";
    for (int s : seek_sequence) cout << s << " ";
    cout << "\nTotal head movements: " << seek_time;
    cout << "\nTotal seek time: " << seek_time << endl;
}

void cscan_DiskScheduling(vector<int> &requests, int head, int disk_size, char direction) {
    int seek_time = 0;
    int n = requests.size();
    vector<int> left, right;
    vector<int> sequence;
    vector<int> seek_sequence;

    for (int i = 0; i < n; i++) {
        if (requests[i] < head)
            left.push_back(requests[i]);
        else
            right.push_back(requests[i]);
    }

    sort(left.begin(), left.end());
    sort(right.begin(), right.end());

    sequence.push_back(head);

        if (direction == 'R') {
        // Move right first
        for (int i = 0; i < right.size(); i++) {
            int dist = abs(right[i] - head);
            seek_time += dist;
            seek_sequence.push_back(dist);
            head = right[i];
            sequence.push_back(head);
        }

        // Wrap around from end to start
        if (!left.empty()) {
            int dist_to_end = abs((disk_size - 1) - head);
            seek_time += dist_to_end;
            seek_sequence.push_back(dist_to_end);
            head = 0;
            int wrap = disk_size - 1; // full wrap
            seek_time += wrap;
            seek_sequence.push_back(wrap);
            sequence.push_back(head);

            // Then service left side from smallest upward
            for (int i = 0; i < left.size(); i++) {
                int dist = abs(left[i] - head);
                seek_time += dist;
                seek_sequence.push_back(dist);
                head = left[i];
                sequence.push_back(head);
            }
        }
    } else {
        // Move left first
        for (int i = left.size() - 1; i >= 0; i--) {
            int dist = abs(left[i] - head);
            seek_time += dist;
            seek_sequence.push_back(dist);
            head = left[i];
            sequence.push_back(head);
        }

        // Wrap around from start to end
        if (!right.empty()) {
            int dist_to_start = abs(head - 0);
            seek_time += dist_to_start;
            seek_sequence.push_back(dist_to_start);
            head = disk_size - 1;
            int wrap = disk_size - 1;
            seek_time += wrap;
            seek_sequence.push_back(wrap);
            sequence.push_back(head);

            for (int i = right.size() - 1; i >= 0; i--) {
                int dist = abs(right[i] - head);
                seek_time += dist;
                seek_sequence.push_back(dist);
                head = right[i];
                sequence.push_back(head);
            }
        }
    }

    cout << "\nC-SCAN Disk Scheduling:\n";
    cout << "Head sequence: ";
    printSequence(sequence);
    cout << "Seek sequence: ";
    for (int s : seek_sequence) cout << s << " ";
    cout << "\nTotal head movements: " << seek_time;
    cout << "\nTotal seek time: " << seek_time << endl;
}

int main() {
    int head = 50;
    vector<int> requests = {176, 79, 34, 60, 92, 11, 41, 114};
    int disk_size = 200;

    fcfs_DiskScheduling(requests, head);
    sstf_DiskScheduling(requests, head);
    scan_DiskScheduling(requests, head, disk_size, 'R');
    cscan_DiskScheduling(requests, head, disk_size, 'R');

    return 0;
}