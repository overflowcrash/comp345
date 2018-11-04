#include "MapReader.h"

bool MapReader::read(string mapFile)
{
    cout << "Reading map " << mapFile << " ..." << endl;
	ifstream input1(mapFile);
	string s;

	if (input1.is_open() == false) {
		cerr << "Error opening the file" << endl;
        return false;
	}

	ifstream *inputPt = &input1;
	while (getline(input1, s)) {
		if (s == "[Continents]")
			readContinents(inputPt);
		if (s == "[Territories]")
			readTerritories(inputPt);
	}

	if (this->contLines.empty() or this->terrLines.empty()) {
		cerr << "The map is invalid " << endl;
		return false;
	}
    return true;
}

void MapReader::readTerritories(ifstream* territories)
{
	string s;
	while (getline(*territories, s)) {
		if (s.empty())
            break;
	    this->terrLines.push_back(s);
	}
}

void MapReader::readContinents(ifstream* territories)
{
	string s;
	while (getline(*territories, s)) {
		if (s.empty())
            break;
	    this->contLines.push_back(s);
	}
}


int MapReader::getNbOfNodes()
{
    int count = 0;
    for (string line: this->contLines)
    {
        int nbOfCountries = stoi(line.substr(line.find("=") + 1));
        count += nbOfCountries;
    }
    return count;
}

void MapReader::load(Map &map)
{
    // Continents
    std::map<string, Map::Graph&> continents;
    std::map<string, Map::Graph&>::iterator continentsIt;

    for (string line: this->contLines)
    {
        string name = line.substr(0, line.find("="));
        int nbOfCountries = stoi(line.substr(line.find("=") + 1));
        cout << "Adding Continent " << name <<
                " with " << nbOfCountries << " country nodes" << endl;
        Continent* continent = new Continent(name);
        continents.insert(
            pair<string, Map::Graph&>(name, map.addContinent(continent))
        );
    }

    // Territories
    std::map<string, int> territories;
    std::map<string, int>::iterator territoriesIt;

    for (string line: this->terrLines)
    {
        string name;
        int count = 0;
        stringstream linestream(line);
        string segment;
        while (getline(linestream, segment, ','))
        {
            switch (count)
            {
                // name
                case 0:
                    name = segment;
                    cout << "Adding Territory " << name << " with edges to ";
                    break;
                // x pixel
                case 1:
                    break;
                // y pixel
                case 2:
                    break;
                // adjacents
                default:
                    cout << segment << " ";
                    // check if continent or territory
                    //continentsIt =
                    break;
            }
            count++;
        }
        cout << endl;
    }
}