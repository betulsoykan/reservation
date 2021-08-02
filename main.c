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

    void (*printR)(struct FlightStore *self,const char *firstName,const char *lastName);
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
                (self->flights[i].passengers[j].LastName) == lastname){
                struct Flight *flight = &self->flights[i];
                return flight;
            }
        }
    }

    return NULL;
}

void printReservation_(struct FlightStore *self,const char *firstName,const char *lastName){
     struct Flight *flight = self->searchFlightByName(self,firstName,lastName);
     if (flight == NULL){
         printf("No reservation!");
     }
     else{
         printf("%s %s %d %s %s",firstName,lastName,flight->Number,flight->DepartureCity,flight->ArrivalCity);
     }
}

void printFlight_(struct Flight *f) {
    if (f == NULL) {
        printf("No flights found");
    } else {
        printf("%d %s %s\n", f->passengerCount, f->DepartureCity, f->DepartureDay);
    }
}

int main() {
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

    return 0;
}