#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

// Enum to represent different types of exercises
enum class ExerciseType { CARDIO, STRENGTH };

// Structure to hold exercise details
struct Exercise {
  string name;         // Name of the exercise
  ExerciseType type;   // Type of exercise (Cardio or Strength)
  int durationOrSets;  // Duration for cardio, sets for strength
  int intensityOrReps; // Intensity for cardio, reps for strength
  int weight;          // For strength training
  string date;         // Date of exercise
};

// Class to track and manage exercises
class FitnessTracker {
private:
  vector<Exercise> exerciseLog; // Log of all exercises
  vector<pair<string, vector<Exercise>>>
      exerciseDatabase; // vector of string and exercise pairs, where the string
                        // is an identifier for the exercise

public:
  void addExerciseToLog(const Exercise &exercise) {
    exerciseLog.push_back(exercise);
    // lambda function to sort the exercises by their date
    sort(exerciseLog.begin(), exerciseLog.end(),
         [](const Exercise &a, const Exercise &b) -> bool {
           return a.date < b.date;
         });
  }

  // Display all exercises in the log
  void displayExerciseLog() const {
    cout << "Exercise Log:\n";
    for (const Exercise &exercise : exerciseLog) {
      cout << "Date: " << exercise.date << ", Exercise: " << exercise.name
           << ", Type: ";
      if (exercise.type == ExerciseType::CARDIO)
        cout << "Cardio, Duration: " << exercise.durationOrSets << " mins"
             << endl;
      else
        cout << "Strength, Sets: " << exercise.durationOrSets
             << ", Reps: " << exercise.intensityOrReps
             << ", Weight: " << exercise.weight << " lbs" << endl;
    }
  }

  // Add an exercise to the database
  void addExerciseToDatabase(const Exercise &exercise) {
    // Check if there's an entry for the exercise date in the database
    vector<pair<string, vector<Exercise>>>::iterator it = find_if(
        exerciseDatabase.begin(), exerciseDatabase.end(),
        [&exercise](const pair<string, vector<Exercise>> &entry) -> bool {
          return entry.first == exercise.date;
        });

    if (it == exerciseDatabase.end()) {
      // If not, create a new entry with the date and exercise
      exerciseDatabase.emplace_back(
          make_pair(exercise.date, vector<Exercise>{exercise}));
    } else {
      // If entry exists, add the exercise to that date's entry
      it->second.push_back(exercise);
    }
  }

  // Display the exercise database sorted by date
  void displayExerciseDatabase() const {
    cout << "Exercise Database (Sorted by Date):\n";
    for (const pair<string, vector<Exercise>> &entry : exerciseDatabase) {
      cout << "\nDate: " << entry.first << "\n";
      for (const Exercise &exercise : entry.second) {
        cout << "    Exercise: " << exercise.name << ", Type: ";
        if (exercise.type == ExerciseType::CARDIO)
          cout << "Cardio, Duration: " << exercise.durationOrSets << " mins"
               << endl;
        else
          cout << "Strength, Sets: " << exercise.durationOrSets
               << ", Reps: " << exercise.intensityOrReps
               << ", Weight: " << exercise.weight << " lbs" << endl;
      }
    }
  }

  // Export 1RM (One Rep Max) data for a specific exercise to a file
  void export1RMData(const string &exerciseName, const string &filename) const {
    ofstream outputFile(filename);

    if (outputFile.is_open()) {
      outputFile << "Date,1RM\n";
      for (const pair<string, vector<Exercise>> &entry : exerciseDatabase) {
        for (const Exercise &exercise : entry.second) {
          if (exercise.name == exerciseName &&
              exercise.type == ExerciseType::STRENGTH) {
            // Calculate 1RM and write to file
            double oneRepMax = static_cast<double>(exercise.weight) /
                               (1.0278 - 0.0278 * exercise.intensityOrReps);
            outputFile << entry.first << "," << oneRepMax << "\n";
          }
        }
      }
      outputFile.close();
      cout << "1RM data exported to " << filename << endl;
    } else {
      cout << "Unable to open file for writing." << endl;
    }
  }
};

// Function to create a cardio exercise
Exercise createCardioExercise() {
  Exercise exercise;
  exercise.type = ExerciseType::CARDIO;

  cout << "Enter cardio exercise name: ";
  cin.ignore();
  getline(cin, exercise.name);

  cout << "Enter duration (in minutes): ";
  cin >> exercise.durationOrSets;

  exercise.intensityOrReps = 0; // Cardio doesn't have reps
  exercise.weight = 0;          // No weight for cardio

  cout << "Enter date of exercise (YYYY-MM-DD): ";
  cin >> exercise.date;

  return exercise;
}

// Function to create a strength exercise
Exercise createStrengthExercise() {
  Exercise exercise;
  exercise.type = ExerciseType::STRENGTH;

  cout << "Enter strength training exercise name: ";
  cin.ignore();
  getline(cin, exercise.name);

  cout << "Enter number of sets: ";
  cin >> exercise.durationOrSets;

  cout << "Enter number of reps: ";
  cin >> exercise.intensityOrReps;

  cout << "Enter weight (in lbs): ";
  cin >> exercise.weight;

  cout << "Enter date of exercise (YYYY-MM-DD): ";
  cin >> exercise.date;

  return exercise;
}

// Main function
int main() {
  FitnessTracker tracker;

  char addMore = 'y';
  do {
    cout << "Select exercise type:\n";
    cout << "1. Cardio\n";
    cout << "2. Strength Training\n";
    int choice;
    cin >> choice;

    Exercise newExercise;
    switch (choice) {
    case 1:
      newExercise = createCardioExercise();
      break;
    case 2:
      newExercise = createStrengthExercise();
      break;
    default:
      cout << "Invalid choice.\n";
      continue;
    }

    tracker.addExerciseToDatabase(newExercise);
    tracker.addExerciseToLog(newExercise);

    cout << "Do you want to add another exercise? (y/n): ";
    cin >> addMore;
  } while (addMore == 'y' || addMore == 'Y');

  tracker.displayExerciseDatabase();
  tracker.export1RMData("Bench Press", "1RM_data.csv");

  return 0;
}
