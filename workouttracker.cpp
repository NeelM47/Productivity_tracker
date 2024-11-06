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

	auto writeExercises = [&outFile](const std::vector<Exercise>& exercises) {
		size_t size = exercises.size();
		std::cout << "Writing "<< size << " exercises to the file." << std::endl;

		outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));
		for (const auto& exercise : exercises) {
			size_t nameLength = exercise.name.size();
			size_t repsLength = exercise.repetitions.size();
			
			std::cout << "Writing exercise: " << exercise.name <<", Repetitions: " << exercise.repetitions << std::endl;
			std::cout << "Name Length: " << nameLength <<", Name: " << exercise.name << std::endl;
			// Write name length and name
            outFile.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
            outFile.write(exercise.name.c_str(), nameLength);

            // Write repetitions length and repetitions
			std::cout << "Repetitions Length: " << repsLength << ", Repetitions: " << exercise.repetitions << std::endl;
            outFile.write(reinterpret_cast<const char*>(&repsLength), sizeof(repsLength));
            outFile.write(exercise.repetitions.c_str(), repsLength);
		}
	};
	
    writeExercises(arms);
    writeExercises(core);
    writeExercises(legs);

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

int readWorkoutFromFile(const std::string& filename,
						 std::vector<Exercise>& arms,
						 std::vector<Exercise>& core,
						 std::vector<Exercise>& legs,
						 size_t& index) {
	std::ifstream inFile(filename, std::ios::binary);	

    if (!inFile) {
        std::cerr << "Error opening file !" << std::endl;
        return 1;
    }

	std::cout << "Initial read position: " << inFile.tellg() << std::endl;
	inFile.read(reinterpret_cast<char*>(&index), sizeof(index));
	std::cout << "After read position: " << inFile.tellg() << std::endl;
	std::cout << "Index is: " << index << std::endl;
	size_t totalExercises = 3;

    // Function to read exercises from file and load into the vector
    auto readExercises = [&inFile](std::vector<Exercise>& exercises, size_t position) {
        size_t size;
		//inFile.seekg(151, std::ios::beg); 
		//inFile.seekg(240, std::ios::beg); 
		//std::cout << "Initial read position: " << inFile.tellg() << std::endl;
		
		inFile.seekg(position, std::ios::beg);
        inFile.read(reinterpret_cast<char*>(&size), sizeof(size));  // Read number of exercises
        exercises.resize(size);

        for (size_t i = 0; i < size; ++i) {
            size_t nameLength, repsLength;

            // Read name length and name
            inFile.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
            exercises[i].name.resize(nameLength);
            inFile.read(&exercises[i].name[0], nameLength);

            // Read repetitions length and repetitions
            inFile.read(reinterpret_cast<char*>(&repsLength), sizeof(repsLength));
            exercises[i].repetitions.resize(repsLength);
            inFile.read(&exercises[i].repetitions[0], repsLength);
        }
    };

    // Read all exercises
	if (index == 0){
    	readExercises(arms, 8);
	} else if (index == 1){
    	readExercises(core, 151);
	} else if (index == 2){
    	readExercises(legs, 240);
	} else {
    	std::cerr << "Index out of bounds! No exercises to display." << std::endl;
	}

    inFile.close();  // Close the file after reading

	return index;

}

int main() {

	string accomp;
	size_t index=0;
	// system("clear");
	
    // Define the workout routine
 //   vector<Exercise> arms = {
 //       {"Pushups", "40-30"},
 //       {"Diamond Pushups", "20"},
 //       {"One Hand Pushups", "(10-10) (10-10)"},
 //       {"Weight", "70-70"}
 //   };

 //   vector<Exercise> core = {
 //       {"Plank", "4-3-3-3-3"},
 //       {"Crunches", "30"},
 //       {"Ab Roll", "25"}
 //   };

 //   vector<Exercise> legs = {
 //       {"Stretching", "90"},
 //       {"Situps", "60"},
 //       {"Pistol Squats", "10-10"},
 //       {"Chair", "3-3 min"},
 //       {"Stretching (Tadasana)", "4 min"}
 //   };

//	writeWorkoutToFile("workout.bin", arms, core, legs, index);

	std::vector<Exercise> arms, core, legs;

	int retindex = readWorkoutFromFile("workout.bin", arms, core, legs, index);

	if (retindex == 0){
    	std::cout << "Arms Exercises:\n";
    	for (const auto& exercise : arms) {
    	    std::cout << "- " << exercise.name << ": " << exercise.repetitions << std::endl;
    	}
	} else if (retindex == 1){
    	std::cout << "\nCore Exercises:\n";
    	for (const auto& exercise : core) {
    	    std::cout << "- " << exercise.name << ": " << exercise.repetitions << std::endl;
    	}
	} else if (retindex == 2){
    	std::cout << "\nLegs Exercises:\n";
    	for (const auto& exercise : legs) {
    	    std::cout << "- " << exercise.name << ": " << exercise.repetitions << std::endl;
    	}
	} else {
    	std::cerr << "Index out of bounds! No exercises to display." << std::endl;
	}

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

