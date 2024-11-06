#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <thread>
#include <cstdlib>

using namespace std;

// Function to get the current time as a string
string getCurrentTime() {
    auto now = chrono::system_clock::now();
    auto now_c = chrono::system_clock::to_time_t(now);
    std::tm *parts = std::localtime(&now_c);

    std::ostringstream oss;
    oss << std::put_time(parts, "%H:%M"); // Format as HH:MM
    return oss.str();
}

// Function to ask if the activity was done
bool isActivityDone(const string& scheduledTime, const string& activity) {
    cout << "\n==========================================\n";
    cout << "Scheduled Time: " << scheduledTime << " | Current Time: " << getCurrentTime() << endl;
    cout << "Activity: " << activity << endl;
    cout << "==========================================\n";
    cout << "Did you complete this activity? (y/n): ";

    char response;
    cin >> response;
    cout << endl;
    return (response == 'y' || response == 'Y');
}

int main() {
    system("clear");
    vector<pair<string, string>> activities = {
        {"8:00", "Wake up in bed."},
        {"8:05", "Go to washroom and brush your teeth."},
        {"9:00", "Ready to exercise next to bed."},
        {"10:30", "Go to the bathroom to have a bath."},
        {"11:30", "Dress and make breakfast in kitchen."},
        {"12:15", "Return to room to eat."},
        {"12:30", "Eat in bed."},
        {"12:30", "Move to bed to study."},
        {"12:35", "Study in bed with earphones."},
        {"20:30", "Make dinner in kitchen."},
        {"21:00", "Read 'The Man Who Mistook His Wife for a Hat.'"}
    };

    // Iterate through each activity
    for (const auto& [time, activity] : activities) {
        if (activity == "Ready to exercise next to bed.") {
            cout << "\nNext activity is exercise. Redirecting to the exercise program...\n";
            
            // Call the exercise program
            int result = system("./workout"); // Execute the exercise program
            
            if (result != 0) {
                cout << "\n ⚠️  There was an issue running the exercise program.\n";
            } else {
                cout << "\n ✅ Exercise completed successfully. Moving on to the next activity.\n";
            }
            
            // Skip the prompt for this activity since it's handled by exercise_program
            continue;
        }

        if (isActivityDone(time, activity)) {
            cout << "✅ Great job! Moving on to the next activity.\n";
        } else {
            cout << "⚠️  No problem! You can catch up later.\n";
        }
		
        // Optional: Wait for a second before the next activity for readability
        //std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    cout << "\nAll activities have been processed. Have a productive day!\n";
    return 0;
}

