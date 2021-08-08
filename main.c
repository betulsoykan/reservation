#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    void (*printFlight)(struct Flight *f);

    void (*printFlightwPassenger)(struct Flight *p);

};


struct FlightStore {
    int flightCount;
    struct Flight (*flights);

    void (*addFlight)(struct FlightStore *self, struct Flight *newFlight);

    int (*makeReservation)(struct FlightStore *self, char *firstName, char *lastName, char *departureCity,
                           char *arrivalCity, char *flightDate);

    struct Flight *(*searchFlightByNumber)(struct FlightStore *self, int number);

    struct Flight *(*searchFlightByDepartureCity)(struct FlightStore *self, const char *departureCity);

    struct Flight *(*searchFlightByArrivalCity)(struct FlightStore *self, const char *arrivalCity);

    struct Flight *(*searchFlightByName)(struct FlightStore *self, const char *firstname, const char *lastname);

    void (*printR)(struct FlightStore *self, const char *firstName, const char *lastName);
};

void addPassenger_(struct Flight *self, struct Passenger *passenger) {
    self->passengers[self->passengerCount] = *passenger;
    self->passengerCount++;
}

void printFlightwPassenger_(struct Flight *p) {
    if (p == NULL) {
        printf("No flights found");
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
        if (self->flights[i].DepartureCity == departureCity && self->flights[i].ArrivalCity == arrivalCity
            && self->flights[i].DepartureDay == flightDate) {
            struct Flight *flight = &self->flights[i];
            struct Passenger newPassenger = {firstName, lastName, departureCity, arrivalCity, flightDate};
            flight->addPassenger(flight, &newPassenger);
            return 0;
        }
    }

    return 1;
}

struct Flight *searchFlightByNumber_(struct FlightStore *self, int number) {
    for (int i = 0; i < sizeof(*self->flights); i++) {
        if (self->flights[i].Number == number) {
            struct Flight *flight = &self->flights[i];
            return flight;
        }
    }

    return NULL;
}

struct Flight *searchFlightByDepartureCity_(struct FlightStore *self, const char *departureCity) {
    for (int i = 0; i < sizeof(*self->flights); i++) {
        if (self->flights[i].DepartureCity == departureCity) {
            struct Flight *flight = &self->flights[i];
            return flight;
        }
    }

    return NULL;
}

struct Flight *searchFlightByArrivalCity_(struct FlightStore *self, const char *arrivalCity) {
    for (int i = 0; i < sizeof(*self->flights); i++) {
        if (self->flights[i].ArrivalCity == arrivalCity) {
            struct Flight *flight = &self->flights[i];
            return flight;
        }
    }

    return NULL;
}

struct Flight *searchFlightByName_(struct FlightStore *self, const char *firstname, const char *lastname) {
    for (int i = 0; i < (self->flightCount); i++) {
        for (int j = 0; j < self->flights[i].passengerCount; j++) {
            if ((self->flights[i].passengers[j].FirstName) == firstname &&
                (self->flights[i].passengers[j].LastName) == lastname) {
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
        printf("No reservation!");
    } else {
        printf("%s %s %d %s %s", firstName, lastName, flight->Number, flight->DepartureCity, flight->ArrivalCity);
    }
}

void printFlight_(struct Flight *f) {
    if (f == NULL) {
        printf("No flights found");
    } else {
        printf("%d %s %s\n", f->passengerCount, f->DepartureCity, f->DepartureDay);
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

int readFile(char *fileName) {
    int lineCount = 0;
    printf("reading the file: %s\n", fileName);
    FILE *in_file = fopen(fileName, "r");
    if (in_file == NULL) {
        printf("Error! Could not open file\n");
        exit(-1);
    }
    char *flightCount = readLineForwardFrom(in_file, lineCount);
    lineCount++;
    printf("flightCount: %d\n", atoi(flightCount));
    while(lineCount <= atoi(flightCount)) {
        printf("flight: %s\n", readLineForwardFrom(in_file,lineCount));
        lineCount++;
    }
    char *reservationCount = readLineForwardFrom(in_file, lineCount);
    lineCount++;
    printf("ReservationCount: %d\n", atoi(reservationCount));
    for(int i = 0;i < atoi(reservationCount);i++) {
        printf("reservation: %s\n", readLineForwardFrom(in_file,lineCount));
        lineCount++;
    }
    return 1;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("you are not allowed to enter more than one file. File count: %d", argc - 1);
        return -1;
    }
    /* for (int i = 0; i < argc; ++i) {
         printf("argv[%d]: %s\n", i, argv[i]);
     }*/

    readFile(argv[1]);
    /*
    struct FlightStore fs = {0, malloc(2 * sizeof(struct Flight)), addFlight_, makeReservation_,
                             searchFlightByNumber_, searchFlightByDepartureCity_, searchFlightByArrivalCity_,
                             searchFlightByName_,printReservation_};
    struct Flight newFlight1 = {0, 1, "Nevsehir", "Istanbul", "29/08/2021", "15:35", "29/08/2021", "16:35",
                                malloc(300 * sizeof(struct Passenger)), addPassenger_, printFlight_,
                                printFlightwPassenger_};
    struct Passenger newPassenger1 = {"oguzhan", "soykan"};
    newFlight1.addPassenger(&newFlight1, &newPassenger1);
    struct Passenger newPassenger2 = {"betul", "soykan"};
    newFlight1.addPassenger(&newFlight1, &newPassenger2);

    struct Flight newFlight2 = {0, 2, "Kayseri", "Istanbul", "25/08/2021", "15:35", "25/08/2021", "16:35",
                                malloc(300 * sizeof(struct Passenger)), addPassenger_, printFlight_,
                                printFlightwPassenger_};
    fs.addFlight(&fs, &newFlight1);
    fs.addFlight(&fs, &newFlight2);
    if (fs.makeReservation(&fs, "bani", "soykan", "Edırne", "Istanbul", "29/08/2021") == 0) {
        printf("Reserved\n");
    } else {
        printf("No reservation\n");
    }
    */

    return 0;
}