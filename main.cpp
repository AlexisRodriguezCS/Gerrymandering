//
//   main.cpp
//   Alexis Rodriguez
//	 arodr247
//   CS 251
//	 Project 1 - Gerrymandering
//	 Description:
//   Program reads two files and then allows you to search for a state.
//   Then you can plot the states district's parties percentage per district
//   or request the stats to see if the state was gerrymandered.
//
//   Instructions for creative component:
//   In order for the component to work, the data has to be loaded and a state
//   has to be searched and found. If there's isn't then errors warnings will
//   be displayed. The creative component outputs the searched state votes for
//   for both parties and the differnce.
//   Example:
//   Enter command: votes
//
//	 Total Votes for Illinois: 3544644
//	 Total Democractic Votes: 1822779
//	 Total Republican Votes: 1721865
//	 Democractics had 100914 more votes than the Republicans.

#include <math.h>			// to round numbers
#include <string>
#include <iostream>
#include <sstream>          // to remove whitepspace
#include <fstream>          // to read file
#include <algorithm>		// to lower the string
#include "ourvector.h"
using namespace std;



struct District{
    int num = 0;  // District number
    double rVotes = 0;   // Republican votes
    double dVotes = 0;   // Democractic votes
};

struct State{
    string name;
    int totalDistricts = 0;
    int eVoters = 0;  // eligible voters
    ourvector<District> Districts;
};

void setName(ourvector<State>& Data, string dataLine, int i);
string toLowerCase(string line);
void displayMenu(string dataLoaded, string statePH, string& commandPH);
void displayLines(double totalDs, double totalRs);
void checkForErrors(string dataLoaded, bool stateFound, bool& error);
void plot(ourvector<State>& Data, int num, string load, bool stateFound);
void replaceText(string& line , string text, string replaceText);
void setData(ourvector<State>& Data, string line, int i);
void eVotes(ourvector<State>& Data, string dataLine);
void calOverHalf(double totalVotes, double& overHalf);
void wastedVotes(ourvector<State>& Data, int stateN, int i, double& wDem,
	double& wRep, int overHalf);
double calEG(double wDem, double wRep, double tVotes);
void displayGA(double eGap, int dSize, double wDem, double wRep, double eVote);
void getGerrymandered(ourvector<State>& Data, int stateNum);
void stats(ourvector<State>& Data, int stateN, string load, bool stateFound);
bool search(ourvector<State>& Data, string state, string& statePH, int& stateN);
void searchError(bool stateFound);
string trimString(string& passedString);
bool loadFirst(ourvector<State>& Data, string fileName);
void loadSec(ourvector<State>& Data, string fileName, string& load);
void readFiles(ourvector<State>& Data, string f1, string f2, string& load);
void getVotes(ourvector<State>& Data, int index, int& rVotes, int& dVotes);
void splitInput(string input, string& split1, string& split2);
void votes(ourvector<State>& Data, int index, string load, bool stateFound);

int main() {
    ourvector<State> Data;  // hold all the states
    bool stateFound = false;
    int stateNum;  // hold the index when search is called
    string firstFile, secondFile, commandPH, stateSearched, input;
    string restOfInput;  // used when the input first gets splits
    string dataLoaded = "No";
    string statePH = "N/A";

    cout << "Welcome to the Gerrymandering App!" << endl;

    do {
        displayMenu(dataLoaded, statePH, input);
        splitInput(input, commandPH, restOfInput);
        if (commandPH == "load") {
    		splitInput(restOfInput, firstFile, secondFile);
        	readFiles(Data, firstFile, secondFile, dataLoaded);
        }
        if (commandPH == "search" && dataLoaded == "Yes") {
        	stateSearched = toLowerCase(restOfInput);
            stateFound = search(Data, stateSearched, statePH, stateNum);
            searchError(stateFound);
        } else if (commandPH == "search" && dataLoaded == "No") {
        	cout << endl;
        	cout << "No data loaded, please load data first." << endl;
        }
        if (commandPH == "stats") {
        	stats(Data, stateNum, dataLoaded, stateFound);
        }
        if (commandPH == "plot") {
        	plot(Data, stateNum, dataLoaded, stateFound);
        }
        if (commandPH == "votes") {
        	votes(Data, stateNum, dataLoaded, stateFound);
        }
    } while (toLowerCase(commandPH) != "exit");
    cout << endl;
    return 0;
}

//----------------------------------------------------------------------------->
void setName(ourvector<State>& Data, string dataLine, int i) {
	//  sets the state name by getting the string from 0 to the first comma
	string stateName = dataLine.substr(0, dataLine.find(','));
    Data[i].name = stateName;
    cout << "..." << stateName;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
string toLowerCase(string line) {
	// function to lower any string passed through using <algorithm>
    transform(line.begin(), line.end(), line.begin(), ::tolower);
    return line;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void displayMenu(string dataLoaded, string statePH, string& commandPH) {
	//  function that display the dataLoaded, commandPH, and gets input
	cout << endl;
    cout << "Data loaded? " << dataLoaded << endl;
    cout << "State: " << statePH << endl;
    cout << endl;
    cout << "Enter command: ";
    getline(cin, commandPH, '\n');
    cout << endl;
    cout << "-----------------------------" << endl;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void displayLines(double totalDs, double totalRs) {
	//  called when plot is enter to print of either 'D' or/and 'R'
	if (totalRs > 0 && totalDs > 0) {
		// when the string has both 'D' and 'R'
		cout << string(totalDs, 'D') << string(totalRs, 'R') << endl;
	} else if (totalRs == 0 && totalDs == 100) {
		// if all demVotes and no repVotes
		cout << string(totalDs, 'D') << endl;
	} else if (totalDs == 0 && totalRs == 100) {
		// if all repVotes and no demVotes
		cout << string(totalRs, 'R') << endl;
	} else {
		// no votes
		cout << endl;
	}
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void checkForErrors(string dataLoaded, bool stateFound, bool& error) {
	//  sets error to true if any if statement is true
	if (dataLoaded == "Yes" && stateFound) {  // no errors
        error = false;
    } else if (dataLoaded == "No") {
    	error = true;
        cout << endl;
    	cout << "No data loaded, please load data first." << endl;
    } else if (!stateFound) {
    	error = true;
        cout << endl;
    	cout << "No state indicated, please search for state first." << endl;
    }
}
//----------------------------------------------------------------------------->

//----------------------------------------------------------------------------->
void plot(ourvector<State>& Data, int index, string load, bool stateFound) {
    double totalDs = 0, totalRs = 0;
    double demVotes = 0, repVotes = 0;  // used to shorten code
    bool error;
    checkForErrors(load, stateFound, error);
    if (!error) {  // if checkForErrors doesn't fnd any errors
    	cout << endl;
    	for (int i = 0; i < Data[index].Districts.size(); i++) {
	        cout<< "District: " << i + 1 << endl;
	        demVotes = Data[index].Districts[i].dVotes;
	        repVotes = Data[index].Districts[i].rVotes;
	        totalDs = demVotes/(demVotes + repVotes);  // returns amount of 'D's
	        totalDs = floor(totalDs * 100.00);  // rounds down
	        totalRs = 100.00 - totalDs;  // returns amount of 'R's to print
	        displayLines(totalDs, totalRs);
    	}
    }
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void replaceText(string& line , string text, string replaceText) {
	//  looks for text in line to replace with replaceText
	size_t pos = line.find(text);  // get the value where the text is found
	while (pos != string::npos) {
		line.replace(pos, text.length(), replaceText);
		pos = line.find(text, pos + replaceText.size());
	}
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void setData(ourvector<State>& Data, string line, int stateNum) {
	//  sets the dVotes, rVotes, totalDistricts, and Districts[i].num
    line = line.substr(line.find(',')+ 1);  //  makes line = after first comma
    //  line = 1,33425,56578,2,5546457,444545,3,45657,452353
    string demVotes, repVotes, distNum;  // hold string numbers
    int i = 0;
    size_t pos = line.find(',');  // finds the first comma in string

    while (line.find(',') != string::npos) {  
    	// ^while the line doesn't reach the end (since its being change)^
    	Data.at(stateNum).Districts.push_back(District());  // makes space
    	//---------------------------------------------------
    	distNum = line.substr(0, line.find(','));  // Ex = 1
    	line = line.substr(pos + distNum.size(), line.size());
    	//  ^line gets rid of distNum from line to get next number^
    	//  line = 33425,56578,2,5546457,444545,3,45657,452353
    	Data[stateNum].Districts[i].num = stoi(distNum);
    	//---------------------------------------------------
    	demVotes = line.substr(0, line.find(',')); // Ex = 33425
    	line = line.substr(pos + demVotes.size(), line.size());
    	//  ^line gets rid of demVotes from line to get next number^
    	//  line = 56578,2,5546457,444545,3,45657,452353
    	Data[stateNum].Districts[i].dVotes = stoi(demVotes);
    	//---------------------------------------------------
    	if (line.find(',') == string::npos) {  
    		//  ^check if the line reaches the end^
    		repVotes = line.substr(0, line.find(' '));
    		line = "";
    	} else {
    		// ^still hasn't reached the end^
    		repVotes = line.substr(0, line.find(',')); // Ex: 56578
    		line = line.substr(pos + repVotes.size(), line.size());
    		//  line = 2,5546457,444545,3,45657,452353
    	}
    	Data[stateNum].Districts[i].rVotes = stoi(repVotes);
    	//---------------------------------------------------
    	i++;
    }
    Data[stateNum].totalDistricts = stoi(distNum);
    cout << "..."<< stoi(distNum) << " districts total" << endl;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void eVotes(ourvector<State>& Data, string dataLine) {
	// sets the eligibleVoters for each state that was in the first file
    string stateName = dataLine.substr(0, dataLine.find(','));
    string eligibleVoters;
    for (int i = 0; i < Data.size(); i++) {
        if (stateName == Data[i].name) {
        	// if stateName from 2nd file is in 1st file
            cout << "..." << stateName << "...";
            eligibleVoters = dataLine.substr(dataLine.find(',') + 1);
            Data[i].eVoters = stoi(eligibleVoters);  // turns string into int
            cout << stoi(eligibleVoters) << " eligible voters" << endl;
        }
    }
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void calOverHalf(double totalVotes, double& overHalf) {
	// calculates overHalf for Gerrymandering factor
	totalVotes = totalVotes + 1;  // plus 1 because: 10/2 == 5 but overHalf == 6
	overHalf = round(totalVotes / 2);
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void wastedVotes(ourvector<State>& Data, int stateNum, int i,
	double& wastedDemVotes, double& wastedRepVotes, int overHalf) {
	// calculates wasted votes for each party in Districts[i]
	double repVotes = 0, demVotes = 0;
	repVotes = Data[stateNum].Districts[i].rVotes;  // holds the votes for
	demVotes = Data[stateNum].Districts[i].dVotes;  // the current Districts[i]

	if (demVotes > repVotes) {
		wastedDemVotes += (demVotes - overHalf);  // votes not need to win
		wastedRepVotes += repVotes;  // votes wasted since party lost
	} else if (repVotes > demVotes) {
		wastedDemVotes += demVotes;
		wastedRepVotes += (repVotes - overHalf);
	}
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
double calEG(double wastedDemVotes, double wastedRepVotes, double totalVotes) {
	// returns efficiencyGap by checking which party had more wasted votes
	double efficiencyGap = 0;
	if (wastedDemVotes > wastedRepVotes) {
		efficiencyGap = (wastedDemVotes - wastedRepVotes) / totalVotes;
	} else if (wastedRepVotes > wastedDemVotes) {
		efficiencyGap = (wastedRepVotes - wastedDemVotes) / totalVotes;
	}
	efficiencyGap = efficiencyGap * 100;
	return efficiencyGap;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void displayGA(double eGap, int districtSize, double demVotes, double repVotes,
	double eVotes) {
	// checks if state was Gerrymandered then displays the stats
	string Gerrymandered = "No";
	string gerrymanderedAgainst = "";
	int wastedDemVotes = demVotes;	 // to print full number
	int wastedRepVoters = repVotes;  // to print full number
	int eligibleVotes = eVotes;  // to print full number

	if (wastedDemVotes > wastedRepVoters) {
		gerrymanderedAgainst = "Democrats";
	} else {
		gerrymanderedAgainst = "Republicans";
	}

	if (eGap >= 7.0 && districtSize >= 3) {
		Gerrymandered = "Yes";
	}
	cout << endl;
	cout << "Gerrymandered: " << Gerrymandered << endl;
	if (Gerrymandered == "Yes") {
		cout << "Gerrymandered against: " << gerrymanderedAgainst << endl;
		cout << "Efficiency Factor: " << eGap << "%" << endl;
	}
	cout << "Wasted Democratic votes: " << wastedDemVotes << endl;
    cout << "Wasted Republican votes: " << wastedRepVoters << endl;
    cout << "Eligible voters: " << eligibleVotes << endl;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void getGerrymandered(ourvector<State>& Data, int stateNum) {
    int stateDistrictSize = Data[stateNum].Districts.size();
    double totalStateVotes = 0, overHalf = 0, totalDistrictVotes = 0;
    double wastedDem = 0, wastedRep = 0;
    double demVotes = 0, eVoters = 0 , repVotes = 0;
    double eGap = 0;
    for ( int i = 0; i < stateDistrictSize; i++ ) {
    	repVotes = Data[stateNum].Districts[i].rVotes;
    	demVotes = Data[stateNum].Districts[i].dVotes;
    	totalDistrictVotes = repVotes + demVotes;
    	// ^used to calculate overHalf^
    	totalStateVotes = totalStateVotes + repVotes + demVotes;
    	// ^used to calculate efficiencyGap^
		eVoters = Data[stateNum].eVoters;
        calOverHalf(totalDistrictVotes, overHalf);
        wastedVotes(Data, stateNum, i, wastedDem, wastedRep, overHalf);
    }
    eGap = calEG(wastedDem, wastedRep, totalStateVotes);  // efficiencyGap
    displayGA(eGap, stateDistrictSize, wastedDem, wastedRep, eVoters);
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void stats(ourvector<State>& Data, int stateN, string load, bool stateFound) {
	// called when stats is inputed but first checks for errors
	bool error;
	checkForErrors(load, stateFound, error);
    if (!error) {
        getGerrymandered(Data, stateN);
    }
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void searchError(bool stateFound) {
	// called when input state is searched to check if it was founded
	if (!stateFound) {
		cout << endl;
        cout << "State does not exist, search again." << endl;
    }
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
bool search(ourvector<State>& Data, string inputedState, string& statePH,
	int& stateNum) {
	// searches for inputedState in the Data vector and returns a bool
	bool stateFound = false;
	string stateInVector;
	int dataSize = Data.size();
    for (int i = 0; i < dataSize; i++) {
        stateInVector = toLowerCase(Data[i].name);
        if (inputedState == stateInVector) {
            statePH = Data[i].name;
            stateNum = i;
            stateFound = true;
        }
    }
    return stateFound;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
string trimString(string& passedString) {
    string whitespace = (" \t\n\r");
    passedString.erase(passedString.find_last_not_of(whitespace) + 1);
    return passedString;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
bool loadFirst(ourvector<State>& Data, string fileName) {
	// reads in the first file using the fileName
	bool error = false;
	string fileData;
	ifstream inFS;
	inFS.open(fileName);

	if (!inFS.is_open()) {
		cout << endl;
		cout << "Invalid first file, try again." << endl;
		error = true;
	} else if (!error) {  // if the file did open
		cout << endl;
		cout << "Reading: " << fileName << endl;
		int i = 0;
		getline(inFS, fileData);  // get the first line of data
		while (!inFS.eof()) {
			Data.push_back(State());  // make space
			setName(Data, fileData, i);
			replaceText(fileData, "AL", "1");  // checks if line has "AL"
			setData(Data, fileData, i);
			i++;
			getline(inFS, fileData);
		}
	}
	inFS.close();
	return error;  // returns error depending if the file did/didn't open
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void loadSec(ourvector<State>& Data, string fileName, string& load) {
	// loads the seconds file, checks for errors, then sets eVoters
	bool error = false;
	string fileData;  // holds each line while going through file
	ifstream inFS;
	inFS.open(fileName);

	if (!inFS.is_open()) {
		cout << endl;
		cout << "Invalid second file, try again." << endl;
		error = true;
	} else if (!error) {  // if the file did open
		cout << endl;
		cout << "Reading: " << fileName << endl;
		getline(inFS, fileData);  // gets the first line from file
		while (!inFS.eof()) {  // while the file doesn't reach the end
			eVotes(Data, fileData);
			getline(inFS, fileData);
		}
		load = "Yes";
	}
	inFS.close();
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void readFiles(ourvector<State>& Data, string f1, string f2, string& load) {
	// first checks if data is loaded then reads in both f1(file 1) and f2
	if (load == "Yes") {  //  if the data is loaded
		cout << endl;
        cout << "Already read data in, exit and start over." << endl;
	} else {
		bool firstError = loadFirst(Data, f1);
		// if first file doesn't open then it doesn't read the second one
		if (!firstError) {
			loadSec(Data, f2, load);  //  pass Data vec, file 2 and load
		}
	}
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void splitInput(string input, string& split1, string& split2) {
	// takes one string by finding the first space and splits it up into two
	// Ex: "districts.txt eligible_voters.txt"
    split1 = input.substr(0, input.find(' '));
	split2 = input.substr(input.find(' ') + 1, input.length());
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void getVotes(ourvector<State>& Data, int index, int& repVotes, int& demVotes) {
	// creative componet function that gets the votes using the index where
	// the state selected is located in Data vector then updates repVotes
	// and demVotes by adding up each loop
	int districtSize = Data[index].Districts.size();
	for (int i = 0; i < districtSize; i++) {
		demVotes += Data[index].Districts[i].dVotes;
		repVotes += Data[index].Districts[i].rVotes;
	}
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void printVotes(ourvector<State>& Data, int index, int repVotes, int demVotes) {
	// creative componet function that prints total votes for selected state
	// plus the differnce of votes
	int totalVotes = repVotes + demVotes;
	int differnceOfVotes = 0;
	string party1 = "", party2 = "";
	if (demVotes > repVotes) {
		differnceOfVotes = demVotes - repVotes;
		party1 = "Democractics";
		party2 = "Republicans";
	} else if (repVotes > demVotes) {
		differnceOfVotes = repVotes - demVotes;
		party1 = "Republicans";
		party2 = "Democractics";
	}
	cout << endl;
	cout << "Total Votes for " << Data[index].name;
	cout << ": " << totalVotes << endl;
	cout << "Total Democractic Votes: " << demVotes << endl;
	cout << "Total Republican Votes: " << repVotes << endl;
	cout << party1 << " had " << differnceOfVotes << " more votes than the ";
	cout << party2 << "." << endl;
}
//<-----------------------------------------------------------------------------

//----------------------------------------------------------------------------->
void votes(ourvector<State>& Data, int index, string load, bool stateFound) {
	// creative componet fucntion gets/prints the selected state votes info
	// but first checks if theres any errors
	bool error;
	int repVotes = 0, demVotes = 0;
	checkForErrors(load, stateFound, error);
	if (!error) {
		getVotes(Data, index, repVotes, demVotes);
		printVotes(Data, index, repVotes, demVotes);
	}
}
//<-----------------------------------------------------------------------------