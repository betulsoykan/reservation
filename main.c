#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPERATION_FLIGHTNUMBER "flightNumber"
#define OPERATION_DEPARTURE "departure"
#define OPERATION_ARRIVAL "arrival"
#define OPERATION_PRINT "print"
#define OPERATION_PRINTWP "printwP"
#define OPERATION_PRINTR "printR"

struct Passenger {
    char *FirstName;
    char *LastName;
    char *DepartureCity;
    char *ArrivalCity;
    char *DepartureDay;
    int FlightNumber;
};

struct Flight {
    int passengerCount;
    int Number;
    char *DepartureCity;
    char *ArrivalCity;
    char *DepartureDay;
    char *DepartureTime;
    char *ArrivalDay;
    char *ArrivalTime;
    struct Passenger (*passengers);

    void (*addPassenger)(struct Flight *self, struct Passenger *passenger);

    void (*printFlightwPassenger)(struct Flight *p);
};


struct FlightStore {
    int flightCount;
    struct Flight (*flights);

    void (*addFlight)(struct FlightStore *self, struct Flight *newFlight);

    int (*makeReservation)(struct FlightStore *self, char *firstName, char *lastName, char *departureCity,
                           char *arrivalCity, char *flightDate);

    struct Flight *(*searchFlightByNumber)(struct FlightStore *self, int number);

    struct FlightResult (*searchFlightByDepartureCity)(struct FlightStore *self, const char *departureCity);

    struct Flight *(*searchFlightByArrivalCity)(struct FlightStore *self, const char *arrivalCity);

    struct Flight *(*searchFlightByName)(struct FlightStore *self, const char *firstname, const char *lastname);

    void (*printR)(struct FlightStore *self, const char *firstName, const char *lastName);

    void (*printFlight)(struct FlightStore *self, struct Flight *flight);

    char **functions;
};

char *trim(char *text) {
    return strtok(text, "\n");
}

void addPassenger_(struct Flight *self, struct Passenger *passenger) {
    self->passengers[self->passengerCount] = *passenger;
    self->passengerCount++;
}

void printFlightwPassenger_(struct Flight *p) {
    if (p == NULL) {
        printf("No flights found\n");
    } else {
        printf("%d %s %s %s %s %d\n", p->Number, p->DepartureCity, p->ArrivalCity, p->DepartureTime, p->ArrivalTime,
               p->passengerCount);
        for (int i = 0; i < p->passengerCount; i++) {
            printf("%s %s\n", p->passengers[i].FirstName, p->passengers[i].LastName);
        }
    }
}

void addFlight_(struct FlightStore *self, struct Flight *newFlight) {
    self->flights[self->flightCount] = *newFlight;
    self->flightCount++;
}

int makeReservation_(struct FlightStore *self, char *firstName, char *lastName, char *departureCity,
                     char *arrivalCity, char *flightDate) {
    for (int i = 0; i < self->flightCount; i++) {
        if (strcmp(self->flights[i].DepartureCity, departureCity) == 0 &&
            strcmp(self->flights[i].ArrivalCity, arrivalCity) == 0 &&
            strcmp(self->flights[i].DepartureDay, flightDate) == 0) {
            struct Flight *flight = &self->flights[i];
            struct Passenger newPassenger = {firstName, lastName, departureCity, arrivalCity, flightDate};
            flight->addPassenger(flight, &newPassenger);
            return 0;
        }
    }

    return 1;
}

struct Flight *searchFlightByNumber_(struct FlightStore *self, int number) {
    for (int i = 0; i < self->flightCount; i++) {
        if (self->flights[i].Number == number) {
            struct Flight *flight = &self->flights[i];
            return flight;
        }
    }

    return NULL;
}

struct FlightResult {
    int count;
    struct Flight **flights;
};

struct FlightResult searchFlightByDepartureCity_(struct FlightStore *self, const char *departureCity) {
    struct FlightResult result = {0, malloc(100 * sizeof(struct Flight))};
    int counter = 0;
    for (int i = 0; i < self->flightCount; i++) {
        if (strcmp(self->flights[i].DepartureCity, departureCity) == 0) {
            struct Flight *flight = &self->flights[i];
            result.flights[counter] = flight;
            counter++;
        }
    }

    result.count = counter;
    return result;
}

struct Flight *searchFlightByArrivalCity_(struct FlightStore *self, const char *arrivalCity) {
    for (int i = 0; i < self->flightCount; i++) {
        if (strcmp(self->flights[i].ArrivalCity, arrivalCity) == 0) {
            struct Flight *flight = &self->flights[i];
            return flight;
        }
    }

    return NULL;
}

struct Flight *searchFlightByName_(struct FlightStore *self, const char *firstname, const char *lastname) {
    for (int i = 0; i < (self->flightCount); i++) {
        for (int j = 0; j < self->flights[i].passengerCount; j++) {
            if (strcmp(self->flights[i].passengers[j].FirstName, firstname) == 0 &&
                strcmp(self->flights[i].passengers[j].LastName, lastname) == 0) {
                struct Flight *flight = &self->flights[i];
                return flight;
            }
        }
    }

    return NULL;
}

void printReservation_(struct FlightStore *self, const char *firstName, const char *lastName) {
    struct Flight *flight = self->searchFlightByName(self, firstName, lastName);
    if (flight == NULL) {
        printf("No reservation!\n");
    } else {
        printf("%s %s %d %s %s\n", firstName, lastName, flight->Number, flight->DepartureCity, flight->ArrivalCity);
    }
}

void printFlight_(struct FlightStore *self, struct Flight *f) {
    if (f == NULL) {
        printf("No flight found\n");
    } else {
        printf("%d %s %s %s %s %s %s\n", f->Number, f->DepartureCity, f->ArrivalCity, f->DepartureDay, f->DepartureTime,
               f->ArrivalDay, f->ArrivalTime);
    }
}

char *readLineForwardFrom(FILE *in_file, int reqLine) {
    char line[1000] = "";
    char *found = malloc(1000);
    int current = 0;
    while (fgets(line, 1000, in_file) != NULL) {
        if (current == reqLine) {
            strcpy(found, line);
            fseek(in_file, 0, SEEK_SET);
            return found;
        } else {
            current++;
        }
    }
    fseek(in_file, 0, SEEK_SET);

    return found;
}

char **parsLineToArray(char *line, int size) {
    char *token = strtok(line, " ");
    char **array = (char **) malloc((size + 1) * sizeof(char *));
    int count = 0;

    while (token != NULL) {
        array[count] = token;
        token = strtok(NULL, " ");
        count++;
    }

    return array;
}


struct FlightStore readFile(char *fileName) {
    int lineCount = 0;
    FILE *in_file = fopen(fileName, "r");
    if (in_file == NULL) {
        printf("Error! Could not open file\n");
        exit(-1);
    }
    char *flightCount = readLineForwardFrom(in_file, lineCount);
    lineCount++;
    struct FlightStore fs = {0, malloc(atoi(flightCount) * sizeof(struct Flight)), addFlight_, makeReservation_,
                             searchFlightByNumber_, searchFlightByDepartureCity_, searchFlightByArrivalCity_,
                             searchFlightByName_, printReservation_, printFlight_,
                             (char **) malloc(10 * sizeof(char *))};

    while (lineCount <= atoi(flightCount)) {
        char *line = readLineForwardFrom(in_file, lineCount);

        lineCount++;
        char **readFlight = parsLineToArray(line, 7);
        struct Flight f = {0,
                           atoi(readFlight[0]),
                           trim(readFlight[1]),
                           trim(readFlight[2]),
                           trim(readFlight[3]),
                           trim(readFlight[4]),
                           trim(readFlight[5]),
                           trim(readFlight[6]),
                           malloc(300 * sizeof(struct Passenger)), addPassenger_,
                           printFlightwPassenger_};

        fs.addFlight(&fs, &f);
    }

    char *reservationCount = readLineForwardFrom(in_file, lineCount);
    lineCount++;
    for (int i = 0; i < atoi(reservationCount); i++) {
        char **readReservation = parsLineToArray(readLineForwardFrom(in_file, lineCount), 5);
        char *name = trim(readReservation[0]);
        char *surname = trim(readReservation[1]);
        char *departure = trim(readReservation[2]);
        char *arrival = trim(readReservation[3]);
        char *departureDate = trim(readReservation[4]);
        int reservationResult = fs.makeReservation(&fs, name, surname,
                                                   departure,
                                                   arrival,
                                                   departureDate);
        if (reservationResult == 1) {
            printf("%s %s %s %s %s No reservation\n", name, surname, departure, arrival, departureDate);
        }
        lineCount++;
    }

    int counter = 0;
    while (strcmp(readLineForwardFrom(in_file, lineCount), "END\n") != 0) {
        fs.functions[counter] = readLineForwardFrom(in_file, lineCount);
        lineCount++;
        counter++;
    }
    fclose(in_file);
    return fs;
}

void handleFunction(char **func, struct FlightStore fs) {
    char *command = trim(func[0]); //search, printR, printW
    if (strcmp("search", command) == 0) {
        char *operation = trim(func[1]);
        char *parameter = trim(func[2]);
        if (strcmp(OPERATION_FLIGHTNUMBER, operation) == 0) {
            struct Flight *f = fs.searchFlightByNumber(&fs, atoi(parameter));
            fs.printFlight(&fs, f);
        }
        if (strcmp(OPERATION_DEPARTURE, operation) == 0) {
            struct FlightResult result = fs.searchFlightByDepartureCity(&fs, parameter);
            for (int i = 0; i < result.count; ++i) {
                struct Flight *f = result.flights[i];
                fs.printFlight(&fs, f);
            }
        }
        if (strcmp(OPERATION_ARRIVAL, operation) == 0) {
            struct Flight *f = fs.searchFlightByArrivalCity(&fs, parameter);
            fs.printFlight(&fs, f);
        }
    } else {
        if (strcmp(OPERATION_PRINT, command) == 0)
            fs.printFlight(&fs, fs.searchFlightByNumber(&fs, atoi(func[1])));
        if (strcmp(OPERATION_PRINTR, command) == 0)
            fs.printR(&fs, trim(func[1]), trim(func[2]));
        if (strcmp(OPERATION_PRINTWP, command) == 0) {
            struct Flight *flight = fs.searchFlightByNumber(&fs, atoi(trim(func[1])));
            flight->printFlightwPassenger(flight);
        }
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("you are not allowed to enter more than one file. File count: %d", argc - 1);
        return -1;
    }

    struct FlightStore fs = readFile(argv[1]);
    for (int i = 0; i < 8; ++i) {
        handleFunction(parsLineToArray(fs.functions[i], 3), fs);
    }

    return 0;
}

