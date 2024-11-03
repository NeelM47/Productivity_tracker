#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>

using namespace std;

struct Exercise {
	std::string name;
	std::string repetitions;
};

void writeExercises(std::ofstream& outFile, const std::vector<Exercise>& exercises) {
    size_t size = exercises.size();
    outFile.write(reinterpret_cast<const char*>(&size), sizeof(size)); // Write size
    for (const auto& exercise : exercises) {
        size_t nameLength = exercise.name.size();
        outFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        outFile.write(exercise.name.c_str(), nameLength);

        size_t repsLength = exercise.repetitions.size();
        outFile.write(reinterpret_cast<const char*>(&repsLength), sizeof(repsLength));
        outFile.write(exercise.repetitions.c_str(), repsLength);
    }
}

void writeWorkoutToFile(const std::string& filename,
                         const std::vector<Exercise>& arms,
                         const std::vector<Exercise>& core,
                         const std::vector<Exercise>& legs,
                         size_t& index) {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }

    // Write the current index
    outFile.write(reinterpret_cast<const char*>(&index), sizeof(index));

    // Write exercises for arms
    size_t armsSize = arms.size();
    outFile.write(reinterpret_cast<const char*>(&armsSize), sizeof(armsSize)); // Write the number of arm exercises

    writeExercises(outFile, arms);
    writeExercises(outFile, core);
    writeExercises(outFile, legs);

    outFile.close();
}

void updateIndexInFile(const std::string& filename, size_t index) {
    // Open the file in read and write mode
    std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error opening file for updating index!" << std::endl;
        return;
    }

    // Move the file pointer to the beginning of the file to read the current index
	file.read(reinterpret_cast<char*>(&index), sizeof(index));

	index = (index + 1) % 3;
	file.seekp(0);
	file.write(reinterpret_cast<const char*>(&index), sizeof(index));

    file.close();
}

void readWorkoutFromFile(const std::string& filename,
						 std::vector<Exercise>& arms,
						 std::vector<Exercise>& core,
						 std::vector<Exercise>& legs,
						 size_t& index) {

    std::fstream file;

	file.open(filename, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error opening file !" << std::endl;
        return;
    }

	file.read(reinterpret_cast<char*>(&index), sizeof(index));
	// std::cout << "Current index: " << index << std::endl;

	// Read exercises for arms
    size_t armsSize;
	//std::cout << "armSize: " << armsSize << std::endl;
    file.read(reinterpret_cast<char*>(&armsSize), sizeof(armsSize)); // Read number of arm exercises
	arms.reserve(armsSize);
    arms.resize(armsSize);

    for (size_t i = 0; i < armsSize; ++i) {
        size_t nameLength, repsLength;

        // Read name
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        arms[i].name.resize(nameLength);
        file.read(&arms[i].name[0], nameLength);

        // Read repetitions
        file.read(reinterpret_cast<char*>(&repsLength), sizeof(repsLength));
        arms[i].repetitions.resize(repsLength);
        file.read(&arms[i].repetitions[0], repsLength);
    }

    // Read exercises for core
    size_t coreSize;
	//std::cout << "coreSize: " << coreSize << std::endl;
    file.read(reinterpret_cast<char*>(&coreSize), sizeof(coreSize)); // Read number of core exercises
	core.reserve(coreSize);
    core.resize(coreSize);

    for (size_t i = 0; i < coreSize; ++i) {
        size_t nameLength, repsLength;

        // Read name
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        core[i].name.resize(nameLength);
        file.read(&core[i].name[0], nameLength);

        // Read repetitions
        file.read(reinterpret_cast<char*>(&repsLength), sizeof(repsLength));
        core[i].repetitions.resize(repsLength);
        file.read(&core[i].repetitions[0], repsLength);
    }

    // Read exercises for legs
    size_t legsSize;
	//std::cout << "legsSize: " << legsSize << std::endl;
    file.read(reinterpret_cast<char*>(&legsSize), sizeof(legsSize)); // Read number of leg exercises
	legs.reserve(legsSize);
    legs.resize(legsSize);
    for (size_t i = 0; i < legsSize; ++i) {
        size_t nameLength, repsLength;

        // Read name
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        legs[i].name.resize(nameLength);
        file.read(&legs[i].name[0], nameLength);

        // Read repetitions
        file.read(reinterpret_cast<char*>(&repsLength), sizeof(repsLength));
        legs[i].repetitions.resize(repsLength);
        file.read(&legs[i].repetitions[0], repsLength);
    }

	size_t totalExercises = 3;

    if (index < totalExercises) {
        if (index == 0) {
            std::cout << "\nToday's exercise: Arms\n";
			for (const auto& exercise : arms) {
				std::cout << "- " << exercise.name << ": " << exercise.repetitions << std::endl;
        	}
        } else if (index == 1) {
            std::cout << "\nToday's exercise: Core\n";
			for (const auto& exercise : core) {
            std::cout << "- " << exercise.name << ": " << exercise.repetitions << std::endl;
            }
        } else if (index == 2) {
            std::cout << "\nToday's exercise: Legs\n";
			for (const auto& exercise : legs) {
            std::cout << "- " << exercise.name << ": " << exercise.repetitions << std::endl;
        	}
        }
    } else {
        std::cerr << "Index out of bounds! No exercises to display." << std::endl;
    }

    file.close();
}


int main() {

	string accomp;
	size_t index=0;
	// system("clear");
	
    // Define the workout routine
//    vector<Exercise> arms = {
//        {"Pushups", "40-30"},
//        {"Diamond Pushups", "20"},
//        {"One Hand Pushups", "(10-10) (10-10)"},
//        {"Weight", "70-70"}
//    };
//
//    vector<Exercise> core = {
//        {"Plank", "4-3-3-3-3"},
//        {"Crunches", "30"},
//        {"Ab Roll", "25"}
//    };
//
//    vector<Exercise> legs = {
//        {"Stretching", "90"},
//        {"Situps", "60"},
//        {"Pistol Squats", "10-10"},
//        {"Chair", "3-3 min"},
//        {"Stretching (Tadasana)", "4 min"}
//    };

//	  writeWorkoutToFile("workout.bin", arms, core, legs, index);

	std::vector<Exercise> arms, core, legs;

	readWorkoutFromFile("workout.bin", arms, core, legs, index);

	cout <<"\nIs today's exercise done ? ";
	cin >>accomp;

	if (accomp == "y" || accomp == "Y") {
		cout << "Task accomplished !!!" << endl;
		updateIndexInFile("workout.bin", index);
	} else if (accomp == "n" || accomp == "N") {
		cout << "Task completion failed !!!" << endl;
	} else {
		cout << "Invalid input. Please enter 'y' or 'n'." << endl;
	}
		
    return 0;
}

